#include "instructions.h"

// address modes

uint8_t op_abs(CPU_6502 *cpu)
{
	cpu->last_abs_addr = cpu6502_read_rom_addr(cpu);
	return 0;
}

uint8_t op_abx(CPU_6502 *cpu)
{
	uint16_t addr = cpu6502_read_rom_addr(cpu) + cpu->regs.x;
	cpu->last_abs_addr = addr;

	if (ABS16_HI(cpu->last_abs_addr) != ABS16_HI(addr))
		return 1;
	else
		return 0;
}

uint8_t op_aby(CPU_6502 *cpu)
{
	uint16_t addr = cpu6502_read_rom_addr(cpu) + cpu->regs.y;
	cpu->last_abs_addr = addr;

	return ABS16_HI(cpu->last_abs_addr) != ABS16_HI(addr) ? 1 : 0;
}

uint8_t op_imm(CPU_6502 *cpu)
{
	cpu->last_abs_addr = ++cpu->regs.pc;
	return 0;
}

uint8_t op_imp(CPU_6502 *cpu)
{
	cpu->cache = cpu->regs.a;
	return 0;
}

uint8_t op_ind(CPU_6502 *cpu)
{
	uint16_t ptr = cpu6502_read_rom_addr(cpu);

	if (ptr & 255)
		// emulate page boundary hardware bug
		cpu->last_abs_addr = cpu6502_read_addr(cpu, ptr);
	else
		// normal behavior
		cpu->last_abs_addr = cpu6502_read_addr(cpu, ptr + 1);

	return 0;
}

uint8_t op_izx(CPU_6502 *cpu)
{
	cpu->last_abs_addr = cpu6502_read_addr(cpu, (cpu6502_read_rom(cpu) + cpu->regs.x) & 255);
	return 0;
}

uint8_t op_izy(CPU_6502 *cpu)
{
	uint16_t t = cpu6502_read_rom(cpu);
	uint16_t lo = cpu6502_read(cpu, t & 255);
	uint16_t hi = cpu6502_read(cpu, (t + 1) & 255);

	cpu->last_abs_addr = (hi << 8) | lo + cpu->regs.y;

	return ABS16_HI(cpu->last_abs_addr) != hi << 8 ? 1 : 0;
}

uint8_t op_rel(CPU_6502 *cpu)
{
	cpu->last_rel_addr = cpu6502_read_rom(cpu);

	// check for signed bit
	if (cpu->last_rel_addr & 128)
		cpu->last_rel_addr |= 0xFF00;

	return 0;
}

uint8_t op_zp(CPU_6502 *cpu)
{
	cpu->last_abs_addr = cpu6502_read_rom(cpu);
	return 0;
}

uint8_t op_zpx(CPU_6502 *cpu)
{
	cpu->last_rel_addr = (cpu6502_read_rom(cpu) + cpu->regs.x) & 255;
	return 0;
}

uint8_t op_zpy(CPU_6502 *cpu)
{
	cpu->last_rel_addr = (cpu6502_read_rom(cpu) + cpu->regs.y) & 255;
	return 0;
}


// branching

uint8_t op_bcc(CPU_6502 *cpu)
{
	if (!cpu->regs.flags.c)
		cpu6502_branch(cpu);
	return 0;
}

uint8_t op_bcs(CPU_6502 *cpu)
{
	if (cpu->regs.flags.c)
		cpu6502_branch(cpu);
	return 0;
}

uint8_t op_beq(CPU_6502 *cpu)
{
	if (cpu->regs.flags.z)
		cpu6502_branch(cpu);
	return 0;
}

uint8_t op_bmi(CPU_6502 *cpu)
{
	if (cpu->regs.flags.n)
		cpu6502_branch(cpu);
	return 0;
}

uint8_t op_bne(CPU_6502 *cpu)
{
	if (!cpu->regs.flags.z)
		cpu6502_branch(cpu);
	return 0;
}

uint8_t op_bpl(CPU_6502 *cpu)
{
	if (!cpu->regs.flags.n)
		cpu6502_branch(cpu);
	return 0;
}

uint8_t op_bvc(CPU_6502 *cpu)
{
	if (!cpu->regs.flags.v)
		cpu6502_branch(cpu);
	return 0;
}

uint8_t op_bvs(CPU_6502 *cpu)
{
	if (cpu->regs.flags.v)
		cpu6502_branch(cpu);
	return 0;
}


// status bit manipulation

uint8_t op_clc(CPU_6502 *cpu)
{
	cpu->regs.flags.c = 0;
	return 0;
}

uint8_t op_cld(CPU_6502 *cpu)
{
	cpu->regs.flags.d = 0;
	return 0;
}

uint8_t op_cli(CPU_6502 *cpu)
{
	cpu->regs.flags.i = 0;
	return 0;
}

uint8_t op_clv(CPU_6502 *cpu)
{
	cpu->regs.flags.v = 0;
	return 0;
}

uint8_t op_sec(CPU_6502 *cpu)
{
	cpu->regs.flags.c = 1;
	return 0;
}

uint8_t op_sed(CPU_6502 *cpu)
{
	cpu->regs.flags.d = 1;
	return 0;
}

uint8_t op_sei(CPU_6502 *cpu)
{
	cpu->regs.flags.i = 1;
	return 0;
}


// interrupts

uint8_t op_brk(CPU_6502 *cpu)
{
	// differs slightly from cpu6502_interrupt

	cpu->regs.pc++;

	cpu->regs.flags.i = 1;
	cpu6502_stack_write_addr(cpu, cpu->regs.pc);

	cpu->regs.flags.b = 1;
	cpu6502_stack_write(cpu, *(uint8_t *)&cpu->regs.flags);
	cpu->regs.flags.b = 0;

	cpu->regs.pc = cpu6502_read_addr(cpu, IRQ_ADDR);

	return 0;
}

void op_irq(CPU_6502 *cpu)
{
	if (!cpu->regs.flags.i)
		cpu6502_interrupt(cpu, IRQ_ADDR, 7);
}

void op_nmi(CPU_6502 *cpu)
{
	cpu6502_interrupt(cpu, NMI_ADDR, 8);
}

uint8_t op_rti(CPU_6502 *cpu)
{
	uint8_t bits = cpu6502_stack_read(cpu);

	// restore flags
	cpu->regs.flags = *(STATUS_6502 *)&bits;
	cpu->regs.flags.b &= ~cpu->regs.flags.b;
	cpu->regs.flags.u &= ~cpu->regs.flags.u;

	// restore counter
	cpu->regs.pc = cpu6502_stack_read_addr(cpu);

	return 0;
}

uint8_t op_rts(CPU_6502 *cpu)
{
	cpu->regs.pc = cpu6502_stack_read_addr(cpu);
	return 0;
}


// pushing/popping

uint8_t op_pha(CPU_6502 *cpu)
{
	cpu6502_stack_write(cpu, cpu->regs.a);
	return 0;
}

uint8_t op_php(CPU_6502 *cpu)
{
	cpu->regs.flags.b = 1;
	cpu->regs.flags.u = 1;
	cpu6502_stack_write(cpu, cpu6502_flags(cpu));
	cpu->regs.flags.b = 0;
	cpu->regs.flags.u = 0;
	return 0;
}

uint8_t op_pla(CPU_6502 *cpu)
{
	cpu->regs.a = cpu6502_stack_read(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.a);

	return 0;
}

uint8_t op_plp(CPU_6502 *cpu)
{
	cpu6502_stack_read(cpu);
	cpu->regs.flags.u = 1;
	return 0;
}


// arith

uint8_t op_adc(CPU_6502 *cpu)
{
	uint16_t tmp = cpu->regs.a + cpu6502_fetch(cpu) + cpu->regs.flags.c;

	cpu6502_flags_cnz(cpu, tmp);
	cpu->regs.flags.v = ~((cpu->regs.a ^ cpu->cache) & (cpu->regs.a ^ tmp) & 128);

	cpu->regs.a = tmp & 255;

	return 1;
}

uint8_t op_dec(CPU_6502 *cpu)
{
	uint8_t tmp = cpu6502_fetch(cpu) - 1;
	cpu6502_write_last(cpu, tmp);
	cpu6502_flags_nz(cpu, cpu->regs.x);

	return 0;
}

uint8_t op_dex(CPU_6502 *cpu)
{
	cpu6502_flags_nz(cpu, --cpu->regs.x);
	return 0;
}

uint8_t op_dey(CPU_6502 *cpu)
{
	cpu6502_flags_nz(cpu, --cpu->regs.y);
	return 0;
}

uint8_t op_inc(CPU_6502 *cpu)
{
	uint8_t tmp = cpu6502_fetch(cpu) + 1;
	cpu6502_write_last(cpu, tmp);
	cpu6502_flags_nz(cpu, tmp);

	return 0;
}

uint8_t op_inx(CPU_6502 *cpu)
{
	cpu6502_flags_nz(cpu, ++cpu->regs.x);
	return 0;
}

uint8_t op_iny(CPU_6502 *cpu)
{
	cpu6502_flags_nz(cpu, ++cpu->regs.y);
	return 0;
}

uint8_t op_sbc(CPU_6502 *cpu)
{
	uint16_t value = cpu6502_fetch(cpu) ^ 255; // invert the value
	uint16_t tmp = cpu->regs.a + value + cpu->regs.flags.c;

	cpu6502_flags_cnz(cpu, tmp);
	cpu->regs.flags.v = (tmp ^ cpu->regs.a) & ((tmp ^ value) & 128);

	cpu->regs.a = tmp & 255;

	return 1;
}


// bitwise

uint8_t op_and(CPU_6502 *cpu)
{
	cpu->regs.a &= cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.a);

	return 1;
}

uint8_t op_asl(CPU_6502 *cpu)
{
	uint16_t tmp = cpu6502_fetch(cpu) << 1;

	cpu6502_flags_cnz(cpu, tmp);

	if (cpu->ops[cpu->last_op].addr_mode == &op_imp)
		cpu->regs.a = tmp & 255;
	else
		cpu6502_write_last(cpu, tmp & 255);

	return 0;
}

uint8_t op_eor(CPU_6502 *cpu)
{
	cpu->regs.a ^= cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.a);

	return 1;
}

uint8_t op_lsr(CPU_6502 *cpu)
{
	cpu->regs.flags.c = cpu6502_fetch(cpu) & 1 ? 1 : 0;
	uint16_t tmp = cpu->cache >> 1;
	cpu6502_flags_nz(cpu, tmp);

	if (cpu->ops[cpu->last_op].addr_mode == &op_imp)
		cpu->regs.a = tmp & 255;
	else
		cpu6502_write_last(cpu, tmp & 255);

	return 0;
}

uint8_t op_ora(CPU_6502 *cpu)
{
	cpu->regs.a |= cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.a);

	return 1;
}

uint8_t op_rol(CPU_6502 *cpu)
{
	uint16_t tmp = (cpu6502_fetch(cpu) << 1) | cpu->regs.flags.c;
	cpu6502_flags_cnz(cpu, tmp);

	if (cpu->ops[cpu->last_op].addr_mode == &op_imp)
		cpu->regs.a = tmp & 255;
	else
		cpu6502_write_last(cpu, tmp & 255);

	return 0;
}

uint8_t op_ror(CPU_6502 *cpu)
{
	cpu6502_fetch(cpu);
	uint16_t tmp = (cpu->cache << 7) | (cpu->cache >> 1);
	cpu->regs.flags.c = cpu->cache & 1 ? 1 : 0;
	cpu6502_flags_nz(cpu, tmp);

	if (cpu->ops[cpu->last_op].addr_mode == &op_imp)
		cpu->regs.a = tmp & 255;
	else
		cpu6502_write_last(cpu, tmp & 255);

	return 0;
}


// comparison

uint8_t op_bit(CPU_6502 *cpu)
{
	uint16_t tmp = cpu->regs.a & cpu6502_fetch(cpu);
	cpu->regs.flags.z = tmp & 255 == 0 ? 1 : 0;
	cpu->regs.flags.n = cpu->cache & 128 ? 1 : 0;
	cpu->regs.flags.v = cpu->cache & 64 ? 1 : 0;

	return 0;
}

uint8_t op_cmp(CPU_6502 *cpu)
{
	uint16_t tmp = cpu->regs.a - cpu6502_fetch(cpu);
	cpu->regs.flags.c = cpu->regs.a >= cpu->cache ? 1 : 0;
	cpu6502_flags_nz(cpu, tmp);

	return 1;
}

uint8_t op_cpx(CPU_6502 *cpu)
{
	uint16_t tmp = cpu->regs.x - cpu6502_fetch(cpu);
	cpu->regs.flags.c = cpu->regs.x >= cpu->cache ? 1 : 0;
	cpu6502_flags_nz(cpu, tmp);

	return 1;
}

uint8_t op_cpy(CPU_6502 *cpu)
{
	uint16_t tmp = cpu->regs.y - cpu6502_fetch(cpu);
	cpu->regs.flags.c = cpu->regs.y >= cpu->cache ? 1 : 0;
	cpu6502_flags_nz(cpu, tmp);

	return 1;
}


// jumping

uint8_t op_jmp(CPU_6502 *cpu)
{
	cpu->regs.pc = cpu->last_abs_addr;
	return 0;
}

uint8_t op_jsr(CPU_6502 *cpu)
{
	cpu6502_stack_write_addr(cpu, cpu->regs.pc);
	cpu->regs.pc = cpu->last_abs_addr;
	return 0;
}


// loading

uint8_t op_lda(CPU_6502 *cpu)
{
	cpu->regs.a = cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.a);
	return 1;
}

uint8_t op_ldx(CPU_6502 *cpu)
{
	cpu->regs.x = cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.x);
	return 1;
}

uint8_t op_ldy(CPU_6502 *cpu)
{
	cpu->regs.y = cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.y);
	return 1;
}


// storing

uint8_t op_sta(CPU_6502 *cpu)
{
	cpu6502_write_last(cpu, cpu->regs.a);
	return 0;
}

uint8_t op_stx(CPU_6502 *cpu)
{
	cpu6502_write_last(cpu, cpu->regs.x);
	return 0;
}

uint8_t op_sty(CPU_6502 *cpu)
{
	cpu6502_write_last(cpu, cpu->regs.y);
	return 0;
}


// transferring

uint8_t op_tax(CPU_6502 *cpu)
{
	cpu->regs.x = cpu->regs.a;
	cpu6502_flags_nz(cpu, cpu->regs.x);
	return 0;
}

uint8_t op_tay(CPU_6502 *cpu)
{
	cpu->regs.y = cpu->regs.a;
	cpu6502_flags_nz(cpu, cpu->regs.y);
	return 0;
}

uint8_t op_tsx(CPU_6502 *cpu)
{
	cpu->regs.x = cpu->regs.sp;
	cpu6502_flags_nz(cpu, cpu->regs.x);
	return 0;
}

uint8_t op_txa(CPU_6502 *cpu)
{
	cpu->regs.a = cpu->regs.x;
	cpu6502_flags_nz(cpu, cpu->regs.a);
	return 0;
}

uint8_t op_txs(CPU_6502 *cpu)
{
	cpu->regs.sp = cpu->regs.x;
	return 0;
}

uint8_t op_tya(CPU_6502 *cpu)
{
	cpu->regs.a = cpu->regs.y;
	cpu6502_flags_nz(cpu, cpu->regs.a);
	return 0;
}


// illegal opcodes

uint8_t op_alr(CPU_6502 *cpu)
{
	return op_and(cpu) + op_lsr(cpu);
}

uint8_t op_anc(CPU_6502 *cpu)
{
	return op_and(cpu) + op_sec(cpu);
}

uint8_t op_ane(CPU_6502 *cpu)
{
	cpu->regs.a = cpu6502_magic(cpu) & cpu->regs.x & cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.a);

	return 0;
}

uint8_t op_arr(CPU_6502 *cpu)
{
	return op_and(cpu) + op_ror(cpu);
}

uint8_t op_dcp(CPU_6502 *cpu)
{
	return op_dec(cpu) + op_cmp(cpu);
}

uint8_t op_isc(CPU_6502 *cpu)
{
	return op_inc(cpu) + op_sbc(cpu);
}

uint8_t op_jam(CPU_6502 *cpu)
{
	cpu->regs.pc = 0xFFFF;
	memset(&cpu->regs.flags, 0, sizeof(STATUS_6502));
	return 0;
}

uint8_t op_las(CPU_6502 *cpu)
{
	return 0;
}

uint8_t op_lax(CPU_6502 *cpu)
{
	return op_lda(cpu) + op_ldx(cpu);
}

uint8_t op_lxa(CPU_6502 *cpu)
{
	cpu->regs.x = cpu->regs.a = cpu6502_magic(cpu) & cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.x);
	return 0;
}

uint8_t op_nop(CPU_6502 *cpu)
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

uint8_t op_rla(CPU_6502 *cpu)
{
	return op_rol(cpu) + op_and(cpu);
}

uint8_t op_rra(CPU_6502 *cpu)
{
	return op_ror(cpu) + op_adc(cpu);
}

uint8_t op_sax(CPU_6502 *cpu)
{
	cpu6502_write_last(cpu, cpu->regs.a & cpu->regs.x);
	return 0;
}

uint8_t op_sbx(CPU_6502 *cpu)
{
	cpu->regs.x &= cpu->regs.a;
	return op_sbc(cpu);
}

uint8_t op_sha(CPU_6502 *cpu)
{
	cpu6502_write_last(cpu, cpu->regs.a & cpu->regs.x & (ABS16_HI(cpu->last_abs_addr) + 1));
	return 0;
}

uint8_t op_shx(CPU_6502 *cpu)
{
	cpu6502_write_last(cpu, cpu->regs.x & (ABS16_HI(cpu->last_abs_addr) + 1));
	return 0;
}

uint8_t op_shy(CPU_6502 *cpu)
{
	cpu6502_write_last(cpu, cpu->regs.y & (ABS16_HI(cpu->last_abs_addr) + 1));
	return 0;
}

uint8_t op_slo(CPU_6502 *cpu)
{
	return op_asl(cpu) + op_ora(cpu);
}

uint8_t op_sre(CPU_6502 *cpu)
{
	return op_lsr(cpu) + op_eor(cpu);
}

uint8_t op_tas(CPU_6502 *cpu)
{
	cpu->regs.sp = cpu->regs.a & cpu->regs.x;
	cpu6502_write_last(cpu, cpu->regs.a & cpu->regs.x & (ABS16_HI(cpu->last_abs_addr) + 1));
}

uint8_t op_usbc(CPU_6502 *cpu)
{
	return op_sbc(cpu) + op_nop(cpu);
}
