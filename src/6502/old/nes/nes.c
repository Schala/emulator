#include <stdlib.h>

#include "../../util.h"
#include "ppu.h"
#include "rom.h"
#include "nes.h"

NES * nes_alloc(SDL_Window *screen)
{
	NES *nes = (NES *)calloc(1, sizeof(NES));
	nes->bus = bus6502_alloc(0xFFFF);
	nes_cpu_alloc(nes);
	nes_ppu_alloc(nes, screen);

	return nes;
}

void nes_free(NES *nes)
{
	if (!nes) return;

	if (nes->cpu) cpu6502_free(nes->cpu);
	if (nes->rom) nes_rom_free(nes); // free ROM before PPU, or segfault
	if (nes->ppu) nes_ppu_free(nes);
	if (nes->bus) bus6502_free(nes->bus);

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

void nes_reset(NES *nes)
{
	if (!nes) return;

	cpu6502_reset(nes->cpu);
	nes->cycles = 0;
}

void nes_clock(NES *nes)
{
	if (!nes) return;

	nes_ppu_clock(nes);

	// CPU runs 3 times slower than PPU
	if (nes->cycles++ % 3 == 0)
		cpu6502_clock(nes->cpu);
}

void nes_print_prg(const NES *nes)
{
	hexdump(nes->rom->prg, nes->map->prg_size * nes->rom->header.prg_pages);
}

void nes_print_chr(const NES *nes)
{
	hexdump(nes->rom->chr, nes->map->chr_size * nes->rom->header.chr_pages);
}
