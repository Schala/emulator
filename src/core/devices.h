#ifndef _CORE_DEVICES_H
#define _CORE_DEVICES_H

#include <map>
#include <string_view>

class Bus;

typedef std::vector<uint8_t>::iterator RAMIterator;

struct AddressMapping
{
	bool isOwner;
	size_t startAddress;
	size_t endAddress;
};

// Base class for bus-connected devices
class Device
{
public:
	// Allocate a new device, given an initial owning bus
	Device(Bus *);

	// Allocate a new device, given an initial owning bus, and one pair of start and end addresses in RAM
	Device(Bus *, size_t, size_t);

	// Zero out the device's RAM occupation before disposal
	virtual ~Device();

	// Add an occupied address range in bus RAM
	void AddRange(size_t startAddr, size_t endAddr, size_t busIndex = 0, bool owner = true);

	// Read RAM range
	std::vector<uint8_t> Read(size_t addr, size_t size, size_t busIndex = 0) const;

	// Read address from RAM address
	size_t ReadAddress(size_t addr, size_t busIndex = 0) const;

	// Read byte from RAM address
	uint8_t ReadByte(size_t addr, size_t busIndex = 0) const;

	uint32_t ReadDWord(size_t addr, size_t busIndex = 0) const;

	uint16_t ReadWord(size_t addr, size_t busIndex = 0) const;

	void Write(size_t addr, const std::vector<uint8_t> &data, size_t busIndex = 0);

	// Write address to RAM
	void WriteAddress(size_t addr, size_t vector, size_t busIndex = 0);

	// Write data to RAM address
	void WriteByte(size_t addr, uint8_t data, size_t busIndex = 0);

	void WriteDWord(size_t addr, uint32_t data, size_t busIndex = 0);

	void WriteWord(size_t addr, uint16_t data, size_t busIndex = 0);
protected:
	std::vector<Bus *> buses; // Should be non-owned pointers so we can cast to derivatives
	std::map<Bus *, std::vector<AddressMapping>> addressMap;
};

class Processor : public Device
{
public:
	// CPU clock operation (execute one instruction)
	virtual void Clock() = 0;
};

// Central processing unit
class CPU : public Processor
{
public:
	CPU(Bus *, size_t, size_t, size_t, size_t, size_t);

	// Disassemble from the specified address for the specified length
	virtual void Disassemble(size_t, size_t) = 0;

	// Read address from RAM
	virtual size_t FetchAddress() = 0;

	// Fetch and cache a byte from the cached absolute address
	virtual uint8_t FetchByte() = 0;

	// Read byte from last used address
	uint8_t ReadByteFromLastAddress() const;

	// Read address from ROM
	virtual size_t ReadROMAddress() = 0;

	// Read byte from ROM
	uint8_t ReadROMByte();

	uint32_t ReadROMDWord();

	uint16_t ReadROMWord();

	// Reset CPU state
	virtual void Reset() = 0;

	// Read address from stack
	virtual size_t StackReadAddress() = 0;

	// Read data from stack
	virtual uint8_t StackReadByte() = 0;

	// Write address to stack
	virtual void StackWriteAddress(size_t) = 0;

	// Write data to stack
	virtual void StackWriteByte(uint8_t) = 0;

	// Fetch an address, write to it, and return the address
	size_t WriteByteToFetchedAddress(uint8_t);

	// Write byte to last used address
	void WriteByteToLastAddress(uint8_t);
protected:
	size_t lastAbsAddress;
	size_t lastRelAddress;
	size_t counter;
	size_t stackBase;
	size_t stackInit;
	size_t stackPtr;
	size_t resetVector;
	std::vector<std::pair<size_t, std::string_view>> disassembly;
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

	// Read RAM range
	std::vector<uint8_t> Read(size_t, size_t) const;

	// Read address from RAM address
	virtual size_t ReadAddress(size_t) const = 0;

	// Read byte from RAM address
	uint8_t ReadByte(size_t) const;

	// Read double word from address
	virtual uint32_t ReadDWord(size_t) const;

	// Read word from address
	virtual uint16_t ReadWord(size_t) const;

	void Write(size_t, const std::vector<uint8_t> &);

	// Write address to RAM address
	virtual void WriteAddress(size_t, size_t) = 0;

	// Write data to RAM address
	void WriteByte(size_t, uint8_t);

	virtual void WriteDWord(size_t, uint32_t);

	virtual void WriteWord(size_t, uint16_t);
protected:
	std::vector<uint8_t> ram;
	std::vector<Device *> devices; // non-owning
};

#endif // _CORE_DEVICES_H
