#ifndef _CORE_UTILITY_H
#define _CORE_UTILITY_H

#include <cstdint>

constexpr uint16_t Hi16(uint16_t value)
{
	return value & 0xFF00;
}

constexpr bool IsBinary(char c)
{
	if (c == '0' || c == '1')
		return true;
	return false;
}

constexpr bool IsNewLine(char c)
{
	switch (c)
	{
		case '\n':
		case '\r':
			return true;
		default:
			return false;
	}
}

#endif // _CORE_UTILITY_H
