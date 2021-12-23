#ifndef _DEVICES_H
#define _DEVICES_H

#include <stdint.h>

#define RAM_SIZE 65536
#define STACK_BASE_ADDR 256
#define OPCODE_TABLE_SZE 256
#define IRQ_ADDR 0xFFFE
#define NMI_ADDR 0xFFFA
#define RESET_ADDR 0xFFFC

#define ABS_HI(x) ((x) & 0xFF00)

typedef struct _DEV {
	void *data;
	_DEV *next;
} DEV;

typedef struct _BUS {
	uint8_t ram[RAM_SIZE];
	DEV *dev_list; // devices
} BUS;

BUS * bus_alloc();
void bus_add_device(BUS *, void *);
void * bus_device(BUS *, size_t);

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

// Registers
typedef struct _REGS {
	uint8_t a; // accumulator
	uint8_t x;
	uint8_t y;
	uint8_t sp; // stack pointer
	STATUS flags;
	uint16_t pc; // program counter
} REGS;

// CPU
typedef struct _CPU {
	uint8_t cache;
	uint8_t last_op;
	uint8_t next_cycles;
	uint16_t last_abs_addr;
	uint16_t last_rel_addr;
	BUS *bus;
	REGS regs;
} CPU;

// Opcodes
typedef struct _OPC {
	uint8_t cycles;
	char *sym;
	uint8_t (*addr_mode)(CPU *);
	uint8_t (*op)(CPU *);
} OPC;

CPU * cpu_alloc(BUS *);

inline uint8_t cpu_read(CPU *cpu, uint16_t addr)
{
	return cpu->bus->ram[addr];
}

inline void cpu_write(CPU *cpu, uint16_t addr, uint8_t data)
{
	cpu->bus->ram[addr] = data;
}

inline uint16_t cpu_fetch_addr(CPU *cpu)
{
	return (cpu_read(cpu, cpu->last_abs_addr) << 8) |  cpu_read(cpu, cpu->last_abs_addr + 1);
}

inline void cpu_stack_read(CPU *cpu)
{
	cpu_read(cpu, STACK_BASE_ADDR + (++cpu->regs.sp));
}

inline uint16_t cpu_stack_read_addr(CPU *cpu)
{
	return cpu_stack_read(cpu) | (cpu_stack_read(cpu) << 8);
}

inline void cpu_stack_write(CPU *cpu, uint8_t data)
{
	cpu_write(cpu, STACK_BASE_ADDR + (cpu->regs.sp--), data);
}

inline void cpu_stack_write_addr(CPU *cpu, uint16_t addr)
{
	cpu_stack_write(cpu, (addr >> 8) & 255);
	cpu_stack_write(cpu, addr & 255);
}

inline void cpu_interrupt(CPU *cpu, uint16_t new_abs_addr, uint8_t new_cycles)
{
	// write the counter's current value to stack
	cpu_stack_write_addr(cpu->regs.pc);

	// set and write flags register to stack too
	cpu->regs.flags.b = 0;
	cpu->regs.flags.u = 1;
	cpu->regs.flags.i = 1;
	cpu_stack_write(cpu, *(uint8_t *)&cpu->regs.flags);

	// get a new counter
	cpu->last_abs_addr = new_abs_addr;
	cpu->regs.pc = cpu_fetch_addr(cpu);

	cpu->cycles = new_cycles;
}

#endif // _DEVICES_H
