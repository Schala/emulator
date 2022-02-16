#include "../core/utility.h"
#include "bus_le16.h"

BusLE16::BusLE16(size_t ramSize): Bus(ramSize)
{
}

size_t BusLE16::ReadAddress(size_t addr) const
{
	return ReadByte(addr) | (ReadByte(addr + 1) << 8);
}

void BusLE16::WriteAddress(size_t addr, size_t vector)
{
	WriteByte(addr++, vector & 255);
	WriteByte(addr, Hi16(vector) >> 8);
}
