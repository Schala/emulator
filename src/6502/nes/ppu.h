#ifndef _NES_PPU_H
#define _NES_PPU_H

//#include <random>

#include "../../core/engine.h"
#include "../devices.h"

class NES;

// Picture processor unit for the NES
class PPU2C02 : public Device
{
public:
	static const std::array<uint32_t, 64> Palette;

	PPU2C02(NES &, SDL_Renderer *);

	~PPU2C02();

	// Advance the clock, which is relentless
	void Clock();

	// Read from CPU bus
	uint8_t CPURead(uint16_t) const;

	// Write to CPU bus
	void CPUWrite(uint16_t, uint8_t);

	Bus6502 * GetBus();

	bool IsFrameDone() const;

	void NextFrame();

	//void NoiseTest();

	// Read byte from RAM address
	uint8_t Read(uint16_t) const;

	// Return the color from the palette in RAM
	SDL_Color & ReadRAMPaletteColor(uint8_t, uint8_t) const;

	// Write byte to RAM address
	void Write(uint16_t, uint8_t);
private:
	// Ending offset in CPU bus RAM
	static constexpr uint16_t CPUBusEndAddress = 0x3FFF;

	// Starting offset in the CPU bus RAM
	static constexpr uint16_t CPUBusStartAddress = 0x2000;

	struct
	{
		bool
			frameDone : 1;
	} m_flags;

	int16_t m_scanline; // screen row
	uint16_t m_cycle; // screen column
	SDL_Renderer *m_renderer;
	NES &m_nes;
	Bus6502 m_ppuBus; // dedicated second bus
	std::array<Sprite, 2> m_nameTbl;
	std::array<Sprite, 2> m_patTbl;
	Sprite m_ramPalette;

	// noise test
	/*std::mt19937 m_mt;
	std::bernoulli_distribution m_rng;*/
};

#endif // _NES_PPU_H
