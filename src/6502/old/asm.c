#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
	#define _CRT_SECURE_NO_DEPRECATE
#endif // _MSC_VER

#define IDENT_LEN 32

typedef enum _ADDR_MODE
{
	AM_NONE = 0, // jam
	AM_ABS,
	AM_ABX,
	AM_ABY,
	AM_IMM,
	AM_IMP,
	AM_IND,
	AM_IZX,
	AM_IZY,
	AM_REL,
	AM_ZP,
	AM_ZPX,
	AM_ZPY
} ADDR_MODE;

typedef struct _OP
{
	ADDR_MODE addr_mode;
	const char *keyword;
} OP;

// multipurpose
typedef struct _FLAGS
{
	uint8_t
		dec : 1,
		ind : 1;
} FLAGS;

typedef struct _EXPR
{
	FLAGS flags;
	uint16_t value;
	const OP *op;
	struct _EXPR *next;
} EXPR;

typedef struct _LABEL
{
	char *txt;
	struct _LABEL *next;
};

typedef enum _TOKEN
{
	TOK_EOF,
	TOK_BIN,
	TOK_DEC,
	TOK_HEX,
	TOK_IDENT,
	TOK_OP,
	TOK_X,
	TOK_Y
} TOKEN;

static OP OPS[] = {
	{ AM_IMP, "brk" },
	{ AM_IZX, "ora" },
	{ AM_NONE, "jam" },
	{ AM_IZX, "slo" },
	{ AM_ZP, "nop" },
	{ AM_ZP, "ora" },
	{ AM_ZP, "asl" },
	{ AM_ZP, "slo" },
	{ AM_IMP, "php" },
	{ AM_IMM, "ora" },
	{ AM_IMP, "asl" },
	{ AM_IMM, "anc" },
	{ AM_ABS, "nop" },
	{ AM_ABS, "ora" },
	{ AM_ABS, "asl" },
	{ AM_ABS, "slo" },

	// 1x
	{ AM_REL, "bpl" },
	{ AM_IZY, "ora" },
	{ AM_NONE, "jam" },
	{ AM_IZY, "slo" },
	{ AM_ZPX, "nop" },
	{ AM_ZPX, "ora" },
	{ AM_ZPX, "asl" },
	{ AM_ZPX, "slo" },
	{ AM_IMP, "clc" },
	{ AM_ABY, "ora" },
	{ AM_IMP, "nop" },
	{ AM_ABY, "slo" },
	{ AM_ABX, "nop" },
	{ AM_ABX, "ora" },
	{ AM_ABX, "asl" },
	{ AM_ABX, "slo" },

	// 2x
	{ AM_ABS, "jsr" },
	{ AM_IZX, "and" },
	{ AM_NONE, "jam" },
	{ AM_IZX, "rla" },
	{ AM_ZP, "bit" },
	{ AM_ZP, "and" },
	{ AM_ZP, "rol" },
	{ AM_ZP, "rla" },
	{ AM_IMP, "plp" },
	{ AM_IMM, "and" },
	{ AM_IMP, "rol" },
	{ AM_IMM, "anc" },
	{ AM_ABS, "bit" },
	{ AM_ABS, "and" },
	{ AM_ABS, "rol" },
	{ AM_ABS, "rla" },

	// 3x
	{ AM_REL, "bmi" },
	{ AM_IZY, "and" },
	{ AM_NONE, "jam" },
	{ AM_IZY, "rla" },
	{ AM_ZPX, "nop" },
	{ AM_ZPX, "and" },
	{ AM_ZPX, "rol" },
	{ AM_ZPX, "rla" },
	{ AM_IMP, "sec" },
	{ AM_ABY, "and" },
	{ AM_IMP, "nop" },
	{ AM_ABY, "rla" },
	{ AM_ABX, "nop" },
	{ AM_ABX, "and" },
	{ AM_ABX, "rol" },
	{ AM_ABX, "rla" },

	// 4x
	{ AM_IMP, "rti" },
	{ AM_IZX, "eor" },
	{ AM_NONE, "jam" },
	{ AM_IZX, "sre" },
	{ AM_ZP, "nop" },
	{ AM_ZP, "eor" },
	{ AM_ZP, "lsr" },
	{ AM_ZP, "sre" },
	{ AM_IMP, "pha" },
	{ AM_IMM, "eor" },
	{ AM_IMP, "lsr" },
	{ AM_ABS, "alr" },
	{ AM_ABS, "jmp" },
	{ AM_ABS, "eor" },
	{ AM_ABS, "lsr" },
	{ AM_ABS, "sre" },

	// 5x
	{ AM_REL, "bvc" },
	{ AM_IZY, "eor" },
	{ AM_NONE, "jam" },
	{ AM_IZY, "sre" },
	{ AM_ZPX, "nop" },
	{ AM_ZPX, "eor" },
	{ AM_ZPX, "lsr" },
	{ AM_ZPX, "sre" },
	{ AM_IMP, "cli" },
	{ AM_ABY, "eor" },
	{ AM_IMP, "nop" },
	{ AM_ABY, "sre" },
	{ AM_ABX, "nop" },
	{ AM_ABX, "eor" },
	{ AM_ABX, "lsr" },
	{ AM_ABX, "sre" },

	// 6x
	{ AM_IMP, "rts" },
	{ AM_IZX, "adc" },
	{ AM_NONE, "jam" },
	{ AM_IZX, "rra" },
	{ AM_ZP, "nop" },
	{ AM_ZP, "adc" },
	{ AM_ZP, "ror" },
	{ AM_ZP, "rra" },
	{ AM_IMP, "pla" },
	{ AM_IMM, "adc" },
	{ AM_IMP, "ror" },
	{ AM_IMM, "arr" },
	{ AM_IND, "jmp" },
	{ AM_ABS, "adc" },
	{ AM_ABS, "ror" },
	{ AM_ABS, "rra" },

	// 7x
	{ AM_REL, "bvs" },
	{ AM_IZY, "adc" },
	{ AM_NONE, "jam" },
	{ AM_IZY, "rra" },
	{ AM_ZPX, "nop" },
	{ AM_ZPX, "adc" },
	{ AM_ZPX, "ror" },
	{ AM_ZPX, "rra" },
	{ AM_IMP, "sei" },
	{ AM_ABY, "adc" },
	{ AM_IMP, "nop" },
	{ AM_ABY, "rra" },
	{ AM_ABX, "nop" },
	{ AM_ABX, "adc" },
	{ AM_ABX, "ror" },
	{ AM_ABX, "rra" },

	// 8x
	{ AM_IMM, "nop" },
	{ AM_IZX, "sta" },
	{ AM_IMM, "nop" },
	{ AM_IZX, "sax" },
	{ AM_ZP, "sty" },
	{ AM_ZP, "sta" },
	{ AM_ZP, "stx" },
	{ AM_ZP, "sax" },
	{ AM_IMP, "dey" },
	{ AM_IMM, "nop" },
	{ AM_IMP, "txa" },
	{ AM_IMM, "ane" },
	{ AM_ABS, "sty" },
	{ AM_ABS, "sta" },
	{ AM_ABS, "stx" },
	{ AM_ABS, "sax" },

	// 9x
	{ AM_REL, "bcc" },
	{ AM_IZY, "sta" },
	{ AM_NONE, "jam" },
	{ AM_IZY, "sha" },
	{ AM_ZPX, "sty" },
	{ AM_ZPX, "sta" },
	{ AM_ZPY, "stx" },
	{ AM_ZPY, "sax" },
	{ AM_IMP, "tya" },
	{ AM_ABY, "sta" },
	{ AM_IMP, "txs" },
	{ AM_ABY, "tas" },
	{ AM_ABX, "shy" },
	{ AM_ABX, "sta" },
	{ AM_ABY, "shx" },
	{ AM_ABY, "sha" },

	// Ax
	{ AM_IMM, "ldy" },
	{ AM_IZX, "lda" },
	{ AM_IMM, "ldx" },
	{ AM_IZX, "lax" },
	{ AM_ZP, "ldy" },
	{ AM_ZP, "lda" },
	{ AM_ZP, "ldx" },
	{ AM_ZP, "lax" },
	{ AM_IMP, "tay" },
	{ AM_IMM, "lda" },
	{ AM_IMP, "tax" },
	{ AM_IMM, "lxa" },
	{ AM_ABS, "ldy" },
	{ AM_ABS, "lda" },
	{ AM_ABS, "ldx" },
	{ AM_ABS, "lax" },

	// Bx
	{ AM_REL, "bcs" },
	{ AM_IZY, "lda" },
	{ AM_NONE, "jam" },
	{ AM_IZY, "lax" },
	{ AM_ZPX, "ldy" },
	{ AM_ZPX, "lda" },
	{ AM_ZPY, "ldx" },
	{ AM_ZPY, "lax" },
	{ AM_IMP, "clv" },
	{ AM_ABY, "lda" },
	{ AM_IMP, "tsx" },
	{ AM_ABY, "las" },
	{ AM_ABX, "ldy" },
	{ AM_ABX, "lda" },
	{ AM_ABY, "ldx" },
	{ AM_ABY, "lax" },

	// Cx
	{ AM_IMM, "cpy" },
	{ AM_IZX, "cmp" },
	{ AM_IMM, "nop" },
	{ AM_IZX, "dcp" },
	{ AM_ZP, "cpy" },
	{ AM_ZP, "cmp" },
	{ AM_ZP, "dec" },
	{ AM_ZP, "dcp" },
	{ AM_IMP, "iny" },
	{ AM_IMM, "cmp" },
	{ AM_IMP, "dex" },
	{ AM_IMM, "sbx" },
	{ AM_ABS, "cpy" },
	{ AM_ABS, "cmp" },
	{ AM_ABS, "dec" },
	{ AM_ABS, "dcp" },

	// Dx
	{ AM_REL, "bne" },
	{ AM_IZY, "cmp" },
	{ AM_NONE, "jam" },
	{ AM_IZY, "dcp" },
	{ AM_ZPX, "nop" },
	{ AM_ZPX, "cmp" },
	{ AM_ZPX, "dec" },
	{ AM_ZPX, "dcp" },
	{ AM_IMP, "cld" },
	{ AM_ABY, "cmp" },
	{ AM_IMP, "nop" },
	{ AM_ABY, "dcp" },
	{ AM_ABX, "nop" },
	{ AM_ABX, "cmp" },
	{ AM_ABX, "dec" },
	{ AM_ABX, "dcp" },

	// Ex
	{ AM_IMM, "cpx" },
	{ AM_IZX, "sbc" },
	{ AM_IMM, "nop" },
	{ AM_IZX, "isc" },
	{ AM_ZP, "cpx" },
	{ AM_ZP, "sbc" },
	{ AM_ZP, "inc" },
	{ AM_ZP, "isc" },
	{ AM_IMP, "inx" },
	{ AM_IMM, "sbc" },
	{ AM_IMP, "nop" },
	{ AM_IMM, "usbc" },
	{ AM_ABS, "cpx" },
	{ AM_ABS, "sbc" },
	{ AM_ABS, "inc" },
	{ AM_ABS, "isc" },

	// Fx
	{ AM_REL, "beq" },
	{ AM_IZY, "sbc" },
	{ AM_NONE, "jam" },
	{ AM_IZY, "isc" },
	{ AM_ZPX, "nop" },
	{ AM_ZPX, "sbc" },
	{ AM_ZPX, "inc" },
	{ AM_ZPX, "isc" },
	{ AM_IMP, "sed" },
	{ AM_ABY, "sbc" },
	{ AM_IMP, "nop" },
	{ AM_ABY, "isc" },
	{ AM_ABX, "nop" },
	{ AM_ABX, "sbc" },
	{ AM_ABX, "inc" },
	{ AM_ABX, "isc" }
};

typedef struct _STATE
{
	char last_char;
	char ident[IDENT_LEN - 1];
	FLAGS flags;
	uint16_t val;
	TOKEN last_tok;
	EXPR *exprs;
	LABEL *labels;
	FILE *src;
} STATE;

// peek at a char in file, but don't advance
static char fpeek(FILE *file)
{
	fpos_t pos;

	fgetpos(file, &pos);
	char c = fgetc(file);
	fsetpos(file, &pos);

	return c;
}

// Non-zero if the given string is an operator
static int isop(const char *s)
{
	for (int i = 0; i < 256; i++)
		if (strcmp(tolower(s), OPS[I].keyword) == 0)
			return 1;

	return 0;
}

// Non-zero if the given character is a newline character
static int isnewline(char c)
{
	switch (c)
	{
		case '\r':
		case '\n':
			return 1;
		default:
			return 0;
	}
}

// Non-zero if the given char is binary (0 or 1)
static int isbin(char c)
{
	if (c == '0' || c == '1')
		return 1;

	return 0;
}

static int next_tok(STATE *state)
{
	// skip whitespace
	while (isspace(state->last_char))
		state->last_char = fgetc(state->src);

	// skip comments
	if (state->last_char == ';')
		while (!isnewline(state->last_char))
			state->last_char = fgetc(state->src);

	if (isalpha(state->last_char))
	{
		// register
		switch (tolower(state->last_char))
		{
			case 'x': return TOK_X;
			case 'y': return TOK_Y;
			default: ;
		}

		memset(&state->ident, 0, IDENT_LEN);
		state->ident[0] = state->last_char;

		for (int i = 1; i < IDENT_LEN - 1; i++)
		{
			state->last_char = fgetc(state->src);
			if (!isalnum(state->last_char)
				break;
			state->ident[i] = state->last_char;
		}

		// operation
		for (int i = 0; i < 256; i++)
			if (strcmp(&state->ident, OPS[i].keyword)
				return TOK_OP;

		return TOK_IDENT;
	}

	// binary literal
	if (state->last == '%')
	{
		char bin[17];

		memset(&bin, 0, 17);
		for (int i = 0; i < 16; i++)
		{
			state->last_char = fgetc(state->src);
			if (!isbin(state->last_char))
				break;
			bin[i] = state->last_char;
		}

		state->val = (uint16_t)strtoul(&bin, NULL, 2);
		return TOK_BIN;
	}

	// decimal
	if (state->flags.dec && isdigit(state->last))
	{
		char dec[6];

		memset(&dec, 0, 6);
		dec[0] = state->last_char;

		for (int i = 1; i < 5; i++)
		{
			state->last_char = fgetc(state->src);
			if (!isdigit(state->last_char))
				break;
			dec[i] = state->last_char;
		}

		state->val = (uint16_t)atol(&dec);
		return TOK_DEC;
	}

	// hex
	if (isxdigit(state->last))
	{
		char hex[5];

		memset(&hex, 0, 5);
		for (int i = 0; i < 4; i++)
		{
			state->last_char = fgetc(state->src);
			if (!isxdigit(state->last_char))
				break;
			hex[i] = state->last_char;
		}

		state->val = (uint16_t)strtoul(&hex, NULL, 16);
		return TOK_HEX;
	}

	// return last_char as a fallback value
	int fallback = state->last;
	state->last_char = fgetc(state->src);
	return fallback;
}

static STATE * state_alloc(FILE *file)
{
	STATE *state = (STATE *)calloc(1, sizeof(STATE));
	state->exprs = NULL;
	state->labels = NULL;
	state->src = file;
	state->last_char = ' ';

	return state;
}

static void state_free(STATE *state)
{
	if (!state) return;

	EXPR *eit = NULL;
	EXPR *enext = state->exprs;

	while (enext)
	{
		eit = enext;
		enext = eit->next;
		free(eit);
	}

	LABEL *lit = NULL;
	LABEL *lnext = state->labels;

	while (lnext)
	{
		lit = lnext;
		lnext = lit->next;
		free(lit);
	}

	free(state);
	state = NULL;
}

static EXPR * next

// parses an operation
static EXPR * parse_op(STATE *state)
{
	EXPR *exp
}

static int parse(STATE *state)
{

	while (!feof(file))
	{
		switch (next_token(state))
		{

		}
	}
}

int main(int argc, char **argv)
{
	state_free(state);

	return 0;
}
