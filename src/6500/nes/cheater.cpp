#include <bit>
#include <fstream>

#include "cheater.h"

static std::map<char, uint8_t> GameGenieChars =
{
	{ 'A', 0 },
	{ 'P', 1 },
	{ 'Z', 2 },
	{ 'L', 3 },
	{ 'G', 4 },
	{ 'I', 5 },
	{ 'T', 6 },
	{ 'Y', 7 },
	{ 'E', 8 },
	{ 'O', 9 },
	{ 'X', 10 },
	{ 'U', 11 },
	{ 'K', 12 },
	{ 'S', 13 },
	{ 'V', 14 },
	{ 'N', 15 }
};

NESCheater::NESCheater(BusLE16 *bus, NESROM *rom): Device(bus),
	m_rom(rom)
{
}

void NESCheater::AddCompare(uint16_t addr, uint8_t data, uint8_t cmp)
{
	m_compares[addr] = std::make_pair(data, cmp);
}

void NESCheater::AddGameGenieCode(const std::string &cheat)
{
	std::array<uint8_t, 8> c;

	for (int i = 0; i < cheat.size(); i++)
		c[i] = GameGenieChars[cheat.at(i)];

	uint16_t addr = 0x8000 + (((c[3] & 7) << 12) | ((c[5] & 7) << 8) |
			((c[4] & 8) << 8) | ((c[2] & 7) << 4) | ((c[1] & 8) << 4) |
			(c[4] & 7) | (c[3] & 8));

	if (cheat.size() == 6)
	{
		uint8_t data = ((c[1] & 7) << 4) | ((c[0] & 8) << 4) |
			(c[0] & 7) | (c[5] & 8);

		m_intercepts[addr] = data;
	}
	else
	{
		uint8_t data = ((c[1] & 7) << 4) | ((c[0] & 8) << 4) |
			(c[0] & 7) | (c[7] & 8);

		uint8_t compare = ((c[7] & 7) << 4) | ((c[6] & 8) << 4) |
			(c[6] & 7) | (c[5] & 8);

		m_compares[addr] = std::make_pair(data, compare);
	}
}

void NESCheater::AddIntercept(uint16_t addr, uint8_t data)
{
	m_intercepts[addr] = data;
}

void NESCheater::CPUWriteByte(uint16_t addr, uint8_t data)
{
	if (!m_intercepts.contains(addr))
		if (m_compares.contains(addr))
			if (m_compares[addr].second == data)
				m_rom->CPUWriteByte(addr, m_compares[addr].first);
			else
				m_rom->CPUWriteByte(addr, data);
}

void NESCheater::Save(const std::filesystem::path &path) const
{
	std::ofstream cheatFile(path, std::ios::binary);

	// this will be our file header
	uint32_t hash = m_rom->Hash();
	uint8_t intSize = m_intercepts.size();
	uint8_t cmpSize = m_compares.size();

	cheatFile.write(std::bit_cast<const char *>(&hash), sizeof(uint32_t));
	cheatFile.put(static_cast<char>(intSize));
	cheatFile.put(static_cast<char>(cmpSize));

	for (auto &[addr, data] : m_intercepts)
	{
		cheatFile.write(std::bit_cast<const char *>(&addr), sizeof(uint16_t));
		cheatFile.put(static_cast<char>(data));
	}

	for (auto &[addr, pair] : m_compares)
	{
		cheatFile.write(std::bit_cast<const char *>(&addr), sizeof(uint16_t));
		cheatFile.put(static_cast<char>(pair.first));
		cheatFile.put(static_cast<char>(pair.second));
	}
}

void NESCheater::UpdateInterceptRAM()
{
	for (auto [addr, data] : m_intercepts)
		WriteByte(addr, data);
}
