#include <stdio.h>
#include <stdlib.h>

#include "../util.h"
#include "nes.h"

static NES_MAPPER MAPPERS[] = {
	// 0x
	{ 16384, 0x8000, 0xFFFF, 8192, 0, 0x1FFF, 0, 0x1FFF, "NROM" },

	// Ax
	// a0
	// a1
	{ 32768, 0x8000, 0xFFFF, 8192, 0, 0x1FFF, 0x6000, 0x7FFF, "FS304" }
};

void nes_ppu_alloc(NES *nes)
{
	if (!nes) return;

	nes->ppu = (NES_PPU *)calloc(1, sizeof(NES_PPU));
	nes->ppu->bus = bus6502_alloc(NES_PPU_BUS_RAM_SIZE);
	nes->ppu->cpu_node = bus6502_add_device(nes->bus, nes->ppu, NES_PPU_START_MAIN_ADDR, NES_PPU_END_MAIN_ADDR);
	nes->ppu->ppu_node = bus6502_add_device(nes->ppu->bus, nes->ppu, 0, 0);
}

void nes_ppu_free(NES *nes)
{
	if (!nes) return;

	bus6502_free_device(nes->bus, nes->ppu);
	bus6502_free(nes->ppu->bus); // no need to free device from its own exclusive bus
	free(nes->ppu);
	nes->ppu = NULL;
}

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
	fread(nes->rom->prg, header.chr_pages, nes->map->chr_size, f);

	fclose(f);

	nes->rom->header = header;

	// memory mapping as per mapper specs
	cpu6502_map(nes->cpu, nes->map->ram_start_addr, nes->map->ram_end_addr);
	nes->ppu->ppu_node->ram_offset = nes->map->chr_start_addr;
	nes->ppu->ppu_node->ram_size = nes->map->chr_end_addr + 1;

	//nes->rom->prg_node = bus6502_add_device(nes->bus, nes->rom, NES_ROM_ADDR, 0xFFFF);
}

void nes_rom_free(NES *nes)
{
	if (!nes) return;
	if (!nes->rom) return;

	bus6502_free_device(nes->bus, nes->rom);
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

NES * nes_alloc()
{
	NES *nes = (NES *)malloc(sizeof(NES));
	nes->bus = bus6502_alloc(0xFFFF);
	nes_cpu_alloc(nes);
	nes_ppu_alloc(nes);
	nes->rom = NULL;
	nes->map = NULL;

	return nes;
}

void nes_free(NES *nes)
{
	if (!nes) return;

	if (nes->cpu) cpu6502_free(nes->cpu);
	if (nes->ppu) nes_ppu_free(nes);
	if (nes->bus) bus6502_free(nes->bus);
	if (nes->rom) nes_rom_free(nes);

	free(nes);
	nes = NULL;
}

void nes_cpu_alloc(NES *nes)
{
	if (!nes) return;

	nes-> cpu = cpu6502_alloc(nes->bus, 0, 0);
}

uint8_t nes_read(const NES *nes, uint16_t addr)
{
	if (addr <= nes->map->ram_end_addr)
		return cpu6502_read(nes->cpu, addr & NES_CPU_ADDR_MASK);
	else if (addr >= NES_PPU_START_MAIN_ADDR && addr <= NES_PPU_END_MAIN_ADDR)
		return nes->bus->ram[addr & NES_PPU_ADDR_MASK];
}

void nes_write(NES *nes, uint16_t addr, uint8_t data)
{
	if (addr <= nes->map->ram_end_addr)
		cpu6502_write(nes->cpu, addr & NES_CPU_ADDR_MASK, data);
	else if (addr >= NES_PPU_START_MAIN_ADDR && addr <= NES_PPU_END_MAIN_ADDR)
		nes->bus->ram[addr & NES_PPU_ADDR_MASK] = data;
}

uint8_t nes_ppu_read(const NES *nes, uint16_t addr)
{
	addr &= nes->map->chr_end_addr;
	return 0;
}

void nes_ppu_write(NES *nes, uint16_t addr, uint8_t data)
{
	addr &= nes->map->chr_end_addr;
}

uint8_t nes_ppu_read_cpu(NES *nes, uint16_t addr)
{
	uint8_t data = 0;

	switch (addr)
	{
		case 0: break; // control
		case 1: break; // mask
		case 2: break; // status
		case 3: break; // OAM address
		case 4: break; // OAM data
		case 5: break; // scroll
		case 6: break; // PPU address
		case 7: break; // PPU data
	}

	return data;
}

void nes_ppu_write_cpu(NES *nes, uint16_t addr, uint8_t data)
{
	switch (addr)
	{
		case 0: break; // control
		case 1: break; // mask
		case 2: break; // status
		case 3: break; // OAM address
		case 4: break; // OAM data
		case 5: break; // scroll
		case 6: break; // PPU address
		case 7: break; // PPU data
	}
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

int nes_map_cpu(NES *nes, uint16_t addr, uint32_t *mapped_addr)
{
	if (addr >= nes->map->prg_start_addr && addr <= nes->map->prg_end_addr)
	{
		*mapped_addr = addr & nes->rom->header.prg_pages > 1 ? nes->map->prg_size - 1 :
			(nes->map->prg_size >> 2) - 1;
		return 1;
	}

	return 0;
}

int nes_map_ppu(NES *nes, uint16_t addr, uint32_t *mapped_addr)
{
	if (addr >= nes->map->chr_start_addr && addr <= nes->map->chr_end_addr)
	{
		*mapped_addr = addr & nes->rom->header.chr_pages > 1 ? nes->map->chr_size - 1 :
			(nes->map->chr_size >> 2) - 1;
		return 1;
	}

	return 0;
}

void nes_print_prg(const NES *nes)
{
	hexdump(nes->rom->prg, nes->map->prg_size * nes->rom->header.prg_pages);
}

void nes_print_chr(const NES *nes)
{
	hexdump(nes->rom->chr, nes->map->chr_size * nes->rom->header.chr_pages);
}
