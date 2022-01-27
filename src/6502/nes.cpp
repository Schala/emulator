#include <algorithm>
#include <fmt/core.h>
#include <fstream>
#include <stdexcept>

#include "nes.h"

static constexpr SDL_Color ColorFromU32(uint32_t u)
{
	return
	{
		static_cast<uint8_t>((u & 0xFF000000) >> 24),
		static_cast<uint8_t>((u & 0xFF0000) >> 16),
		static_cast<uint8_t>((u & 0xFF00) >> 8),
		static_cast<uint8_t>(u & 255)
	};
}

// PPU2C02

const std::array<uint32_t, PPU2C02::PaletteSize> PPU2C02::Palette =
{
	0x545454FF,
	0x1E74FF,
	0x81090FF,
	0x300088FF,
	0x440064FF,
	0x5C0030FF,
	0x540400FF,
	0x3C1800FF,
	0x202A00FF,
	0x83A00FF,
	0x4000FF,
	0x3C00FF,
	0x323CFF,
	255,
	255,
	255,

	0x989698FF,
	0x84CC4FF,
	0x3032ECFF,
	0x5C1EE4FF,
	0x8814B0FF,
	0xA01464FF,
	0x982220FF,
	0x783C00FF,
	0x545A00FF,
	0x287200FF,
	0x87C00FF,
	0x7628FF,
	0x6678FF,
	255,
	255,
	255,

	0xECEEECFF,
	0x4C9AECFF,
	0x787CECFF,
	0xB062ECFF,
	0xE454ECFF,
	0xEC58B4FF,
	0xEC6A64FF,
	0xD48820FF,
	0xA0AA00FF,
	0x74C400FF,
	0x4CD020FF,
	0x38CC6CFF,
	0x38B4CCFF,
	0x383838FF,
	255,
	255,

	0xECEEECFF,
	0xA8CCECFF,
	0xBCBCECFF,
	0xD4B2ECFF,
	0xECAEECFF,
	0xECAED4FF,
	0xECB4B0FF,
	0xE4C490FF,
	0xCCD278FF,
	0xB4DE78FF,
	0xA8E290FF,
	0x98E2B4FF,
	0xA0D6E4FF,
	0xA0A2A0FF,
	255,
	255
};

PPU2C02::PPU2C02(NES &nes, SDL_Renderer *renderer):
	Device(nes.GetBus(), 0, 0x3FFF),
	m_scanline(0),
	m_cycle(0),
	m_renderer(renderer),
	m_nes(nes),
	m_ppuBus(Bus6502(0x4000)),
	m_nameTbl(
	{
		Sprite(32, 32),
		Sprite(32, 32)
	}),
	m_patTbl(
	{
		Sprite(64, 64),
		Sprite(64, 64)
	}),
	m_ramPalette(Sprite(16, 4))
{
	m_flags.frameDone = false;

	// Give the renderer a solid fill colour instead of copying what's underneath
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	m_ppuBus.Add(this);
}

PPU2C02::~PPU2C02()
{
}

void PPU2C02::Clock()
{
	if (++m_cycle >= 341)
	{
		m_cycle = 0;

		if (++m_scanline >= 261)
		{
			m_scanline = -1;
			m_flags.frameDone = true;
		}
	}
}

uint8_t PPU2C02::CPURead(uint16_t addr) const
{
	uint8_t data = 0;

	switch (addr)
	{
		case 0: break; // control
		case 1: break; // mask
		case 2: break; // status
		case 3: break; // OAM address
		case 4: break; // OAM data
		case 5: break; // scroll
		case 6: break; // PPU address
		case 7: break; // PPU data
	}

	return data;
}

void PPU2C02::CPUWrite(uint16_t addr, uint8_t data)
{
	switch (addr)
	{
		case 0: break; // control
		case 1: break; // mask
		case 2: break; // status
		case 3: break; // OAM address
		case 4: break; // OAM data
		case 5: break; // scroll
		case 6: break; // PPU address
		case 7: break; // PPU data
	}
}

Bus6502 * PPU2C02::GetBus()
{
	return &m_ppuBus;
}

bool PPU2C02::IsFrameDone() const
{
	return m_flags.frameDone;
}

void PPU2C02::NextFrame()
{
	m_flags.frameDone = false;
}

/*void PPU2C02::NoiseTest()
{
	SDL_Color c = ColorFromU32(Palette[m_rng(m_mt) ? 63 : 48]);
	SDL_SetRenderDrawColor(m_renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawPoint(m_renderer, m_cycle - 1, m_scanline);
}*/

uint8_t PPU2C02::Read(uint16_t addr) const
{
	addr &= 0x3FFF;

	if (addr >= 0 && addr <= 0x1FFF) // pattern
	else if (addr >= 0x2000 && addr <= 0x3EFF) // nametable
	else // palette?
}

SDL_Color & PPU2C02::ReadRAMPaletteColor(uint8_t palette, uint8_t pixel) const
{
	return m_ramPalette[Read(0x3F00 + (palette << 2) + pixel)];
}

void PPU2C02::Write(uint16_t addr, uint8_t data)
{
	addr &= 0x3FFF;

	if (addr >= 0 && addr <= 0x1FFF) // pattern
	else if (addr >= 0x2000 && addr <= 0x3EFF) // nametable
	else // palette?
}


// NESROM

NESROM::NESROM(NES &nes, const std::filesystem::path &path):
	Device(nes.GetBus(), 0, 0),
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


// NES

NES::NES(SDL_Renderer *renderer):
	m_cycles(0),
	m_rom(nullptr),
	m_bus(Bus6502(0xFFFF)),
	m_cpu(MOS6502(&m_bus, 0, 0x1FFF)),
	m_ppu(PPU2C02(*this, renderer))
{
}

NES::~NES()
{
	if (m_rom) delete m_rom;
}

void NES::Clock()
{
	m_ppu.Clock();

	// CPU runs 3 times slower than PPU
	if (m_cycles++ % 3 == 0)
		m_cpu.Clock();
}

Bus6502 * NES::GetBus()
{
	return &m_bus;
}

PPU2C02 * NES::GetPPU()
{
	return &m_ppu;
}

void NES::LoadROM(const std::filesystem::path &path)
{
	if (m_rom) delete m_rom;
	m_rom = new NESROM(*this, path);
}

uint8_t NES::Read(uint16_t addr) const
{
	// system ram
	if (addr >= 0 && addr <= 0x1FFF)
		return m_cpu.Read(addr & 0x7FF);
	// PPU ram
	else if (addr >= 0x2000 && addr <= 0x3FFF)
		return m_ppu.CPURead(addr & 7);
	// rom?
	else
		return m_rom->CPURead(addr);
}

void NES::Reset()
{
	m_cpu.Reset();
	m_cycles = 0;
}

void NES::Write(uint16_t addr, uint8_t data)
{
	// system ram
	if (addr >= 0 && addr <= 0x1FFF)
		m_cpu.Write(addr & 0x7FF, data);
	// PPU ram
	else if (addr >= 0x2000 && addr <= 0x3FFF)
		m_ppu.CPUWrite(addr & 7, data);
	// rom?
	else
		m_rom->CPUWrite(addr, data);
}
