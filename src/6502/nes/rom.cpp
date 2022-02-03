#include <algorithm>
#include <bit>
#include <fmt/core.h>
#include <fstream>
#include <stdexcept>

#include "nrom.h"
#include "rom.h"

NESROM::NESROM(NES &nes, const std::filesystem::path &path):
	Device(nes.GetBus()),
	m_nes(nes)
{
	std::ifstream romFile(path, std::ios::binary);
	romFile.read(std::bit_cast<char *>(&m_header), HeaderSize);

	if (std::equal(m_header.magic.begin(), m_header.magic.end(), Magic.begin()))
	{
		m_prg.resize(m_header.prgPages * 16384);
		romFile.read(std::bit_cast<char *>(m_prg.data()), m_prg.size());

		m_chr.resize(m_header.chrPages * 8192);
		romFile.read(std::bit_cast<char *>(m_chr.data()), m_chr.size());

		m_mapperID = (m_header.mapperInfo.typeHi << 4) | m_header.mapperInfo.typeLo;

		switch (m_mapperID)
		{
			case 0: m_mapper = new NROM(m_header.prgPages, m_header.chrPages); break;
			default:
				throw std::runtime_error(fmt::format("Unsupported mapper ID: {}", m_mapperID));
		}
	}

	nes.GetPPU()->GetBus()->Add(this);
}

NESROM::~NESROM()
{
	m_nes.GetPPU()->GetBus()->Remove(this);
	if (m_mapper) delete m_mapper;
}

uint8_t NESROM::CPURead(uint16_t addr) const
{
	uint32_t mappedAddr = 0;

	if (m_mapper->CPUMapRead(addr, mappedAddr))
		return m_prg[mappedAddr];
	return 0;
}

void NESROM::CPUWrite(uint16_t addr, uint8_t data)
{
	uint32_t mappedAddr = 0;

	if (m_mapper->CPUMapWrite(addr, mappedAddr))
		m_prg[mappedAddr] = data;
}

uint8_t NESROM::PPURead(uint16_t addr) const
{
	uint32_t mappedAddr = 0;

	if (m_mapper->PPUMapRead(addr, mappedAddr))
		return m_chr[mappedAddr];
	return 0;
}

void NESROM::PPUWrite(uint16_t addr, uint8_t data)
{
	uint32_t mappedAddr = 0;

	if (m_mapper->PPUMapWrite(addr, mappedAddr))
		m_chr[mappedAddr] = data;
}
