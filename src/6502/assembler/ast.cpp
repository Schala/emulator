#include <utility>

#include "ast.h"

AddressExpr6502::AddressExpr6502(uint16_t addr):
	m_addr(addr)
{
}

BinaryExpr6502::BinaryExpr6502(Token6502 op, Expr6502Ptr lhs, Expr6502Ptr rhs):
	m_op(op),
	m_lhs(lhs),
	m_rhs(rhs)
{
}

CallExpr6502::CallExpr6502(const char *callee, std::vector<Expr6502Ptr> args):
	m_callee(callee),
	m_args(std::move(args))
{
}

CommandExpr6502::CommandExpr6502(uint16_t addr, Token6502 cmd, Expr6502Ptr operand):
	m_addr(addr),
	m_cmd(cmd),
	m_operand(operand)
{
}

Expr6502::~Expr6502()
{
}

LabelExpr6502::LabelExpr6502(uint16_t addr, const char *name):
	m_addr(addr),
	m_name(name)
{
}

OperationExpr6502::OperationExpr6502(uint16_t addr, const Operation6502 &op, Expr6502Ptr operand):
	m_addr(addr),
	m_op(op),
	m_operand(operand)
{
}

StringExpr6502::StringExpr6502(const char *s):
	m_str(s)
{
}

ValueExpr6502::ValueExpr6502(uint8_t value):
	m_val(value)
{
}

VariableExpr6502::VariableExpr6502(const char *name):
	m_name(name)
{
}
