#include <ctype.h>
#include <stdio.h>

#include "6502/nes.h"

int main(int argc, char **argv)
{
	//BUS_6502 *bus = bus6502_alloc();
	//CPU_6502 *cpu = cpu6502_alloc(bus);

	NES *nes = nes_alloc();
	nes_rom_alloc(nes, argv[1]);

	/*uint8_t one_hundred_doors[] = {
		0xa9, 0x0, 0xa2, 0x64, 0x95, 0xc8, 0xca, 0xd0, 0xfb, 0x95,
		0xc8, 0xa0, 0x1, 0xc0, 0x65, 0xb0, 0x12, 0x98, 0xc9, 0x65,
		0xb0, 0xa, 0xaa, 0xfe, 0x0, 0x2, 0x84, 0x1, 0x65, 0x1, 0x90,
		0xf2, 0xc8, 0xd0, 0xea, 0xa2, 0x64, 0xbd, 0x0, 0x2, 0x29, 0x1,
		0x9d, 0x0, 0x2, 0xca, 0xd0, 0xf5
	};*/

	/*uint8_t olc_code[] = {
		0xa2, 10, 0x8e, 0, 0, 0xa2, 3, 0x8e,
		1, 0, 0xac, 0, 0, 0xa9, 0, 24,
		0x6d, 1, 0, 0x88, 0xd0, 0xfa, 0x8d, 2,
		0, 0xea, 0xea, 0xea
	};

	bus6502_load(bus, (uint8_t *)olc_code, 28, 0x8000);
	bus6502_reset_vec(bus, 0x8000);
	cpu6502_disasm(cpu, 0x8000, 28);*/

	printf("Ready! %u\n");

	if (nes->rom)
	{
		nes_print_rom_info(nes);
		printf("\n");
	}

	while (1)
	{
		char c = 0;
		scanf("%c", &c);

		switch (toupper(c))
		{
			case 'A':
				printf("\n");
				disasm6502_print(nes->cpu->disasm);
				printf("\n");
				break;
			case 'B':
			{
				DISASM_6502 *disasm = disasm6502(nes->rom->prg, nes->map->prg_size *
					nes->rom->header.prg_pages);
				disasm6502_print(disasm);
				disasm6502_free(disasm);
				printf("\n");
			}
				break;
			case 'C':
				nes_print_chr(nes);
				break;
			case 'D':
				printf("\n");
				//bus6502_ram_dump(bus);
				bus6502_print_ram(nes->bus);
				printf("\n");
				break;
			case 'F':
				printf("\n");
				cpu6502_print_disasm(nes->cpu, 5);
				printf("\n");
				break;
			case 'P':
				nes_print_prg(nes);
				break;
			case 'Q':
				goto exit_loop;
				break;
			case 'R':
				printf("\n");
				cpu6502_print_regs(nes->cpu);
				printf("\n");
				break;
			case 'S':
				disasm6502(nes->rom->chr, nes->map->chr_size * nes->rom->header.chr_pages);
				printf("\n");
				break;
			default:
				do cpu6502_clock(nes->cpu); while (nes->cpu->cycles != 0);
				printf("Counter @ %04X\n", nes->cpu->regs.pc);
		}
	}

exit_loop:
	//nes_rom_free(rom);
	nes_free(nes);
	//cpu6502_free(cpu);
	//bus6502_free(bus);

	return 0;
}
