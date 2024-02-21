#pragma once

/// An execution process
template <std::endian Endian, size_t PtrSize>
class TProcess<Endian, PtrSize> : std::enable_shared_from_this<TProcess<Endian, PtrSize>>
{
public:
	typedef TMemory<Endian> MemT;
	typedef std::weak_ptr<MemT> MemP;
	typedef TProcessor<Endian> ProcT;
	type std::shared_ptr<ProcT> ProcP;
	typedef TStack<Endian, PtrSize> StackT;

	TProcess(ProcP processor, MemP rom, size_t offset, size_t size, size_t resetVec,size_t interruptVec,
			 size_t stackOffset, size_t stackSize):
		m_owner(processor),
		m_rom(rom),
		m_lastAbsAddr(0),
		m_lastRelAddr(0),
		m_counter(resetVec),
		m_stackPtr(stackInit),
		m_reset(resetVec),
		m_int(interruptVec),
		m_stackOffs(stackOffset),
		m_cache(0)
	{
		m_stack(shared_from_this(), RangeT(m_data, stackOffset, stackOffset + stackSize));
	}


	ProcP Owner() const
	{
		return m_owner;
	}


	constexpr std::vector<uint8_t> Dump() const
	{
		return m_stack.Dump();
	}


	constexpr size_t StackOffset() const
	{
		return m_stackOffs;
	}


	constexpr size_t StackPointerOffset() const
	{
		return m_stack.Offset();
	}


	std::vector<uint8_t> StackRead(size_t size)
	{
		return m_stack.Read(size);
	}


	template <std::unsigned_integral T> T StackRead()
	{
		return m_stack.Read();
	}


	void StackWrite(std::vector<uint8_t> data)
	{
		m_stack.Write(data);
	}


	template <std::unsigned_integral T> void StackWrite(T value)
	{
		m_stack.Write(value);
	}


	constexpr size_t StackSize() const
	{
		return m_stack.Size();
	}



	// Read from last used address
	template <std::unsigned_integral T> T ReadFromLastAddress()
	{
		m_cache = m_owner->Read(m_owner->PrimaryBus(), m_lastAbsAddr);
		return m_cache;
	}


	// Read data from ROM
	template <std::unsigned_integral T> T ReadROM()
	{
		m_cache = m_owner->ReadROM(
	}


	virtual void Run()
	{
	}


	template <std::unsigned_integral T> virtual T Fetch() = 0;



	// Reset process state
	virtual void Reset() = 0;


	// Fetch an address, write to it, and return the address
	template <std::unsigned_integral T> size_t WriteToFetchedAddress(T data)
	{
	}


	// Write to last used address
	template <std::unsigned_integral T> void WriteToLastAddress(T data)
	{
	}
protected:
	ProcP m_owner;
	MemP m_rom;
	StackT m_stack;
	size_t m_lastAbsAddr;
	long m_lastRelAddr;
	size_t m_counter;
	size_t m_reset;
	size_t m_int;
	size_t m_stackOffs;
	size_t m_cache;
}
