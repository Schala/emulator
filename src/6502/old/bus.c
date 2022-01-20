#include <stdio.h>
#include <stdlib.h>

#include "../util.h"
#include "bus.h"

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
