#ifndef _NES_H
#define _NES_H

#include "../cpu.h"
#include "rom.h"

class NES
{
public:
	NES(SDL_Renderer *);
	~NES();

	// Runs NES clocks
	void Clock();

	// Retrieves a pointer to the main CPU bus
	BusLE16 * GetBus();

	MOS6500 * GetCPU();

	PPU2C02 * GetPPU();

	NESROM * GetROM();

	void LoadROM(const std::filesystem::path &);

	// Read byte from RAM address
	uint8_t ReadByte(uint16_t);

	// Resets the NES
	void Reset();

	// Write byte to RAM address
	void WriteByte(uint16_t, uint8_t);
private:
	size_t m_cycles;
	std::unique_ptr<NESROM> m_rom;
	BusLE16 m_bus;
	MOS6500 m_cpu;
	PPU2C02 m_ppu;
};

#endif // _NINTENDO_H
