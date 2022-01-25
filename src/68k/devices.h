#ifndef _68K_DEVICES_H
#define _68K_DEVICES_H

#include <array>

#include "../core/devices.h"

class Motorola68K : public Device
{
private:
	struct
	{
		struct
		{
			uint16_t
				t : 2,
				s : 1,
				m : 1,
				_4 : 1,
				i : 3, // interrupt
				_8 : 3,
				x : 1, // extend
				n : 1, // negative
				z : 1, // zero
				v : 1, // overflow
				c : 1; // carry
		} p;

		std::array<uint32_t, 8> d;
		std::array<uint32_t, 7> a;
		uint32_t sp;
		uint32_t pc;
	} m_regs;
};

#endif // _68K_DEVICES_H
