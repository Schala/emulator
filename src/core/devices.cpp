#include <algorithm>
#include <boost/endian/conversion.hpp>
#include <cstring>
#include <utility>

#include "devices.h"

using be = boost::endian;

// Device

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
Device<AddressType, DataType, EndianType>::Device(Device<AddressType, DataType, EndianType>::BusType *bus):
	bus(bus)
{
	bus->Add(this);
}

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
Device<AddressType, DataType, EndianType>::Device(Device<AddressType, DataType, EndianType>::BusType *bus,
		AddressType startAddr, AddressType endAddr):
	bus(bus)
{
	AddRange(startAddr, endAddr);
	bus->Add(this);
}

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
Device<AddressType, DataType, EndianType>::~Device()
{
	// zero out the occupied RAM
	for (auto &addrRange : addressMap)
		std::fill(addrRange.first, addrRange.second, 0);

	bus->Remove(this);
}

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
void Device<AddressType, DataType, EndianType>::AddRange(AddressType startAddr, AddressType endAddr)
{
	addressMap.push_back(std::make_pair(
		bus->GetRAMIterator() + startAddr,
		bus->GetRAMIterator() + (endAddr + 1)
	));
}

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
DataType Device<AddressType, DataType, EndianType>::Write(AddressType addr, DataType data)
{
	return bus->Write(addr, data);
}

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
AddressType Device<AddressType, DataType, EndianType>::WriteAddress(AddressType addr, AddressType outAddr)
{
	return bus->WriteAddress(addr, outAddr);
}

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
DataType Device<AddressType, DataType, EndianType>::Read(AddressType addr)
{
	return bus->Read(addr);
}

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
AddressType Device<AddressType, DataType, EndianType>::ReadAddress(AddressType addr)
{
	return bus->ReadAddress(addr);
}

// CPU

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
AddressType CPU<AddressType, DataType, EndianType>::FetchAddress()
{

}


// Bus

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
Bus<AddressType, DataType, EndianType>::Bus(AddressType ramSize):
	m_ram(std::vector<uint8_t>(ramSize, 0))
{
}

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
Bus<AddressType, DataType, EndianType>::~Bus()
{
}

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
void Bus<AddressType, DataType, EndianType>::Add(Device *dev)
{
	m_devices.push_back(dev);
}

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
void Bus<AddressType, DataType, EndianType>::Remove(Device *dev)
{
	auto it = std::find_if(m_devices.begin(), m_devices.end(),
		[&](Device *d) { return d == dev; });
	if (it != m_devices.end())
		m_devices.erase(it);
}

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
Bus<AddressType, DataType, EndianType>::RAMIterator Bus<AddressType, DataType, EndianType>::GetRAMIterator()
{
	return m_ram.begin();
}

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
Bus<AddressType, DataType, EndianType>::RAMIterator Bus<AddressType, DataType, EndianType>::LoadIntoRAM(
	const std::vector<uint8_t> &data, AddressType addr)
{
	return std::copy(data.begin(), data.end(), m_ram.begin() + addr);
}

/*
template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
void Bus<AddressType, DataType, EndianType>::DumpRAM(size_t iteration)
{
}*/

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
DataType Bus<AddressType, DataType, EndianType>::Read(AddressType addr) const
{
	DataType value = 0;

	std::memcpy(&value, &bus[addr], sizeof(DatType));

	return EndianType == std::endian::little ? value : be::native_to_big(value);
}

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
DataType Bus<AddressType, DataType, EndianType>::ReadAddress(AddressType addr) const
{
	AddressType retAddr = 0;

	std::memcpy(&retAddr, &bus[addr], sizeof(AddressType));

	return EndianType == std::endian::little ? retAddr : be::native_to_big(retAddr);
}

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
void Bus<AddressType, DataType, EndianType>::Write(AddressType addr, DataType data)
{
	if (EndianType != std::endian::native)
		be::endian_reverse_inplace(data);

	std::memcpy(&bus[addr], &data, sizeof(DataType));
}

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
void Bus<AddressType, DataType, EndianType>::WriteAddress(AddressType addr, AddressType outAddr)
{
	if (EndianType != std::endian::native)
		be::endian_reverse_inplace(outAddr);

	std::memcpy(&bus[addr], &outAddr, sizeof(AddressType));
}
