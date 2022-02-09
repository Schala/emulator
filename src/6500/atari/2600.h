#ifndef _ATARI_2600_H
#define _ATARI_2600_H

#include "../device.h"

class Atari2600
{
private:
	Bus6502 m_bus;
	MOS6502 m_cpu;
};

#endif // _ATARI_2600_H
