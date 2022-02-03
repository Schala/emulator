#ifndef _NES_H
#define _NES_H

#include "rom.h"

class NES
{
public:
	NES(SDL_Renderer *);
	~NES();

	// Runs NES clocks
	void Clock();

	// Retrieves a pointer to the main CPU bus
	Bus6502 * GetBus();

	PPU2C02 * GetPPU();

	void LoadROM(const std::filesystem::path &);

	// Read byte from RAM address
	uint8_t Read(uint16_t) const;

	// Resets the NES
	void Reset();

	// Write byte to RAM address
	void Write(uint16_t, uint8_t);
private:
	static constexpr size_t BusRAM = 65536;

	size_t m_cycles;
	NESROM *m_rom;
	Bus6502 m_bus;
	MOS6502 m_cpu;
	PPU2C02 m_ppu;
};

#endif // _NINTENDO_H
