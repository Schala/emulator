#include "fs304.h"

FS304::FS304(uint8_t numPRGBanks, uint8_t numCHRBanks): NESMapper(numPRGBanks, numCHRBanks)
{
}

bool FS304::CPUMapRead(uint16_t addr, uint32_t &mappedAddr)
{
	if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		// Mirror the ROM read based on the ROM file offset, pending 16kb or 32kb
		mappedAddr = addr & (numPRGBanks > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}

	return false;
}

bool FS304::CPUMapWrite(uint16_t addr, uint32_t &mappedAddr)
{
	return false;
}

bool FS304::PPUMapRead(uint16_t addr, uint32_t &mappedAddr)
{
	if (addr >= 0 && addr <= 0x1FFF)
	{
		// no mapping at all
		mappedAddr = addr;
		return true;
	}

	return false;
}

bool FS304::PPUMapWrite(uint16_t, uint32_t &)
{
	// character rom is not written to
	return false;
}
