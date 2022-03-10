#include "nrom.h"

NROM::NROM(uint8_t numPRGBanks, uint8_t numCHRBanks): NESMapper(numPRGBanks, numCHRBanks)
{
}

bool NROM::CPUMapRead(uint16_t addr, uint32_t &mappedAddr)
{
	if (addr >= 32768 && addr <= 65535)
	{
		// Mirror the ROM read based on the ROM file offset, pending 16kb or 32kb
		mappedAddr = addr & (numPRGBanks > 1 ? 32767 : 16383);
		return true;
	}

	return false;
}

bool NROM::CPUMapWrite(uint16_t addr, uint32_t &mappedAddr)
{
	return false;
}

bool NROM::PPUMapRead(uint16_t addr, uint32_t &mappedAddr)
{
	if (addr >= 0 && addr <= 16383)
	{
		// no mapping at all
		mappedAddr = addr;
		return true;
	}

	return false;
}

bool NROM::PPUMapWrite(uint16_t, uint32_t &)
{
	// character rom is not written to
	return false;
}
