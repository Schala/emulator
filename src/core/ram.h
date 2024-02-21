#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <concepts>
#include <iterator>
#include <memory>
#include <mutex>
#include <numeric>
#include <ranges>
#include <utility>
#include <vector>

/* todo: add to read/write device funcs when mapping is fixed:
 * 	address %= addressMap[buses.at(busIndex)][mappingIndex].endAddress;*/


/// The status of a heap or stack
enum class EMemoryStatus
{
	Good,
	Overflow,
	Underflow, // stack only
};


template <std::endian Endian> class TRandomAccessMemory<Endian>
{
public:
	typedef std::vector<uint8_t>::iterator PtrT;
	typedef TDevice<Endian> DevT;
	typedef std::shared_ptr<DevT> DevP;
	typedef std::ranges::subrange<PtrT> RangeT;


	/// Construct from general RAM block
	TRandomAccessMemory(DevP owner, RangeT &range, size_t offset):
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


	/// Return whether or not a write operation will cause an overflow
	constexpr bool WillOverflow(size_t size) const
	{
		return size > (Offset() + Size());
	}


	constexpr bool ValidAddress(size_t address) const
	{
		return address >= Offset() && address < (Offset() + Size());
	}


	constexpr size_t AddressToOffset(size_t address) const
	{
		return address - Offset();
	}


	constexpr size_t OffsetToAddress(PtrT it) const
	{
		return std::ranges::distance(m_data.cbegin(), it) + Offset();
	}


	DevP Owner() const
	{
		return m_owner;
	}


	constexpr EMemoryStatus Status() const
	{
		return m_status;
	}


	constexpr std::vector<uint8_t> Dump() const
	{
		const std::lock_guard<std::mutex> lock(m_mtx); // for consistency

		std::vector<uint8_t> data(Size(), 0);
		std::copy(m_data.cbegin(), m_data.cend(), data.begin());
		return std::move(data);
	}


	/// Read data
	constexpr std::vector<uint8_t> Read(size_t address, size_t size) const
	{
		if (!ValidAddress(address) || m_status != EMemoryStatus::Good)
			return std::vector<uint8_t>();

		if (WillOverflow(size))
		{
			m_status = EMemoryStatus::Overflow;
			return std::vector<uint8_t>();
		}

		std::vector<uint8_t> data(size, 0);

		std::copy(std::next(m_data.cbegin(), AddressToOffset(address)),
			std::next(m_data.cbegin(), address + size), data.begin());
		return std::move(data);
	}


	/// Read value from RAM address
	template <std::unsigned_integral T> constexpr T Read(size_t address) const
	{
		if (!ValidAddress(address) || m_status != EMemoryStatus::Good)
			return 0;

		if (WillOverflow(size))
		{
			m_status = EMemoryStatus::Overflow;
			return 0;
		}

		if constexpr (sizeof(T) == 1)
			return m_data.at(address);
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


	/// Write data to address
	void Write(size_t address, const std::vector<uint8_t> &data)
	{
		if (!ValidAddress(address) || m_status != EMemoryStatus::Good)
			return;

		const std::lock_guard<std::mutex> lock(m_mtx);
		std::copy(data.cbegin(), data.cend(), std::next(m_data.begin(), address));
	}


	/// Write value to RAM address
	template <std::unsigned_integral T> void Write(size_t address, T value)
	{
		if (!ValidAddress(address) || m_status != EMemoryStatus::Good)
			return;

		const std::lock_guard<std::mutex> lock(m_mtx);

		if constexpr (sizeof(T) == 1)
			m_data[address] = value;
		else
		{
			auto val = std::bit_cast<std::array<uint8_t, sizeof(T)>>(value);

			if constexpr (std::endian::native == Endian)
				m_data.insert(std::next(m_data.begin(), address), val.cbegin(), val.cend());
			else
				m_data.insert(std::next(m_data.begin(), address), val.crbegin(), val.crend());
		}
	}

private:
	DevP m_owner;
	RangeT m_data;
	std::mutex m_mtx;
	size_t m_offs;
	EMemoryStatus m_status;
};
