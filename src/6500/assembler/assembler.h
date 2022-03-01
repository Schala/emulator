#ifndef _6500_ASSEMBLER_H
#define _6500_ASSEMBLER_H

#include <list>
#include <map>
#include <ostream>
#include <vector>

#include "lexer.h"

class Assembler6500
{
public:
	Assembler6500(const char *);
	~Assembler6500();
	//void CodeGen(std::ostream &);
	void Parse();
	void ResolveSymbols();

	std::list<Token6500> m_ast;
	std::map<std::string, uint16_t> m_syms;
private:
	Lexer6500 m_lex;
	std::vector<uint8_t> m_gen;

	void EmitByte(uint8_t);
	void EmitWord(uint16_t);
	void Expression(Token6500);
	uint16_t FindLabelOffset(const std::string &) const;
	void Statement(Token6500);
};

#endif // _6500_ASSEMBLER_H
