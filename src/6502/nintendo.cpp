#include <algorithm>
#include <format>
#include <fstream>
#include <stdexcept>

#include "nintendo.h"

// PPU2C02

static const PPU2C02::Palette = {
	{ 84, 84, 84, SDL_ALPHA_OPAQUE },
	{ 0, 30, 116, SDL_ALPHA_OPAQUE },
	{ 8, 16, 144, SDL_ALPHA_OPAQUE },
	{ 48, 0, 136, SDL_ALPHA_OPAQUE },
	{ 68, 0, 100, SDL_ALPHA_OPAQUE },
	{ 92, 0, 48, SDL_ALPHA_OPAQUE },
	{ 84, 4, 0, SDL_ALPHA_OPAQUE },
	{ 60, 24, 0, SDL_ALPHA_OPAQUE },
	{ 32, 42, 0, SDL_ALPHA_OPAQUE },
	{ 8, 58, 0, SDL_ALPHA_OPAQUE },
	{ 0, 64, 0, SDL_ALPHA_OPAQUE },
	{ 0, 60, 0, SDL_ALPHA_OPAQUE },
	{ 0, 50, 60, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },

	{ 152, 150, 152, SDL_ALPHA_OPAQUE },
	{ 8, 76, 196, SDL_ALPHA_OPAQUE },
	{ 48, 50, 236, SDL_ALPHA_OPAQUE },
	{ 92, 30, 228, SDL_ALPHA_OPAQUE },
	{ 136, 20, 176, SDL_ALPHA_OPAQUE },
	{ 160, 20, 100, SDL_ALPHA_OPAQUE },
	{ 152, 34, 32, SDL_ALPHA_OPAQUE },
	{ 120, 60, 0, SDL_ALPHA_OPAQUE },
	{ 84, 90, 0, SDL_ALPHA_OPAQUE },
	{ 40, 114, 0, SDL_ALPHA_OPAQUE },
	{ 8, 124, 0, SDL_ALPHA_OPAQUE },
	{ 0, 118, 40, SDL_ALPHA_OPAQUE },
	{ 0, 102, 120, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },

	{ 236, 238, 236, SDL_ALPHA_OPAQUE },
	{ 76, 154, 236, SDL_ALPHA_OPAQUE },
	{ 120, 124, 236, SDL_ALPHA_OPAQUE },
	{ 176, 98, 236, SDL_ALPHA_OPAQUE },
	{ 228, 84, 236, SDL_ALPHA_OPAQUE },
	{ 236, 88, 180, SDL_ALPHA_OPAQUE },
	{ 236, 106, 100, SDL_ALPHA_OPAQUE },
	{ 212, 136, 32, SDL_ALPHA_OPAQUE },
	{ 160, 170, 0, SDL_ALPHA_OPAQUE },
	{ 116, 196, 0, SDL_ALPHA_OPAQUE },
	{ 76, 208, 32, SDL_ALPHA_OPAQUE },
	{ 56, 204, 108, SDL_ALPHA_OPAQUE },
	{ 56, 180, 204, SDL_ALPHA_OPAQUE },
	{ 60, 60, 60, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },

	{ 236, 238, 236, SDL_ALPHA_OPAQUE },
	{ 168, 204, 236, SDL_ALPHA_OPAQUE },
	{ 188, 188, 236, SDL_ALPHA_OPAQUE },
	{ 212, 178, 236, SDL_ALPHA_OPAQUE },
	{ 236, 174, 236, SDL_ALPHA_OPAQUE },
	{ 236, 174, 212, SDL_ALPHA_OPAQUE },
	{ 236, 180, 176, SDL_ALPHA_OPAQUE },
	{ 228, 196, 144, SDL_ALPHA_OPAQUE },
	{ 204, 210, 120, SDL_ALPHA_OPAQUE },
	{ 180, 222, 120, SDL_ALPHA_OPAQUE },
	{ 168, 226, 144, SDL_ALPHA_OPAQUE },
	{ 152, 226, 180, SDL_ALPHA_OPAQUE },
	{ 160, 214, 228, SDL_ALPHA_OPAQUE },
	{ 160, 162, 160, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE }
};

PPU2C02::PPU2C02(NES &nes, SDL_Renderer *renderer):
	Device(dynamic_cast<Bus *>(nes.GetBus(), 0, 0)),
	m_scanline(0),
	m_cycle(0),
	m_renderer(renderer),
	m_nes(nes),
	m_ppuBus(Bus6502(BusRAM))
{
	m_flags.frame_done = false;

	for (auto &p : m_nameTbl)
		p = Sprite(ScreenWidth, ScreenHeight);

	for (auto &p : m_patTbl)
		p = Sprite(PatternDimension, PatternDimension);

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
	if (++m_cycle >= MaxCycles)
	{
		m_cycle = 0;

		if (++m_scanline >= MaxScanlines)
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

Bus6502 * PPU2C02::GetBus() const
{
	return &m_ppuBus;
}

void PPU2C02::NoiseTest()
{
	SDL_Color c = Palette[m_rng(m_mt) ? 63 : 48];
	SDL_SetRenderDrawColor(m_renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawPoint(m_renderer, m_cycle - 1, m_scanline);
}

uint8_t PPU2C02::Read(uint16_t addr) const
{

}

void PPU2C02::Write(uint16_t addr, uint8_t data)
{
}


// NESROM

NESROM::NESROM(NES &nes, const std::filesystem::path &path):
	Device(dynamic_cast<Bus *>(nes.GetBus(), 0, 0)),
	m_nes(nes)
{
	std::ifstream romFile(path, std::ios::binary);
	romFile.read(std::bit_cast(&m_header), HeaderSize);

	if (std::equal(m_header.magic.begin(), m_header.magic.end(), Magic.begin()))
	{
		m_prg.resize(m_header.prgPages * 16384);
		romFile.read(std::bit_cast(m_prg.data()), m_prg.size());

		m_chr.resize(m_header.chrPages * 8192);
		romFile.read(std::bit_cast(m_chr.data()), m_chr.size());

		m_mapperID = (m_header.mapperInfo.typeHi << 4) | m_header.mapperInfo.typeLo;

		switch (m_mapperID)
		{
			case 0: m_mapper = new NROM(m_header.prgPages, m_header.chrPages); break;
			default:
				throw std::runtime_error(std::format("Unsupported mapper ID: {}", m_mapperID));
		}
	}

	nes.GetPPU()->GetBus()->Add(this);
}

NESROM::~NESROM()
{
	nes.GetPPU()->GetBus()->Remove(this);
}

uint8_t NESROM::CPURead(uint16_t addr) const
{
	uint32_t mappedAddr = 0;

	if (m_mapper->CPUMapRead(addr, &mappedAddr))
		return m_prg[mappedAddr];
	return 0;
}

void NESROM::CPUWrite(uint16_t addr, uint8_t data)
{
	uint32_t mappedAddr = 0;

	if (m_mapper->CPUMapWrite(addr, &mappedAddr))
		m_prg[mappedAddr] = data;
}

uint8_t NESROM::PPURead(uint16_t addr) const
{
	uint32_t mappedAddr = 0;

	if (m_mapper->PPUMapRead(addr, &mappedAddr))
		return m_chr[mappedAddr];
	return 0;
}

void NESROM::PPUWrite(uint16_t addr, uint8_t data)
{
	uint32_t mappedAddr = 0;

	if (m_mapper->PPUMapWrite(addr, &mappedAddr))
		m_chr[mappedAddr] = data;
}


// NES

NES::NES(SDL_Renderer *renderer):
	m_cycles(0),
	m_rom(nullptr),
	m_bus(Bus6502(BusRAM))
{
	m_cpu = CPU6502(m_bus, 0, 0x1FFF);
	m_ppu = PPU2C02(*this, renderer);
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

void NES::LoadROM(const std::filesystem::path &path)
{
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
