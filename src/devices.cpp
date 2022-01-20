#include <algorithm>
#include <utility>

#include "devices.h"

Device::Device(Bus *bus, size_t startAddr, size_t endAddr):
	m_ramBeginIter(bus->GetRAMIterator() + startAddr),
	m_ramEndIter(bus->GetRAMIterator() + (endAddr + 1)),
	m_bus(bus)
{
	bus->Add(this);
}

Device::~Device()
{
	// zero out the occupied RAM
	std::fill(m_ramBeginIter, m_ramEndIter, 0);

	bus.Remove(this);
}

Bus::Bus(size_t ramSize):
	m_ram(std::vector<uint8_t>(0, ramSize))
{
}

Bus::~Bus()
{
	for (auto &d : m_devices)
		delete d;
}

void Bus::Add(Device *dev)
{
	m_devices.push_back(dev);
}

void Bus::Remove(Device *dev)
{
	auto it = std::find_if(m_devices.begin(), m_devices.end(), [](Device *d) { d == dev; });
		if (it != m_devices.end())
			m_devices.erase(it);
}

RAMIterator Bus::GetRAMIterator() const
{
	return m_ram.begin();
}

Bus::RAMIterator Bus::LoadIntoRAM(const std::vector<uint8_t> &data, size_t addr)
{
	return std::copy(data.begin(), data.end(), m_ram.begin() + addr);
}

/*void Bus::RAMDump(size_t iteration)
{
}*/
