#ifndef _NES_H
#define _NES_H

#include <SDL2/SDL.h>

#include "../cpu.h"

// Address mask for a CPU in RAM
#define NES_CPU_ADDR_MASK 0x7FF

// offset in RAM to load ROM
#define NES_ROM_ADDR 0x4020

#define NTSC_FPS 60.0f

typedef struct _NES_ROM NES_ROM;
typedef struct _PPU_2C02 PPU_2C02;

typedef struct _NES_MAPPER
{
	uint16_t prg_size;
	uint16_t prg_start_addr;
	uint16_t prg_end_addr;
	uint16_t chr_size;
	uint16_t chr_start_addr;
	uint16_t chr_end_addr;
	uint16_t ram_start_addr;
	uint16_t ram_end_addr;
	const char *id;
} NES_MAPPER;

// The NES system itself
typedef struct _NES
{
	size_t cycles; // global cycle count
	BUS_6502 *bus;
	CPU_6502 *cpu;
	PPU_2C02 *ppu;
	NES_ROM *rom;
	NES_MAPPER *map;
} NES;

// Allocate the NES system
NES * nes_alloc(SDL_Window *);

// Allocates the NES CPU, mapped appropriately in RAM
void nes_cpu_alloc(NES *);

// Deallocate the NES system
void nes_free(NES *);

// Read byte from RAM address
uint8_t nes_read(const NES *, uint16_t);

// Write byte to RAM address
void nes_write(NES *, uint16_t, uint8_t);

// print ROM info to console
void nes_print_rom_info(const NES *);

// Runs NES clocks
void nes_clock(NES *);

// Resets the NES
void nes_reset(NES *);

// Map an address to the CPU
int nes_map_cpu(NES *, uint16_t, uint32_t *);

// Prints PRG memory
void nes_print_prg(const NES *);

// Prints CHR memory
void nes_print_chr(const NES *);

#endif // _NES_H
