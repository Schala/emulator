#ifndef _EMU_UTILITY_H
#define _EMU_UTILITY_H

#include <cctype>
#include <cstdint>
#include <string>

constexpr uint16_t Hi16(uint16_t value)
{
	return value & 0xFF00;
}

bool IsBlankLine(std::string &s)
{
	bool blank = true;

	for (char c : s)
		if (!std::isspace(c))
		{
			blank = false;
			break;
		}

	return blank;
}

#endif // _EMU_UTILITY_H
