#ifndef _NES_H
#define _NES_H

#include "dev.h"

// ROM file header magic
#define NES_ROM_MAGIC "NES\x1A"

// Address mask for a CPU in RAM
#define NES_CPU_ADDR_MASK 0x7FF

// Address mask for a CPU in RAM
#define NES_PPU_ADDR_MASK 7

// Starting offset in RAM for PPU
#define NES_PPU_START_MAIN_ADDR 0x2000

// End offset for a PPU in RAM
#define NES_PPU_END_ADDR 0x3FFF

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

// PPU palette size in bytes
#define NES_PALETTE_SIZE 32

// offset in RAM to load ROM
#define NES_ROM_ADDR 0x4020

// Size of one program memory page
#define NES_PRG_PAGE_SIZE 16384

// Size of one character memory page
#define NES_CHR_PAGE_SIZE 8192

// 2C02 picture processing unit
typedef struct _NES_PPU
{
	uint8_t vram[NES_VRAM_SIZE];
	uint8_t palette[NES_PALETTE_SIZE];
	BUS *bus; // PPU's own bus
	DEV_6502 *cpu_node; // device node in CPU bus device tree
	DEV_6502 *ppu_node; // device node in its own bus
} NES_PPU;

// ROM header flags
typedef struct _NES_ROM_MAPPER_INFO
{
	uint8_t
		mirror_vertical : 1,
		battery_backed_ram : 1, // 6000-7FFF
		trainer : 1, // 7000-71FF
		four_screen_vram_layout : 1,
		type_lo: 4,
		vs_system_cart : 1,
		reserved : 3,
		type_hi: 4;
} NES_ROM_MAPPER_INFO;

// ROM file header
typedef struct _NES_ROM_HEADER
{
	char magic[4]; // must be NES_ROM_MAGIC
	uint8_t prg_pages; // ROM data
	uint8_t chr_pages; // graphics data
	NES_ROM_MAPPER_INFO mapper_info;
	uint8_t ram_pages;
	uint8_t is_pal;
	uint8_t reserved[6];
} NES_ROM_HEADER;

// NES ROM (cartridge)
typedef struct _NES_ROM
{
	uint8_t mapper_id;
	DEV_6502 *prg_node; // program
	DEV_6502 *chr_node; // sprite patterns
	uint8_t *prg; // PRG memory
	uint8_t *chr; // CHR memory
	NES_ROM_HEADER header;
} NES_ROM;

// The NES system itself
typedef struct _NES
{
	BUS_6502 *bus;
	CPU_6502 *cpu;
	NES_PPU *ppu;
	NES_ROM *rom;
} NES;

// Allocate the NES system
NES * nes_alloc();

// Allocates the NES CPU, mapped appropriately in RAM
CPU_6502 * nes_cpu_alloc(NES *);

// Deallocate the NES system
void nes_free(NES *);

// Validate ROM and load into RAM, returning a new ROM pointer
NES_ROM * nes_rom_alloc(NES *, const char *);

// Deallocate ROM
void nes_rom_free(NES *);

// Initialise a new PPU on the NES
void nes_ppu_alloc(NES *);

// Deallocate PPU
void nes_ppu_free(NES *);

// Read byte from RAM address
uint8_t nes_read(const NES *, uint16_t);

// Write byte to RAM address
void nes_write(NES *, uint16_t, uint8_t);

// Read byte from RAM address
uint8_t nes_ppu_read(const NES *, uint16_t);

// Write byte to RAM address
void nes_ppu_write(NES *, uint16_t, uint8_t);

// print ROM info to console
void nes_print_rom_info(const NES *);

#endif // _NES_H
