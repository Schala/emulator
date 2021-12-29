#ifndef _MIPS_ISA_H
#define _MIPS_ISA_H

#include "devices.h"

// add
void mips_add(R3000 *);

// add immediate (const number)
void mips_addi(R3000 *);

// add immediate unsigned
void mips_addiu(R3000 *);

// add unsigned
void mips_addu(R3000 *);

// bitwise and
void mips_and(R3000 *);

// bitwise and immediate
void mips_andi(R3000 *);

// branch on equal
void mips_beq(R3000 *);

// branch on greater or equal
void mips_bge(R3000 *);

// branch on greater than
void mips_bgt(R3000 *);

// branch on less or equal
void mips_ble(R3000 *);

// branch on less than
void mips_blt(R3000 *);

// branch on not equal
void mips_bne(R3000 *);

// divide
void mips_div(R3000 *);

// jump to address
void mips_j(R3000 *);

// jump and link
void mips_jal(R3000 *);

// jump to address in register
void mips_jr(R3000 *);

// load address
void mips_la(R3000 *);

// load immediate
void mips_li(R3000 *);

// load immediate constant into upper 16 bits, zeroes lower 16 bits
void mips_lui(R3000 *);

// load word
void mips_lw(R3000 *);

// move from hi register
void mips_mfhi(R3000 *);

// move from lo register
void mips_mflo(R3000 *);

// general register move
void mips_move(R3000 *);

// multiply without overflow
void mips_mul(R3000 *);

// multiply
void mips_mult(R3000 *);

// bitwise or
void mips_or(R3000 *);

// shift left logical
void mips_sll(R3000 *);

// set on less than
void mips_slt(R3000 *);

// set on less than immediate
void mips_slti(R3000 *);

// shift right logical
void mips_srl(R3000 *);

// subtract
void mips_sub(R3000 *);

// subtract unsigned
void mips_subu(R3000 *);

// store word
void mips_sw(R3000 *);

#endif // _MIPS_ISA_H
