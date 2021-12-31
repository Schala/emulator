#include <stdio.h>
#include <stdlib.h>

#include "nes.h"

void nes_ppu_alloc(NES *nes)
{
	if (!nes) return;

	nes->ppu = (NES_PPU *)calloc(1, sizeof(NES_PPU));
	nes->ppu->bus = bus6502_alloc(NES_PPU_BUS_RAM_SIZE);
	nes->ppu->cpu_node = bus6502_add_device(nes->bus, ppu, NES_PPU_START_ADDR, NES_PPU_END_ADDR);
	nes->ppu->ppu_node = bus6502_add_device(nes->ppu->bus, ppu, 0, NES_PPU_BUS_RAM_SIZE - 1);
}

void nes_ppu_free(NES *nes)
{
	if (!nes) return;

	bus6502_free_device(nes->bus, nes->ppu);
	bus6502_free(nes->ppu->bus); // no need to free device from its own exclusive bus
	free(nes->ppu);
	nes->ppu = NULL;
}

nes_rom_alloc(NES *nes, const char *filepath)
{
	if (!nes) return NULL;

	FILE *f = fopen(filepath, "rb");
	NES_ROM_HEADER header;

	fread(&header, sizeof(NES_ROM_HEADER), 1, f);

	if (strncmp(header.magic, NES_ROM_MAGIC, 4) != 0 || !filepath)
	{
		fclose(f);
		return NULL;
	}

	nes->rom = (NES_ROM *)malloc(sizeof(NES_ROM));

	nes->rom->prg = (uint8_t *)malloc(header.prg_pages * NES_PRG_PAGE_SIZE);
	fread(nes->rom->prg, header.prg_pages, NES_PRG_PAGE_SIZE, f);

	nes->rom->chr = (uint8_t *)malloc(header.chr_pages * NES_CHR_PAGE_SIZE);
	fread(nes->rom->prg, header.chr_pages, NES_CHR_PAGE_SIZE, f);

	fclose(f);

	nes->rom->header = header;
	nes->rom->mapper_id = (header.mapper_info.type_hi << 4) | header.mapper_info.type_lo;

	nes->rom->prg_node = bus6502_add_device(nes->bus, nes->rom, NES_ROM_ADDR, 0xFFFF);

	return rom;
}

void nes_rom_free(NES *nes)
{
	if (!nes) return;
	if (!nes->rom) return;

	bus6502_free_device(NES->rom->bus, nes->rom);
	free(nes->rom->prg);
	free(nes->rom->chr);
	free(nes->rom);
	nes->rom = NULL;
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
	printf("Mapper ID: %X\n", nes_rom_mapper_id(&header->mapper_info));
}

NES * nes_alloc()
{
	NES *nes = (NES *)malloc(sizeof(NES));
	nes->bus = bus6502_alloc(0xFFFF);
	nes_cpu_alloc(nes);
	nes_ppu_alloc(nes);
	nes->rom = NULL;

	return nes;
}

void nes_free(NES *nes)
{
	if (!nes) return;

	if (nes->cpu) cpu6502_free(nes->cpu);
	if (nes->ppu) cpu6502_free(nes->ppu);
	if (nes->bus) bus6502_free(nes->bus);
	if (nes->rom) nes_rom_free(nes->rom);

	free(nes);
	nes = NULL;
}

void nes_cpu_alloc(NES *nes)
{
	if (!nes) return;

	nes-> cpu = cpu6502_alloc(nes->bus, NES_CPU_START_ADDR, NES_CPU_END_ADDR);
}

uint8_t nes_read(const NES *nes, uint16_t addr)
{
	if (addr <= NES_CPU_ADDR_MASK)
		return cpu6502_read(nes->cpu, addr);
	else if (addr >= NES_PPU_START_MAIN_ADDR && addr <= NES_PPU_END_ADDR)
		return nes->bus->ram[addr & NES_PPU_ADDR_MASK];
}

void nes_write(NES *nes, uint16_t addr, uint8_t data)
{
	if (addr <= NES_CPU_ADDR_MASK)
		cpu6502_write(nes->cpu, addr, data);
	else if (addr >= NES_PPU_START_MAIN_ADDR && addr <= NES_PPU_END_ADDR)
		nes->bus->ram[addr & NES_PPU_ADDR_MASK] = data;
}

uint8_t nes_ppu_read(const NES *nes, uint16_t addr)
{
	addr &= NES_PPU_END_ADDR;
	return 0;
}

// Write byte to RAM address
void nes_ppu_write(NES *nes, uint16_t addr, uint8_t data)
{
	addr &= NES_PPU_END_ADDR;
}
