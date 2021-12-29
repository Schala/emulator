#include <stdio.h>
#include <stdlib.h>

#include "nes.h"

NES_PPU * nes_ppu_alloc(BUS_6502 *bus)
{
	NES_PPU *ppu = (NES_PPU *)malloc(sizeof(NES_PPU));
	ppu->bus = bus;

	return ppu;
}

void nes_ppu_free(NES_PPU *ppu)
{
	free(ppu);
	ppu = NULL;
}

NES_ROM * nes_rom_alloc(BUS_6502 *bus, const char *filepath)
{
	FILE *f = fopen(filepath, "rb");
	NES_ROM_HEADER header;

	fread(&header, sizeof(NES_ROM_HEADER), 1, f);

	if (strncmp(header.magic, NES_ROM_MAGIC, 4) != 0 || !filepath)
	{
		fclose(f);
		return NULL;
	}

	NES_ROM *rom = (NES_ROM *)malloc(sizeof(NES_ROM));
	rom->bus = bus;
	rom->header = header;

	bus6502_add_device(bus, rom);

	fclose(f);

	return rom;
}

void nes_rom_free(NES_ROM *rom)
{
	bus6502_free_device(rom->bus, rom);
	free(rom);
	rom = NULL;
}

void nes_print_rom_info(const NES_ROM_HEADER *header)
{
	printf("Magic: %c%c%c\\x%2X\n", header->magic[0], header->magic[1], header->magic[2], header->magic[3]);
	printf("PRG pages: %u\n", header->prg_pages);
	printf("CHR pages: %u\n", header->chr_pages);
	printf("Mapper:\n");
	if (header->mapper_info.mirror_vertical) printf("\t- vertical mirroring\n");
	if (header->mapper_info.battery_backed_ram) printf("\t- battery backed RAM\n");
	if (header->mapper_info.trainer) printf("\t- trainer\n");
	if (header->mapper_info.four_screen_vram_layout) printf("\t- 4 screen VRAM layout\n");
	if (header->mapper_info.vs_system_cart) printf("\t- VS system cartridge\n");
	printf("RAM pages: %u\n", header->ram_pages);
	printf("Format: %s\n", header->is_pal ? "PAL" : "NTSC");
	printf("Mapper ID: %X\n", nes_rom_mapper_id(&header->mapper_info));
}
