#include <iostream>
#include <map>
#include <utility>

#include "assembler.h"

static const std::map<Token6502, char> Precedence =
{
	{ Token6502::And, 5 },
	{ Token6502::BankByte, 1 },
	{ Token6502::BitAnd, 2 },
	{ Token6502::BitNot, 1 },
	{ Token6502::BitOr, 3 },
	{ Token6502::BitXOr, 2 },
	{ Token6502::Divide, 2 },
	{ Token6502::Equal, 4 },
	{ Token6502::GreaterEqual, 4 },
	{ Token6502::GreaterThan, 4 },
	{ Token6502::HiByte, 1 },
	{ Token6502::LessEqual, 4 },
	{ Token6502::LessThan, 4 },
	{ Token6502::LoByte, 1 },
	{ Token6502::Minus, 3 },
	{ Token6502::Mod, 2 },
	{ Token6502::Multiply, 2 },
	{ Token6502::Not, 7 },
	{ Token6502::NotEqual, 4 },
	{ Token6502::Or, 6 },
	{ Token6502::Plus, 3 },
	{ Token6502::ShL, 2 },
	{ Token6502::ShR, 2 },
	{ Token6502::XOr, 5 }
};

static uint8_t GetPrecedence(Token6502 token)
{
	if (!Precedence.contains(token)) return -1;
	return Precedence[token];
}

Assembler6502::Assembler6502():
	m_lastToken(Token6502::Invalid),
	m_lexer(Lexer6502(*this))
{
}

Assembler6502::~Assembler6502()
{
	if (m_src.is_open()) m_src.close();
}

bool Assembler6502::EndOfFile()
{
	return m_src.eof();
}

std::istream & Assembler6502::GetStream()
{
	return m_src;
}

void Assembler6502::OpenSourceFile(const std::filesystem::path &path)
{
	if (m_src.is_open()) m_src.close();
	m_src.open(path);
}

std::unique_ptr<Expr6502> Assembler6502::ParseBinaryExpr(uint8_t precedence, Expr6502Ptr lhs)
{
}

std::unique_ptr<Expr6502> Assembler6502::ParseIdentifierExpr()
{
	std::string_view name = m_lexer.LastIdentifier();
	m_lastToken =
}

std::unique_ptr<Expr6502> Assembler6502::ParseParenthesesExpr()
{
	m_lastToken = m_lexer.NextToken(); // eat (
	auto expr =
}

std::unique_ptr<Expr6502> Assembler6502::ParsePrimary()
{
	switch (m_lastToken)
	{
		case Token6502::Identifier: return ParseIdentif
	}
}

std::unique_ptr<Expr6502> Assembler6502::ParseStringExpr()
{
	auto result = std::make_unique<StringExpr6502>(m_lexer.LastIdentifier());
	return std::move(result);
}

std::unique_ptr<Expr6502> Assembler6502::ParseValueExpr()
{
	auto result = std::make_unique<ValueExpr6502>(m_lexer.LastValue());
	return std::move(result);
}

void Assembler6502::Reset()
{
	m_lastToken = Token6502::Invalid;
	m_src.seekg(0);
}
