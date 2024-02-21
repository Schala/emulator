#pragma once

template <std::endian Endian> class TProcess<Endian>;

/// A stack frame for a process
template <std::endian Endian, size_t PtrSize> class TStack<Endian, PtrSize>
{
public:
	typedef std::vector<uint8_t>::iterator PtrT;
	typedef TProcess<Endian, PtrSize> ProcT;
	typedef std::weak_ptr<ProcT> ProcP;
	typedef std::ranges::subrange<PtrT> RangeT;

	/// Construct from a pointer and size
	TStack(ProcP owner, RangeT &range):
		m_owner(owner),
		m_frame(range),
		m_ptr(ResetPointer(range)),
		m_status(EStackStatus::Good)
	{
	}


	/// Zero out the stack before destruction
	virtual ~TStack()
	{
		std::fill(m_frame.begin(), m_frame.end(), 0);
	}


	constexpr size_t Offset() const
	{
		return std::ranges::distance(m_frame.cbegin(), m_ptr);
	}


	static constexpr PtrT ResetPointer(const RangeT &range)
	{
		return std::prev(range.end(), PtrSize);
	}


	/// Return whether or not a write operation will cause an overflow
	constexpr bool WillOverflow(size_t size) const
	{
		return std::ranges::distance(m_ptr, m_frame.cend()) > size;
	}


	/// Return whether or not a write operation will cause an underflow
	constexpr bool WillUnderflow(size_t size) const
	{
		return std::addressof(m_frame[0]) > (std::addressof(m_frame[Offset()]) - size);
	}


	constexpr size_t Size() const
	{
		return std::ranges::size(m_data);
	}


	ProcP Owner() const
	{
		return m_owner;
	}


	constexpr std::vector<uint8_t> Dump() const
	{
		std::vector<uint8_t> data(Size(), 0);
		std::copy(m_frame.cbegin(), m_frame.cend(), data.begin());
		return std::move(data);
	}


	constexpr EMemoryStatus Status() const
	{
		return m_status;
	}


	/// Read data
	std::vector<uint8_t> Read(size_t size)
	{
		if (m_status != EMemoryStatus::Good) return std::vector<uint8_t>();

		if (WillOverflow(size))
		{
			m_status = EMemoryStatus::Overflow;
			return std::vector<uint8_t>();
		}

		std::vector<uint8_t> data(size, 0);

		std::copy(m_ptr, std::next(m_ptr, size), data.begin());
		return std::move(data);
	}


	/// Read value
	template <std::unsigned_integral T> T Read() const
	{
		if (m_status != EMemoryStatus::Good) return 0;

		if (WillOverflow(size))
		{
			m_status = EMemoryStatus::Overflow;
			return 0;
		}

		if constexpr (sizeof(T) == 1)
			return *(m_ptr++);
		else
		{
			auto data = Read(sizeof(T));
			T result = 0;
			if constexpr (std::endian::native == Endian)
				result = std::accumulate(data.cbegin(), data.cend(), 0,
					[](T acc, uint8_t b)
					{
						return (acc << 8) | static_cast<T>(b);
					});
			else
				result = std::accumulate(data.crbegin(), data.crend(), 0,
					[](T acc, uint8_t b)
					{
						return (acc << 8) | static_cast<T>(b);
					});
			return result;
		}
	}


	/// Write data
	void Write(const std::vector<uint8_t> &data)
	{
		if (m_status != EMemoryStatus::Good) return;

		const std::lock_guard<std::mutex> lock(m_mtx);

		if (WillUnderflow(size))
		{
			m_status = EMemoryStatus::Underflow;
			return;
		}

		std::prev(m_ptr, data.size());
		std::copy(data.cbegin(), data.cend(), m_ptr);
	}


	/// Write value
	template <std::unsigned_integral T> void Write(T value)
	{
		if (m_status != EMemoryStatus::Good) return;

		const std::lock_guard<std::mutex> lock(m_mtx);

		if (WillUnderflow(size))
		{
			m_status = EMemoryStatus::Underflow;
			return;
		}

		if constexpr (sizeof(T) == 1)
			*(--m_ptr) = value;
		else
		{
			auto val = std::bit_cast<std::array<uint8_t, sizeof(T)>>(value);
			std::prev(m_ptr, sizeof(T));

			if constexpr (std::endian::native == Endian)
				std::copy(data.cbegin(), data.cend(), m_ptr);
			else
				std::copy(data.crbegin(), data.crend(), m_ptr);
		}
	}

private:
	ProcP m_owner;
	RangeT m_frame;
	PtrT m_ptr;
	std::mutex m_mtx;
	EMemoryStatus m_status;
};
