#ifndef _6502_BUS_H
#define _6502_BUS_H

#include <stdint.h>
#include <string.h>

// Absolute address when reset is called
#define RESET_ADDR_6502 0xFFFC

// Bus device tree node
typedef struct _DEV_6502
{
	uint16_t ram_offset; // start offset in RAM on bus
	uint16_t ram_size; // how much RAM does the device use?
	void *data; // Actual device pointer
	struct _DEV_6502 *next;
} DEV_6502;

// The bus is responsible for making available data to various devices
typedef struct _BUS_6502
{
	uint16_t ram_size;
	uint8_t *ram;
	DEV_6502 *dev_list;
} BUS_6502;

// Allocate a new bus with the specified RAM amount
BUS_6502 * bus6502_alloc(uint16_t);

// Add a device to a bus mapped to an offset in RAM and how much RAM used
DEV_6502 * bus6502_add_device(BUS_6502 *, void *, uint16_t, uint16_t);

// Get a device from bus's device tree at given index
void * bus6502_device(BUS_6502 *, size_t);

// Deallocate bus
void bus6502_free(BUS_6502 *);

// Remove device from bus device tree
void bus6502_free_device(BUS_6502 *, void *);

// Loads into RAM
static inline uint8_t * bus6502_load(BUS_6502 *bus, const uint8_t *data, size_t length, uint16_t addr)
{
	return (uint8_t *)memcpy(&bus->ram[addr], data, length);
}

// Print RAM to stdout
void bus6502_print_ram(const BUS_6502 *);

// Dump bus RAM to file with iteration appended to filename
int bus6502_ram_dump(const BUS_6502 *, size_t);

// Sets the reset vector in RAM
static inline uint8_t * bus6502_reset_vec(BUS_6502 *bus, uint16_t addr)
{
	return (uint8_t *)memcpy(&bus->ram[RESET_ADDR_6502], &addr, 2);
}

#endif // _6502_BUS_H
