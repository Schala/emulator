#ifndef _6502_AST_H
#define _6502_AST_H

#include <memory>

#include "lexer.h"

enum class AddressMode6502 : uint8_t
{
	None, // jam
	Absolute,
	AbsoluteX,
	AbsoluteY,
	Immediate,
	Implied,
	Indirect,
	IndirectX,
	IndirectY,
	Relative,
	ZeroPage,
	ZeroPageX,
	ZeroPageY,
};

struct Operation6502
{
	AddressMode6502 Mode;
	std::string_view Label;
};

// Base class for all expressions
class Expr6502
{
public:
	virtual ~Expr6502();
};

typedef std::unique_ptr<Expr6502> Expr6502Ptr;

// Value expression
class ValueExpr6502 : public Expr6502
{
public:
	ValueExpr6502(uint8_t);
private:
	uint8_t m_val;
};

// Address expression
class AddressExpr6502 : public Expr6502
{
public:
	AddressExpr6502(uint16_t);
private:
	uint16_t m_addr;
};

// String expression
class StringExpr6502 : public Expr6502
{
public:
	StringExpr6502(const char *);
private:
	std::string_view m_str;
};

// Variable expression
class VariableExpr6502 : public Expr6502
{
public:
	VariableExpr6502(const char *);
private:
	std::string_view m_name;
};

// Binary operation expression
class BinaryExpr6502 : public Expr6502
{
public:
	BinaryExpr6502(Token6502, Expr6502Ptr, Expr6502Ptr);
private:
	Token6502 m_op;
	Expr6502Ptr m_lhs;
	Expr6502Ptr m_rhs;
};

// Operation expression
class OperationExpr6502 : public Expr6502
{
public:
	OperationExpr6502(uint16_t, const Operation6502 &, Expr6502Ptr);
private:
	uint16_t m_addr;
	const Operation6502 &m_op;
	Expr6502Ptr m_operand;
};

// Command expression
class CommandExpr6502 : public Expr6502
{
public:
	CommandExpr6502(uint16_t, Token6502, Expr6502Ptr);
private:
	uint16_t m_addr;
	Token6502 m_cmd;
	Expr6502Ptr m_operand;
};

// Label expression
class LabelExpr6502 : public Expr6502
{
public:
	LabelExpr6502(uint16_t, const char *);
private:
	uint16_t m_addr;
	std::string_view m_name;
};

// Macro/proc call expression
class CallExpr6502 : public Expr6502
{
public:
	CallExpr6502(const char *, std::vector<Expr6502Ptr>);
private:
	std::string_view m_callee;
	std::vector<Expr6502Ptr> m_args;
};

#endif // _6502_AST_H
