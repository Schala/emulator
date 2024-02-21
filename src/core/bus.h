#pragma once


/// Provides read/write access between various devices and the RAM
template <std::endian Endian> class TBus<Endian>: std::enable_shared_from_this<TBus<Endian>>
{
public:
	typedef TDevice<Endian> DevT;
	typedef std::shared_ptr<DevT> DevP;
	typedef TMemory<Endian> MemT;
	typedef std::shared_ptr<MemT> MemP;
	typedef std::weak_ptr<MemT> MemWP;
	typedef std::ranges::subrange<PtrT> RangeT;

	TBus(size_t ramSize):
		m_ram(ramSize)
	{
	}


	bool RangeAvailable(size_t offset, size_t size)
	{
		for (auto &block : m_pool)
			if (offset >= block->Offset() && offset < block->Size() &&
				size >= block->Offset() && size < block->Size())
					return false;
		return true;
	}


	/// "Allocate" memory block
	MemWP Allocate(DevP dev, size_t offset, size_t size)
	{
		const std::lock_guard<std::mutex> lock(m_mtx);
		if (RangeAvailable(offset, size)) return;

		auto mem = std::make_shared(dev, RangeT(offset, offset + size), offset);
		m_pool.push_back(mem);

		MemWP wp = m_pool.back();
		m_map[dev] = wp;
		return wp;
	}


	/// "Deallocate" memory block via owning device
	void Deallocate(DevP dev)
	{
		for (auto &[dev, block] : m_map)

	}


	/// Add device to bus
	void Add(DevP dev)
	{
		const std::lock_guard<std::mutex> lock(m_mtx);
		m_devs.push_back(dev);
	}


	/// Remove device from bus
	void Remove(DevP dev)
	{
		const std::lock_guard<std::mutex> lock(m_mtx);

		auto it = std::find_if(m_devs.begin(), m_devs.end(),
			[&](DevT *d) { return d == dev; });
		if (it != m_devs.end())
			m_devs.erase(it);
	}

protected:
	std::vector<DevP> m_devs;
	std::vector<MemP> m_pool;
	std::vector<uint8_t> m_mem;
	std::mutex m_mtx;
	size_t m_ram;
};

typedef TBus<std::endian::big> TBusBE;
typedef std::shared_ptr<TBusBE> PBusBE;
typedef TBus<std::endian::little> TBusLE;
typedef std::shared_ptr<TBusLE> PBusLE;
