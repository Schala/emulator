#ifndef _NES_H
#define _NES_H

#include "devices.h"

#define NES_ROM_MAGIC "NES\x1A"
#define NES_ROM_ADDR 0x8000

typedef struct _NES_PPU {
	BUS_6502 *bus;
} NES_PPU;

enum NES_ERROR
{
	INVALID_NES_ROM = -1
};

typedef struct _NES_ROM_MAPPER {
	uint8_t
		mirror_vertical : 1,
		battery_backed_ram : 1, // 6000-7FFF
		trainer : 1, // 7000-71FF
		four_screen_vram_layout : 1,
		type_lo: 4,
		vs_system_cart : 1,
		reserved_09 : 3,
		type_hi: 4;
} NES_ROM_MAPPER;

typedef struct _NES_ROM_HEADER {
	char magic[4];
	uint8_t prg_pages; // ROM data
	uint8_t chr_pages; // graphics data
	NES_ROM_MAPPER mapper;
	uint8_t ram_pages;
	uint8_t is_pal;
	uint8_t reserved[6];
} NES_ROM_HEADER;

NES_PPU * nes_ppu_alloc(BUS_6502 *);
void nes_ppu_free(NES_PPU *);

int nes_load_rom(BUS_6502 *, const char *);

#endif // _NES_H
