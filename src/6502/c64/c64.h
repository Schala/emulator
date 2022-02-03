#ifndef _C64_H
#define _C64_H

#include "tape.h"

class C64
{
public:
	C64()//SDL_Renderer *);
	~C64();

	void Clock();

	Bus6502 * GetBus() const;
	MOS6502 * GetCPU() const;

	void LoadTape(const std::filesystem::path &);
private:
	size_t m_cycles;
	C64Tape *m_tape;
	Bus6502 m_bus;
	MOS6502 m_cpu;
};

#endif // _C64_H
