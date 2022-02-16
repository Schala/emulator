#ifndef _C64_H
#define _C64_H

#include "../cpu.h"

class C64
{
public:
	C64();//SDL_Renderer *);
	~C64();

	void Clock();

	BusLE16 * GetBus() const;
	MOS6500 * GetCPU() const;

	void LoadTape(const std::filesystem::path &);
private:
	size_t m_cycles;
	Device *m_rom;
	BusLE16 m_bus;
	MOS6500 m_cpu;
};

#endif // _C64_H
