#include <iostream>

#include "nes.h"

class Emulator : public Engine
{
public:
	Emulator(const char *rom): Engine(60.0f, false),
		m_nes(new NES(nullptr))
	{
		m_nes->LoadROM(rom);
	}

	virtual ~Emulator()
	{
		if (m_nes) delete m_nes;
	}
protected:
	/*void Resized(int w, int h) override
	{
		SDL_RenderSetScale(m_renderer, w / 256, h / 240);
	}*/

	void Updated(float) override
	{
		/*while (true)
		{
			if (!m_nes->GetPPU()->IsFrameDone())
			{
				m_nes->Clock();
				//m_nes->GetPPU()->NoiseTest();
			}
			else break;
		}

		m_nes->GetPPU()->NextFrame();*/

		m_nes->Clock();
		if (m_nes->GetCPU()->Cycles() == 0)
			std::cout << m_nes->GetCPU()->FrameInfo() << '\n';
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
