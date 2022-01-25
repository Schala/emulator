#ifndef _CORE_DEVICES_H
#define _CORE_DEVICES_H

#include <cstdint>
#include <vector>

typedef std::vector<uint8_t>::iterator RAMIterator;

class Bus;

// Base class for bus-connected devices
class Device
{
public:
	// Allocate a new device, given an owning bus, and start and end addresses in RAM
	Device(Bus *, size_t, size_t);

	// Zero out the device's RAM occupation before disposal
	virtual ~Device();
protected:
	RAMIterator m_ramBeginIter;
	RAMIterator m_ramEndIter;
	Bus *m_bus; // Should be a non-owned pointer so we can cast to derivatives
};

// Provides read/write access between various devices and the RAM
class Bus
{
public:
	Bus(size_t);
	virtual ~Bus();

	// Device operations
	void Add(Device *);
	void Remove(Device *);

	// dump RAM to file
	//void DumpRAM(size_t);

	// Iterator to beginning of RAM, for assigning device RAM iterators
	inline RAMIterator GetRAMIterator();

	// load data into RAM at offset
	RAMIterator LoadIntoRAM(const std::vector<uint8_t> &, size_t);
protected:
	std::vector<uint8_t> m_ram;
	std::vector<Device *> m_devices; // non-owning
};

#endif // _CORE_DEVICES_H
