#ifndef _DEVICES_H
#define _DEVICES_H

#include <stdint.h>
#include <string.h>

#define RAM_SIZE 65536
#define STACK_BASE_ADDR 256
#define STACK_PTR_INIT 254
#define RESET_ADDR 0xFFFC
#define MAGIC_VAL 255

#define ABS_HI(x) ((x) & 0xFF00)

typedef struct _DEV DEV;

// Bus device tree node
struct _DEV {
	void *data; // Actual device pointer
	DEV *next;
};

// The bus is responsible for making available data to various devices
typedef struct _BUS {
	uint8_t ram[RAM_SIZE];
	DEV *dev_list;
} BUS;

// Allocate a new bus.
BUS * bus_alloc();

// Add a device to a bus.
void bus_add_device(BUS *, void *);

// Get a device from bus's device tree at given index
void * bus_device(BUS *, size_t);

// Deallocate bus
void bus_free(BUS *);

// Remove device from bus device tree
void bus_free_device(BUS *, void *);

// Dump bus RAM to file with iteration appended to filename
int bus_ram_dump(BUS *, size_t);

// Register status flags
typedef struct _STATUS {
	uint8_t
		c : 1, // carry
		z : 1, // zero
		i : 1, // no interrupts
		d : 1, // decimal mode
		b : 1, // break
		u : 1, // unused
		v : 1, // overflow
		n : 1; // negative
} STATUS;

// CPU registers
typedef struct _REGS {
	uint8_t a; // accumulator
	uint8_t x;
	uint8_t y;
	uint8_t sp; // stack pointer
	STATUS flags;
	uint16_t pc; // program counter
} REGS;

typedef struct _CPU CPU;

// Metadata for the CPU's various operations
typedef struct _OPC {
	uint8_t cycles;
	char *sym; // mneumonic for (dis)assembly
	uint8_t (*addr_mode)(CPU *);
	uint8_t (*op)(CPU *);
} OPC;

// The CPU processes data available via its bus
struct _CPU {
	uint8_t cache;
	uint8_t cycles; // remaining cycles for current operation
	uint8_t last_op;
	uint16_t last_abs_addr;
	uint16_t last_rel_addr;
	BUS *bus;
	const OPC *ops;
	REGS regs;
};

// Allocate a new CPU, given a parent bus
CPU * cpu_alloc(BUS *);

// CPU clock operation
void cpu_clock(CPU *);

// Fetch and cache a byte from the cached absolute address
uint8_t cpu_fetch(CPU *);

// Deallocate CPU
void cpu_free(CPU *);

// Reset CPU state
void cpu_reset(CPU *);

// Common functionality for branch instructions
static inline void cpu_branch(CPU *cpu)
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
static inline uint8_t cpu_read(const CPU *cpu, uint16_t addr)
{
	return cpu->bus->ram[addr];
}

// Read address from RAM address
static inline uint16_t cpu_read_addr(const CPU *cpu, uint16_t addr)
{
	return cpu_read(cpu, addr) | (cpu_read(cpu, addr) << 8);
}

// Read byte from ROM
static inline uint8_t cpu_read_rom(CPU *cpu)
{
	return cpu_read(cpu, cpu->regs.pc++);
}

// Read address from ROM
static inline uint16_t cpu_read_rom_addr(CPU *cpu)
{
	return (cpu_read_rom(cpu) << 8) | cpu_read_rom(cpu);
}

// Write byte to RAM address
static inline void cpu_write(CPU *cpu, uint16_t addr, uint8_t data)
{
	cpu->bus->ram[addr] = data;
}

// Write byte to last used address
static inline void cpu_write_last(CPU *cpu, uint8_t data)
{
	cpu_write(cpu, cpu->last_abs_addr, data);
}

// Read address from RAM
static inline uint16_t cpu_fetch_addr(const CPU *cpu)
{
	return (cpu_read(cpu, cpu->last_abs_addr) << 8) |  cpu_read(cpu, cpu->last_abs_addr + 1);
}

// Return status flags register as a byte
static inline uint8_t cpu_flags(const CPU *cpu)
{
	return *(uint8_t *)&cpu->regs.flags;
}

// (Re)initialise status flags register
static inline void cpu_flags_init(CPU *cpu)
{
	memset(&cpu->regs.flags, 0, sizeof(STATUS));
	cpu->regs.flags.u = 1;
}

// Set carry, negative, and/or zero bits of status flags register, given a 16-bit value
static inline void cpu_flags_cnz(CPU *cpu, uint16_t value)
{
	cpu->regs.flags.c = value > 255 ? 1 : 0;
	cpu->regs.flags.z = value & 255 == 0 ? 1 : 0;
	cpu->regs.flags.n = value & 128 ? 1 : 0;
}

// Set negative and/or zero bits of status flags register, given a value
static inline void cpu_flags_nz(CPU *cpu, uint16_t value)
{
	cpu->regs.flags.z = (value & 255) == 0 ? 1 : 0;
	cpu->regs.flags.n = value & 128 ? 1 : 0;
}

// Unstable magic value equation for some illegal opcodes
static inline uint8_t cpu_magic(CPU *cpu)
{
	return (cpu->regs.a | MAGIC_VAL);
}

// Read byte from stack
static inline uint8_t cpu_stack_read(CPU *cpu)
{
	return cpu_read(cpu, STACK_BASE_ADDR + (++cpu->regs.sp));
}

// Read address from stack
static inline uint16_t cpu_stack_read_addr(CPU *cpu)
{
	return cpu_stack_read(cpu) | (cpu_stack_read(cpu) << 8);
}

// Write byte to stack
static inline void cpu_stack_write(CPU *cpu, uint8_t data)
{
	cpu_write(cpu, STACK_BASE_ADDR + (cpu->regs.sp--), data);
}

// Write address to stack
static inline void cpu_stack_write_addr(CPU *cpu, uint16_t addr)
{
	cpu_stack_write(cpu, (addr >> 8) & 255);
	cpu_stack_write(cpu, addr & 255);
}

// Common functionality for interrupt operations
static inline void cpu_interrupt(CPU *cpu, uint16_t new_abs_addr, uint8_t new_cycles)
{
	// write the counter's current value to stack
	cpu_stack_write_addr(cpu, cpu->regs.pc);

	// set and write flags register to stack too
	cpu->regs.flags.b = 0;
	cpu->regs.flags.u = 1;
	cpu->regs.flags.i = 1;
	cpu_stack_write(cpu, cpu_flags(cpu));

	// get a new counter
	cpu->last_abs_addr = new_abs_addr;
	cpu->regs.pc = cpu_fetch_addr(cpu);

	cpu->cycles = new_cycles;
}

#endif // _DEVICES_H
