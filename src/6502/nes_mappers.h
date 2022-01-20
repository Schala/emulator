#ifndef _NES_MAPPERS_H
#define _NES_MAPPERS_H

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
private:
	uint8_t m_numPRGBanks;
	uint8_t m_numCHRBanks;
};

// Mapper ID 0
class NROM : public NESMapper
{
public:
	NROM(uint8_t, uint8_t);
	bool CPUMapRead(uint16_t, uint32_t &) override;
	bool CPUMapWrite(uint16_t, uint32_t &) override;
	bool PPUMapRead(uint16_t, uint32_t &) override;
	bool PPUMapWrite(uint16_t, uint32_t &) override;
};

#endif // _NES_MAPPERS_H
