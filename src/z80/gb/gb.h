#ifndef _GB_H
#define _GB_H

#include "cpu.h"
#include "rom.h"

class GB
{
public:
	GB(SDL_Renderer *);
	~GB();

	void LoadROM(const std::filesystem::path &);

	// Read byte from RAM address
	uint8_t ReadByte(uint16_t);

	// Write byte to RAM address
	void WriteByte(uint16_t, uint8_t);
private:
	size_t m_cycles;
	std::unique_ptr<GBROM> m_rom;
	BusLE16 m_bus;
	LR35902 m_cpu;
};

#endif // _GB_H
