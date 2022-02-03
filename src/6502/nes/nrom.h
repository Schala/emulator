#ifndef _NES_NROM_H
#define _NES_NROM_H

#include "mapper.h"

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

#endif // _NES_NROM_H
