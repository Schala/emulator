#ifndef _MIPS_LIB_H
#define _MIPS_LIB_H

#include "devices.h"

// exits program
void mips_exit(R3000 *);

// exits program with integer result
void mips_exit2(R3000 *);

// print character
void mips_print_char(R3000 *);

// print 64 bit float
void mips_print_double(R3000 *);

// print 32 bit float
void mips_print_float(R3000 *);

// print 32 bit integer
void mips_print_int(R3000 *);

// print null terminated string
void mips_print_string(R3000 *);

// read character
void mips_read_char(R3000 *);

// read 64 bit float
void mips_read_double(R3000 *);

// read 32 bit float
void mips_read_float(R3000 *);

// read 32 bit integer
void mips_read_int(R3000 *);

// read null terminated string (identical to fgets)
void mips_read_string(R3000 *);

// returns address to block of memory containing N additional bytes (dynamic allocation)
void mips_sbrk(R3000 *);

#endif // _MIPS_LIB_H
