#ifndef _NINTENDO_H
#define _NINTENDO_H

#include <filesystem>
#include <random>

#include "../engine.h"
#include "devices.h"
#include "nes_mappers.h"

class NES;

// Picture processor unit for the NES
class PPU2C02 : public Device
{
public:
	// Unscaled screen height
	static constexpr uint32_t ScreenHeight = 240;

	// Unscaled screen width
	static constexpr uint32_t ScreenWidth = 256;

	PPU2C02(NES &, SDL_Renderer *);

	~PPU2C02();

	// Advance the clock, which is relentless
	void Clock();

	// Read from CPU bus
	uint8_t CPURead(uint16_t) const;

	// Write to CPU bus
	void CPUWrite(uint16_t, uint8_t);

	inline Bus6502 * GetBus() const;

	void NoiseTest();

	// Read byte from RAM address
	uint8_t Read(uint16_t) const;

	// Write byte to RAM address
	void Write(uint16_t, uint8_t);
private:
	static const std::array<SDL_Color, PaletteSize> Palette;

	static constexpr uint16_t AddressMask = 7;

	// How much RAM to allocate to PPU dedicated Bus
	static constexpr uint16_t BusRAM = 0x4000;

	// Ending offset in CPU bus RAM
	static constexpr uint16_t CPUBusEndAddress = 0x3FFF;

	// Starting offset in the CPU bus RAM
	static constexpr uint16_t CPUBusStartAddress = 0x2000;

	// Max cycles in a screen row
	static constexpr uint16_t MaxCycles = 341;

	// Max scalines in a frame
	static constexpr int16_t MaxScanlines 261;

	static constexpr uint16_t PaletteAddress = 0x3F00;

	static constexpr uint8_t PaletteSize = 64;

	// Both the X and Y pattern dimensions
	static constexpr uint8_t PatternDimension = 128;

	static constexpr uint16_t VRAMStartAddress = 0x2000;

	static constexpr uint16_t VRAMEndAddress = 0x2FFF;

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

	// noise test
	std::mt19937 m_mt;
	std::bernoulli_distribution m_rng;
};

class NESROM : public Device
{
public:
	NESROM(NES &, const std::filesystem::path &);
	~NESROM();

	// Read from CPU bus
	uint8_t CPURead(uint16_t) const;

	// Write to CPU bus
	void CPUWrite(uint16_t, uint8_t);

	// Read from PPU bus
	uint8_t PPURead(uint16_t) const;

	// Write to PPU bus
	void PPUWrite(uint16_t, uint8_t);
private:
	static constexpr size_t HeaderSize = 16;

	// File header magic
	static constexpr std::array<char, 4> Magic = { 'N', 'E', 'S', 26 };

	uint8_t m_mapperID;

	struct
	{
		std::array<char, 4> magic; // must be equal to Magic
		uint8_t prgPages; // ROM data
		uint8_t chrPages; // graphics data

		struct
		{
			uint16_t
				mirrorVertical : 1,
				batteryBackedRAM : 1,
				trainer : 1,
				fourScreenVRAMLayout : 1,
				typeLo: 4,
				vsSystemCart : 1,
				reserved : 3,
				typeHi: 4;
		} mapperInfo;

		uint8_t ramPages;
		bool isPAL;
		std::array<uint8_t, 6> reserved;
	} m_header;

	NES &m_nes;
	const NESMapper *m_mapper;
	std::vector<uint8_t> m_prg; // PRG memory
	std::vector<uint8_t> m_chr; // CHR memory
};

class NES
{
public:
	NES(SDL_Renderer *);
	~NES();

	// Runs NES clocks
	void Clock();

	// Retrieves a pointer to the main CPU bus
	inline Bus6502 * GetBus() const;

	inline PPU2C02 * GetPPU() const;

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
	CPU6502 m_cpu;
	NESROM *m_rom;
	Bus6502 m_bus;
	PPU2C02 m_ppu;
};

#endif // _NINTENDO_H
