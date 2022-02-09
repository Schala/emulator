#include "nes.h"
#include "ppu.h"
#include "rom.h"

static const SDL_Color PALETTE[] = {
	{ 84, 84, 84, SDL_ALPHA_OPAQUE },
	{ 0, 30, 116, SDL_ALPHA_OPAQUE },
	{ 8, 16, 144, SDL_ALPHA_OPAQUE },
	{ 48, 0, 136, SDL_ALPHA_OPAQUE },
	{ 68, 0, 100, SDL_ALPHA_OPAQUE },
	{ 92, 0, 48, SDL_ALPHA_OPAQUE },
	{ 84, 4, 0, SDL_ALPHA_OPAQUE },
	{ 60, 24, 0, SDL_ALPHA_OPAQUE },
	{ 32, 42, 0, SDL_ALPHA_OPAQUE },
	{ 8, 58, 0, SDL_ALPHA_OPAQUE },
	{ 0, 64, 0, SDL_ALPHA_OPAQUE },
	{ 0, 60, 0, SDL_ALPHA_OPAQUE },
	{ 0, 50, 60, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },

	{ 152, 150, 152, SDL_ALPHA_OPAQUE },
	{ 8, 76, 196, SDL_ALPHA_OPAQUE },
	{ 48, 50, 236, SDL_ALPHA_OPAQUE },
	{ 92, 30, 228, SDL_ALPHA_OPAQUE },
	{ 136, 20, 176, SDL_ALPHA_OPAQUE },
	{ 160, 20, 100, SDL_ALPHA_OPAQUE },
	{ 152, 34, 32, SDL_ALPHA_OPAQUE },
	{ 120, 60, 0, SDL_ALPHA_OPAQUE },
	{ 84, 90, 0, SDL_ALPHA_OPAQUE },
	{ 40, 114, 0, SDL_ALPHA_OPAQUE },
	{ 8, 124, 0, SDL_ALPHA_OPAQUE },
	{ 0, 118, 40, SDL_ALPHA_OPAQUE },
	{ 0, 102, 120, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },

	{ 236, 238, 236, SDL_ALPHA_OPAQUE },
	{ 76, 154, 236, SDL_ALPHA_OPAQUE },
	{ 120, 124, 236, SDL_ALPHA_OPAQUE },
	{ 176, 98, 236, SDL_ALPHA_OPAQUE },
	{ 228, 84, 236, SDL_ALPHA_OPAQUE },
	{ 236, 88, 180, SDL_ALPHA_OPAQUE },
	{ 236, 106, 100, SDL_ALPHA_OPAQUE },
	{ 212, 136, 32, SDL_ALPHA_OPAQUE },
	{ 160, 170, 0, SDL_ALPHA_OPAQUE },
	{ 116, 196, 0, SDL_ALPHA_OPAQUE },
	{ 76, 208, 32, SDL_ALPHA_OPAQUE },
	{ 56, 204, 108, SDL_ALPHA_OPAQUE },
	{ 56, 180, 204, SDL_ALPHA_OPAQUE },
	{ 60, 60, 60, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },

	{ 236, 238, 236, SDL_ALPHA_OPAQUE },
	{ 168, 204, 236, SDL_ALPHA_OPAQUE },
	{ 188, 188, 236, SDL_ALPHA_OPAQUE },
	{ 212, 178, 236, SDL_ALPHA_OPAQUE },
	{ 236, 174, 236, SDL_ALPHA_OPAQUE },
	{ 236, 174, 212, SDL_ALPHA_OPAQUE },
	{ 236, 180, 176, SDL_ALPHA_OPAQUE },
	{ 228, 196, 144, SDL_ALPHA_OPAQUE },
	{ 204, 210, 120, SDL_ALPHA_OPAQUE },
	{ 180, 222, 120, SDL_ALPHA_OPAQUE },
	{ 168, 226, 144, SDL_ALPHA_OPAQUE },
	{ 152, 226, 180, SDL_ALPHA_OPAQUE },
	{ 160, 214, 228, SDL_ALPHA_OPAQUE },
	{ 160, 162, 160, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },
	{ 0, 0, 0, SDL_ALPHA_OPAQUE },
};

void nes_ppu_alloc(NES *nes, SDL_Window *screen)
{
	if (!nes) return;

	nes->ppu = (PPU_2C02 *)calloc(1, sizeof(PPU_2C02));
	nes->ppu->bus = bus6502_alloc(NES_PPU_BUS_RAM_SIZE);
	nes->ppu->cpu_node = bus6502_add_device(nes->bus, nes->ppu, NES_PPU_START_MAIN_ADDR, NES_PPU_END_MAIN_ADDR);
	nes->ppu->ppu_node = bus6502_add_device(nes->ppu->bus, nes->ppu, 0, 0);

	nes->ppu->palette = PALETTE;
	nes->ppu->screen = screen;
	nes->ppu->renderer = SDL_CreateRenderer(screen, -1, 0);
	nes->ppu->name_tbl[0] = sprite_alloc(NES_SCREEN_WIDTH, NES_SCREEN_HEIGHT);
	nes->ppu->name_tbl[1] = sprite_alloc(NES_SCREEN_WIDTH, NES_SCREEN_HEIGHT);
	nes->ppu->pat_tbl[0] = sprite_alloc(NES_PATTERN_DIMENSION, NES_PATTERN_DIMENSION);
	nes->ppu->pat_tbl[1] = sprite_alloc(NES_PATTERN_DIMENSION, NES_PATTERN_DIMENSION);

	// Give the renderer a solid fill colour instead of copying what's underneath
	SDL_SetRenderDrawColor(nes->ppu->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(nes->ppu->renderer);
}

void nes_ppu_clock(NES *nes)
{
	if (!nes) return;

	if (++nes->ppu->cycle >= NES_PPU_MAX_CYCLES)
	{
		nes->ppu->cycle = 0;

		if (++nes->ppu->scanline >= NES_MAX_SCANLINES)
		{
			nes->ppu->scanline = -1;
			nes->ppu->flags.frame_complete = 1;
		}
	}
}

void nes_ppu_free(NES *nes)
{
	if (!nes) return;

	bus6502_free_device(nes->bus, nes->ppu);
	bus6502_free(nes->ppu->bus); // no need to free PPU from its own exclusive bus

	free(nes->ppu->name_tbl[0]);
	free(nes->ppu->name_tbl[1]);
	free(nes->ppu->pat_tbl[0]);
	free(nes->ppu->pat_tbl[1]);
	SDL_DestroyRenderer(nes->ppu->renderer);

	free(nes->ppu);
	nes->ppu = NULL;
}

void nes_ppu_noise_test(NES *nes)
{
	if (!nes) return;

	SDL_Color c = nes->ppu->palette[(rand() % 2) ? 63 : 48];
	SDL_SetRenderDrawColor(nes->ppu->renderer, c.r, c.g, c.b, c.a);
	SDL_RenderDrawPoint(nes->ppu->renderer, nes->ppu->cycle - 1, nes->ppu->scanline);
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
