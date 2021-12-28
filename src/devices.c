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
		// 0X
		{ 7, "BRK", &op_imp, &op_brk },
		{ 6, "ORA", &op_izx, &op_ora },
		{ 0, "JAM", NULL, &op_jam },
		{ 8, "SLO", &op_izx, &op_slo },
		{ 2, "NOP", &op_zp, &op_nop },
		{ 3, "ORA", &op_zp, &op_ora },
		{ 5, "ASL", &op_zp, &op_asl },
		{ 5, "SLO", &op_zp, &op_slo },
		{ 3, "PHP", &op_imp, &op_php },
		{ 2, "ORA", &op_imm, &op_ora },
		{ 5, "ASL", &op_zp, &op_asl },
		{ 2, "ANC", &op_imm, &op_anc },
		{ 4, "NOP", &op_abs, &op_nop },
		{ 4, "ORA", &op_abs, &op_ora },
		{ 6, "ASL", &op_abs, &op_asl },
		{ 6, "SLO", &op_abs, &op_slo },

		// 1X
		{ 2, "BPL", &op_rel, &op_bpl },
		{ 5, "ORA", &op_izy, &op_ora },
		{ 0, "JAM", NULL, &op_jam },
		{ 8, "SLO", &op_izy, &op_slo },
		{ 4, "NOP", &op_zpx, &op_nop },
		{ 4, "ORA", &op_zpx, &op_ora },
		{ 6, "ASL", &op_zpx, &op_asl },
		{ 5, "SLO", &op_zpx, &op_slo },
		{ 2, "CLC", &op_imp, &op_clc },
		{ 4, "ORA", &op_aby, &op_ora },
		{ 5, "NOP", &op_imp, &op_nop },
		{ 2, "SLO", &op_aby, &op_slo },
		{ 4, "NOP", &op_abx, &op_nop },
		{ 4, "ORA", &op_abx, &op_ora },
		{ 7, "ASL", &op_abx, &op_asl },
		{ 6, "SLO", &op_abx, &op_slo },

		// 2X
		{ 6, "JSR", &op_abs, &op_jsr },
		{ 6, "AND", &op_izx, &op_and },
		{ 0, "JAM", NULL, &op_jam },
		{ 8, "RLA", &op_izx, &op_rla },
		{ 3, "BIT", &op_zp, &op_bit },
		{ 3, "AND", &op_zp, &op_and },
		{ 5, "ROL", &op_zp, &op_rol },
		{ 5, "RLA", &op_zp, &op_rla },
		{ 4, "PLP", &op_imp, &op_plp },
		{ 2, "AND", &op_imm, &op_and },
		{ 6, "ROL", &op_abs, &op_rol },
		{ 2, "ANC", &op_imm, &op_anc },
		{ 4, "BIT", &op_abs, &op_bit },
		{ 4, "AND", &op_abs, &op_and },
		{ 6, "ROL", &op_abs, &op_rol },
		{ 6, "RLA", &op_abs, &op_rla },

		// 3X
		{ 2, "BMI", &op_rel, &op_bmi },
		{ 5, "AND", &op_izy, &op_and },
		{ 0, "JAM", NULL, &op_jam },
		{ 8, "RLA", &op_izy, &op_rla },
		{ 4, "NOP", &op_zpx, &op_nop },
		{ 4, "AND", &op_zpx, &op_and },
		{ 5, "ROL", &op_zpx, &op_rol },
		{ 6, "RLA", &op_zpx, &op_rla },
		{ 4, "SEC", &op_imp, &op_sec },
		{ 4, "AND", &op_aby, &op_and },
		{ 2, "NOP", &op_imp, &op_nop },
		{ 7, "RLA", &op_aby, &op_rla },
		{ 4, "NOP", &op_abx, &op_nop },
		{ 4, "AND", &op_abx, &op_and },
		{ 6, "ROL", &op_abx, &op_rol },
		{ 7, "RLA", &op_abx, &op_rla },

		// 4X
		{ 6, "RTI", &op_imp, &op_rti },
		{ 6, "EOR", &op_izx, &op_eor },
		{ 0, "JAM", NULL, &op_jam },
		{ 8, "SRE", &op_izy, &op_sre },
		{ 3, "NOP", &op_zp, &op_nop },
		{ 3, "EOR", &op_zp, &op_eor },
		{ 5, "LSR", &op_zp, &op_lsr },
		{ 5, "SRE", &op_zp, &op_sre },
		{ 4, "PHA", &op_imp, &op_pha },
		{ 4, "EOR", &op_imm, &op_eor },
		{ 2, "LSR", &op_imp, &op_lsr },
		{ 7, "ALR", &op_abs, &op_alr },
		{ 4, "JMP", &op_abs, &op_jmp },
		{ 4, "EOR", &op_abs, &op_eor },
		{ 6, "LSR", &op_abs, &op_lsr },
		{ 6, "SRE", &op_abs, &op_sre },
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
