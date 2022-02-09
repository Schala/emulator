#ifndef _NES_H
#define _NES_H

#include "rom.h"

class NES : public Processor
{
public:
	NES(SDL_Renderer *);
	~NES();

	// Runs NES clocks
	void Clock() override;

	// Retrieves a pointer to the main CPU bus
	Bus6500 * GetBus();

	PPU2C02 * GetPPU();

	void LoadROM(const std::filesystem::path &);

	// Read byte from RAM address
	uint8_t ReadByte(uint16_t) const;

	// Resets the NES
	void Reset();

	// Write byte to RAM address
	void WriteByte(uint16_t, uint8_t);
private:
	size_t m_cycles;
	NESROM *m_rom;
	Bus6500 m_bus;
	MOS6500 m_cpu;
	PPU2C02 m_ppu;
};

#endif // _NINTENDO_H
