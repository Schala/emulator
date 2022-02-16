#ifndef _BUS_LE16_H
#define _BUS_LE16_H

#include "../core/devices.h"

class BusLE16 : public Bus
{
public:
	BusLE16(size_t);

	// Read address from RAM address
	size_t ReadAddress(size_t) const override;

	// Write address to RAM address
	void WriteAddress(size_t, size_t) override;
};

#endif // _BUS_LE16_H
