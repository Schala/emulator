#ifndef _ATARI_2600_H
#define _ATARI_2600_H

#include "../cpu.h"

class Atari2600
{
private:
	BusLE16 m_bus;
	MOS6500 m_cpu;
};

#endif // _ATARI_2600_H
