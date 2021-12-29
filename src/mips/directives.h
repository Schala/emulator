#ifndef _MIPS_DIRECTIVES_H
#define _MIPS_DIRECTIVES_H

#include "devices.h"

// align next datum on 2^N byte boundary
void mips_align(R3000 *);

// stores ASCII string
void mips_ascii(R3000 *);

// stores null terminated ASCII string
void mips_asciiz(R3000 *);

// stores N 8 bit values successively
void mips_bytes(R3000 *);

// stores N 16-bit values successively
void mips_half(R3000 *);

// leave N bytes in memory empty for later use
void mips_space(R3000 *);

// stores N 32-bit values successively
void mips_word(R3000 *);

#endif // _MIPS_DIRECTIVES_H
