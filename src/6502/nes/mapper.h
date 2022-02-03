#ifndef _NES_MAPPER_H
#define _NES_MAPPER_H

#include <cstdint>

// Base class for mappers
class NESMapper
{
public:
	NESMapper(uint8_t, uint8_t);
	virtual bool CPUMapRead(uint16_t, uint32_t &) = 0;
	virtual bool CPUMapWrite(uint16_t, uint32_t &) = 0;
	virtual bool PPUMapRead(uint16_t, uint32_t &) = 0;
	virtual bool PPUMapWrite(uint16_t, uint32_t &) = 0;
protected:
	uint8_t m_numPRGBanks;
	uint8_t m_numCHRBanks;
};

#endif // _NES_MAPPERS_H
