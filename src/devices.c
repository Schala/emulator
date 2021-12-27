#include <stdio.h>
#include <stdlib.h>

#include "instructions.h"

BUS * bus_alloc()
{
	BUS *bus = (BUS *)calloc(0, sizeof(BUS));

	return bus;
}


void bus_add_device(BUS *bus, void *dev)
{
	if (!bus->dev_list)
		bus->dev_list = (DEV *)calloc(0, sizeof(DEV));

	DEV *it = bus->dev_list;
	while (it->next)
		it = it->next;

	it = (DEV *)calloc(0, sizeof(DEV));
	it->data = dev;
}


void * bus_device(BUS *bus, size_t index)
{
	DEV *it = bus->dev_list;
	while (index--)
		it = it->next;

	return it->data;
}

void bus_free(BUS *bus)
{
	DEV *it = NULL;
	DEV *next = bus->dev_list;

	while (next)
	{
		it = next;
		next = it->next;
		free(it);
	}

	free(bus);
}

void bus_free_device(BUS *bus, void *dev)
{
	DEV *it = bus->dev_list;
	DEV *prev = NULL;

	while (dev != it->data)
	{
		if (!it->next)
			break;

		prev = it;
		it = it->next;
	}

	// only free if we have a match
	if (dev == it->data)
	{
		if (prev)
		{
			DEV *next = it->next;
			free(it);
			prev->next = next;
		}
		else
			free(it);
	}
}

int bus_ram_dump(BUS *bus, size_t iter)
{
	char fmt[BUFSIZ];
	memset(&fmt, 0, BUFSIZ);
	sprintf(&fmt, "6502.%u.dmp", iter);

	FILE *dump = fopen(&fmt, "wb");
	fwrite(&bus->ram, 1, RAM_SIZE, dump);

	return fclose(dump);
}

CPU * cpu_alloc(BUS *bus)
{
	CPU *cpu = (CPU *)calloc(0, sizeof(CPU));
	cpu->bus = bus;

	cpu->ops = {
		{ 7, "BRK", &op_imp, &op_brk },
		{ 6, "ORA", &op_izx, &op_ora },
		{ 2, "???", &op_imp, &ill_op },
		{ 8, "???", &op_imp, &ill_op },
		{ 2, "???", &op_imp, &ill_op },
		{ 3, "ORA", &op_zp, &op_ora },
		{ 5, "ASL", &op_zp, &op_asl },
		{ 4, "???", &op_imp, &ill_op },
		{ 3, "PHP", &op_imp, &op_php },
		{ 2, "ORA", &op_imm, &op_ora },
		{ 5, "ASL", &op_zp, &op_asl },
	};

	bus_add_device(bus, cpu);

	return cpu;
}

void cpu_clock(CPU *cpu)
{
	if (cpu->cycles == 0)
	{
		// get and increment the counter
		cpu->last_op = cpu_read(cpu->regs.pc++);

		// set cycles, see if any additional cycles are needed
		cpu->cycles = cpu->ops[cpu->last_op].cycles;
		cpu->cycles += cpu->ops[cpu->last_op].addr_mode(cpu);
		cpu->cycles += cpu->ops[cpu->last_op].op(cpu);
	}

	cpu->cycles--;
}

uint8_t cpu_fetch(CPU *cpu)
{
	if (!cpu->ops[cpu->last_op].addr_mode == &op_imp)
		cpu->cache = cpu_read(cpu, cpu->last_abs_addr);

	return cpu->cache;
}

void cpu_free(CPU *cpu)
{
	bus_free_device(cpu);
	free(cpu);
}

void cpu_reset(CPU *cpu)
{
	cpu->regs.a = cpu->regs.x = cpu->regs.y = 0;
	cpu_flags_init(cpu);
	cpu->regs.sp = STACK_PTR_INIT;

	cpu->last_abs_addr = RESET_ADDR;
	cpu->regs.pc = cpu_fetch_addr(cpu);

	cpu->last_rel_addr = 0;
	cpu->last_abs_addr = 0;
	cpu->cache = 0;
	cpu->cycles = 8;
}
