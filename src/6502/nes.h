#ifndef _NES_H
#define _NES_H

#include "devices.h"

// ROM file header magic
#define NES_ROM_MAGIC "NES\x1A"

// offset in RAM to load ROM
#define NES_ROM_ADDR 0x8000

// Picture processing unit
typedef struct _NES_PPU
{
	BUS_6502 *bus;
} NES_PPU;

// Initialise a new PPU on specified bus
NES_PPU * nes_ppu_alloc(BUS_6502 *);

// Deallocate PPU
void nes_ppu_free(NES_PPU *);

// ROM header flags
typedef struct _NES_ROM_MAPPER_INFO
{
	uint8_t
		mirror_vertical : 1,
		battery_backed_ram : 1, // 6000-7FFF
		trainer : 1, // 7000-71FF
		four_screen_vram_layout : 1,
		type_lo: 4,
		vs_system_cart : 1,
		reserved : 3,
		type_hi: 4;
} NES_ROM_MAPPER_INFO;

// Returns the mapper ID of the ROM
static inline uint8_t nes_rom_mapper_id(const NES_ROM_MAPPER_INFO *mi)
{
	return (mi->type_hi << 4) | mi->type_lo;
}

// ROM file header
typedef struct _NES_ROM_HEADER
{
	char magic[4]; // must be NES_ROM_MAGIC
	uint8_t prg_pages; // ROM data
	uint8_t chr_pages; // graphics data
	NES_ROM_MAPPER_INFO mapper_info;
	uint8_t ram_pages;
	uint8_t is_pal;
	uint8_t reserved[6];
} NES_ROM_HEADER;

// NES ROM (cartridge)
typedef struct _NES_ROM
{
	BUS_6502 *bus;
	NES_ROM_HEADER header;
} NES_ROM;

// Validate ROM and load into RAM, returning a new ROM pointer
NES_ROM * nes_rom_alloc(BUS_6502 *, const char *);

// Deallocate ROM
void nes_rom_free(NES_ROM *);

// print ROM info to console
void nes_print_rom_info(const NES_ROM_HEADER *); // likely to change arg type

#endif // _NES_H
