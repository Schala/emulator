#ifndef _C64_H
#define _C64_H

#include "../devices.h"

class C64
{
public:
	C64();//SDL_Renderer *);
	~C64();

	void Clock();

	Bus6500 * GetBus() const;
	MOS6500 * GetCPU() const;

	void LoadTape(const std::filesystem::path &);
private:
	size_t m_cycles;
	Device *m_rom;
	Bus6500 m_bus;
	MOS6500 m_cpu;
};

#endif // _C64_H
