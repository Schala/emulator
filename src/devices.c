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
		{ 2, "ASL", &op_imp, &op_asl },
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
		{ 6, "SLO", &op_zpx, &op_slo },
		{ 2, "CLC", &op_imp, &op_clc },
		{ 4, "ORA", &op_aby, &op_ora },
		{ 2, "NOP", &op_imp, &op_nop },
		{ 7, "SLO", &op_aby, &op_slo },
		{ 4, "NOP", &op_abx, &op_nop },
		{ 4, "ORA", &op_abx, &op_ora },
		{ 7, "ASL", &op_abx, &op_asl },
		{ 7, "SLO", &op_abx, &op_slo },

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
		{ 2, "ROL", &op_imp, &op_rol },
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
		{ 6, "ROL", &op_zpx, &op_rol },
		{ 6, "RLA", &op_zpx, &op_rla },
		{ 2, "SEC", &op_imp, &op_sec },
		{ 4, "AND", &op_aby, &op_and },
		{ 2, "NOP", &op_imp, &op_nop },
		{ 7, "RLA", &op_aby, &op_rla },
		{ 4, "NOP", &op_abx, &op_nop },
		{ 4, "AND", &op_abx, &op_and },
		{ 7, "ROL", &op_abx, &op_rol },
		{ 7, "RLA", &op_abx, &op_rla },

		// 4X
		{ 6, "RTI", &op_imp, &op_rti },
		{ 6, "EOR", &op_izx, &op_eor },
		{ 0, "JAM", NULL, &op_jam },
		{ 8, "SRE", &op_izx, &op_sre },
		{ 3, "NOP", &op_zp, &op_nop },
		{ 3, "EOR", &op_zp, &op_eor },
		{ 5, "LSR", &op_zp, &op_lsr },
		{ 5, "SRE", &op_zp, &op_sre },
		{ 3, "PHA", &op_imp, &op_pha },
		{ 2, "EOR", &op_imm, &op_eor },
		{ 2, "LSR", &op_imp, &op_lsr },
		{ 2, "ALR", &op_abs, &op_alr },
		{ 3, "JMP", &op_abs, &op_jmp },
		{ 4, "EOR", &op_abs, &op_eor },
		{ 6, "LSR", &op_abs, &op_lsr },
		{ 6, "SRE", &op_abs, &op_sre },

		// 5X
		{ 2, "BVC", &op_rel, &op_bvc },
		{ 5, "EOR", &op_izy, &op_eor },
		{ 0, "JAM", NULL, &op_jam },
		{ 8, "SRE", &op_izy, &op_sre },
		{ 4, "NOP", &op_zpx, &op_nop },
		{ 4, "EOR", &op_zpx, &op_eor },
		{ 6, "LSR", &op_zpx, &op_lsr },
		{ 6, "SRE", &op_zpx, &op_sre },
		{ 2, "CLI", &op_imp, &op_cli },
		{ 4, "EOR", &op_aby, &op_eor },
		{ 2, "NOP", &op_imp, &op_nop },
		{ 7, "SRE", &op_aby, &op_sre },
		{ 4, "NOP", &op_abx, &op_nop },
		{ 4, "EOR", &op_abx, &op_eor },
		{ 7, "LSR", &op_abx, &op_lsr },
		{ 7, "SRE", &op_abx, &op_sre },

		// 6X
		{ 6, "RTS", &op_imp, &op_rts },
		{ 6, "ADC", &op_izx, &op_adc },
		{ 0, "JAM", NULL, &op_jam },
		{ 8, "RRA", &op_izx, &op_rra },
		{ 3, "NOP", &op_zp, &op_nop },
		{ 3, "ADC", &op_zp, &op_adc },
		{ 5, "ROR", &op_zp, &op_ror },
		{ 5, "RRA", &op_zp, &op_rra },
		{ 4, "PLA", &op_imp, &op_pla },
		{ 2, "ADC", &op_imm, &op_adc },
		{ 2, "ROR", &op_imp, &op_ror },
		{ 2, "ARR", &op_imm, &op_arr },
		{ 5, "JMP", &op_ind, &op_jmp },
		{ 4, "ADC", &op_abs, &op_adc },
		{ 6, "ROR", &op_abs, &op_ror },
		{ 6, "RRA", &op_abs, &op_rra },

		// 7X
		{ 2, "BVS", &op_rel, &op_bvs },
		{ 5, "ADC", &op_izy, &op_adc },
		{ 0, "JAM", NULL, &op_jam },
		{ 8, "RRA", &op_izy, &op_rra },
		{ 4, "NOP", &op_zpx, &op_nop },
		{ 4, "ADC", &op_zpx, &op_adc },
		{ 6, "ROR", &op_zpx, &op_ror },
		{ 6, "RRA", &op_zpx, &op_rra },
		{ 2, "SEI", &op_imp, &op_sei },
		{ 4, "ADC", &op_aby, &op_adc },
		{ 2, "NOP", &op_imp, &op_nop },
		{ 7, "RRA", &op_aby, &op_rra },
		{ 4, "NOP", &op_abx, &op_nop },
		{ 4, "ADC", &op_abx, &op_adc },
		{ 7, "ROR", &op_abx, &op_ror },
		{ 7, "RRA", &op_abx, &op_rra },

		// 8X
		{ 2, "NOP", &op_imm, &op_nop },
		{ 6, "STA", &op_izx, &op_sta },
		{ 2, "NOP", &op_imm, &op_nop },
		{ 6, "SAX", &op_izx, &op_sax },
		{ 3, "STY", &op_zp, &op_sty },
		{ 3, "STA", &op_zp, &op_sta },
		{ 3, "STX", &op_zp, &op_stx },
		{ 3, "SAX", &op_zp, &op_sax },
		{ 2, "DEY", &op_imp, &op_dey },
		{ 2, "NOP", &op_imm, &op_nop },
		{ 2, "TXA", &op_imp, &op_txa },
		{ 2, "ANE", &op_imm, &op_ane },
		{ 4, "STY", &op_abs, &op_sty },
		{ 4, "STA", &op_abs, &op_sta },
		{ 4, "STX", &op_abs, &op_stx },
		{ 4, "SAX", &op_abs, &op_sax },

		// 9X
		{ 2, "BCC", &op_rel, &op_bcc },
		{ 6, "STA", &op_izy, &op_sta },
		{ 0, "JAM", NULL, &op_jam },
		{ 6, "SHA", &op_izy, &op_sha },
		{ 4, "STY", &op_zpx, &op_sty },
		{ 4, "STA", &op_zpx, &op_sta },
		{ 4, "STX", &op_zpy, &op_stx },
		{ 4, "SAX", &op_zpy, &op_sax },
		{ 2, "TYA", &op_imp, &op_tya },
		{ 5, "STA", &op_aby, &op_sta },
		{ 2, "TXS", &op_imp, &op_txs },
		{ 5, "TAS", &op_aby, &op_tas },
		{ 5, "SHY", &op_abx, &op_shy },
		{ 5, "STA", &op_abx, &op_sta },
		{ 6, "SHX", &op_aby, &op_shx },
		{ 5, "SHA", &op_aby, &op_sha },

		// AX
		{ 2, "LDY", &op_imm, &op_ldy },
		{ 6, "LDA", &op_izx, &op_lda },
		{ 2, "LDX", &op_imm, &op_ldx },
		{ 6, "LAX", &op_izx, &op_lax },
		{ 3, "LDY", &op_zp, &op_ldy },
		{ 3, "LDA", &op_zp, &op_lda },
		{ 3, "LDX", &op_zp, &op_ldx },
		{ 3, "LAX", &op_zp, &op_lax },
		{ 2, "TAY", &op_imp, &op_tay },
		{ 2, "LDA", &op_imm, &op_lda },
		{ 2, "TAX", &op_imp, &op_tax },
		{ 2, "LXA", &op_imm, &op_lxa },
		{ 4, "LDY", &op_abs, &op_ldy },
		{ 4, "LDA", &op_abs, &op_lda },
		{ 4, "LDX", &op_abs, &op_ldx },
		{ 4, "LAX", &op_abs, &op_lax },

		// BX
		{ 2, "BCS", &op_rel, &op_bcs },
		{ 5, "LDA", &op_izy, &op_lda },
		{ 0, "JAM", NULL, &op_jam },
		{ 5, "LAX", &op_izy, &op_lax },
		{ 4, "LDY", &op_zpx, &op_ldy },
		{ 4, "LDA", &op_zpx, &op_lda },
		{ 4, "LDX", &op_zpy, &op_ldx },
		{ 4, "LAX", &op_zpy, &op_lax },
		{ 2, "CLV", &op_imp, &op_clv },
		{ 4, "LDA", &op_aby, &op_lda },
		{ 2, "TSX", &op_imp, &op_tsx },
		{ 4, "LAS", &op_aby, &op_las },
		{ 4, "LDY", &op_abx, &op_ldy },
		{ 4, "LDA", &op_abx, &op_lda },
		{ 4, "LDX", &op_aby, &op_ldx },
		{ 4, "LAX", &op_aby, &op_lax },

		// CX
		{ 2, "CPY", &op_imm, &op_cpy },
		{ 6, "CMP", &op_izx, &op_cmp },
		{ 2, "NOP", &op_imm, &op_nop },
		{ 8, "DCP", &op_izx, &op_dcp },
		{ 3, "CPY", &op_zp, &op_cpy },
		{ 3, "CMP", &op_zp, &op_cmp },
		{ 5, "DEC", &op_zp, &op_dec },
		{ 5, "DCP", &op_zp, &op_dcp },
		{ 2, "INY", &op_imp, &op_iny },
		{ 2, "CMP", &op_imm, &op_cmp },
		{ 2, "DEX", &op_imp, &op_dex },
		{ 2, "SBX", &op_imm, &op_sbx },
		{ 4, "CPY", &op_abs, &op_cpy },
		{ 4, "CMP", &op_abs, &op_cmp },
		{ 6, "DEC", &op_abs, &op_dec },
		{ 6, "DCP", &op_abs, &op_dcp },

		// DX
		{ 2, "BNE", &op_rel, &op_bne },
		{ 5, "CMP", &op_izy, &op_cmp },
		{ 0, "JAM", NULL, &op_jam },
		{ 8, "DCP", &op_izy, &op_dcp },
		{ 4, "NOP", &op_zpx, &op_nop },
		{ 4, "CMP", &op_zpx, &op_cmp },
		{ 6, "DEC", &op_zpx, &op_dec },
		{ 6, "DCP", &op_zpx, &op_dcp },
		{ 2, "CLD", &op_imp, &op_cld },
		{ 4, "CMP", &op_aby, &op_cmp },
		{ 2, "NOP", &op_imp, &op_nop },
		{ 7, "DCP", &op_aby, &op_dcp },
		{ 4, "NOP", &op_abx, &op_nop },
		{ 4, "CMP", &op_abx, &op_cmp },
		{ 7, "DEC", &op_abx, &op_dec },
		{ 7, "DCP", &op_abx, &op_dcp },

		// EX
		{ 2, "CPX", &op_imm, &op_cpx },
		{ 6, "SBC", &op_izx, &op_sbc },
		{ 2, "NOP", &op_imm, &op_nop },
		{ 8, "ISC", &op_izx, &op_isc },
		{ 3, "CPX", &op_zp, &op_cpx },
		{ 3, "SBC", &op_zp, &op_sbc },
		{ 5, "INC", &op_zp, &op_inc },
		{ 5, "ISC", &op_zp, &op_isc },
		{ 2, "INX", &op_imp, &op_inx },
		{ 2, "SBC", &op_imm, &op_sbc },
		{ 2, "NOP", &op_imp, &op_nop },
		{ 2, "USBC", &op_imm, &op_usbc },
		{ 4, "CPX", &op_abs, &op_cpx },
		{ 4, "SBC", &op_abs, &op_sbc },
		{ 6, "INC", &op_abs, &op_inc },
		{ 6, "ISC", &op_abs, &op_isc },

		// FX
		{ 2, "BEQ", &op_rel, &op_beq },
		{ 5, "SBC", &op_izy, &op_sbc },
		{ 0, "JAM", NULL, &op_jam },
		{ 4, "ISC", &op_izy, &op_isc },
		{ 4, "NOP", &op_zpx, &op_nop },
		{ 4, "SBC", &op_zpx, &op_sbc },
		{ 6, "INC", &op_zpx, &op_inc },
		{ 6, "ISC", &op_zpx, &op_isc },
		{ 2, "SED", &op_imp, &op_sed },
		{ 4, "SBC", &op_aby, &op_sbc },
		{ 2, "NOP", &op_imp, &op_nop },
		{ 7, "ISC", &op_aby, &op_isc },
		{ 4, "NOP", &op_abx, &op_nop },
		{ 4, "SBC", &op_abx, &op_sbc },
		{ 7, "INC", &op_abx, &op_inc },
		{ 7, "ISC", &op_abx, &op_isc },
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
