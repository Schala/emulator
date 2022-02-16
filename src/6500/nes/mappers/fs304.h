#ifndef _NES_FS304_H
#define _NES_FS304_H

#include "../mapper.h"

// Mapper ID 162
class FS304 : public NESMapper
{
public:
	FS304(uint8_t, uint8_t);
	bool CPUMapRead(uint16_t, uint32_t &) override;
	bool CPUMapWrite(uint16_t, uint32_t &) override;
	bool PPUMapRead(uint16_t, uint32_t &) override;
	bool PPUMapWrite(uint16_t, uint32_t &) override;
};

#endif // _NES_FS304_H
