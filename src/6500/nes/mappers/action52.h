#ifndef _NES_ACTION52_H
#define _NES_ACTION52_H

#include "../mapper.h"

// Mapper ID 228
class Action52 : public NESMapper
{
public:
	Action52(uint8_t, uint8_t);
	bool CPUMapRead(uint16_t, uint32_t &) override;
	bool CPUMapWrite(uint16_t, uint32_t &) override;
	bool PPUMapRead(uint16_t, uint32_t &) override;
	bool PPUMapWrite(uint16_t, uint32_t &) override;
};

#endif // _NES_ACTION52_H
