#ifndef _CORE_LEXER_H
#define _CORE_LEXER_H

#include "../generic/scanner.h"

template <class TID, class TValue>
struct Token
{
	TID ID;
	size_t Line;
	size_t Column;
	TValue Value;
};

#endif // _CORE_LEXER_H
