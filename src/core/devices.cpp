#include <algorithm>
#include <cstring>
#include <utility>

#include "devices.h"

// Device

template <std::endian Endian, size_t Bits>
Device<Endian, Bits>::Device(Bus *bus)
{
	bus->Add(this);
	m_buses.push_back(bus);
}


Device::Device(Bus *bus, size_t startAddr, size_t endAddr)
{
	buses.push_back(bus);
	AddRange(startAddr, endAddr, 0);
	buses.back()->Add(this);
}

Device::~Device()
{
	// zero out the occupied RAM, if owned
	for (auto &[bus, mappings] : addressMap)
	{
		for (auto &mapping : mappings)
			if (mapping.isOwner)
				std::fill(bus->GetRAMIterator() + mapping.startAddress,
					bus->GetRAMIterator() + mapping.endAddress, 0);
		bus->Remove(this);
	}
}

void Device::AddRange(size_t startAddr, size_t endAddr, size_t busIndex, bool owner)
{
	addressMap[buses[busIndex]].push_back({ owner, startAddr, endAddr + 1 });
}

std::vector<uint8_t> Device::Read(size_t addr, size_t size, size_t busIndex, size_t mappingIndex)
{
	//addr %= addressMap[buses[busIndex]][mappingIndex].endAddress;
	return buses[busIndex]->Read(addr, size);
}

size_t Device::ReadAddress(size_t addr, size_t busIndex, size_t mappingIndex)
{
	//addr %= addressMap[buses[busIndex]][mappingIndex].endAddress;
	return buses[busIndex]->ReadAddress(addr);
}

uint8_t Device::ReadByte(size_t addr, size_t busIndex, size_t mappingIndex)
{
	//addr %= addressMap[buses[busIndex]][mappingIndex].endAddress;
	return buses[busIndex]->ReadByte(addr);
}

uint32_t Device::ReadDWord(size_t addr, size_t busIndex, size_t mappingIndex)
{
	//addr %= addressMap[buses[busIndex]][mappingIndex].endAddress;
	return buses[busIndex]->ReadDWord(addr);
}

uint16_t Device::ReadWord(size_t addr, size_t busIndex, size_t mappingIndex)
{
	//addr %= addressMap[buses[busIndex]][mappingIndex].endAddress;
	return buses[busIndex]->ReadWord(addr);
}

void Device::Write(size_t addr, const std::vector<uint8_t> &data, size_t busIndex, size_t mappingIndex)
{
	//addr %= addressMap[buses[busIndex]][mappingIndex].endAddress;
	return buses[busIndex]->Write(addr, data);
}

void Device::WriteAddress(size_t addr, size_t vector, size_t busIndex, size_t mappingIndex)
{
	//addr %= addressMap[buses[busIndex]][mappingIndex].endAddress;
	buses[busIndex]->WriteAddress(addr, vector);
}

void Device::WriteByte(size_t addr, uint8_t data, size_t busIndex, size_t mappingIndex)
{
	//addr %= addressMap[buses[busIndex]][mappingIndex].endAddress;
	buses[busIndex]->WriteByte(addr, data);
}

void Device::WriteDWord(size_t addr, uint32_t data, size_t busIndex, size_t mappingIndex)
{
	//addr %= addressMap[buses[busIndex]][mappingIndex].endAddress;
	buses[busIndex]->WriteDWord(addr, data);
}

void Device::WriteWord(size_t addr, uint16_t data, size_t busIndex, size_t mappingIndex)
{
	//addr %= addressMap[buses[busIndex]][mappingIndex].endAddress;
	buses[busIndex]->WriteWord(addr, data);
}


// Processor

Processor::Processor(Bus *bus): Device(bus)
{
}

Processor::Processor(Bus *bus, size_t startAddr, size_t endAddr): Device(bus, startAddr, endAddr)
{
}


// CPU

CPU::CPU(Bus *bus, size_t startAddr, size_t endAddr, size_t resetVector,
		size_t stackBase, size_t stackInit):
	Processor(bus, startAddr, endAddr),
	lastAbsAddress(resetVector),
	lastRelAddress(0),
	counter(0),
	stackBase(stackBase),
	stackInit(stackInit),
	stackPtr(stackInit),
	resetVector(resetVector)
{
}

size_t CPU::ReadAddressFromLastAddress()
{
	return ReadAddress(lastAbsAddress);
}

uint8_t CPU::ReadByteFromLastAddress()
{
	return ReadByte(lastAbsAddress);
}

uint8_t CPU::ReadROMByte()
{
	return ReadByte(counter++);
}

uint32_t CPU::ReadROMDWord()
{
	uint32_t value = ReadDWord(counter);
	counter += 4;
	return value;
}

uint16_t CPU::ReadROMWord()
{
	uint16_t value = ReadWord(counter);
	counter += 2;
	return value;
}

uint8_t CPU::StackReadByte()
{
	return ReadByte(stackBase + ++stackPtr);
}

void CPU::StackWriteByte(uint8_t data)
{
	WriteByte(stackBase + stackPtr--, data);
}

size_t CPU::WriteByteToFetchedAddress(uint8_t data)
{
	size_t addr = FetchAddress();
	WriteByte(addr, data);
	return addr;
}

void CPU::WriteByteToLastAddress(uint8_t data)
{
	WriteByte(lastAbsAddress, data);
}


// Bus

Bus::Bus(size_t ramSize):
	ram(std::vector<uint8_t>(ramSize, 0))
{
}

Bus::~Bus()
{
}

void Bus::Add(Device *dev)
{
	devices.push_back(dev);
}

void Bus::Remove(Device *dev)
{
	auto it = std::find_if(devices.begin(), devices.end(),
		[&](Device *d) { return d == dev; });
	if (it != devices.end())
		devices.erase(it);
}

RAMIterator Bus::GetRAMIterator()
{
	return ram.begin();
}

/*

void Bus::DumpRAM(size_t iteration)
{
}*/

std::vector<uint8_t> Bus::Read(size_t addr, size_t size) const
{
	std::vector<uint8_t> data(size, 0);

	std::copy(ram.begin() + addr, ram.begin() + addr + size, data.begin());

	return std::move(data);
}

uint8_t Bus::ReadByte(size_t addr) const
{
	return ram.at(addr);
}

uint32_t Bus::ReadDWord(size_t addr) const
{
	return ram.at(addr);
}

uint16_t Bus::ReadWord(size_t addr) const
{
	return ram.at(addr);
}

void Bus::Write(size_t addr, const std::vector<uint8_t> &data)
{
	std::copy(data.begin(), data.end(), ram.begin() + addr);
}

void Bus::WriteByte(size_t addr, uint8_t data)
{
	ram[addr] = data;
}

void Bus::WriteDWord(size_t addr, uint32_t data)
{
	WriteByte(addr, data & 255);
}

void Bus::WriteWord(size_t addr, uint16_t data)
{
	WriteByte(addr, data & 255);
}
