#ifndef _CORE_LEXER_H
#define _CORE_LEXER_H

#include <cstddef>

template <class TID, class TValue>
struct Token
{
	TID ID;
	size_t Line;
	size_t Column;
	TValue Value;
};

template <class TID>
struct TokenAttr
{
	struct
	{
		bool
			RightAssociative : 1,
			Binary : 1,
			Unary : 1;
	} Flags;

	int Precedence;
	TID ID;
};

#endif // _CORE_LEXER_H
