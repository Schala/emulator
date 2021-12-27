#include "instructions.h"

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
	cpu->last_abs_addr = cpu_read_addr(cpu, (cpu_read_rom(cpu) + cpu->regs.x) & 255);
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

uint8_t op_sec(CPU *cpu)
{
	cpu->regs.flags.c = 1;
	return 0;
}

uint8_t op_sed(CPU *cpu)
{
	cpu->regs.flags.d = 1;
	return 0;
}

uint8_t op_sei(CPU *cpu)
{
	cpu->regs.flags.i = 1;
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

uint8_t op_rts(CPU *cpu)
{
	cpu->regs.pc = cpu_stack_read_addr();
	cpu->regs.pc++;

	return 0;
}


// pushing/popping

uint8_t op_pha(CPU *cpu)
{
	cpu_stack_write(cpu->regs.a);
	return 0;
}

uint8_t op_php(CPU *cpu)
{
	cpu->regs.flags.b = 1;
	cpu->regs.flags.u = 1;
	cpu_stack_write(cpu_flags(cpu));
	cpu->regs.flags.b = 0;
	cpu->regs.flags.u = 0;
	return 0;
}

uint8_t op_pla(CPU *cpu)
{
	cpu->regs.a = cpu_stack_read(cpu);
	cpu_flags_nz(cpu, cpu->regs.a);

	return 0;
}

uint8_t op_plp(CPU *cpu)
{
	cpu_stack_read(cpu);
	cpu->regs.flags.u = 1;
	return 0;
}


// arith

uint8_t op_adc(CPU *cpu)
{
	uint16_t tmp = cpu->regs.a + cpu_fetch(cpu) + cpu->regs.flags.c;

	cpu_flags_cnz(cpu, tmp);
	cpu->regs.flags.v = ~(((cpu->regs.a ^ cpu->cache) & (cpu->regs.a ^ tmp) & 128);

	cpu->regs.a = tmp & 255;

	return 1;
}

uint8_t op_dec(CPU *cpu)
{
	uint8_t tmp = cpu_fetch(cpu) - 1;
	cpu_write_last(cpu, tmp);
	cpu_flags_nz(cpu, cpu->regs.x);

	return 0;
}

uint8_t op_dex(CPU *cpu)
{
	cpu_flags_nz(cpu, --cpu->regs.x);
	return 0;
}

uint8_t op_dey(CPU *cpu)
{
	cpu_flags_nz(cpu, --cpu->regs.y);
	return 0;
}

uint8_t op_inc(CPU *cpu)
{
	uint8_t tmp = cpu_fetch(cpu) + 1;
	cpu_write_last(cpu, tmp);
	cpu_flags_nz(cpu, tmp);

	return 0;
}

uint8_t op_inx(CPU *cpu)
{
	cpu_flags_nz(cpu, ++cpu->regs.x);
	return 0;
}

uint8_t op_iny(CPU *cpu)
{
	cpu_flags_nz(cpu, ++cpu->regs.y);
	return 0;
}

uint8_t op_sdc(CPU *cpu)
{
	uint16_t value = cpu_fetch(cpu) ^ 255; // invert the value
	uint16_t tmp = cpu->regs.a + value + cpu->regs.flags.c;

	cpu_flags_cnz(cpu, tmp);
	cpu->regs.flags.v = (tmp ^ cpu->regs.a) & ((tmp ^ value) & 128);

	cpu->regs.a = tmp & 255;

	return 1;
}


// bitwise

uint8_t op_and(CPU *cpu)
{
	cpu->regs.a &= cpu_fetch(cpu);
	cpu_flags_nz(cpu, cpu->regs.a);

	return 1;
}

uint8_t op_asl(CPU *cpu)
{
	uint16_t tmp = cpu_fetch(cpu) << 1;

	cpu_flags_cnz(cpu, tmp);

	if (cpu->ops[cpu->last_op].addr_mode == &op_imp)
		cpu->regs.a = tmp & 255;
	else
		cpu_write_last(cpu, tmp & 255);

	return 0;
}

uint8_t op_eor(CPU *cpu)
{
	cpu->regs.a ^= cpu_fetch(cpu);
	cpu_flags_nz(cpu, cpu->regs.a);

	return 1;
}

uint8_t op_lsr(CPU *cpu)
{
	cpu->regs.flags.c = cpu_fetch(cpu) & 1 ? 1 : 0;
	uint16_t tmp = cpu->cache >> 1;
	cpu_flags_nz(cpu, tmp);

	if (cpu->ops[cpu->last_op].addr_mode == &op_imp)
		cpu->regs.a = tmp & 255;
	else
		cpu_write_last(cpu, tmp & 255);

	return 0;
}

uint8_t op_ora(CPU *cpu)
{
	cpu->regs.a |= cpu_fetch(cpu);
	cpu_flags_nz(cpu, cpu->regs.a);

	return 1;
}

uint8_t op_rol(CPU *cpu)
{
	uint16_t tmp = (cpu_fetch(cpu) << 1) | cpu->regs.flags.c;
	cpu_flags_cnz(cpu, tmp);

	if (cpu->ops[cpu->last_op].addr_mode == &op_imp)
		cpu->regs.a = tmp & 255;
	else
		cpu_write_last(cpu, tmp & 255);

	return 0;
}

uint8_t op_ror(CPU *cpu)
{
	cpu_fetch(cpu);
	uint16_t tmp = (cpu->cache << 7) | (cpu->cache >> 1);
	cpu->regs.flags.c = cpu->cache & 1 ? 1 : 0;
	cpu_flags_nz(cpu, tmp);

	if (cpu->ops[cpu->last_op].addr_mode == &op_imp)
		cpu->regs.a = tmp & 255;
	else
		cpu_write_last(cpu, tmp & 255);

	return 0;
}


// comparison

uint8_t op_cmp(CPU *cpu)
{
	uint16_t tmp = cpu->regs.a - cpu_fetch(cpu);
	cpu->regs.flags.c = cpu->regs.a >= cpu->cache ? 1 : 0;
	cpu_flags_nz(cpu, tmp);

	return 1;
}

uint8_t op_cpx(CPU *cpu)
{
	uint16_t tmp = cpu->regs.x - cpu_fetch(cpu);
	cpu->regs.flags.c = cpu->regs.x >= cpu->cache ? 1 : 0;
	cpu_flags_nz(cpu, tmp);

	return 1;
}

uint8_t op_cpy(CPU *cpu)
{
	uint16_t tmp = cpu->regs.y - cpu_fetch(cpu);
	cpu->regs.flags.c = cpu->regs.y >= cpu->cache ? 1 : 0;
	cpu_flags_nz(cpu, tmp);

	return 1;
}


// jumping

uint8_t op_jmp(CPU *cpu)
{
	cpu->regs.pc = cpu->last_abs_addr;
	return 0;
}

uint8_t op_jsr(CPU *cpu)
{
	cpu_stack_write_addr(cpu->regs.pc);
	cpu->regs.pc = cpu->last_abs_addr;
	return 0;
}


// loading

uint8_t op_lda(CPU *cpu)
{
	cpu->regs.a = cpu_fetch(cpu);
	cpu_flags_nz(cpu, cpu->regs.a);
	return 1;
}

uint8_t op_ldx(CPU *cpu)
{
	cpu->regs.x = cpu_fetch(cpu);
	cpu_flags_nz(cpu, cpu->regs.x);
	return 1;
}

uint8_t op_ldy(CPU *cpu)
{
	cpu->regs.y = cpu_fetch(cpu);
	cpu_flags_nz(cpu, cpu->regs.y);
	return 1;
}


// storing

uint8_t op_sta(CPU *cpu)
{
	cpu_write_last(cpu, cpu->regs.a);
	return 0;
}

uint8_t op_stx(CPU *cpu)
{
	cpu_write_last(cpu, cpu->regs.x);
	return 0;
}

uint8_t op_sty(CPU *cpu)
{
	cpu_write_last(cpu, cpu->regs.y);
	return 0;
}


// transferring

uint8_t op_tax(CPU *cpu)
{
	cpu->regs.x = cpu->regs.a;
	cpu_flags_nz(cpu, cpu->regs.x);
	return 0;
}

uint8_t op_tax(CPU *cpu)
{
	cpu->regs.x = cpu->regs.a;
	cpu_flags_nz(cpu, cpu->regs.x);
	return 0;
}

uint8_t op_tay(CPU *cpu)
{
	cpu->regs.y = cpu->regs.a;
	cpu_flags_nz(cpu, cpu->regs.y);
	return 0;
}

uint8_t op_tsx(CPU *cpu)
{
	cpu->regs.x = cpu->regs.sp;
	cpu_flags_nz(cpu, cpu->regs.x);
	return 0;
}

uint8_t op_txa(CPU *cpu)
{
	cpu->regs.a = cpu->regs.x;
	cpu_flags_nz(cpu, cpu->regs.a);
	return 0;
}

uint8_t op_txs(CPU *cpu)
{
	cpu->regs.sp = cpu->regs.x;
	return 0;
}

uint8_t op_tya(CPU *cpu)
{
	cpu->regs.a = cpu->regs.y;
	cpu_flags_nz(cpu, cpu->regs.a);
	return 0;
}


// illegal opcodes

uint8_t op_alr(CPU *cpu)
{
	return op_and(cpu) + op_lsr(cpu);
}

uint8_t op_anc(CPU *cpu)
{
	return op_and(cpu) + op_sec(cpu);
}

uint8_t op_ane(CPU *cpu)
{
	cpu->regs.a = cpu_magic(cpu) & cpu->regs.x & cpu_fetch(cpu);
	cpu_flags_nz(cpu, cpu->regs.a);

	return 0;
}

uint8_t op_arr(CPU *cpu)
{
	return op_and(cpu) + op_ror(cpu);
}

uint8_t op_dcp(CPU *cpu)
{
	return op_dec(cpu) + op_cmp(cpu);
}

uint8_t op_isc(CPU *cpu)
{
	return op_inc(cpu) + op_sbc(cpu);
}

uint8_t op_jam(CPU *cpu)
{
	cpu->regs.pc = 0xFFFF;
	memset(&cpu->regs.flags, 0, sizeof(STATUS));
	return 0;
}

uint8_t op_las(CPU *cpu)
{
	return 0;
}

uint8_t op_lax(CPU *cpu)
{
	return op_lda(cpu) + op_ldx(cpu);
}

uint8_t op_lxa(CPU *cpu)
{
	cpu->regs.x = cpu->regs.a = cpu_magic(cpu) & cpu_fetch(cpu);
	cpu_flags_nz(cpu, cpu->regs.x);
	return 0;
}

uint8_t op_nop(CPU *cpu)
{
	switch (cpu->last_op)
	{
		case 0x1C:
		case 0x3C:
		case 0x5C:
		case 0x7C:
		case 0xDC:
		case 0xFC:
			return 1;
		default:
			return 0;
	}
}

uint8_t op_rla(CPU *cpu)
{
	return op_rol(cpu) + op_and(cpu);
}

uint8_t op_rra(CPU *cpu)
{
	return op_ror(cpu) + op_adc(cpu);
}

uint8_t op_sax(CPU *cpu)
{
	cpu_write_last(cpu, cpu->regs.a & cpu->regs.x);
	return 0;
}

uint8_t op_sbx(CPU *cpu)
{
	cpu->regs.x &= cpu->regs.a;
	return op_sbc(cpu);
}

uint8_t op_sha(CPU *cpu)
{
	cpu_write_last(cpu, cpu->regs.a & cpu->regs.x & (ABS_HI(cpu->last_abs_addr) + 1));
	return 0;
}

uint8_t op_shx(CPU *cpu)
{
	cpu_write_last(cpu, cpu->regs.x & (ABS_HI(cpu->last_abs_addr) + 1));
	return 0;
}

uint8_t op_shy(CPU *cpu)
{
	cpu_write_last(cpu, cpu->regs.y & (ABS_HI(cpu->last_abs_addr) + 1));
	return 0;
}

uint8_t op_slo(CPU *cpu)
{
	return op_asl(cpu) + op_ora(cpu);
}

uint8_t op_sre(CPU *cpu)
{
	return op_lsr(cpu) + op_eor(cpu);
}

uint8_t op_tas(CPU *cpu)
{
	cpu->regs.sp = cpu->regs.a & cpu->regs.x;
	cpu_write_last(cpu, cpu->regs.a & cpu->regs.x & (ABS_HI(cpu->last_abs_addr) + 1));
}

uint8_t op_usbc(CPU *cpu)
{
	return op_sbc(cpu) + op_nop(cpu);
}
