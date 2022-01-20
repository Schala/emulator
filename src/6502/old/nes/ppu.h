#ifndef _NES_PPU_H
#define _NES_PPU_H

#include "../../eng.h"
#include "nes.h"

// Address mask for a PPU in RAM
#define NES_PPU_ADDR_MASK 7

// Starting offset in RAM for PPU
#define NES_PPU_START_MAIN_ADDR 0x2000

// End offset for a PPU in RAM
#define NES_PPU_END_MAIN_ADDR 0x3FFF

// PPU bus ram size
#define NES_PPU_BUS_RAM_SIZE 0x4000

// Staring offset in RAM for a nametable
#define NES_VRAM_START_ADDR 0x2000

// End offset for a nametable in RAM
#define NES_VRAM_END_ADDR 0x2FFF

// VRAM size for PPU
#define NES_VRAM_SIZE 2048

// offset in RAM for a palette
#define NES_PALETTE_ADDR 0x3F00

// Max cycles in a screen row
#define NES_PPU_MAX_CYCLES 341

// Max scanlines in a frame
#define NES_MAX_SCANLINES 261

#define NES_SCREEN_WIDTH 256

#define NES_SCREEN_HEIGHT 240

#define NES_PATTERN_DIMENSION 128

// State of PPU
typedef struct _STATE_2C02
{
	uint8_t
		frame_complete : 1;
} STATE_2C02;

// 2C02 picture processing unit
struct _PPU_2C02
{
	STATE_2C02 flags;
	uint8_t vram[NES_VRAM_SIZE];
	int16_t scanline; // screen row
	int16_t cycle; // screen column
	const SDL_Color *palette;
	SDL_Window *screen;
	SDL_Renderer *renderer;
	SPRITE *name_tbl[2];
	SPRITE *pat_tbl[2];
	BUS_6502 *bus; // PPU's own bus
	DEV_6502 *cpu_node; // device node in CPU bus device tree
	DEV_6502 *ppu_node; // device node in its own bus
};

// Initialise a new PPU on the NES
void nes_ppu_alloc(NES *, SDL_Window *);

// Advance the clock, which is relentless
void nes_ppu_clock(NES *);

// Deallocate PPU
void nes_ppu_free(NES *);

// Test noise
void nes_ppu_noise_test(NES *);

// PPU read from CPU
uint8_t nes_ppu_read_cpu(NES *, uint16_t);

// PPU write to CPU
void nes_ppu_write_cpu(NES *, uint16_t, uint8_t);

// Read byte from RAM address
uint8_t nes_ppu_read(const NES *, uint16_t);

// Write byte to RAM address
void nes_ppu_write(NES *, uint16_t, uint8_t);

// Map an address to the PPU
int nes_map_ppu(NES *, uint16_t, uint32_t *);

// Update the renderer
static inline void nes_ppu_update(NES *nes)
{
	SDL_RenderPresent(nes->ppu->renderer);
}

#endif // _NES_PPU_H
