#pragma once

#include <map>

template <std::endian Endian> class TBus<Endian>;

/// Base class for bus-connected devices, given an endian and bit count
template <std::endian Endian> class TDevice<Endian>
{
public:
	typedef TMemory<Endian> MemT;
	typedef std::weak_ptr<MemT> MemP;
	typedef TBus<Endian> BusT;
	typedef std::weak_ptr<TBus> BusP;


	/// Allocate a new device, given an initial associating bus
	TDevice(BusP bus):
		m_primaryBus(bus)
	{
		bus->Add(this);
	}


	/// Allocate a new device, given an an initial associating bus and initial memory block
	TDevice(BusP bus, MemP block):
		m_primaryBus(bus)
	{
		bus->Add(this);
		m_map[bus] = block;
	}


	/// Zero out the device's RAM occupation before disposal
	virtual ~TDevice()
	{
		for (auto &[bus, mappings] : m_map)
		{
			for (auto &mapping : mappings)
				if (mapping.isOwner)
					std::fill(mapping->memory.begin(), mapping->memory.end(), 0);
			bus->Remove(this);
		}
	}


	BusP PrimaryBus() const
	{
		return m_primaryBus;
	}


	/// Add an accessible memory block for the device
	void AddMemoryBlock(BusP bus, MemP block)
	{
		m_map[bus] = block;
	}


	/// Removes a memory block for the device from the specified bus
	size_t RemoveMemoryBlock(BusP bus)
	{
		return m_map.erase(bus);
	}


	/// Read RAM range
	const std::vector<uint8_t> Read(BusP bus, size_t address, size_t size)
	{
		return m_map[bus]->Read(address, size);
	}


	/// Read value from RAM address
	template <std::unsigned_integral T> constexpr T Read(BusP bus, size_t address)
	{
		return m_map[bus]->Read(address);
	}


	void Write(BusP bus, size_t address, const std::vector<uint8_t> &data)
	{
		m_map[bus]->Write(address, data);
	}


	/// Write value to RAM
	template <std::unsigned_integral T> void Write(BusP bus, size_t address, T value)
	{
		m_map[bus]->Write(address, value);
	}
protected:
	std::map<BusP, MemP> m_map;
	BusP m_primaryBus;
};

typedef Device<std::endian::big> TDeviceBE;
typedef std::shared_ptr<TDeviceBE> PDeviceBE;
typedef Device<std::endian::little> TDeviceLE;
typedef std::shared_ptr<TDeviceLE> PDeviceLE;
