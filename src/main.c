#include <ctype.h>
#include <stdio.h>

#include "6502/nes.h"

int main(int argc, char **argv)
{
	BUS_6502 *bus = bus6502_alloc();
	CPU_6502 *cpu = cpu6502_alloc(bus);
	//NES_ROM *rom = nes_rom_alloc(bus, argv[1]);

	uint8_t one_hundred_doors[] = {
		0xa9, 0x0, 0xa2, 0x64, 0x95, 0xc8, 0xca, 0xd0, 0xfb, 0x95,
		0xc8, 0xa0, 0x1, 0xc0, 0x65, 0xb0, 0x12, 0x98, 0xc9, 0x65,
		0xb0, 0xa, 0xaa, 0xfe, 0x0, 0x2, 0x84, 0x1, 0x65, 0x1, 0x90,
		0xf2, 0xc8, 0xd0, 0xea, 0xa2, 0x64, 0xbd, 0x0, 0x2, 0x29, 0x1,
		0x9d, 0x0, 0x2, 0xca, 0xd0, 0xf5
	};

	bus6502_load(bus, (uint8_t *)one_hundred_doors, 48, 0x8000);
	bus6502_reset_vec(bus, 0x8000);
	cpu6502_reset(cpu);

	/*if (rom)
	{
		nes_print_rom_info(&rom->header);
		printf("\n");
	}*/

	while (1)
	{
		switch (toupper(getchar()))
		{
			case '\n':
				do
				{
					cpu6502_clock(cpu);
					//cpu6502_disasm(cpu);
				} while (cpu->cycles != 0);
				printf("Offset %04X\n", cpu->regs.pc);
				break;
			case 'D':
				printf("\n");
				//bus6502_ram_dump(bus);
				bus6502_print_ram(bus);
				printf("\n");
				break;
			case 'Q':
				goto exit_loop;
				break;
			case 'R':
				printf("\n");
				cpu6502_print_regs(cpu);
				printf("\n");
				break;
			default:
				printf("Unknown input, ignored\n");
				continue;
		}
	}

exit_loop:
	//nes_rom_free(rom);
	cpu6502_free(cpu);
	bus6502_free(bus);

	return 0;
}
