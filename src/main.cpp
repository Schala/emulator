#include <iostream>

#include "engine.h"
#include "6502/nintendo.h"

class Emulator : public Engine
{
public:
	Emulator():
		m_nes(new NES(m_renderer))
	{
	}

	virtual ~Emulator()
	{
		if (m_nes) delete m_nes;
	}

	void Resized(int w, int h) override
	{
		SDL_RendererSetScale(m_renderer, w / PPU2C02::ScreenWidth, h / PPU2C02::ScreenHeight);
	}
private:
	NES *m_nes;
};

int main(int argc, char **argv)
{
	Emulator emu;
	return 0;
}
