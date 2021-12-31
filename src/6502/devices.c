#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "instructions.h"

static const OPC_6502 OPCODES[] = {
	// 0x
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

	// 1x
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

	// 2x
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

	// 3x
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

	// 4x
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

	// 5x
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

	// 6x
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

	// 7x
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

	// 8x
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

	// 9x
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

	// Ax
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

	// Bx
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

	// Cx
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

	// Dx
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

	// Ex
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

	// Fx
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

BUS_6502 * bus6502_alloc(uint16_t ram_size)
{
	BUS_6502 *bus = (BUS_6502 *)calloc(1, sizeof(BUS_6502));
	bus->ram = (uint8_t *)calloc(1, ram_size);
	bus->ram_size = ram_size;

	return bus;
}


DEV_6502 * bus6502_add_device(BUS_6502 *bus, void *dev, uint16_t ram_offset, uint16_t ram_size)
{
	if (!bus) return;
	if (!dev) return;

	if (!bus->dev_list)
		bus->dev_list = (DEV_6502 *)calloc(1, sizeof(DEV_6502));

	DEV_6502 *it = bus->dev_list;

	while (it) it = it->next;

	it = (DEV_6502 *)calloc(0, sizeof(DEV_6502));
	it->ram_offset = ram_offset;
	it->ram_size = ram_size
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

void bus6502_print_ram(const BUS_6502 *bus)
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
}

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

		if (op->addr_mode == &op_imm)
			sprintf((char *)&it->rhs, "#$%02X", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &op_zp)
			sprintf((char *)&it->rhs, "$%02X", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &op_zpx)
			sprintf((char *)&it->rhs, "$%02X, X", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &op_zpy)
			sprintf((char *)&it->rhs, "$%02X, Y", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &op_izx)
			sprintf((char *)&it->rhs, "($%02X, X)", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &op_izy)
			sprintf((char *)&it->rhs, "($%02X, Y)", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &op_abs)
		{
			sprintf((char *)&it->rhs, "$%04X", cpu6502_read_addr(cpu, addr));
			addr += 2;
		}
		else if (op->addr_mode == &op_abx)
		{
			sprintf((char *)&it->rhs, "$%04X, X", cpu6502_read_addr(cpu, addr));
			addr += 2;
		}
		else if (op->addr_mode == &op_aby)
		{
			sprintf((char *)&it->rhs, "$%04X, Y", cpu6502_read_addr(cpu, addr));
			addr += 2;
		}
		else if (op->addr_mode == &op_ind)
		{
			sprintf((char *)&it->rhs, "($%04X)", cpu6502_read_addr(cpu, addr));
			addr += 2;
		}
		else if (op->addr_mode == &op_rel)
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

	if (cpu->ops[cpu->last_op].addr_mode != &op_imp)
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

void cpu6502_print_all_disasm(const CPU_6502 *cpu)
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
}

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
