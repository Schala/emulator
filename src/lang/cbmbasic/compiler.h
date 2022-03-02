#ifndef _CBMBASIC_COMPILER_H
#define _CBMBASIC_COMPILER_H

#include <cstdint>
#include <list>
#include <map>
#include <ostream>
#include <vector>

#include "lexer.h"

class CBMBASICCompiler
{
public:
	CBMBASICCompiler(const char *);

private:
	CBMBASICLexer m_lex;
	std::list<CBMBASICToken> m_ast;
	std::vector<uint8_t> m_gen;
	std::map<std::string, CBMBASICTokenValue> m_vars;
};

#endif // _CBMBASIC_COMPILER_H
