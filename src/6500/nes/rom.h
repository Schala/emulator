#ifndef _NES_MAPPED_ROM_H
#define _NES_MAPPED_ROM_H

#include <array>
#include <filesystem>

#include "mapper.h"
#include "ppu.h"

class NESROM : public Device
{
public:
	NESROM(NES &, const std::filesystem::path &);
	~NESROM();

	// Read from CPU bus
	uint8_t CPUReadByte(uint16_t);

	// Write to CPU bus
	void CPUWriteByte(uint16_t, uint8_t);

	uint32_t Hash() const;

	// Read from PPU bus
	uint8_t PPUReadByte(uint16_t);

	// Write to PPU bus
	void PPUWriteByte(uint16_t, uint8_t);
private:
	// File header magic
	static constexpr std::array<char, 4> Magic = { 'N', 'E', 'S', 26 };

	uint8_t m_mapperID;

	struct Header
	{
		std::array<char, 4> magic; // must be equal to Magic
		uint8_t prgPages; // ROM data
		uint8_t chrPages; // graphics data

		struct MapperInfo
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
		std::array<uint8_t, 6> _reserved9;
	} m_header;

	uint32_t m_hash;
	NES &m_nes;
	NESMapper *m_mapper;
	std::vector<uint8_t> m_prg; // PRG memory
	std::vector<uint8_t> m_chr; // CHR memory

	void GenerateHash();
};

#endif // _NES_MAPPED_ROM_H
