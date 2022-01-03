#include "isa.h"

// address modes

uint8_t am6502_abs(CPU_6502 *cpu)
{
	cpu->last_abs_addr = cpu6502_read_rom_addr(cpu);
	return 0;
}

uint8_t am6502_abx(CPU_6502 *cpu)
{
	uint16_t addr = cpu6502_read_rom_addr(cpu) + cpu->regs.x;
	cpu->last_abs_addr = addr;

	if (ABS16_HI(cpu->last_abs_addr) != ABS16_HI(addr))
		return 1;
	else
		return 0;
}

uint8_t am6502_aby(CPU_6502 *cpu)
{
	uint16_t addr = cpu6502_read_rom_addr(cpu) + cpu->regs.y;
	cpu->last_abs_addr = addr;

	return ABS16_HI(cpu->last_abs_addr) != ABS16_HI(addr) ? 1 : 0;
}

uint8_t am6502_imm(CPU_6502 *cpu)
{
	cpu->last_abs_addr = ++cpu->regs.pc;
	return 0;
}

uint8_t am6502_imp(CPU_6502 *cpu)
{
	cpu->cache = cpu->regs.a;
	return 0;
}

uint8_t am6502_ind(CPU_6502 *cpu)
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

uint8_t am6502_izx(CPU_6502 *cpu)
{
	cpu->last_abs_addr = cpu6502_read_addr(cpu, (cpu6502_read_rom(cpu) + cpu->regs.x) & 255);
	return 0;
}

uint8_t am6502_izy(CPU_6502 *cpu)
{
	uint16_t t = cpu6502_read_rom(cpu);
	uint16_t lo = cpu6502_read(cpu, t & 255);
	uint16_t hi = cpu6502_read(cpu, (t + 1) & 255);

	cpu->last_abs_addr = (hi << 8) | lo + cpu->regs.y;

	return ABS16_HI(cpu->last_abs_addr) != hi << 8 ? 1 : 0;
}

uint8_t am6502_rel(CPU_6502 *cpu)
{
	cpu->last_rel_addr = cpu6502_read_rom(cpu);

	// check for signed bit
	if (cpu->last_rel_addr & 128)
		cpu->last_rel_addr |= 0xFF00;

	return 0;
}

uint8_t am6502_zp(CPU_6502 *cpu)
{
	cpu->last_abs_addr = cpu6502_read_rom(cpu);
	return 0;
}

uint8_t am6502_zpx(CPU_6502 *cpu)
{
	cpu->last_rel_addr = (cpu6502_read_rom(cpu) + cpu->regs.x) & 255;
	return 0;
}

uint8_t am6502_zpy(CPU_6502 *cpu)
{
	cpu->last_rel_addr = (cpu6502_read_rom(cpu) + cpu->regs.y) & 255;
	return 0;
}


// branching

uint8_t op6502_bcc(CPU_6502 *cpu)
{
	if (!cpu->regs.flags.c)
		cpu6502_branch(cpu);
	return 0;
}

uint8_t op6502_bcs(CPU_6502 *cpu)
{
	if (cpu->regs.flags.c)
		cpu6502_branch(cpu);
	return 0;
}

uint8_t op6502_beq(CPU_6502 *cpu)
{
	if (cpu->regs.flags.z)
		cpu6502_branch(cpu);
	return 0;
}

uint8_t op6502_bmi(CPU_6502 *cpu)
{
	if (cpu->regs.flags.n)
		cpu6502_branch(cpu);
	return 0;
}

uint8_t op6502_bne(CPU_6502 *cpu)
{
	if (!cpu->regs.flags.z)
		cpu6502_branch(cpu);
	return 0;
}

uint8_t op6502_bpl(CPU_6502 *cpu)
{
	if (!cpu->regs.flags.n)
		cpu6502_branch(cpu);
	return 0;
}

uint8_t op6502_bvc(CPU_6502 *cpu)
{
	if (!cpu->regs.flags.v)
		cpu6502_branch(cpu);
	return 0;
}

uint8_t op6502_bvs(CPU_6502 *cpu)
{
	if (cpu->regs.flags.v)
		cpu6502_branch(cpu);
	return 0;
}


// state bit manipulation

uint8_t op6502_clc(CPU_6502 *cpu)
{
	cpu->regs.flags.c = 0;
	return 0;
}

uint8_t op6502_cld(CPU_6502 *cpu)
{
	cpu->regs.flags.d = 0;
	return 0;
}

uint8_t op6502_cli(CPU_6502 *cpu)
{
	cpu->regs.flags.i = 0;
	return 0;
}

uint8_t op6502_clv(CPU_6502 *cpu)
{
	cpu->regs.flags.v = 0;
	return 0;
}

uint8_t op6502_sec(CPU_6502 *cpu)
{
	cpu->regs.flags.c = 1;
	return 0;
}

uint8_t op6502_sed(CPU_6502 *cpu)
{
	cpu->regs.flags.d = 1;
	return 0;
}

uint8_t op6502_sei(CPU_6502 *cpu)
{
	cpu->regs.flags.i = 1;
	return 0;
}


// interrupts

uint8_t op6502_brk(CPU_6502 *cpu)
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

void op6502_irq(CPU_6502 *cpu)
{
	if (!cpu->regs.flags.i)
		cpu6502_interrupt(cpu, IRQ_ADDR, 7);
}

void op6502_nmi(CPU_6502 *cpu)
{
	cpu6502_interrupt(cpu, NMI_ADDR, 8);
}

uint8_t op6502_rti(CPU_6502 *cpu)
{
	uint8_t bits = cpu6502_stack_read(cpu);

	// restore flags
	cpu->regs.flags = *(STATE_6502 *)&bits;
	cpu->regs.flags.b &= ~cpu->regs.flags.b;
	cpu->regs.flags.u &= ~cpu->regs.flags.u;

	// restore counter
	cpu->regs.pc = cpu6502_stack_read_addr(cpu);

	return 0;
}

uint8_t op6502_rts(CPU_6502 *cpu)
{
	cpu->regs.pc = cpu6502_stack_read_addr(cpu);
	return 0;
}


// pushing/popping

uint8_t op6502_pha(CPU_6502 *cpu)
{
	cpu6502_stack_write(cpu, cpu->regs.a);
	return 0;
}

uint8_t op6502_php(CPU_6502 *cpu)
{
	cpu->regs.flags.b = 1;
	cpu->regs.flags.u = 1;
	cpu6502_stack_write(cpu, cpu6502_flags(cpu));
	cpu->regs.flags.b = 0;
	cpu->regs.flags.u = 0;
	return 0;
}

uint8_t op6502_pla(CPU_6502 *cpu)
{
	cpu->regs.a = cpu6502_stack_read(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.a);

	return 0;
}

uint8_t op6502_plp(CPU_6502 *cpu)
{
	cpu6502_stack_read(cpu);
	cpu->regs.flags.u = 1;
	return 0;
}


// arith

uint8_t op6502_adc(CPU_6502 *cpu)
{
	uint16_t tmp = cpu->regs.a + cpu6502_fetch(cpu) + cpu->regs.flags.c;

	cpu6502_flags_cnz(cpu, tmp);
	cpu->regs.flags.v = ~((cpu->regs.a ^ cpu->cache) & (cpu->regs.a ^ tmp) & 128);

	cpu->regs.a = tmp & 255;

	return 1;
}

uint8_t op6502_dec(CPU_6502 *cpu)
{
	uint8_t tmp = cpu6502_fetch(cpu) - 1;
	cpu6502_write_last(cpu, tmp);
	cpu6502_flags_nz(cpu, cpu->regs.x);

	return 0;
}

uint8_t op6502_dex(CPU_6502 *cpu)
{
	cpu6502_flags_nz(cpu, --cpu->regs.x);
	return 0;
}

uint8_t op6502_dey(CPU_6502 *cpu)
{
	cpu6502_flags_nz(cpu, --cpu->regs.y);
	return 0;
}

uint8_t op6502_inc(CPU_6502 *cpu)
{
	uint8_t tmp = cpu6502_fetch(cpu) + 1;
	cpu6502_write_last(cpu, tmp);
	cpu6502_flags_nz(cpu, tmp);

	return 0;
}

uint8_t op6502_inx(CPU_6502 *cpu)
{
	cpu6502_flags_nz(cpu, ++cpu->regs.x);
	return 0;
}

uint8_t op6502_iny(CPU_6502 *cpu)
{
	cpu6502_flags_nz(cpu, ++cpu->regs.y);
	return 0;
}

uint8_t op6502_sbc(CPU_6502 *cpu)
{
	uint16_t value = cpu6502_fetch(cpu) ^ 255; // invert the value
	uint16_t tmp = cpu->regs.a + value + cpu->regs.flags.c;

	cpu6502_flags_cnz(cpu, tmp);
	cpu->regs.flags.v = (tmp ^ cpu->regs.a) & ((tmp ^ value) & 128);

	cpu->regs.a = tmp & 255;

	return 1;
}


// bitwise

uint8_t op6502_and(CPU_6502 *cpu)
{
	cpu->regs.a &= cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.a);

	return 1;
}

uint8_t op6502_asl(CPU_6502 *cpu)
{
	uint16_t tmp = cpu6502_fetch(cpu) << 1;

	cpu6502_flags_cnz(cpu, tmp);

	if (cpu->ops[cpu->last_op].addr_mode == &am6502_imp)
		cpu->regs.a = tmp & 255;
	else
		cpu6502_write_last(cpu, tmp & 255);

	return 0;
}

uint8_t op6502_eor(CPU_6502 *cpu)
{
	cpu->regs.a ^= cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.a);

	return 1;
}

uint8_t op6502_lsr(CPU_6502 *cpu)
{
	cpu->regs.flags.c = cpu6502_fetch(cpu) & 1 ? 1 : 0;
	uint16_t tmp = cpu->cache >> 1;
	cpu6502_flags_nz(cpu, tmp);

	if (cpu->ops[cpu->last_op].addr_mode == &am6502_imp)
		cpu->regs.a = tmp & 255;
	else
		cpu6502_write_last(cpu, tmp & 255);

	return 0;
}

uint8_t op6502_ora(CPU_6502 *cpu)
{
	cpu->regs.a |= cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.a);

	return 1;
}

uint8_t op6502_rol(CPU_6502 *cpu)
{
	uint16_t tmp = (cpu6502_fetch(cpu) << 1) | cpu->regs.flags.c;
	cpu6502_flags_cnz(cpu, tmp);

	if (cpu->ops[cpu->last_op].addr_mode == &am6502_imp)
		cpu->regs.a = tmp & 255;
	else
		cpu6502_write_last(cpu, tmp & 255);

	return 0;
}

uint8_t op6502_ror(CPU_6502 *cpu)
{
	cpu6502_fetch(cpu);
	uint16_t tmp = (cpu->cache << 7) | (cpu->cache >> 1);
	cpu->regs.flags.c = cpu->cache & 1 ? 1 : 0;
	cpu6502_flags_nz(cpu, tmp);

	if (cpu->ops[cpu->last_op].addr_mode == &am6502_imp)
		cpu->regs.a = tmp & 255;
	else
		cpu6502_write_last(cpu, tmp & 255);

	return 0;
}


// comparison

uint8_t op6502_bit(CPU_6502 *cpu)
{
	uint16_t tmp = cpu->regs.a & cpu6502_fetch(cpu);
	cpu->regs.flags.z = tmp & 255 == 0 ? 1 : 0;
	cpu->regs.flags.n = cpu->cache & 128 ? 1 : 0;
	cpu->regs.flags.v = cpu->cache & 64 ? 1 : 0;

	return 0;
}

uint8_t op6502_cmp(CPU_6502 *cpu)
{
	uint16_t tmp = cpu->regs.a - cpu6502_fetch(cpu);
	cpu->regs.flags.c = cpu->regs.a >= cpu->cache ? 1 : 0;
	cpu6502_flags_nz(cpu, tmp);

	return 1;
}

uint8_t op6502_cpx(CPU_6502 *cpu)
{
	uint16_t tmp = cpu->regs.x - cpu6502_fetch(cpu);
	cpu->regs.flags.c = cpu->regs.x >= cpu->cache ? 1 : 0;
	cpu6502_flags_nz(cpu, tmp);

	return 1;
}

uint8_t op6502_cpy(CPU_6502 *cpu)
{
	uint16_t tmp = cpu->regs.y - cpu6502_fetch(cpu);
	cpu->regs.flags.c = cpu->regs.y >= cpu->cache ? 1 : 0;
	cpu6502_flags_nz(cpu, tmp);

	return 1;
}


// jumping

uint8_t op6502_jmp(CPU_6502 *cpu)
{
	cpu->regs.pc = cpu->last_abs_addr;
	return 0;
}

uint8_t op6502_jsr(CPU_6502 *cpu)
{
	cpu6502_stack_write_addr(cpu, cpu->regs.pc);
	cpu->regs.pc = cpu->last_abs_addr;
	return 0;
}


// loading

uint8_t op6502_lda(CPU_6502 *cpu)
{
	cpu->regs.a = cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.a);
	return 1;
}

uint8_t op6502_ldx(CPU_6502 *cpu)
{
	cpu->regs.x = cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.x);
	return 1;
}

uint8_t op6502_ldy(CPU_6502 *cpu)
{
	cpu->regs.y = cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.y);
	return 1;
}


// storing

uint8_t op6502_sta(CPU_6502 *cpu)
{
	cpu6502_write_last(cpu, cpu->regs.a);
	return 0;
}

uint8_t op6502_stx(CPU_6502 *cpu)
{
	cpu6502_write_last(cpu, cpu->regs.x);
	return 0;
}

uint8_t op6502_sty(CPU_6502 *cpu)
{
	cpu6502_write_last(cpu, cpu->regs.y);
	return 0;
}


// transferring

uint8_t op6502_tax(CPU_6502 *cpu)
{
	cpu->regs.x = cpu->regs.a;
	cpu6502_flags_nz(cpu, cpu->regs.x);
	return 0;
}

uint8_t op6502_tay(CPU_6502 *cpu)
{
	cpu->regs.y = cpu->regs.a;
	cpu6502_flags_nz(cpu, cpu->regs.y);
	return 0;
}

uint8_t op6502_tsx(CPU_6502 *cpu)
{
	cpu->regs.x = cpu->regs.sp;
	cpu6502_flags_nz(cpu, cpu->regs.x);
	return 0;
}

uint8_t op6502_txa(CPU_6502 *cpu)
{
	cpu->regs.a = cpu->regs.x;
	cpu6502_flags_nz(cpu, cpu->regs.a);
	return 0;
}

uint8_t op6502_txs(CPU_6502 *cpu)
{
	cpu->regs.sp = cpu->regs.x;
	return 0;
}

uint8_t op6502_tya(CPU_6502 *cpu)
{
	cpu->regs.a = cpu->regs.y;
	cpu6502_flags_nz(cpu, cpu->regs.a);
	return 0;
}


// illegal opcodes

uint8_t op6502_alr(CPU_6502 *cpu)
{
	return op6502_and(cpu) + op6502_lsr(cpu);
}

uint8_t op6502_anc(CPU_6502 *cpu)
{
	return op6502_and(cpu) + op6502_sec(cpu);
}

uint8_t op6502_ane(CPU_6502 *cpu)
{
	cpu->regs.a = cpu6502_magic(cpu) & cpu->regs.x & cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.a);

	return 0;
}

uint8_t op6502_arr(CPU_6502 *cpu)
{
	return op6502_and(cpu) + op6502_ror(cpu);
}

uint8_t op6502_dcp(CPU_6502 *cpu)
{
	return op6502_dec(cpu) + op6502_cmp(cpu);
}

uint8_t op6502_isc(CPU_6502 *cpu)
{
	return op6502_inc(cpu) + op6502_sbc(cpu);
}

uint8_t op6502_jam(CPU_6502 *cpu)
{
	cpu->regs.pc = 0xFFFF;
	memset(&cpu->regs.flags, 0, sizeof(STATE_6502));
	return 0;
}

uint8_t op6502_las(CPU_6502 *cpu)
{
	return 0;
}

uint8_t op6502_lax(CPU_6502 *cpu)
{
	return op6502_lda(cpu) + op6502_ldx(cpu);
}

uint8_t op6502_lxa(CPU_6502 *cpu)
{
	cpu->regs.x = cpu->regs.a = cpu6502_magic(cpu) & cpu6502_fetch(cpu);
	cpu6502_flags_nz(cpu, cpu->regs.x);
	return 0;
}

uint8_t op6502_nop(CPU_6502 *cpu)
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

uint8_t op6502_rla(CPU_6502 *cpu)
{
	return op6502_rol(cpu) + op6502_and(cpu);
}

uint8_t op6502_rra(CPU_6502 *cpu)
{
	return op6502_ror(cpu) + op6502_adc(cpu);
}

uint8_t op6502_sax(CPU_6502 *cpu)
{
	cpu6502_write_last(cpu, cpu->regs.a & cpu->regs.x);
	return 0;
}

uint8_t op6502_sbx(CPU_6502 *cpu)
{
	cpu->regs.x &= cpu->regs.a;
	return op6502_sbc(cpu);
}

uint8_t op6502_sha(CPU_6502 *cpu)
{
	cpu6502_write_last(cpu, cpu->regs.a & cpu->regs.x & (ABS16_HI(cpu->last_abs_addr) + 1));
	return 0;
}

uint8_t op6502_shx(CPU_6502 *cpu)
{
	cpu6502_write_last(cpu, cpu->regs.x & (ABS16_HI(cpu->last_abs_addr) + 1));
	return 0;
}

uint8_t op6502_shy(CPU_6502 *cpu)
{
	cpu6502_write_last(cpu, cpu->regs.y & (ABS16_HI(cpu->last_abs_addr) + 1));
	return 0;
}

uint8_t op6502_slo(CPU_6502 *cpu)
{
	return op6502_asl(cpu) + op6502_ora(cpu);
}

uint8_t op6502_sre(CPU_6502 *cpu)
{
	return op6502_lsr(cpu) + op6502_eor(cpu);
}

uint8_t op6502_tas(CPU_6502 *cpu)
{
	cpu->regs.sp = cpu->regs.a & cpu->regs.x;
	cpu6502_write_last(cpu, cpu->regs.a & cpu->regs.x & (ABS16_HI(cpu->last_abs_addr) + 1));
}

uint8_t op6502_usbc(CPU_6502 *cpu)
{
	return op6502_sbc(cpu) + op6502_nop(cpu);
}
