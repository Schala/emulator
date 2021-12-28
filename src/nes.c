#include <stdio.h>
#include <stdlib.h>

#include "nes.h"

NES_PPU * nes_ppu_alloc(BUS *bus)
{
	NES_PPU *ppu = (NES_PPU *)malloc(sizeof(NES_PPU));
	ppu->bus = bus;

	return ppu;
}

void nes_ppu_free(NES_PPU *ppu)
{
	free(ppu);
}

int nes_load_rom(BUS *bus, const char *filepath)
{
	FILE *rom = fopen(filepath, "rb");
	NES_ROM_HEADER header;

	fread(&header, sizeof(NES_ROM_HEADER), 1, rom);

	if (strncmp(header.magic, NES_ROM_MAGIC, 4) != 0 || !filepath)
		return INVALID_ROM;

	printf("Magic: %c%c%c\\x%2X\n", header.magic[0], header.magic[1], header.magic[2], header.magic[3]);
	printf("PRG pages: %u\n", header.prg_pages);
	printf("CHR pages: %u\n", header.chr_pages);
	printf("Mapper:\n");
	if (header.mapper.mirror_vertical) printf("\t- vertical mirroring\n");
	if (header.mapper.battery_backed_ram) printf("\t- battery backed RAM\n");
	if (header.mapper.trainer) printf("\t- trainer\n");
	if (header.mapper.four_screen_vram_layout) printf("\t- 4 screen VRAM layout\n");
	if (header.mapper.vs_system_cart) printf("\t- VS system cartridge\n");
	printf("RAM pages: %u\n", header.ram_pages);
	printf("Format: %s\n", header.is_pal ? "PAL" : "NTSC");

	fclose(rom);

	return 0;
}
