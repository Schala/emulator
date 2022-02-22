#ifndef _6502_ASSEMBLER_H
#define _6502_ASSEMBLER_H

#include <filesystem>
#include <fstream>
#include <istream>

#include "ast.h"

class Assembler6502
{
public:
	Assembler6502();
	~Assembler6502();
	bool EndOfFile();
	std::istream & GetStream();
	void OpenSourceFile(const std::filesystem::path &);

	// binary_expr ::= lhs op lhs
	std::unique_ptr<Expr6502> ParseBinaryExpr(uint8_t, Expr6502Ptr);
	std::unique_ptr<Expr6502> ParseCommandExpr();
	std::unique_ptr<Expr6502> ParseIdentifierExpr();
	std::unique_ptr<Expr6502> ParseOperationExpr();
	std::unique_ptr<Expr6502> ParseParenthesesExpr();
	std::unique_ptr<Expr6502> ParsePrimary();
	std::unique_ptr<Expr6502> ParseStringExpr();
	std::unique_ptr<Expr6502> ParseValueExpr();
	void Reset();
private:
	uint16_t m_lastBaseOffset;
	uint16_t m_lastOffset;
	Token6502 m_lastToken;
	Lexer6502 m_lexer;
	std::ifstream m_src;
};

#endif // _6502_ASSEMBLER_H
