#include <stdio.h>
#include <stdlib.h>

#include "../util.h"
#include "isa.h"

static const OPC_6502 OPCODES[] = {
	// 0x
	{ 7, "brk", &am6502_imp, &op6502_brk },
	{ 6, "ora", &am6502_izx, &op6502_ora },
	{ 0, "jam", NULL, &op6502_jam },
	{ 8, "slo", &am6502_izx, &op6502_slo },
	{ 2, "nop", &am6502_zp, &op6502_nop },
	{ 3, "ora", &am6502_zp, &op6502_ora },
	{ 5, "asl", &am6502_zp, &op6502_asl },
	{ 5, "slo", &am6502_zp, &op6502_slo },
	{ 3, "php", &am6502_imp, &op6502_php },
	{ 2, "ora", &am6502_imm, &op6502_ora },
	{ 2, "asl", &am6502_imp, &op6502_asl },
	{ 2, "anc", &am6502_imm, &op6502_anc },
	{ 4, "nop", &am6502_abs, &op6502_nop },
	{ 4, "ora", &am6502_abs, &op6502_ora },
	{ 6, "asl", &am6502_abs, &op6502_asl },
	{ 6, "slo", &am6502_abs, &op6502_slo },

	// 1x
	{ 2, "bpl", &am6502_rel, &op6502_bpl },
	{ 5, "ora", &am6502_izy, &op6502_ora },
	{ 0, "jam", NULL, &op6502_jam },
	{ 8, "slo", &am6502_izy, &op6502_slo },
	{ 4, "nop", &am6502_zpx, &op6502_nop },
	{ 4, "ora", &am6502_zpx, &op6502_ora },
	{ 6, "asl", &am6502_zpx, &op6502_asl },
	{ 6, "slo", &am6502_zpx, &op6502_slo },
	{ 2, "clc", &am6502_imp, &op6502_clc },
	{ 4, "ora", &am6502_aby, &op6502_ora },
	{ 2, "nop", &am6502_imp, &op6502_nop },
	{ 7, "slo", &am6502_aby, &op6502_slo },
	{ 4, "nop", &am6502_abx, &op6502_nop },
	{ 4, "ora", &am6502_abx, &op6502_ora },
	{ 7, "asl", &am6502_abx, &op6502_asl },
	{ 7, "slo", &am6502_abx, &op6502_slo },

	// 2x
	{ 6, "jsr", &am6502_abs, &op6502_jsr },
	{ 6, "and", &am6502_izx, &op6502_and },
	{ 0, "jam", NULL, &op6502_jam },
	{ 8, "rla", &am6502_izx, &op6502_rla },
	{ 3, "bit", &am6502_zp, &op6502_bit },
	{ 3, "and", &am6502_zp, &op6502_and },
	{ 5, "rol", &am6502_zp, &op6502_rol },
	{ 5, "rla", &am6502_zp, &op6502_rla },
	{ 4, "plp", &am6502_imp, &op6502_plp },
	{ 2, "and", &am6502_imm, &op6502_and },
	{ 2, "rol", &am6502_imp, &op6502_rol },
	{ 2, "anc", &am6502_imm, &op6502_anc },
	{ 4, "bit", &am6502_abs, &op6502_bit },
	{ 4, "and", &am6502_abs, &op6502_and },
	{ 6, "rol", &am6502_abs, &op6502_rol },
	{ 6, "rla", &am6502_abs, &op6502_rla },

	// 3x
	{ 2, "bmi", &am6502_rel, &op6502_bmi },
	{ 5, "and", &am6502_izy, &op6502_and },
	{ 0, "jam", NULL, &op6502_jam },
	{ 8, "rla", &am6502_izy, &op6502_rla },
	{ 4, "nop", &am6502_zpx, &op6502_nop },
	{ 4, "and", &am6502_zpx, &op6502_and },
	{ 6, "rol", &am6502_zpx, &op6502_rol },
	{ 6, "rla", &am6502_zpx, &op6502_rla },
	{ 2, "sec", &am6502_imp, &op6502_sec },
	{ 4, "and", &am6502_aby, &op6502_and },
	{ 2, "nop", &am6502_imp, &op6502_nop },
	{ 7, "rla", &am6502_aby, &op6502_rla },
	{ 4, "nop", &am6502_abx, &op6502_nop },
	{ 4, "and", &am6502_abx, &op6502_and },
	{ 7, "rol", &am6502_abx, &op6502_rol },
	{ 7, "rla", &am6502_abx, &op6502_rla },

	// 4x
	{ 6, "rti", &am6502_imp, &op6502_rti },
	{ 6, "eor", &am6502_izx, &op6502_eor },
	{ 0, "jam", NULL, &op6502_jam },
	{ 8, "sre", &am6502_izx, &op6502_sre },
	{ 3, "nop", &am6502_zp, &op6502_nop },
	{ 3, "eor", &am6502_zp, &op6502_eor },
	{ 5, "lsr", &am6502_zp, &op6502_lsr },
	{ 5, "sre", &am6502_zp, &op6502_sre },
	{ 3, "pha", &am6502_imp, &op6502_pha },
	{ 2, "eor", &am6502_imm, &op6502_eor },
	{ 2, "lsr", &am6502_imp, &op6502_lsr },
	{ 2, "alr", &am6502_abs, &op6502_alr },
	{ 3, "jmp", &am6502_abs, &op6502_jmp },
	{ 4, "eor", &am6502_abs, &op6502_eor },
	{ 6, "lsr", &am6502_abs, &op6502_lsr },
	{ 6, "sre", &am6502_abs, &op6502_sre },

	// 5x
	{ 2, "bvc", &am6502_rel, &op6502_bvc },
	{ 5, "eor", &am6502_izy, &op6502_eor },
	{ 0, "jam", NULL, &op6502_jam },
	{ 8, "sre", &am6502_izy, &op6502_sre },
	{ 4, "nop", &am6502_zpx, &op6502_nop },
	{ 4, "eor", &am6502_zpx, &op6502_eor },
	{ 6, "lsr", &am6502_zpx, &op6502_lsr },
	{ 6, "sre", &am6502_zpx, &op6502_sre },
	{ 2, "cli", &am6502_imp, &op6502_cli },
	{ 4, "eor", &am6502_aby, &op6502_eor },
	{ 2, "nop", &am6502_imp, &op6502_nop },
	{ 7, "sre", &am6502_aby, &op6502_sre },
	{ 4, "nop", &am6502_abx, &op6502_nop },
	{ 4, "eor", &am6502_abx, &op6502_eor },
	{ 7, "lsr", &am6502_abx, &op6502_lsr },
	{ 7, "sre", &am6502_abx, &op6502_sre },

	// 6x
	{ 6, "rts", &am6502_imp, &op6502_rts },
	{ 6, "adc", &am6502_izx, &op6502_adc },
	{ 0, "jam", NULL, &op6502_jam },
	{ 8, "rra", &am6502_izx, &op6502_rra },
	{ 3, "nop", &am6502_zp, &op6502_nop },
	{ 3, "adc", &am6502_zp, &op6502_adc },
	{ 5, "ror", &am6502_zp, &op6502_ror },
	{ 5, "rra", &am6502_zp, &op6502_rra },
	{ 4, "pla", &am6502_imp, &op6502_pla },
	{ 2, "adc", &am6502_imm, &op6502_adc },
	{ 2, "ror", &am6502_imp, &op6502_ror },
	{ 2, "arr", &am6502_imm, &op6502_arr },
	{ 5, "jmp", &am6502_ind, &op6502_jmp },
	{ 4, "adc", &am6502_abs, &op6502_adc },
	{ 6, "ror", &am6502_abs, &op6502_ror },
	{ 6, "rra", &am6502_abs, &op6502_rra },

	// 7x
	{ 2, "bvs", &am6502_rel, &op6502_bvs },
	{ 5, "adc", &am6502_izy, &op6502_adc },
	{ 0, "jam", NULL, &op6502_jam },
	{ 8, "rra", &am6502_izy, &op6502_rra },
	{ 4, "nop", &am6502_zpx, &op6502_nop },
	{ 4, "adc", &am6502_zpx, &op6502_adc },
	{ 6, "ror", &am6502_zpx, &op6502_ror },
	{ 6, "rra", &am6502_zpx, &op6502_rra },
	{ 2, "sei", &am6502_imp, &op6502_sei },
	{ 4, "adc", &am6502_aby, &op6502_adc },
	{ 2, "nop", &am6502_imp, &op6502_nop },
	{ 7, "rra", &am6502_aby, &op6502_rra },
	{ 4, "nop", &am6502_abx, &op6502_nop },
	{ 4, "adc", &am6502_abx, &op6502_adc },
	{ 7, "ror", &am6502_abx, &op6502_ror },
	{ 7, "rra", &am6502_abx, &op6502_rra },

	// 8x
	{ 2, "nop", &am6502_imm, &op6502_nop },
	{ 6, "sta", &am6502_izx, &op6502_sta },
	{ 2, "nop", &am6502_imm, &op6502_nop },
	{ 6, "sax", &am6502_izx, &op6502_sax },
	{ 3, "sty", &am6502_zp, &op6502_sty },
	{ 3, "sta", &am6502_zp, &op6502_sta },
	{ 3, "stx", &am6502_zp, &op6502_stx },
	{ 3, "sax", &am6502_zp, &op6502_sax },
	{ 2, "dey", &am6502_imp, &op6502_dey },
	{ 2, "nop", &am6502_imm, &op6502_nop },
	{ 2, "txa", &am6502_imp, &op6502_txa },
	{ 2, "ane", &am6502_imm, &op6502_ane },
	{ 4, "sty", &am6502_abs, &op6502_sty },
	{ 4, "sta", &am6502_abs, &op6502_sta },
	{ 4, "stx", &am6502_abs, &op6502_stx },
	{ 4, "sax", &am6502_abs, &op6502_sax },

	// 9x
	{ 2, "bcc", &am6502_rel, &op6502_bcc },
	{ 6, "sta", &am6502_izy, &op6502_sta },
	{ 0, "jam", NULL, &op6502_jam },
	{ 6, "sha", &am6502_izy, &op6502_sha },
	{ 4, "sty", &am6502_zpx, &op6502_sty },
	{ 4, "sta", &am6502_zpx, &op6502_sta },
	{ 4, "stx", &am6502_zpy, &op6502_stx },
	{ 4, "sax", &am6502_zpy, &op6502_sax },
	{ 2, "tya", &am6502_imp, &op6502_tya },
	{ 5, "sta", &am6502_aby, &op6502_sta },
	{ 2, "txs", &am6502_imp, &op6502_txs },
	{ 5, "tas", &am6502_aby, &op6502_tas },
	{ 5, "shy", &am6502_abx, &op6502_shy },
	{ 5, "sta", &am6502_abx, &op6502_sta },
	{ 6, "shx", &am6502_aby, &op6502_shx },
	{ 5, "sha", &am6502_aby, &op6502_sha },

	// ax
	{ 2, "ldy", &am6502_imm, &op6502_ldy },
	{ 6, "lda", &am6502_izx, &op6502_lda },
	{ 2, "ldx", &am6502_imm, &op6502_ldx },
	{ 6, "lax", &am6502_izx, &op6502_lax },
	{ 3, "ldy", &am6502_zp, &op6502_ldy },
	{ 3, "lda", &am6502_zp, &op6502_lda },
	{ 3, "ldx", &am6502_zp, &op6502_ldx },
	{ 3, "lax", &am6502_zp, &op6502_lax },
	{ 2, "tay", &am6502_imp, &op6502_tay },
	{ 2, "lda", &am6502_imm, &op6502_lda },
	{ 2, "tax", &am6502_imp, &op6502_tax },
	{ 2, "lxa", &am6502_imm, &op6502_lxa },
	{ 4, "ldy", &am6502_abs, &op6502_ldy },
	{ 4, "lda", &am6502_abs, &op6502_lda },
	{ 4, "ldx", &am6502_abs, &op6502_ldx },
	{ 4, "lax", &am6502_abs, &op6502_lax },

	// bx
	{ 2, "bcs", &am6502_rel, &op6502_bcs },
	{ 5, "lda", &am6502_izy, &op6502_lda },
	{ 0, "jam", NULL, &op6502_jam },
	{ 5, "lax", &am6502_izy, &op6502_lax },
	{ 4, "ldy", &am6502_zpx, &op6502_ldy },
	{ 4, "lda", &am6502_zpx, &op6502_lda },
	{ 4, "ldx", &am6502_zpy, &op6502_ldx },
	{ 4, "lax", &am6502_zpy, &op6502_lax },
	{ 2, "clv", &am6502_imp, &op6502_clv },
	{ 4, "lda", &am6502_aby, &op6502_lda },
	{ 2, "tsx", &am6502_imp, &op6502_tsx },
	{ 4, "las", &am6502_aby, &op6502_las },
	{ 4, "ldy", &am6502_abx, &op6502_ldy },
	{ 4, "lda", &am6502_abx, &op6502_lda },
	{ 4, "ldx", &am6502_aby, &op6502_ldx },
	{ 4, "lax", &am6502_aby, &op6502_lax },

	// cx
	{ 2, "cpy", &am6502_imm, &op6502_cpy },
	{ 6, "cmp", &am6502_izx, &op6502_cmp },
	{ 2, "nop", &am6502_imm, &op6502_nop },
	{ 8, "dcp", &am6502_izx, &op6502_dcp },
	{ 3, "cpy", &am6502_zp, &op6502_cpy },
	{ 3, "cmp", &am6502_zp, &op6502_cmp },
	{ 5, "dec", &am6502_zp, &op6502_dec },
	{ 5, "dcp", &am6502_zp, &op6502_dcp },
	{ 2, "iny", &am6502_imp, &op6502_iny },
	{ 2, "cmp", &am6502_imm, &op6502_cmp },
	{ 2, "dex", &am6502_imp, &op6502_dex },
	{ 2, "sbx", &am6502_imm, &op6502_sbx },
	{ 4, "cpy", &am6502_abs, &op6502_cpy },
	{ 4, "cmp", &am6502_abs, &op6502_cmp },
	{ 6, "dec", &am6502_abs, &op6502_dec },
	{ 6, "dcp", &am6502_abs, &op6502_dcp },

	// dx
	{ 2, "bne", &am6502_rel, &op6502_bne },
	{ 5, "cmp", &am6502_izy, &op6502_cmp },
	{ 0, "jam", NULL, &op6502_jam },
	{ 8, "dcp", &am6502_izy, &op6502_dcp },
	{ 4, "nop", &am6502_zpx, &op6502_nop },
	{ 4, "cmp", &am6502_zpx, &op6502_cmp },
	{ 6, "dec", &am6502_zpx, &op6502_dec },
	{ 6, "dcp", &am6502_zpx, &op6502_dcp },
	{ 2, "cld", &am6502_imp, &op6502_cld },
	{ 4, "cmp", &am6502_aby, &op6502_cmp },
	{ 2, "nop", &am6502_imp, &op6502_nop },
	{ 7, "dcp", &am6502_aby, &op6502_dcp },
	{ 4, "nop", &am6502_abx, &op6502_nop },
	{ 4, "cmp", &am6502_abx, &op6502_cmp },
	{ 7, "dec", &am6502_abx, &op6502_dec },
	{ 7, "dcp", &am6502_abx, &op6502_dcp },

	// ex
	{ 2, "cpx", &am6502_imm, &op6502_cpx },
	{ 6, "sbc", &am6502_izx, &op6502_sbc },
	{ 2, "nop", &am6502_imm, &op6502_nop },
	{ 8, "isc", &am6502_izx, &op6502_isc },
	{ 3, "cpx", &am6502_zp, &op6502_cpx },
	{ 3, "sbc", &am6502_zp, &op6502_sbc },
	{ 5, "inc", &am6502_zp, &op6502_inc },
	{ 5, "isc", &am6502_zp, &op6502_isc },
	{ 2, "inx", &am6502_imp, &op6502_inx },
	{ 2, "sbc", &am6502_imm, &op6502_sbc },
	{ 2, "nop", &am6502_imp, &op6502_nop },
	{ 2, "usbc", &am6502_imm, &op6502_usbc },
	{ 4, "cpx", &am6502_abs, &op6502_cpx },
	{ 4, "sbc", &am6502_abs, &op6502_sbc },
	{ 6, "inc", &am6502_abs, &op6502_inc },
	{ 6, "isc", &am6502_abs, &op6502_isc },

	// fx
	{ 2, "beq", &am6502_rel, &op6502_beq },
	{ 5, "sbc", &am6502_izy, &op6502_sbc },
	{ 0, "jam", NULL, &op6502_jam },
	{ 4, "isc", &am6502_izy, &op6502_isc },
	{ 4, "nop", &am6502_zpx, &op6502_nop },
	{ 4, "sbc", &am6502_zpx, &op6502_sbc },
	{ 6, "inc", &am6502_zpx, &op6502_inc },
	{ 6, "isc", &am6502_zpx, &op6502_isc },
	{ 2, "sed", &am6502_imp, &op6502_sed },
	{ 4, "sbc", &am6502_aby, &op6502_sbc },
	{ 2, "nop", &am6502_imp, &op6502_nop },
	{ 7, "isc", &am6502_aby, &op6502_isc },
	{ 4, "nop", &am6502_abx, &op6502_nop },
	{ 4, "sbc", &am6502_abx, &op6502_sbc },
	{ 7, "inc", &am6502_abx, &op6502_inc },
	{ 7, "isc", &am6502_abx, &op6502_isc },
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

void bus6502_print_ram(const BUS_6502 *bus)
{
	if (!bus) return;

	hexdump(bus->ram, bus->ram_size);
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
	cpu6502_reset(cpu);

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
			sprintf((char *)&it->rhs, "$%02X, x", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &am6502_zpy)
			sprintf((char *)&it->rhs, "$%02X, y", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &am6502_izx)
			sprintf((char *)&it->rhs, "($%02X, x)", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &am6502_izy)
			sprintf((char *)&it->rhs, "($%02X, y)", cpu6502_read(cpu, addr++));
		else if (op->addr_mode == &am6502_abs)
		{
			sprintf((char *)&it->rhs, "$%04X", cpu6502_read_addr(cpu, addr));
			addr += 2;
		}
		else if (op->addr_mode == &am6502_abx)
		{
			sprintf((char *)&it->rhs, "$%04X, x", cpu6502_read_addr(cpu, addr));
			addr += 2;
		}
		else if (op->addr_mode == &am6502_aby)
		{
			sprintf((char *)&it->rhs, "$%04X, y", cpu6502_read_addr(cpu, addr));
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

	disasm6502_free(cpu->disasm);
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

void disasm6502_print(DISASM_6502 *disasm)
{
	if (!disasm) return;

	DISASM_6502 *it = disasm;

	while (it)
	{
		printf("%04X: %s %s\n", it->addr, it->lhs, it->rhs);
		it = it->next;
	};
}

void cpu6502_print_disasm(const CPU_6502 *cpu, size_t frame_size)
{
	if (!cpu) return;
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
	if (!cpu) return;

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
	if (!cpu) return;

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

void cpu6502_map(CPU_6502 *cpu, uint16_t start, uint16_t end)
{
	if (!cpu) return;

	cpu->node->ram_offset = start;
	cpu->node->ram_size = end + 1;
}

DISASM_6502 * disasm6502(const uint8_t *data, size_t size)
{
	DISASM_6502 *disasm = (DISASM_6502 *)calloc(1, sizeof(DISASM_6502));

	DISASM_6502 *it = disasm;
	size_t offset = 0;

	while (offset < size)
	{
		it->addr = offset & 0xFFFF;

		const OPC_6502 *op = &OPCODES[data[offset++]];

		sprintf((char *)&it->lhs, "%s", op->sym);

		if (op->addr_mode == &am6502_imm)
			sprintf((char *)&it->rhs, "#$%02X", data[offset++]);
		else if (op->addr_mode == &am6502_zp)
			sprintf((char *)&it->rhs, "$%02X", data[offset++]);
		else if (op->addr_mode == &am6502_zpx)
			sprintf((char *)&it->rhs, "$%02X, x", data[offset++]);
		else if (op->addr_mode == &am6502_zpy)
			sprintf((char *)&it->rhs, "$%02X, y", data[offset++]);
		else if (op->addr_mode == &am6502_izx)
			sprintf((char *)&it->rhs, "($%02X, x)", data[offset++]);
		else if (op->addr_mode == &am6502_izy)
			sprintf((char *)&it->rhs, "($%02X, y)", data[offset++]);
		else if (op->addr_mode == &am6502_abs)
			sprintf((char *)&it->rhs, "$%04X", data[offset++] | (data[offset++] << 8));
		else if (op->addr_mode == &am6502_abx)
			sprintf((char *)&it->rhs, "$%04X, x", data[offset++] | (data[offset++] << 8));
		else if (op->addr_mode == &am6502_aby)
			sprintf((char *)&it->rhs, "$%04X, y", data[offset++] | (data[offset++] << 8));
		else if (op->addr_mode == &am6502_ind)
			sprintf((char *)&it->rhs, "($%04X)", data[offset++] | (data[offset++] << 8));
		else if (op->addr_mode == &am6502_rel)
		{
			uint8_t value = data[offset++];

			sprintf((char *)&it->rhs, "$%02X [$%04X]", value, offset + (int8_t)value);
		}

		it->next = (DISASM_6502 *)calloc(1, sizeof(DISASM_6502));
		it = it->next;
	}

	return disasm;
}

void disasm6502_free(DISASM_6502 *disasm)
{
	if (!disasm) return;

	DISASM_6502 *it = NULL;
	DISASM_6502 *next = disasm;

	while (next)
	{
		it = next;
		next = it->next;
		free(it);
	}
}
