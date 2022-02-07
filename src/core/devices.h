#ifndef _CORE_DEVICES_H
#define _CORE_DEVICES_H

#include <bit>
#include <concepts>
#include <string_view>
#include <utility>
#include <vector>

template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
class Bus;

// Base class for bus-connected devices
template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
class Device
{
public:
	typedef Bus<AddressType, DataType, EndianType> BusType;

	// Allocate a new device, given an owning bus
	Device(BusType *);

	// Allocate a new device, given an owning bus, and one pair of start and end addresses in RAM
	Device(BusType *, AddressType, AddressType);

	// Zero out the device's RAM occupation before disposal
	virtual ~Device();

	// Add an occupied address range in bus RAM
	void AddRange(AddressType, AddressType);

	// Read data from RAM address
	DataType Read(AddressType) const;

	// Read address from RAM address
	AddressType ReadAddress(AddressType) const;

	// Write data to RAM address
	void Write(AddressType, DataType);

	// Write address to RAM
	void WriteAddress(AddressType, AddressType);
protected:
	BusType *bus; // Should be a non-owned pointer so we can cast to derivatives
	std::vector<std::pair<BusType::RAMIterator, BusType::RAMIterator>> addressMap;
};

// Central processing unit
template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
class CPU : public Device<AddressType, DataType, EndianType>
{
public:
	// CPU clock operation (execute one instruction)
	virtual void Clock() = 0;

	// Disassemble from the specified address for the specified length
	virtual void Disassemble(AddressType, AddressType) = 0;

	// Fetch and cache a byte from the cached absolute address
	virtual DataType Fetch() = 0;

	// Read address from RAM
	AddressType FetchAddress();

	// Read data from last used address
	DataType ReadFromLastAddress() const;

	// Read byte from ROM
	virtual DataType ReadROM() = 0;

	// Read address from ROM
	AddressType ReadROMAddress();

	// Reset CPU state
	virtual void Reset() = 0;

	void SetResetVector(AddressType);

	// Read data from stack
	DataType StackRead();

	// Read address from stack
	AddressType StackReadAddress();

	// Write data to stack
	void StackWrite(DataType);

	// Write address to stack
	void StackWriteAddress(AddressType);

	// Fetch an address, write to it, and return the address
	AddressType WriteToFetchedAddress(DataType);

	// Write data to last used address
	void WriteToLastAddress(DataType);
protected:
	std::vector<std::pair<AddressType, std::string_view>> disassembly;
};

// Provides read/write access between various devices and the RAM
template <std::unsigned_integral AddressType, std::unsigned_integral DataType, std::endian EndianType>
class Bus
{
public:
	typedef Device<AddressType, DataType, EndianType> DeviceType;
	typedef std::vector<DataType>::iterator RAMIterator;

	Bus(AddressType);
	virtual ~Bus();

	// Device operations
	void Add(DeviceType *);
	void Remove(DeviceType *);

	// dump RAM to file
	//void DumpRAM(size_t);

	// Iterator to beginning of RAM, for assigning device RAM iterators
	inline RAMIterator GetRAMIterator();

	// load data into RAM at offset
	RAMIterator LoadIntoRAM(const std::vector<uint8_t> &, AddressType);

	// Read data from RAM address
	DataType Read(AddressType) const;

	// Read address from RAM address
	AddressType ReadAddress(AddressType) const;

	// Write data to RAM address
	void Write(AddressType, DataType);

	// Write address to RAM address
	void WriteAddress(AddressType, AddressType);
protected:
	std::vector<uint8_t> ram;
	std::vector<DeviceType *> devices; // non-owning
};

#endif // _CORE_DEVICES_H
