#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nes.h"
#include "ppu.h"
#include "rom.h"

static NES_MAPPER MAPPERS[] = {
	// 0x
	{ 16384, 0x8000, 0xFFFF, 8192, 0, 0x1FFF, 0, 0x1FFF, "NROM" },

	// Ax
	// a0
	// a1
	{ 32768, 0x8000, 0xFFFF, 8192, 0, 0x1FFF, 0x6000, 0x7FFF, "FS304" }
};

void nes_rom_alloc(NES *nes, const char *filepath)
{
	if (!nes) return;

	FILE *f = fopen(filepath, "rb");
	NES_ROM_HEADER header;

	fread(&header, sizeof(NES_ROM_HEADER), 1, f);

	if (strncmp(header.magic, NES_ROM_MAGIC, 4) != 0 || !filepath)
		fclose(f);

	// Mapper must be identified first so that we can initialise everything correctly
	nes->rom = (NES_ROM *)malloc(sizeof(NES_ROM));
	nes->rom->mapper_id = (header.mapper_info.type_hi << 4) | header.mapper_info.type_lo;
	nes->map = &MAPPERS[nes->rom->mapper_id];

	nes->rom->prg = (uint8_t *)malloc(header.prg_pages * nes->map->prg_size);
	fread(nes->rom->prg, header.prg_pages, nes->map->prg_size, f);

	nes->rom->chr = (uint8_t *)malloc(header.chr_pages * nes->map->chr_size);
	fread(nes->rom->chr, header.chr_pages, nes->map->chr_size, f);

	fclose(f);

	nes->rom->header = header;

	// memory mapping as per mapper specs
	cpu6502_map(nes->cpu, nes->map->ram_start_addr, nes->map->ram_end_addr);
	nes->ppu->ppu_node->ram_offset = nes->map->chr_start_addr;
	nes->ppu->ppu_node->ram_size = nes->map->chr_end_addr + 1;

	// and add the ROM to each bus
	nes->rom->prg_node = bus6502_add_device(nes->bus, nes->rom, nes->map->prg_start_addr,
		nes->map->prg_end_addr);
	nes->rom->chr_node = bus6502_add_device(nes->ppu->bus, nes->rom, nes->map->chr_start_addr,
		nes->map->chr_end_addr);
}

void nes_rom_free(NES *nes)
{
	if (!nes) return;
	if (!nes->rom) return;

	bus6502_free_device(nes->bus, nes->rom);
	bus6502_free_device(nes->ppu->bus, nes->rom);
	free(nes->rom->prg);
	free(nes->rom->chr);
	free(nes->rom);
	nes->rom = NULL;
	nes->map = NULL;
}

void nes_print_rom_info(const NES *nes)
{
	if (!nes) return;
	if (!nes->rom) return;

	NES_ROM_HEADER *header = &nes->rom->header;

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
	printf("Mapper ID: %X\n", nes->rom->mapper_id);
}

uint8_t nes_rom_read_cpu(NES *nes, uint16_t addr)
{
	uint32_t mapped_addr = 0;
	if (nes_map_cpu(nes, addr, &mapped_addr))
		return nes->rom->prg[mapped_addr];
	return 0;
}

void nes_rom_write_cpu(NES *nes, uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (nes_map_cpu(nes, addr, &mapped_addr))
		nes->rom->prg[mapped_addr] = data;
}

uint8_t nes_rom_read_ppu(NES *nes, uint16_t addr)
{
	uint32_t mapped_addr = 0;
	if (nes_map_ppu(nes, addr, &mapped_addr))
		return nes->rom->chr[mapped_addr];
	return 0;
}

void nes_rom_write_ppu(NES *nes, uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (nes_map_cpu(nes, addr, &mapped_addr))
		nes->rom->chr[mapped_addr] = data;
}
