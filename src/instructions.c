#include "instructions.h"

void no_op()
{
}


// address modes

uint8_t op_abs(CPU *cpu)
{
	cpu->last_abs_addr = cpu_read_rom_addr(cpu);
	return 0;
}

uint8_t op_abx(CPU *cpu)
{
	uint16_t addr = cpu_read_rom_addr(cpu) + cpu->regs.x;
	cpu->last_abs_addr = addr;

	if (ABS_HI(cpu->last_abs_addr) != ABS_HI(addr))
		return 1;
	else
		return 0;
}

uint8_t op_aby(CPU *cpu)
{
	uint16_t addr = cpu_read_rom_addr(cpu) + cpu->regs.y;
	cpu->last_abs_addr = addr;

	return ABS_HI(cpu->last_abs_addr) != ABS_HI(addr)) ? 1 : 0;
}

uint8_t op_imm(CPU *cpu)
{
	cpu->last_abs_addr = ++cpu->regs.pc;
	return 0;
}

uint8_t op_ind(CPU *cpu)
{
	uint16_t ptr = cpu_read_rom_addr(cpu);

	if (ptr & 255)
		// emulate page boundary hardware bug
		cpu->last_abs_addr = cpu_read_addr(cpu, ptr);
	else
		// normal behavior
		cpu->last_abs_addr = cpu_read_addr(cpu, ptr + 1);

	return 0;
}

uint8_t op_izx(CPU *cpu)
{
	uint16_t t = cpu_read_rom(cpu);
	uint16_t lo = cpu_read(cpu, (t + cpu->regs.x) & 255);
	uint16_t hi = cpu_read(cpu, (t + cpu->regs.x + 1) & 255);

	cpu->last_abs_addr = (hi << 8) | lo;
	return 0;
}

uint8_t op_izy(CPU *cpu)
{
	uint16_t t = cpu_read_rom(cpu);
	uint16_t lo = cpu_read(cpu, t & 255);
	uint16_t hi = cpu_read(cpu, (t + 1) & 255);

	cpu->last_abs_addr = (hi << 8) | lo + cpu->regs.y;

	return ABS_HI(cpu->last_abs_addr) != hi << 8 ? 1 : 0;
}

uint8_t op_rel(CPU *cpu)
{
	cpu->last_rel_addr = cpu_read_rom(cpu);

	// check for signed bit
	if (cpu->last_rel_addr & 128)
		cpu->last_rel_addr |= 0xFF00;

	return 0;
}

uint8_t op_zp(CPU *cpu)
{
	cpu->last_abs_addr = cpu_read_rom(cpu);
	return 0;
}

uint8_t op_zpx(CPU *cpu)
{
	cpu->last_rel_addr = (cpu_read_rom(cpu) + cpu->regs.x) & 255;
	return 0;
}

uint8_t op_zpy(CPU *cpu)
{
	cpu->last_rel_addr = (cpu_read_rom(cpu) + cpu->regs.y) & 255;
	return 0;
}


// branching

uint8_t op_bcc(CPU *cpu)
{
	if (!cpu->regs.flags.c)
		cpu_branch(cpu);
	return 0;
}

uint8_t op_bcs(CPU *cpu)
{
	if (cpu->regs.flags.c)
		cpu_branch(cpu);
	return 0;
}

uint8_t op_beq(CPU *cpu)
{
	if (cpu->regs.flags.z)
		cpu_branch(cpu);
	return 0;
}

uint8_t op_bmi(CPU *cpu)
{
	if (cpu->regs.flags.n)
		cpu_branch(cpu);
	return 0;
}

uint8_t op_bne(CPU *cpu)
{
	if (!cpu->regs.flags.z)
		cpu_branch(cpu);
	return 0;
}

uint8_t op_bpl(CPU *cpu)
{
	if (!cpu->regs.flags.n)
		cpu_branch(cpu);
	return 0;
}

uint8_t op_bvc(CPU *cpu)
{
	if (!cpu->regs.flags.v)
		cpu_branch(cpu);
	return 0;
}

uint8_t op_bvs(CPU *cpu)
{
	if (cpu->regs.flags.v)
		cpu_branch(cpu);
	return 0;
}


// status bit manipulation

uint8_t op_clc(CPU *cpu)
{
	cpu->regs.flags.c = 0;
	return 0;
}

uint8_t op_cld(CPU *cpu)
{
	cpu->regs.flags.d = 0;
	return 0;
}

uint8_t op_cli(CPU *cpu)
{
	cpu->regs.flags.i = 0;
	return 0;
}

uint8_t op_clv(CPU *cpu)
{
	cpu->regs.flags.v = 0;
	return 0;
}


// interrupts

uint8_t op_brk(CPU *cpu)
{
	// differs slightly from cpu_interrupt

	cpu->regs.pc++;

	cpu->regs.flags.i = 1;
	cpu_stack_write_addr(cpu->regs.pc);

	cpu->regs.flags.b = 1;
	cpu_stack_write(cpu, *(uint8_t *)&cpu->regs.flags);
	cpu->regs.flags.b = 0;

	cpu->regs.pc = cpu_read_addr(cpu, IRQ_ADDR);

	return 0;
}

void op_irq(CPU *cpu)
{
	if (!cpu->regs.flags.i)
		cpu_interrupt(cpu, IRQ_ADDR, 7);
}

void op_nmi(CPU *cpu)
{
	cpu_interrupt(cpu, NMI_ADDR, 8);
}

uint8_t op_rti(CPU *cpu)
{
	uint8_t bits = cpu_stack_read(cpu);

	// restore flags
	cpu->regs.flags = *(STATUS *)&bits;
	cpu->regs.flags.b &= ~cpu->regs.flags.b;
	cpu->regs.flags.u &= ~cpu->regs.flags.u;

	// restore counter
	cpu->regs.pc = cpu_stack_read_addr(cpu);

	return 0;
}


// accumulator manipulation

uint8_t op_pha(CPU *cpu)
{
	cpu_stack_write(cpu->regs.a);
	return 0;
}

uint8_t op_pla(CPU *cpu)
{
	cpu->regs.a = cpu_stack_read(cpu);
	cpu->regs.flags.z = cpu->regs.a == 0 ? 1 : 0;
	cpu->regs.flags.n = cpu->regs.a & 128 ? 1 : 0;

	return 0;
}


// arith / bitwise

uint8_t op_adc(CPU *cpu)
{
	cpu_fetch(cpu);
	uint16_t tmp = cpu->regs.a + cpu->cache + cpu->regs.flags.c;

	cpu->regs.flags.c = tmp > 255 ? 1 : 0;
	cpu->regs.flags.z = tmp & 255 == 0 ? 1 : 0;
	cpu->regs.flags.n = tmp & 128 ? 1 : 0;
	cpu->regs.flags.v = ~(((cpu->regs.a ^ cpu->cache) & (cpu->regs.a ^ tmp) & 128);

	cpu->regs.a = tmp & 255;

	return 1;
}

uint8_t op_and(CPU *cpu)
{
	cpu_fetch(cpu);
	cpu->regs.a &= cpu->cache;
	cpu->regs.flags.z = cpu->regs.a == 0 ? 1 : 0;
	cpu->regs.flags.n = cpu->regs.a & 128 ? 1 : 0;

	return 1;
}

uint8_t op_asl(CPU *cpu)
{
	cpu_fetch(cpu);
	uint16_t tmp = cpu->cache << 1;

	cpu->regs.flags.c = tmp & 255 > 0 ? 1 : 0;
	cpu->regs.flags.z = tmp & 255 == 0 ? 1 : 0;
	cpu->regs.flags.n = tmp & 128 ? 1 : 0;

	if (cpu->ops[cpu->last_op].addr_mode == &op_imp)
		cpu->regs.a = tmp & 255;
	else
		cpu_write(cpu, cpu->last_abs_addr, tmp & 255);

	return 0;
}

uint8_t op_sdc(CPU *cpu)
{
	cpu_fetch(cpu);
	uint16_t value = cpu->cache ^ 255; // invert the value
	uint16_t tmp = cpu->regs.a + value + cpu->regs.flags.c;

	cpu->regs.flags.c = tmp & 0xFF00 ? 1 : 0;
	cpu->regs.flags.z = tmp & 255 == 0 ? 1 : 0;
	cpu->regs.flags.n = tmp & 128 ? 1 : 0;
	cpu->regs.flags.v = (tmp ^ cpu->regs.a) & ((tmp ^ value) & 128);

	cpu->regs.a = tmp & 255;

	return 1;
}
