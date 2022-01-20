#ifndef _NES_ROM_H
#define _NES_ROM_H

#include <stdint.h>

#include "nes.h"

// ROM file header magic
#define NES_ROM_MAGIC "NES\x1A"

// ROM header flags
typedef struct _NES_ROM_MAPPER_INFO
{
	uint16_t
		mirror_vertical : 1,
		battery_backed_ram : 1,
		trainer : 1,
		four_screen_vram_layout : 1,
		type_lo: 4,
		vs_system_cart : 1,
		reserved : 3,
		type_hi: 4;
} NES_ROM_MAPPER_INFO;

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
struct _NES_ROM
{
	uint8_t mapper_id;
	DEV_6502 *prg_node; // program
	DEV_6502 *chr_node; // sprite patterns
	uint8_t *prg; // PRG memory
	uint8_t *chr; // CHR memory
	NES_ROM_HEADER header;
};

// Validate ROM and load into RAM, returning a new ROM pointer
void nes_rom_alloc(NES *, const char *);

// Deallocate ROM
void nes_rom_free(NES *);

// ROM read from CPU
uint8_t nes_rom_read_cpu(NES *, uint16_t);

// ROM write to CPU
void nes_rom_write_cpu(NES *, uint16_t, uint8_t);

// ROM read from PPU
uint8_t nes_rom_read_ppu(NES *, uint16_t);

// ROM write to PPU
void nes_rom_write_ppu(NES *, uint16_t, uint8_t);

#endif // _NES_ROM_H
