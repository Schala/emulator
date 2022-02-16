#include "action52.h"

Action52::Action52(uint8_t numPRGBanks, uint8_t numCHRBanks): NESMapper(numPRGBanks, numCHRBanks)
{
}

bool Action52::CPUMapRead(uint16_t addr, uint32_t &mappedAddr)
{
	if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		// Mirror the ROM read based on the ROM file offset, pending 16kb or 32kb
		mappedAddr = addr & (numPRGBanks > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}

	return false;
}

bool Action52::CPUMapWrite(uint16_t addr, uint32_t &mappedAddr)
{
	return false;
}

bool Action52::PPUMapRead(uint16_t addr, uint32_t &mappedAddr)
{
	if (addr >= 0 && addr <= 0x1FFF)
	{
		// no mapping at all
		mappedAddr = addr;
		return true;
	}

	return false;
}

bool Action52::PPUMapWrite(uint16_t, uint32_t &)
{
	// character rom is not written to
	return false;
}
