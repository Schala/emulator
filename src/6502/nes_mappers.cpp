#include "nes_mappers.h"

NESMapper::NESMapper(uint8_t numPRGBanks, uint8_t numCHRBanks):
	m_numPRGBanks(numPRGBanks),
	m_numCHRBanks(numCHRBanks)
{
}


// NROM

NROM::NROM(uint8_t numPRGBanks, uint8_t numCHRBanks): NESMapper(numPRGBanks, numCHRBanks)
{
}

bool NROM::CPUMapRead(uint16_t addr, uint32_t &mappedAddr)
{
	if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		// Mirror the ROM read based on the ROM file offset, pending 16kb or 32kb
		mappedAddr = addr & (m_numPRGBanks > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}

	return false;
}

bool NROM::CPUMapWrite(uint16_t addr, uint32_t &mappedAddr)
{
	return CPUMapRead(addr, mappedAddr);
}

bool NROM::PPUMapRead(uint16_t addr, uint32_t &mappedAddr)
{
	if (addr >= 0 && addr <= 0x1FFF)
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
