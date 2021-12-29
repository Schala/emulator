#ifndef _6502_DEVICES_H
#define _6502_DEVICES_H

#include <stdint.h>
#include <string.h>

#define RAM_SIZE_6502 65536
#define STACK_BASE_ADDR_6502 256
#define STACK_PTR_INIT_6502 254
#define RESET_ADDR_6502 0xFFFC
#define MAGIC_VAL_6502 255

#define ABS16_HI(x) ((x) & 0xFF00)

typedef struct _DEV_6502 DEV_6502;

// Bus device tree node
struct _DEV_6502
{
	void *data; // Actual device pointer
	DEV_6502 *next;
};

// The bus is responsible for making available data to various devices
typedef struct _BUS_6502
{
	uint8_t ram[RAM_SIZE];
	DEV_6502 *dev_list;
} BUS_6502;

// Allocate a new bus.
BUS_6502 * bus6502_alloc();

// Add a device to a bus.
void bus6502_add_device(BUS_6502 *, void *);

// Get a device from bus's device tree at given index
void * bus6502_device(BUS_6502 *, size_t);

// Deallocate bus
void bus6502_free(BUS_6502 *);

// Remove device from bus device tree
void bus6502_free_device(BUS_6502 *, void *);

// Dump bus RAM to file with iteration appended to filename
int bus6502_ram_dump(BUS_6502 *, size_t);

// Register status flags
typedef struct _STATUS_6502
{
	uint8_t
		c : 1, // carry
		z : 1, // zero
		i : 1, // no interrupts
		d : 1, // decimal mode
		b : 1, // break
		u : 1, // unused
		v : 1, // overflow
		n : 1; // negative
} STATUS_6502;

// CPU registers
typedef struct _REGS_6502
{
	uint8_t a; // accumulator
	uint8_t x;
	uint8_t y;
	uint8_t sp; // stack pointer
	STATUS_6502 flags;
	uint16_t pc; // program counter
} REGS_6502;

typedef struct _CPU_6502 CPU_6502;

// Metadata for the CPU's various operations
typedef struct _OPC_6502
{
	uint8_t cycles;
	char *sym; // mneumonic for (dis)assembly
	uint8_t (*addr_mode)(CPU_6502 *);
	uint8_t (*op)(CPU_6502 *);
} OPC_6502;

// The CPU processes data available via its bus
struct _CPU_6502
{
	uint8_t cache;
	uint8_t cycles; // remaining cycles for current operation
	uint8_t last_op;
	uint16_t last_abs_addr;
	uint16_t last_rel_addr;
	BUS_6502 *bus;
	const OPC_6502 *ops;
	REGS_6502 regs;
};

// Allocate a new CPU, given a parent bus
CPU_6502 * cpu6502_alloc(BUS_6502 *);

// CPU clock operation
void cpu6502_clock(CPU_6502 *);

// Fetch and cache a byte from the cached absolute address
uint8_t cpu6502_fetch(CPU_6502 *);

// Deallocate CPU
void cpu6502_free(CPU_6502 *);

// Reset CPU state
void cpu6502_reset(CPU_6502 *);

// Common functionality for branch instructions
static inline void cpu6502_branch(CPU_6502 *cpu)
{
	cpu->cycles++;
	cpu->last_abs_addr = cpu->regs.pc + cpu->last_rel_addr;

	// need an additional cycle if different page
	if (ABS_HI(cpu->last_abs_addr) != ABS_HI(cpu->regs.pc))
		cpu->cycles++;

	// jump to address
	cpu->regs.pc = cpu->last_abs_addr;
}

// Read byte from RAM address
static inline uint8_t cpu6502_read(const CPU_6502 *cpu, uint16_t addr)
{
	return cpu->bus->ram[addr];
}

// Read address from RAM address
static inline uint16_t cpu6502_read_addr(const CPU_6502 *cpu, uint16_t addr)
{
	return cpu6502_read(cpu, addr) | (cpu6502_read(cpu, addr) << 8);
}

// Read byte from ROM
static inline uint8_t cpu6502_read_rom(CPU_6502 *cpu)
{
	return cpu6502_read(cpu, cpu->regs.pc++);
}

// Read address from ROM
static inline uint16_t cpu6502_read_rom_addr(CPU_6502 *cpu)
{
	return (cpu6502_read_rom(cpu) << 8) | cpu6502_read_rom(cpu);
}

// Write byte to RAM address
static inline void cpu6502_write(CPU_6502 *cpu, uint16_t addr, uint8_t data)
{
	cpu->bus->ram[addr] = data;
}

// Write byte to last used address
static inline void cpu6502_write_last(CPU_6502 *cpu, uint8_t data)
{
	cpu6502_write(cpu, cpu->last_abs_addr, data);
}

// Read address from RAM
static inline uint16_t cpu6502_fetch_addr(const CPU_6502 *cpu)
{
	return (cpu6502_read(cpu, cpu->last_abs_addr) << 8) |  cpu6502_read(cpu, cpu->last_abs_addr + 1);
}

// Return status flags register as a byte
static inline uint8_t cpu6502_flags(const CPU_6502 *cpu)
{
	return *(uint8_t *)&cpu->regs.flags;
}

// (Re)initialise status flags register
static inline void cpu6502_flags_init(CPU_6502 *cpu)
{
	memset(&cpu->regs.flags, 0, sizeof(STATUS_6502));
	cpu->regs.flags.u = 1;
}

// Set carry, negative, and/or zero bits of status flags register, given a 16-bit value
static inline void cpu6502_flags_cnz(CPU_6502 *cpu, uint16_t value)
{
	cpu->regs.flags.c = value > 255 ? 1 : 0;
	cpu->regs.flags.z = value & 255 == 0 ? 1 : 0;
	cpu->regs.flags.n = value & 128 ? 1 : 0;
}

// Set negative and/or zero bits of status flags register, given a value
static inline void cpu6502_flags_nz(CPU_6502 *cpu, uint16_t value)
{
	cpu->regs.flags.z = (value & 255) == 0 ? 1 : 0;
	cpu->regs.flags.n = value & 128 ? 1 : 0;
}

// Unstable magic value equation for some illegal opcodes
static inline uint8_t cpu6502_magic(CPU_6502 *cpu)
{
	return (cpu->regs.a | MAGIC_VAL_6502);
}

// Read byte from stack
static inline uint8_t cpu6502_stack_read(CPU_6502 *cpu)
{
	return cpu6502_read(cpu, STACK_BASE_ADDR_6502 + (++cpu->regs.sp));
}

// Read address from stack
static inline uint16_t cpu6502_stack_read_addr(CPU_6502 *cpu)
{
	return cpu6502_stack_read(cpu) | (cpu6502_stack_read(cpu) << 8);
}

// Write byte to stack
static inline void cpu6502_stack_write(CPU_6502 *cpu, uint8_t data)
{
	cpu6502_write(cpu, STACK_BASE_ADDR_6502 + (cpu->regs.sp--), data);
}

// Write address to stack
static inline void cpu6502_stack_write_addr(CPU_6502 *cpu, uint16_t addr)
{
	cpu6502_stack_write(cpu, (addr >> 8) & 255);
	cpu6502_stack_write(cpu, addr & 255);
}

// Common functionality for interrupt operations
static inline void cpu6502_interrupt(CPU_6502 *cpu, uint16_t new_abs_addr, uint8_t new_cycles)
{
	// write the counter's current value to stack
	cpu6502_stack_write_addr(cpu, cpu->regs.pc);

	// set and write flags register to stack too
	cpu->regs.flags.b = 0;
	cpu->regs.flags.u = 1;
	cpu->regs.flags.i = 1;
	cpu6502_stack_write(cpu, cpu6502_flags(cpu));

	// get a new counter
	cpu->last_abs_addr = new_abs_addr;
	cpu->regs.pc = cpu6502_fetch_addr(cpu);

	cpu->cycles = new_cycles;
}

#endif // _6502_DEVICES_H
