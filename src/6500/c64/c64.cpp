#include "c64.h"

C64::C64()://SDL_Renderer *renderer)
	m_cycles(0),
	m_rom(nullptr),
	m_bus(Bus6500(0xFFFF)),
	m_cpu(MOS6500(&m_bus, 0, 0x3FF)
{
}

C64::~C64()
{
	if (m_rom) delete m_rom;
}

void C64::Clock()
{
	if (m_cycles++ % 3 == 0)
		m_cpu.Clock();
}

Bus6500 * C64::GetBus() const
{
	return &m_bus;
}

MOS6500 * C64::GetCPU() const
{
	return &m_cpu;
}

void CPU::LoadTape(const std::filesystem::path &path)
{
	if (m_rom) delete m_rom;
	m_rom = new C64Tape(path);
}
