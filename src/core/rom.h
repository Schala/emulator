#pragma once

/// An immutable region of memory
template <std::endian Endian> class TReadOnlyMemory<Endian>
{
public:
	typedef TDevice<Endian> DevT;
	typedef std::shared_ptr<DevT> DevP;
	typedef std::vector<uint8_t>::iterator PtrT;
	typedef std::ranges::subrange<PtrT> RangeT;


	/// Construct from general RAM block
	TReadOnlyMemory(DevP owner, RangeT &range, size_t offset):
		m_data(range),
		m_owner(owner),
		m_offs(offset)
	{
	}


	constexpr size_t Offset() const
	{
		return m_offs;
	}


	constexpr size_t Size() const
	{
		return std::ranges::size(m_data);
	}


	/// Return whether or not a read operation will cause an overflow
	constexpr bool WillOverflow(size_t size) const
	{
		return size > (Offset() + Size());
	}


	DevP Owner() const
	{
		return m_owner;
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
	template <std::unsigned_integral T> constexpr T Read() const
	{
		if (m_status != EMemoryStatus::Good) return 0;

		if (WillOverflow(sizeof(T)))
		{
			m_status = EMemoryStatus::Overflow;
			return 0;
		}

		if constexpr (sizeof(T) == 1)
			return *(m_ptr++)
		else
		{
			auto data = Read(address, sizeof(T));
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

protected:
	DevP m_owner;
	RangeT m_data;
	PtrT m_ptr;
	size_t m_offs;
	EMemoryStatus m_status;
};
