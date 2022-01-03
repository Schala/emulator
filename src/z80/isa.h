#ifndef _Z80_ISA_H
#define _Z80_ISA_H

#include "devices.h"

// add with carry
uint8_t z80_adc(Z80_CPU *);

// add
uint8_t z80_add(Z80_CPU *);

// and
uint8_t z80_and(Z80_CPU *);

// test bit
uint8_t z80_bit(Z80_CPU *);

// call
uint8_t z80_call(Z80_CPU *);

// invert carry flag
uint8_t z80_ccf(Z80_CPU *);

// compare
uint8_t z80_cp(Z80_CPU *);

// compare and decrement
uint8_t z80_cpd(Z80_CPU *);

// compare and decrement and repeat
uint8_t z80_cpdr(Z80_CPU *);

// compare and increment
uint8_t z80_cpi(Z80_CPU *);

// compare and increment and repeat
uint8_t z80_cpir(Z80_CPU *);

// compliment
uint8_t z80_cpl(Z80_CPU *);

// adjust accumulator for add and subtact operations
uint8_t z80_daa(Z80_CPU *);

// decrement
uint8_t z80_dec(Z80_CPU *);

// disable maskable interrupt
uint8_t z80_di(Z80_CPU *);

// decrement and jump if not zero
uint8_t z80_djnz(Z80_CPU *);

// enable interrupt
uint8_t z80_ei(Z80_CPU *);

// exchange
uint8_t z80_ex(Z80_CPU *);

// exchange 2 byte value
uint8_t z80_exx(Z80_CPU *);

// halt until interrupt
uint8_t z80_halt(Z80_CPU *);

// set interrupt mode
uint8_t z80_im(Z80_CPU *);

// place on input
uint8_t z80_in(Z80_CPU *);

// increment
uint8_t z80_inc(Z80_CPU *);

// place on input and decrement
uint8_t z80_ind(Z80_CPU *);

// place on input and decrement and repeat
uint8_t z80_indr(Z80_CPU *);

// place on input and increment
uint8_t z80_ini(Z80_CPU *);

// place on input and increment and repeat
uint8_t z80_inir(Z80_CPU *);

// jump
uint8_t z80_jp(Z80_CPU *);

// conditional jump
uint8_t z80_jr(Z80_CPU *);

// load
uint8_t z80_ld(Z80_CPU *);

// load and decrement
uint8_t z80_ldd(Z80_CPU *);

// load and decrement and repeat
uint8_t z80_lddr(Z80_CPU *);

// load and increment
uint8_t z80_ldi(Z80_CPU *);

// load and increment and repeat
uint8_t z80_ldir(Z80_CPU *);

// negate
uint8_t z80_neg(Z80_CPU *);

// no operation
uint8_t z80_nop(Z80_CPU *);

// or
uint8_t z80_or(Z80_CPU *);

// place on output and decrement and repeat
uint8_t z80_otdr(Z80_CPU *);

// place on output and increment and repeat
uint8_t z80_otir(Z80_CPU *);

// place on output
uint8_t z80_out(Z80_CPU *);

// place on output and decrement
uint8_t z80_outd(Z80_CPU *);

// place on output and increment
uint8_t z80_outi(Z80_CPU *);

// pop
uint8_t z80_pop(Z80_CPU *);

// push
uint8_t z80_push(Z80_CPU *);

// reset bit
uint8_t z80_res(Z80_CPU *);

// return
uint8_t z80_ret(Z80_CPU *);

// return from interrupt
uint8_t z80_reti(Z80_CPU *);

// return from nonmaskable interrupt
uint8_t z80_retn(Z80_CPU *);

// rotate left
uint8_t z80_rl(Z80_CPU *);

// rotate accumulator left
uint8_t z80_rla(Z80_CPU *);

// rotate left with carry
uint8_t z80_rlc(Z80_CPU *);

// rotate accumulator left with carry
uint8_t z80_rlca(Z80_CPU *);

// swap lo/hi bits
uint8_t z80_rld(Z80_CPU *);

// rotate right
uint8_t z80_rr(Z80_CPU *);

// rotate accumulator right
uint8_t z80_rra(Z80_CPU *);

// rotate right with carry
uint8_t z80_rrc(Z80_CPU *);

// rotate accumulator right with carry
uint8_t z80_rrca(Z80_CPU *);

// swap hi/lo bits
uint8_t z80_rrd(Z80_CPU *);

uint8_t z80_rst(Z80_CPU *);

// subtract with carry
uint8_t z80_sbc(Z80_CPU *);

// set carry flag
uint8_t z80_scf(Z80_CPU *);

// set bit
uint8_t z80_set(Z80_CPU *);

// arithmetic shift left
uint8_t z80_sla(Z80_CPU *);

// arithmetic shift right
uint8_t z80_sra(Z80_CPU *);

// logical shift right
uint8_t z80_srl(Z80_CPU *);

// subtract
uint8_t z80_sub(Z80_CPU *);

// exclusive or
uint8_t z80_xor(Z80_CPU *);

#endif // _Z80_ISA_H
