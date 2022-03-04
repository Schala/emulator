#ifndef _6500_ISA_H
#define _6500_ISA_H

#include <cstdint>

enum class Address

class ISA6500
{
public:
	uint16_t ADC(uint8_t);

	uint16_t BPL(uint8_t);
	uint16_t BRK();
	uint16_t BVC(uint8_t);
};

#endif // _6500_ISA_H
