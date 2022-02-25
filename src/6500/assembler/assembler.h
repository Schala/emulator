#ifndef _6500_ASSEMBLER_H
#define _6500_ASSEMBLER_H

#include <list>
#include <map>
#include <vector>

#include "lexer.h"

class Assembler6500
{
public:
	Assembler6500(const char *);
	~Assembler6500();
	//void CodeGen();
	void Parse();
	std::list<Token6500> m_ast;
private:
	Lexer6500 m_lex;
	std::vector<uint8_t> m_gen;
	std::map<std::string, uint16_t> m_syms;

	void Expression(Token6500);
	void Statement(Token6500);
};

#endif // _6500_ASSEMBLER_H
