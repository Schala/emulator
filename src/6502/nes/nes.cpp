#include "nes.h"

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
