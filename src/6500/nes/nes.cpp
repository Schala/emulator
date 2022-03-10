#include "nes.h"

NES::NES(SDL_Renderer *renderer):
	m_cycles(0),
	m_rom(nullptr),
	m_bus(BusLE16(65536)),
	m_cpu(MOS6500(&m_bus, 0, 4095)),
	m_ppu(PPU2C02(*this, renderer))
{
}

NES::~NES()
{
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
	return m_rom.get();
}

void NES::LoadROM(const std::filesystem::path &path)
{
	m_rom.reset(new NESROM(*this, path));
	Reset();
}

uint8_t NES::ReadByte(uint16_t addr)
{
	// system ram
	if (addr >= 0 && addr <= 4095)
		return m_cpu.ReadByte(addr & 2047);
	// PPU ram
	else if (addr >= 8192 && addr <= 16383)
		return m_ppu.CPUReadByte(addr & 7);
	// rom?
	else
		return m_rom->CPUReadByte(addr);
}

void NES::Reset()
{
	m_cpu.Reset();
	m_ppu.Reset();
	m_cycles = 0;
}

void NES::WriteByte(uint16_t addr, uint8_t data)
{
	// system ram
	if (addr >= 0 && addr <= 4095)
		m_cpu.WriteByte(addr & 2047, data);
	// PPU ram
	else if (addr >= 8192 && addr <= 16383)
		m_ppu.CPUWriteByte(addr & 7, data);
	else // rom?
		m_rom->CPUWriteByte(addr, data);
}
