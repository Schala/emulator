#ifndef _EMU_UTILITY_H
#define _EMU_UTILITY_H

#include <cstdint>

constexpr uint16_t Hi16(uint16_t value)
{
	return value & 0xFF00;
}

#endif // _EMU_UTILITY_H
