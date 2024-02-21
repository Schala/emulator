#pragma once

#include "memory.h"
#include "device.h"
#include "bus.h"

typedef std::pair<size_t, std::string> TDisassembly;
typedef std::map<size_t, std::string> TDisassemblyMap;

template <std::endian Endian> class TProcessor<Endian> : public TDevice<Endian>
{
public:
	// Allocate a new device, given an initial owning bus, and one pair of start and end addresses in RAM
	TProcessor(BusP bus, size_t offset, size_t size):
		TDevice<Endian>(bus, offset, size)
	{
	}


	// Processor clock operation (execute one instruction)
	virtual void Clock() = 0;
};


#include "stack.h"
#include "process.h"
#include "cpu.h"
