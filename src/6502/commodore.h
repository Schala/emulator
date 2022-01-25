#ifndef _COMMODORE_H
#define _COMMODORE_H

#include <filesystem>
#include <SDL2/SDL.h>

#include "devices.h"

class C64
{
public:
	C64(SDL_Renderer *);

	void Clock();

	void LoadROM(const std::filesystem::path &);
private:
	Bus6502 m_bus;
	MOS6502 m_cpu;
};

#endif // _COMMODORE_H
