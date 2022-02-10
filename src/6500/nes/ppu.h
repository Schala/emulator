#ifndef _NES_PPU_H
#define _NES_PPU_H

//#include <random>

#include "../../core/engine.h"
#include "../devices.h"

class NES;

// Picture processor unit for the NES
class PPU2C02 : public Processor
{
public:
	static const std::array<uint32_t, 64> Palette;

	PPU2C02(NES &, SDL_Renderer *);

	~PPU2C02();

	// Advance the clock, which is relentless
	void Clock() override;

	// Read from CPU bus
	uint8_t CPUReadByte(uint16_t);

	// Write to CPU bus
	void CPUWriteByte(uint16_t, uint8_t);

	Bus6500 * GetBus();

	bool IsFrameDone() const;

	void NextFrame();

	//void NoiseTest();

	// Read from PPU bus
	uint8_t PPUReadByte(uint16_t);

	// Write to PPU bus
	void PPUWriteByte(uint16_t, uint8_t);

	// Return the color from the palette in RAM
	SDL_Color ReadRAMPaletteColor(uint8_t, uint8_t);
private:
	struct
	{
		bool
			frameDone : 1;
	} m_flags;

	int16_t m_scanline; // screen row
	uint16_t m_cycle; // screen column
	SDL_Renderer *m_renderer;
	NES &m_nes;
	Bus6500 m_ppuBus; // dedicated second bus
	std::array<Sprite, 2> m_nameTbl;
	std::array<Sprite, 2> m_patTbl;
	Sprite m_ramPalette;

	// noise test
	/*std::mt19937 m_mt;
	std::bernoulli_distribution m_rng;*/
};

#endif // _NES_PPU_H
