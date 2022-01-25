#ifndef _EMU_C_AST_H
#define _EMU_C_AST_H

#include <stdarg.h>
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

enum C_TYPE
{
	CT_BYTE,
	CT_PTR,
	CT_VOID,
};

// A variable, holding either an address or a value
typedef struct _CVAR
{
	char name[16];
	uint16_t value;
	C_TYPE type;
} CVAR;

CVAR * cvar_alloc(char *, C_TYPE);
void cvar_free(CVAR *);

// Variable tree node
typedef struct _CVNODE
{
	CVAR *var;
	struct _CVNODE *next;
} CVNODE;

// A binary expression
typedef struct _CBINEXP
{
	C_OP op;
	void *lhs;
	void *rhs;
} CBINEXP;

// Function
typedef struct _CFUNC
{
	char name[16];
	C_TYPE type;
	CVNODE *args;
} CFUNC;

// Function tree node
typedef struct _CFNODE
{
	CFUNC *func;
	struct _CFNODE *next;
} CFNODE;

CFUNC * cfunc_alloc(char *);
void cfunc_add_arg(CVAR *);
void cfunc_free(CFUNC *);

typedef struct _CSTREE
{
	CFNODE *funcs;
	CVNODE *vars;
} CSTREE;

CSTREE * cstree_alloc();
void cstree_free(CSTREE *);

#endif // _EMU_C_AST_H
