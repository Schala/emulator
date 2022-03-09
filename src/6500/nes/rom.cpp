#include <algorithm>
#include <bit>
#include <fmt/core.h>
#include <fstream>
#include <stdexcept>

#include "nes.h"
#include "mappers/nrom.h"

static constexpr std::array<char, 4> Magic = { 'N', 'E', 'S', 26 };

NESROM::NESROM(NES &nes, const std::filesystem::path &path):
	Device(nes.GetBus()),
	m_nes(nes)
{
	std::ifstream romFile(path, std::ios::binary);
	romFile.read(std::bit_cast<char *>(&m_header), 16);

	if (std::equal(m_header.magic.begin(), m_header.magic.end(), Magic.begin()))
	{
		m_prg.resize(m_header.prgPages * 16384);
		romFile.read(std::bit_cast<char *>(m_prg.data()), m_prg.size());

		m_chr.resize(m_header.chrPages * 8192);
		romFile.read(std::bit_cast<char *>(m_chr.data()), m_chr.size());

		m_mapperID = (m_header.mapperInfo.typeHi << 4) | m_header.mapperInfo.typeLo;

		switch (m_mapperID)
		{
			case 0:
				m_mapper.reset(new NROM(m_header.prgPages, m_header.chrPages));
				AddRange(32768, m_header.prgPages > 1 ? 65535 : 49151);
				Write(32768, m_prg);
				break;
			default:
				throw std::runtime_error(fmt::format("Unsupported mapper ID: {}", m_mapperID));
		}
	}

	GenerateHash();
	buses.push_back(nes.GetPPU()->GetBus());
	AddRange(0, 16383, 1);
	buses.back()->Add(this);
}

NESROM::~NESROM()
{
}

uint8_t NESROM::CPUReadByte(uint16_t addr)
{
	uint32_t mappedAddr = 0;

	if (m_mapper->CPUMapRead(addr, mappedAddr))
		return ReadByte(mappedAddr);
	return 0;
}

void NESROM::CPUWriteByte(uint16_t addr, uint8_t data)
{
	uint32_t mappedAddr = 0;

	if (m_mapper->CPUMapWrite(addr, mappedAddr))
		m_prg[mappedAddr] = data;
}

void NESROM::GenerateHash()
{
	m_hash = 0xDEADBEEF;

	for (auto b : m_prg)
		m_hash = ((m_hash << 1) | ((m_hash & 0x80000000) ? 1 : 0)) ^ b;

	for (auto b : m_chr)
		m_hash = ((m_hash << 1) | ((m_hash & 0x80000000) ? 1 : 0)) ^ b;
}

uint32_t NESROM::Hash() const
{
	return m_hash;
}

uint8_t NESROM::PPUReadByte(uint16_t addr)
{
	uint32_t mappedAddr = 0;

	if (m_mapper->PPUMapRead(addr, mappedAddr))
		return m_chr[mappedAddr];
	return 0;
}

void NESROM::PPUWriteByte(uint16_t addr, uint8_t data)
{
	uint32_t mappedAddr = 0;

	if (m_mapper->PPUMapWrite(addr, mappedAddr))
		m_chr[mappedAddr] = data;
}
