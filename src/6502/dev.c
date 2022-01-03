#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "isa.h"

static const OPC_6502 OPCODES[] = {
	// 0x
	{ 7, "BRK", &am6502_imp, &op6502_brk },
	{ 6, "ORA", &am6502_izx, &op6502_ora },
	{ 0, "JAM", NULL, &op6502_jam },
	{ 8, "SLO", &am6502_izx, &op6502_slo },
	{ 2, "NOP", &am6502_zp, &op6502_nop },
	{ 3, "ORA", &am6502_zp, &op6502_ora },
	{ 5, "ASL", &am6502_zp, &op6502_asl },
	{ 5, "SLO", &am6502_zp, &op6502_slo },
	{ 3, "PHP", &am6502_imp, &op6502_php },
	{ 2, "ORA", &am6502_imm, &op6502_ora },
	{ 2, "ASL", &am6502_imp, &op6502_asl },
	{ 2, "ANC", &am6502_imm, &op6502_anc },
	{ 4, "NOP", &am6502_abs, &op6502_nop },
	{ 4, "ORA", &am6502_abs, &op6502_ora },
	{ 6, "ASL", &am6502_abs, &op6502_asl },
	{ 6, "SLO", &am6502_abs, &op6502_slo },

	// 1x
	{ 2, "BPL", &am6502_rel, &op6502_bpl },
	{ 5, "ORA", &am6502_izy, &op6502_ora },
	{ 0, "JAM", NULL, &op6502_jam },
	{ 8, "SLO", &am6502_izy, &op6502_slo },
	{ 4, "NOP", &am6502_zpx, &op6502_nop },
	{ 4, "ORA", &am6502_zpx, &op6502_ora },
	{ 6, "ASL", &am6502_zpx, &op6502_asl },
	{ 6, "SLO", &am6502_zpx, &op6502_slo },
	{ 2, "CLC", &am6502_imp, &op6502_clc },
	{ 4, "ORA", &am6502_aby, &op6502_ora },
	{ 2, "NOP", &am6502_imp, &op6502_nop },
	{ 7, "SLO", &am6502_aby, &op6502_slo },
	{ 4, "NOP", &am6502_abx, &op6502_nop },
	{ 4, "ORA", &am6502_abx, &op6502_ora },
	{ 7, "ASL", &am6502_abx, &op6502_asl },
	{ 7, "SLO", &am6502_abx, &op6502_slo },

	// 2x
	{ 6, "JSR", &am6502_abs, &op6502_jsr },
	{ 6, "AND", &am6502_izx, &op6502_and },
	{ 0, "JAM", NULL, &op6502_jam },
	{ 8, "RLA", &am6502_izx, &op6502_rla },
	{ 3, "BIT", &am6502_zp, &op6502_bit },
	{ 3, "AND", &am6502_zp, &op6502_and },
	{ 5, "ROL", &am6502_zp, &op6502_rol },
	{ 5, "RLA", &am6502_zp, &op6502_rla },
	{ 4, "PLP", &am6502_imp, &op6502_plp },
	{ 2, "AND", &am6502_imm, &op6502_and },
	{ 2, "ROL", &am6502_imp, &op6502_rol },
	{ 2, "ANC", &am6502_imm, &op6502_anc },
	{ 4, "BIT", &am6502_abs, &op6502_bit },
	{ 4, "AND", &am6502_abs, &op6502_and },
	{ 6, "ROL", &am6502_abs, &op6502_rol },
	{ 6, "RLA", &am6502_abs, &op6502_rla },

	// 3x
	{ 2, "BMI", &am6502_rel, &op6502_bmi },
	{ 5, "AND", &am6502_izy, &op6502_and },
	{ 0, "JAM", NULL, &op6502_jam },
	{ 8, "RLA", &am6502_izy, &op6502_rla },
	{ 4, "NOP", &am6502_zpx, &op6502_nop },
	{ 4, "AND", &am6502_zpx, &op6502_and },
	{ 6, "ROL", &am6502_zpx, &op6502_rol },
	{ 6, "RLA", &am6502_zpx, &op6502_rla },
	{ 2, "SEC", &am6502_imp, &op6502_sec },
	{ 4, "AND", &am6502_aby, &op6502_and },
	{ 2, "NOP", &am6502_imp, &op6502_nop },
	{ 7, "RLA", &am6502_aby, &op6502_rla },
	{ 4, "NOP", &am6502_abx, &op6502_nop },
	{ 4, "AND", &am6502_abx, &op6502_and },
	{ 7, "ROL", &am6502_abx, &op6502_rol },
	{ 7, "RLA", &am6502_abx, &op6502_rla },

	// 4x
	{ 6, "RTI", &am6502_imp, &op6502_rti },
	{ 6, "EOR", &am6502_izx, &op6502_eor },
	{ 0, "JAM", NULL, &op6502_jam },
	{ 8, "SRE", &am6502_izx, &op6502_sre },
	{ 3, "NOP", &am6502_zp, &op6502_nop },
	{ 3, "EOR", &am6502_zp, &op6502_eor },
	{ 5, "LSR", &am6502_zp, &op6502_lsr },
	{ 5, "SRE", &am6502_zp, &op6502_sre },
	{ 3, "PHA", &am6502_imp, &op6502_pha },
	{ 2, "EOR", &am6502_imm, &op6502_eor },
	{ 2, "LSR", &am6502_imp, &op6502_lsr },
	{ 2, "ALR", &am6502_abs, &op6502_alr },
	{ 3, "JMP", &am6502_abs, &op6502_jmp },
	{ 4, "EOR", &am6502_abs, &op6502_eor },
	{ 6, "LSR", &am6502_abs, &op6502_lsr },
	{ 6, "SRE", &am6502_abs, &op6502_sre },

	// 5x
	{ 2, "BVC", &am6502_rel, &op6502_bvc },
	{ 5, "EOR", &am6502_izy, &op6502_eor },
	{ 0, "JAM", NULL, &op6502_jam },
	{ 8, "SRE", &am6502_izy, &op6502_sre },
	{ 4, "NOP", &am6502_zpx, &op6502_nop },
	{ 4, "EOR", &am6502_zpx, &op6502_eor },
	{ 6, "LSR", &am6502_zpx, &op6502_lsr },
	{ 6, "SRE", &am6502_zpx, &op6502_sre },
	{ 2, "CLI", &am6502_imp, &op6502_cli },
	{ 4, "EOR", &am6502_aby, &op6502_eor },
	{ 2, "NOP", &am6502_imp, &op6502_nop },
	{ 7, "SRE", &am6502_aby, &op6502_sre },
	{ 4, "NOP", &am6502_abx, &op6502_nop },
	{ 4, "EOR", &am6502_abx, &op6502_eor },
	{ 7, "LSR", &am6502_abx, &op6502_lsr },
	{ 7, "SRE", &am6502_abx, &op6502_sre },

	// 6x
	{ 6, "RTS", &am6502_imp, &op6502_rts },
	{ 6, "ADC", &am6502_izx, &op6502_adc },
	{ 0, "JAM", NULL, &op6502_jam },
	{ 8, "RRA", &am6502_izx, &op6502_rra },
	{ 3, "NOP", &am6502_zp, &op6502_nop },
	{ 3, "ADC", &am6502_zp, &op6502_adc },
	{ 5, "ROR", &am6502_zp, &op6502_ror },
	{ 5, "RRA", &am6502_zp, &op6502_rra },
	{ 4, "PLA", &am6502_imp, &op6502_pla },
	{ 2, "ADC", &am6502_imm, &op6502_adc },
	{ 2, "ROR", &am6502_imp, &op6502_ror },
	{ 2, "ARR", &am6502_imm, &op6502_arr },
	{ 5, "JMP", &am6502_ind, &op6502_jmp },
	{ 4, "ADC", &am6502_abs, &op6502_adc },
	{ 6, "ROR", &am6502_abs, &op6502_ror },
	{ 6, "RRA", &am6502_abs, &op6502_rra },

	// 7x
	{ 2, "BVS", &am6502_rel, &op6502_bvs },
	{ 5, "ADC", &am6502_izy, &op6502_adc },
	{ 0, "JAM", NULL, &op6502_jam },
	{ 8, "RRA", &am6502_izy, &op6502_rra },
	{ 4, "NOP", &am6502_zpx, &op6502_nop },
	{ 4, "ADC", &am6502_zpx, &op6502_adc },
	{ 6, "ROR", &am6502_zpx, &op6502_ror },
	{ 6, "RRA", &am6502_zpx, &op6502_rra },
	{ 2, "SEI", &am6502_imp, &op6502_sei },
	{ 4, "ADC", &am6502_aby, &op6502_adc },
	{ 2, "NOP", &am6502_imp, &op6502_nop },
	{ 7, "RRA", &am6502_aby, &op6502_rra },
	{ 4, "NOP", &am6502_abx, &op6502_nop },
	{ 4, "ADC", &am6502_abx, &op6502_adc },
	{ 7, "ROR", &am6502_abx, &op6502_ror },
	{ 7, "RRA", &am6502_abx, &op6502_rra },

	// 8x
	{ 2, "NOP", &am6502_imm, &op6502_nop },
	{ 6, "STA", &am6502_izx, &op6502_sta },
	{ 2, "NOP", &am6502_imm, &op6502_nop },
	{ 6, "SAX", &am6502_izx, &op6502_sax },
	{ 3, "STY", &am6502_zp, &op6502_sty },
	{ 3, "STA", &am6502_zp, &op6502_sta },
	{ 3, "STX", &am6502_zp, &op6502_stx },
	{ 3, "SAX", &am6502_zp, &op6502_sax },
	{ 2, "DEY", &am6502_imp, &op6502_dey },
	{ 2, "NOP", &am6502_imm, &op6502_nop },
	{ 2, "TXA", &am6502_imp, &op6502_txa },
	{ 2, "ANE", &am6502_imm, &op6502_ane },
	{ 4, "STY", &am6502_abs, &op6502_sty },
	{ 4, "STA", &am6502_abs, &op6502_sta },
	{ 4, "STX", &am6502_abs, &op6502_stx },
	{ 4, "SAX", &am6502_abs, &op6502_sax },

	// 9x
	{ 2, "BCC", &am6502_rel, &op6502_bcc },
	{ 6, "STA", &am6502_izy, &op6502_sta },
	{ 0, "JAM", NULL, &op6502_jam },
	{ 6, "SHA", &am6502_izy, &op6502_sha },
	{ 4, "STY", &am6502_zpx, &op6502_sty },
	{ 4, "STA", &am6502_zpx, &op6502_sta },
	{ 4, "STX", &am6502_zpy, &op6502_stx },
	{ 4, "SAX", &am6502_zpy, &op6502_sax },
	{ 2, "TYA", &am6502_imp, &op6502_tya },
	{ 5, "STA", &am6502_aby, &op6502_sta },
	{ 2, "TXS", &am6502_imp, &op6502_txs },
	{ 5, "TAS", &am6502_aby, &op6502_tas },
	{ 5, "SHY", &am6502_abx, &op6502_shy },
	{ 5, "STA", &am6502_abx, &op6502_sta },
	{ 6, "SHX", &am6502_aby, &op6502_shx },
	{ 5, "SHA", &am6502_aby, &op6502_sha },

	// Ax
	{ 2, "LDY", &am6502_imm, &op6502_ldy },
	{ 6, "LDA", &am6502_izx, &op6502_lda },
	{ 2, "LDX", &am6502_imm, &op6502_ldx },
	{ 6, "LAX", &am6502_izx, &op6502_lax },
	{ 3, "LDY", &am6502_zp, &op6502_ldy },
	{ 3, "LDA", &am6502_zp, &op6502_lda },
	{ 3, "LDX", &am6502_zp, &op6502_ldx },
	{ 3, "LAX", &am6502_zp, &op6502_lax },
	{ 2, "TAY", &am6502_imp, &op6502_tay },
	{ 2, "LDA", &am6502_imm, &op6502_lda },
	{ 2, "TAX", &am6502_imp, &op6502_tax },
	{ 2, "LXA", &am6502_imm, &op6502_lxa },
	{ 4, "LDY", &am6502_abs, &op6502_ldy },
	{ 4, "LDA", &am6502_abs, &op6502_lda },
	{ 4, "LDX", &am6502_abs, &op6502_ldx },
	{ 4, "LAX", &am6502_abs, &op6502_lax },

	// Bx
	{ 2, "BCS", &am6502_rel, &op6502_bcs },
	{ 5, "LDA", &am6502_izy, &op6502_lda },
	{ 0, "JAM", NULL, &op6502_jam },
	{ 5, "LAX", &am6502_izy, &op6502_lax },
	{ 4, "LDY", &am6502_zpx, &op6502_ldy },
	{ 4, "LDA", &am6502_zpx, &op6502_lda },
	{ 4, "LDX", &am6502_zpy, &op6502_ldx },
	{ 4, "LAX", &am6502_zpy, &op6502_lax },
	{ 2, "CLV", &am6502_imp, &op6502_clv },
	{ 4, "LDA", &am6502_aby, &op6502_lda },
	{ 2, "TSX", &am6502_imp, &op6502_tsx },
	{ 4, "LAS", &am6502_aby, &op6502_las },
	{ 4, "LDY", &am6502_abx, &op6502_ldy },
	{ 4, "LDA", &am6502_abx, &op6502_lda },
	{ 4, "LDX", &am6502_aby, &op6502_ldx },
	{ 4, "LAX", &am6502_aby, &op6502_lax },

	// Cx
	{ 2, "CPY", &am6502_imm, &op6502_cpy },
	{ 6, "CMP", &am6502_izx, &op6502_cmp },
	{ 2, "NOP", &am6502_imm, &op6502_nop },
	{ 8, "DCP", &am6502_izx, &op6502_dcp },
	{ 3, "CPY", &am6502_zp, &op6502_cpy },
	{ 3, "CMP", &am6502_zp, &op6502_cmp },
	{ 5, "DEC", &am6502_zp, &op6502_dec },
	{ 5, "DCP", &am6502_zp, &op6502_dcp },
	{ 2, "INY", &am6502_imp, &op6502_iny },
	{ 2, "CMP", &am6502_imm, &op6502_cmp },
	{ 2, "DEX", &am6502_imp, &op6502_dex },
	{ 2, "SBX", &am6502_imm, &op6502_sbx },
	{ 4, "CPY", &am6502_abs, &op6502_cpy },
	{ 4, "CMP", &am6502_abs, &op6502_cmp },
	{ 6, "DEC", &am6502_abs, &op6502_dec },
	{ 6, "DCP", &am6502_abs, &op6502_dcp },

	// Dx
	{ 2, "BNE", &am6502_rel, &op6502_bne },
	{ 5, "CMP", &am6502_izy, &op6502_cmp },
	{ 0, "JAM", NULL, &op6502_jam },
	{ 8, "DCP", &am6502_izy, &op6502_dcp },
	{ 4, "NOP", &am6502_zpx, &op6502_nop },
	{ 4, "CMP", &am6502_zpx, &op6502_cmp },
	{ 6, "DEC", &am6502_zpx, &op6502_dec },
	{ 6, "DCP", &am6502_zpx, &op6502_dcp },
	{ 2, "CLD", &am6502_imp, &op6502_cld },
	{ 4, "CMP", &am6502_aby, &op6502_cmp },
	{ 2, "NOP", &am6502_imp, &op6502_nop },
	{ 7, "DCP", &am6502_aby, &op6502_dcp },
	{ 4, "NOP", &am6502_abx, &op6502_nop },
	{ 4, "CMP", &am6502_abx, &op6502_cmp },
	{ 7, "DEC", &am6502_abx, &op6502_dec },
	{ 7, "DCP", &am6502_abx, &op6502_dcp },

	// Ex
	{ 2, "CPX", &am6502_imm, &op6502_cpx },
	{ 6, "SBC", &am6502_izx, &op6502_sbc },
	{ 2, "NOP", &am6502_imm, &op6502_nop },
	{ 8, "ISC", &am6502_izx, &op6502_isc },
	{ 3, "CPX", &am6502_zp, &op6502_cpx },
	{ 3, "SBC", &am6502_zp, &op6502_sbc },
	{ 5, "INC", &am6502_zp, &op6502_inc },
	{ 5, "ISC", &am6502_zp, &op6502_isc },
	{ 2, "INX", &am6502_imp, &op6502_inx },
	{ 2, "SBC", &am6502_imm, &op6502_sbc },
	{ 2, "NOP", &am6502_imp, &op6502_nop },
	{ 2, "USBC", &am6502_imm, &op6502_usbc },
	{ 4, "CPX", &am6502_abs, &op6502_cpx },
	{ 4, "SBC", &am6502_abs, &op6502_sbc },
	{ 6, "INC", &am6502_abs, &op6502_inc },
	{ 6, "ISC", &am6502_abs, &op6502_isc },

	// Fx
	{ 2, "BEQ", &am6502_rel, &op6502_beq },
	{ 5, "SBC", &am6502_izy, &op6502_sbc },
	{ 0, "JAM", NULL, &op6502_jam },
	{ 4, "ISC", &am6502_izy, &op6502_isc },
	{ 4, "NOP", &am6502_zpx, &op6502_nop },
	{ 4, "SBC", &am6502_zpx, &op6502_sbc },
	{ 6, "INC", &am6502_zpx, &op6502_inc },
	{ 6, "ISC", &am6502_zpx, &op6502_isc },
	{ 2, "SED", &am6502_imp, &op6502_sed },
	{ 4, "SBC", &am6502_aby, &op6502_sbc },
	{ 2, "NOP", &am6502_imp, &op6502_nop },
	{ 7, "ISC", &am6502_aby, &op6502_isc },
	{ 4, "NOP", &am6502_abx, &op6502_nop },
	{ 4, "SBC", &am6502_abx, &op6502_sbc },
	{ 7, "INC", &am6502_abx, &op6502_inc },
	{ 7, "ISC", &am6502_abx, &op6502_isc },
};

BUS_6502 * bus6502_alloc(uint16_t ram_size)
{
	BUS_6502 *bus = (BUS_6502 *)calloc(1, sizeof(BUS_6502));
	bus->ram = (uint8_t *)calloc(1, ram_size);
	bus->ram_size = ram_size;

	return bus;
}


DEV_6502 * bus6502_add_device(BUS_6502 *bus, void *dev, uint16_t ram_offset, uint16_t ram_size)
{
	if (!bus) return NULL;
	if (!dev) return NULL;

	if (!bus->dev_list)
		bus->dev_list = (DEV_6502 *)calloc(1, sizeof(DEV_6502));

	DEV_6502 *it = bus->dev_list;

	while (it) it = it->next;

	it = (DEV_6502 *)calloc(0, sizeof(DEV_6502));
	it->ram_offset = ram_offset;
	it->ram_size = ram_size;
	it->data = dev;

	return it;
}

void * bus6502_device(BUS_6502 *bus, size_t index)
{
	if (!bus) return NULL;

	DEV_6502 *it = bus->dev_list;
	while (index--) it = it->next;
	return it->data;
}

void bus6502_free(BUS_6502 *bus)
{
	if (!bus) return;

	DEV_6502 *it = NULL;
	DEV_6502 *next = bus->dev_list;

	while (next)
	{
		it = next;
		next = it->next;
		free(it);
	}

	free(bus);
	bus = NULL;
}

void bus6502_free_device(BUS_6502 *bus, void *dev)
{
	if (!bus) return;
	if (!dev) return;

	DEV_6502 *it = bus->dev_list;
	DEV_6502 *prev = NULL;

	while (dev != it->data)
	{
		if (!it->next) break;

		prev = it;
		it = it->next;
	}

	// only free if we have a match
	if (dev == it->data)
	{
		if (prev)
		{
			DEV_6502 *next = it->next;
			free(it);
			prev->next = next;
		}
		else
			free(it);
	}
}

/*void bus6502_print_ram(const BUS_6502 *bus)
{
	if (!bus) return;

	for (int y = 0; y < 4096; y++)
	{
		// offset
		printf("%04X: ", y * 16);

		// hex
		for (int x = 0; x < 16; x++)
			printf("%02X ", bus->ram[(y * 16) + x]);

		printf("\t");

		// ASCII
		for (int x = 0; x < 16; x++)
		{
			char c = bus->ram[(y * 16) + x];
			printf("%c", isgraph(c) ? c : '.');
		}

		printf("\n");
	}
}*/

int bus6502_ram_dump(const BUS_6502 *bus, size_t iter)
{
	if (!bus) return -1;

	char fmt[BUFSIZ];
	memset((char *)&fmt, 0, BUFSIZ);
	sprintf((char *)&fmt, "6502.%u.dmp", iter);

	FILE *dump = fopen((char *)&fmt, "wb");
	fwrite(&bus->ram, 1, bus->ram_size, dump);

	return fclose(dump);
}

CPU_6502 * cpu6502_alloc(BUS_6502 *bus, uint16_t start, uint16_t end)
{
	CPU_6502 *cpu = (CPU_6502 *)calloc(1, sizeof(CPU_6502));
	cpu->bus = bus;
	cpu->ops = OPCODES;
	cpu->regs.flags.u = 1;
	cpu->regs.sp = STACK_PTR_INIT_6502;

	cpu->node = bus6502_add_device(bus, cpu, start, end + 1);

	return cpu;
}

void cpu6502_clock(CPU_6502 *cpu)
{
	if (!cpu) return;

	if (cpu->cycles == 0)
	{
		// get and increment the counter
		cpu->last_op = cpu6502_read_rom(cpu);

		// set cycles, see if any additional cycles are needed
		cpu->cycles = cpu->ops[cpu->last_op].cycles;
		cpu->cycles += cpu->ops[cpu->last_op].addr_mode(cpu);
		cpu->cycles += cpu->ops[cpu->last_op].op(cpu);
	}

	cpu->cycles--;
}

void cpu6502_disasm(CPU_6502 *cpu, uint16_t addr, uint16_t size)
{
	if (!cpu) return;
	if (cpu->disasm) cpu6502_disasm_free(cpu);

	cpu->disasm = (DISASM_6502 *)calloc(1, sizeof(DISASM_6502));

	DISASM_6502 *it = cpu->disasm;
	uint16_t start_addr = addr;

	while (addr < (start_addr + size))
	{
		it->addr = addr;

		const OPC_6502 *op = &cpu->ops[cpu6502_read(cpu, addr++)];

		sprintf((char *)&it->lhs, "%s", op->sym);

		if (op->addr_mode == &am6502_imm)
			sprintf((char *)&it->rhs, "#$%02X", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &am6502_zp)
			sprintf((char *)&it->rhs, "$%02X", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &am6502_zpx)
			sprintf((char *)&it->rhs, "$%02X, X", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &am6502_zpy)
			sprintf((char *)&it->rhs, "$%02X, Y", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &am6502_izx)
			sprintf((char *)&it->rhs, "($%02X, X)", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &am6502_izy)
			sprintf((char *)&it->rhs, "($%02X, Y)", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &am6502_abs)
		{
			sprintf((char *)&it->rhs, "$%04X", cpu6502_read_addr(cpu, addr));
			addr += 2;
		}
		else if (op->addr_mode == &am6502_abx)
		{
			sprintf((char *)&it->rhs, "$%04X, X", cpu6502_read_addr(cpu, addr));
			addr += 2;
		}
		else if (op->addr_mode == &am6502_aby)
		{
			sprintf((char *)&it->rhs, "$%04X, Y", cpu6502_read_addr(cpu, addr));
			addr += 2;
		}
		else if (op->addr_mode == &am6502_ind)
		{
			sprintf((char *)&it->rhs, "($%04X)", cpu6502_read_addr(cpu, addr));
			addr += 2;
		}
		else if (op->addr_mode == &am6502_rel)
		{
			uint8_t value = cpu6502_read(cpu, addr++);

			sprintf((char *)&it->rhs, "$%02X [$%04X]", value, addr + (int8_t)value);
		}

		it->next = (DISASM_6502 *)calloc(1, sizeof(DISASM_6502));
		it = it->next;
	}
}

void cpu6502_disasm_free(CPU_6502 *cpu)
{
	if (!cpu) return;
	if (!cpu->disasm) return;

	DISASM_6502 *it = NULL;
	DISASM_6502 *next = cpu->disasm;

	while (next)
	{
		it = next;
		next = it->next;
		free(it);
	}
}

const DISASM_6502 * cpu6502_disasm_get(const CPU_6502 *cpu, uint16_t index)
{
	if (!cpu) return NULL;

	DISASM_6502 *it = cpu->disasm;
	while (index--) it = it->next;
	return it;
}

uint8_t cpu6502_fetch(CPU_6502 *cpu)
{
	if (!cpu) return 0;

	if (cpu->ops[cpu->last_op].addr_mode != &am6502_imp)
		cpu->cache = cpu6502_read(cpu, cpu6502_read_addr(cpu, cpu->last_abs_addr));

	return cpu->cache;
}

void cpu6502_free(CPU_6502 *cpu)
{
	if (!cpu) return;

	bus6502_free_device(cpu->bus, cpu);
	cpu6502_disasm_free(cpu);
	free(cpu);
	cpu = NULL;
}

/*void cpu6502_print_all_disasm(const CPU_6502 *cpu)
{
	if (!cpu->disasm) return;

	DISASM_6502 *it = cpu->disasm;

	while (it)
	{
		printf("%04X: %s %s\n", it->addr, it->lhs, it->rhs);
		it = it->next;
	};
}

void cpu6502_print_disasm(const CPU_6502 *cpu, size_t frame_size)
{
	if (!cpu->disasm) return;

	DISASM_6502 *it = cpu->disasm;
	size_t index = 0;

	while (cpu->last_abs_addr != it->addr)
	{
		if (!it->next) break;
		it = it->next;
		index++;
	}

	if (cpu->last_abs_addr == it->addr)
		if (index < frame_size)
			for (int i = 0; i < index; i++)
			{
				const DISASM_6502 *line = cpu6502_disasm_get(cpu, i);
				printf("%04X: %s %s\n", line->addr, line->lhs, line->rhs);
			}
		else
			for (int i = index - frame_size; i < (index + frame_size); i++)
			{
				const DISASM_6502 *line = cpu6502_disasm_get(cpu, i);
				printf("%04X: %s %s\n", line->addr, line->lhs, line->rhs);
			}
}

void cpu6502_print_regs(const CPU_6502 *cpu)
{
	printf("A: %u\tX: %u\tY: %u\n", cpu->regs.a, cpu->regs.x, cpu->regs.y);
	printf("SP: $%04X\tPC: $%04X\n", cpu->regs.sp, cpu->regs.pc);

	printf("FLAGS: ");
	if (cpu->regs.flags.c) printf("C"); else printf("x");
	if (cpu->regs.flags.z) printf("Z"); else printf("x");
	if (cpu->regs.flags.i) printf("I"); else printf("x");
	if (cpu->regs.flags.d) printf("D"); else printf("x");
	if (cpu->regs.flags.b) printf("B"); else printf("x");
	if (cpu->regs.flags.u) printf("U"); else printf("x");
	if (cpu->regs.flags.v) printf("V"); else printf("x");
	if (cpu->regs.flags.n) printf("N\n"); else printf("x\n");
}*/

void cpu6502_reset(CPU_6502 *cpu)
{
	cpu->regs.a = cpu->regs.x = cpu->regs.y = 0;
	cpu6502_flags_init(cpu);
	cpu->regs.sp = STACK_PTR_INIT_6502;

	cpu->last_abs_addr = RESET_ADDR_6502;
	cpu->regs.pc = cpu6502_fetch_addr(cpu);

	cpu->last_rel_addr = 0;
	cpu->last_abs_addr = 0;
	cpu->cache = 0;
	cpu->cycles = 8;
}
