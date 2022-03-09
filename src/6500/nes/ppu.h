#ifndef _NES_PPU_H
#define _NES_PPU_H

//#include <random>

#include "../../core/engine.h"
#include "../../generic/bus_le16.h"

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

	BusLE16 * GetBus();

	// Get the 16x16 pattern table as a sprite
	Sprite & GetPatternTable(uint8_t, uint8_t);

	bool IsFrameDone() const;

	void NextFrame();

	//void NoiseTest();

	// Read from PPU bus
	uint8_t PPUReadByte(uint16_t);

	// Write to PPU bus
	void PPUWriteByte(uint16_t, uint8_t);

	// Return the color from the palette in RAM
	SDL_Color ReadRAMPaletteColor(uint8_t, uint8_t);

	// Update PPU registers
	void UpdateRegisters();
private:
	struct
	{
		bool
			frameDone : 1,
			addressLatch : 1; // fetching PPU address register takes 2 cycles
	} m_flags;

	union
	{
		struct
		{
			bool
				_0 : 1,
				_1 : 1,
				_2 : 1,
				_3 : 1,
				_4 : 1,
				spriteOverflow : 1,
				sprite0Hit : 1,

				// when our cycles are > 240 and <= 261 (off screen), good time for for CPU manipulation of PPU,
				// otherwise we'd get graphical artifacts
				verticalBlank : 1;
		} p;

		uint8_t reg;
	} m_status;

	union
	{
		struct
		{
			bool
				grayscale : 1,
				renderBGLeft : 1,
				renderFGLeft : 1,
				renderBG : 1,
				renderFG : 1,
				enhanceRed : 1,
				enhanceGreen : 1,
				enhanceBlue : 1;
		} p;

		uint8_t reg;
	} m_mask;

	union
	{
		struct
		{
			bool
				nameTableX : 1,
				nameTableY : 1,
				increment : 1,
				patternFG : 1,
				patternBG : 1,
				spriteSize : 1,
				slave : 1, // unused
				nmi : 1;
		} p;

		uint8_t reg;
	} m_ctrl;

	uint8_t m_cache;
	uint16_t m_lastAddr;
	int16_t m_scanline; // screen row
	uint16_t m_cycle; // screen column
	SDL_Renderer *m_renderer;
	NES &m_nes;
	BusLE16 m_ppuBus; // dedicated second bus
	std::array<uint8_t, 32> m_palTbl;
	std::array<uint8_t, 2048> m_nameTbl;
	std::array<uint8_t, 4096> m_patTbl;
	std::array<Sprite, 2> m_sprNameTbl;
	std::array<Sprite, 2> m_sprPatTbl;
	Sprite m_ramPalette;

	// noise test
	/*std::mt19937 m_mt;
	std::bernoulli_distribution m_rng;*/
};

#endif // _NES_PPU_H
