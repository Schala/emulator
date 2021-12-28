#include <stdio.h>

#include "nes.h"

int main(int argc, char **argv)
{
	/*BUS *bus = bus_alloc();
	CPU *cpu = cpu_alloc(bus);

	cpu_free(cpu);
	bus_free(bus);*/

	if (nes_load_rom(NULL, argv[1]) == 0)
		printf("\nDone!\n");
	else
		printf("Failure\n");

	return 0;
}
