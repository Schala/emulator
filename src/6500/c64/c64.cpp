#include "c64.h"

C64::C64()://SDL_Renderer *renderer)
	m_cycles(0),
	m_tape(nullptr),
	m_bus(Bus6502(0xFFFF)),
	m_cpu(MOS6502(&m_bus, 0, 0x3FF)
{
}

C64::~C64()
{
	if (m_tape) delete m_tape;
}

void C64::Clock()
{
	if (m_cycles++ % 3 == 0)
		m_cpu.Clock();
}

Bus6502 * C64::GetBus() const
{
	return &m_bus;
}

MOS6502 * C64::GetCPU() const
{
	return &m_cpu;
}

void CPU::LoadTape(const std::filesystem::path &path)
{
	if (m_tape) delete m_tape;
	m_tape = new C64Tape(path);
}
