#ifndef _EMU_C_AST_H
#define _EMU_C_AST_H

#include <stdint.h>

enum C_OP
{
	COP_ADD, // +
	COP_ADDA, // +=
	COP_AND, // &&
	COP_ASS, // =
	COP_BAND, // &
	COP_BANDA, // &=
	COP_BOR, // |
	COP_BORA, // |=
	COP_CMPA, // ~=
	COP_DEC, // --
	COP_DIV, // /
	COP_DIVA, // /=
	COP_EQ, // ==
	COP_GTE, // >=
	COP_INC, // ++
	COP_LS, // <<
	COP_LSA, // <<=
	COP_LTE, // <=
	COP_MUL, // *
	COP_MULA, // *=
	COP_NEQ, // !=
	COP_OR, // ||
	COP_RS, // >>
	COP_RSA, // >>=
	COP_SUB, // -
	COP_SUBA, // -=
	COP_TRN, // ?
	COP_XOR, // ^
	COP_XORA // ^=
};

// A variable, holding either an address or a value
typedef struct _CVAR
{
	char name[16];

	union
	{
		uint16_t addr;
		uint8_t value;
	};
} CVAR;

// Variable tree node
typedef struct _CVNODE
{
	CVAR *var;
	CVNODE *next;
} CVNODE;

// A binary expression
typedef struct _CBINEXP
{
	C_OP op;
	void *lhs;
	void *rhs;
} CBINEXP;

// Function call
typedef struct _CFUNC
{
	char name[16];
	CVNODE *args;
} CFUNC;

#endif // _EMU_C_AST_H
