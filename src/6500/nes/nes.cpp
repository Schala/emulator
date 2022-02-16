#include "nes.h"

NES::NES(SDL_Renderer *renderer):
	m_cycles(0),
	m_rom(nullptr),
	m_bus(BusLE16(0xFFFF)),
	m_cpu(MOS6500(&m_bus, 0, 0x1FFF)),
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

BusLE16 * NES::GetBus()
{
	return &m_bus;
}

MOS6500 * NES::GetCPU()
{
	return &m_cpu;
}

PPU2C02 * NES::GetPPU()
{
	return &m_ppu;
}

NESROM * NES::GetROM()
{
	return m_rom;
}

void NES::LoadROM(const std::filesystem::path &path)
{
	if (m_rom) delete m_rom;
	m_rom = new NESROM(*this, path);
	m_cpu.Reset();
}

uint8_t NES::ReadByte(uint16_t addr)
{
	// system ram
	if (addr >= 0 && addr <= 0x1FFF)
		return m_cpu.ReadByte(addr & 0x7FF);
	// PPU ram
	else if (addr >= 0x2000 && addr <= 0x3FFF)
		return m_ppu.CPUReadByte(addr & 7);
	// rom?
	else
		return m_rom->CPUReadByte(addr);
}

void NES::Reset()
{
	m_cpu.Reset();
	m_cycles = 0;
}

void NES::WriteByte(uint16_t addr, uint8_t data)
{
	// system ram
	if (addr >= 0 && addr <= 0x1FFF)
		m_cpu.WriteByte(addr & 0x7FF, data);
	// PPU ram
	else if (addr >= 0x2000 && addr <= 0x3FFF)
		m_ppu.WriteByte(addr & 7, data);
	// rom?
	else
		m_rom->WriteByte(addr, data);
}
