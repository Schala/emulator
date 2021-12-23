#include <stdlib.h>

#include "devices.h"

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


CPU * cpu_alloc(BUS *bus)
{
	CPU *cpu = (CPU *)calloc(0, sizeof(CPU));
	cpu->bus = bus;

	bus_add_device(bus, cpu);

	return cpu;
}
