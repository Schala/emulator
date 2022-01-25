#include <iostream>

#include "core/engine.h"
#include "6502/nes.h"

class Emulator : public Engine
{
public:
	Emulator(const char *rom):
		Engine(PPU2C02::NTSCFrameRate),
		m_nes(new NES(m_renderer))
	{
		m_nes->LoadROM(rom);
	}

	virtual ~Emulator()
	{
		if (m_nes) delete m_nes;
	}
protected:
	void Resized(int w, int h) override
	{
		SDL_RenderSetScale(m_renderer, w / PPU2C02::ScreenWidth, h / PPU2C02::ScreenHeight);
	}

	void Updated(float) override
	{
		while (true)
		{
			if (!m_nes->GetPPU()->IsFrameDone())
			{
				m_nes->Clock();
				m_nes->GetPPU()->NoiseTest();
			}
			else break;
		}

		m_nes->GetPPU()->NextFrame();
	}
private:
	NES *m_nes;
};

int main(int argc, char **argv)
{
	Emulator emu(argv[1]);
	emu.Start();
	return 0;
}
