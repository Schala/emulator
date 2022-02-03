#include <algorithm>
#include <utility>

#include "devices.h"

Device::Device(Bus *bus):
	m_bus(bus)
{
	bus->Add(this);
}

Device::Device(Bus *bus, size_t startAddr, size_t endAddr):
	m_bus(bus)
{
	AddRange(startAddr, endAddr);
	bus->Add(this);
}

Device::~Device()
{
	// zero out the occupied RAM
	for (auto &addrRange : m_addrMap)
		std::fill(addrRange.first, addrRange.second, 0);

	m_bus->Remove(this);
}

void Device::AddRange(size_t startAddr, size_t endAddr)
{
	m_addrMap.push_back(std::make_pair(
		bus->GetRAMIterator() + startAddr,
		bus->GetRAMIterator() + (endAddr + 1)
	));
}

Bus::Bus(size_t ramSize):
	m_ram(std::vector<uint8_t>(ramSize, 0))
{
}

Bus::~Bus()
{
}

void Bus::Add(Device *dev)
{
	m_devices.push_back(dev);
}

void Bus::Remove(Device *dev)
{
	auto it = std::find_if(m_devices.begin(), m_devices.end(),
		[&](Device *d) { return d == dev; });
	if (it != m_devices.end())
		m_devices.erase(it);
}

RAMIterator Bus::GetRAMIterator()
{
	return m_ram.begin();
}

RAMIterator Bus::LoadIntoRAM(const std::vector<uint8_t> &data, size_t addr)
{
	return std::copy(data.begin(), data.end(), m_ram.begin() + addr);
}

/*void Bus::DumpRAM(size_t iteration)
{
}*/
