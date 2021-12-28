#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

#include "devices.h"

#define IRQ_ADDR 0xFFFE
#define NMI_ADDR 0xFFFA

// None of these should be inlined, as we need them to be addressable.
// All except interrupts return the number of additional cycles possibly needed.


// address modes

// Absolute address mode
uint8_t op_abs(CPU *);

// Absolute address mode with X register offset
uint8_t op_abx(CPU *);

// Absolute address mode with Y register offset
uint8_t op_aby(CPU *);

// Immediate address mode
uint8_t op_imm(CPU *);

// Implied address mode
uint8_t op_imp(CPU *);

// Indirect address mode (pointer access)
uint8_t op_ind(CPU *);

// Indirect address mode of zero-page with X register offset
uint8_t op_izx(CPU *);

// Indirect address mode of zero-page with Y register offset
uint8_t op_izy(CPU *);

// Relative address mode (branching)
uint8_t op_rel(CPU *);

// Zero-page address mode
uint8_t op_zp(CPU *);

// Zero-page address mode with X register offset
uint8_t op_zpx(CPU *);

// Zero-page address mode with Y register offset
uint8_t op_zpy(CPU *);


// branching

// Branching if carry bit clear
uint8_t op_bcc(CPU *);

// Branching if carry bit set
uint8_t op_bcs(CPU *);

// Branching if equal
uint8_t op_beq(CPU *);

// Branching if negative
uint8_t op_bmi(CPU *);

// Branching if not equal
uint8_t op_bne(CPU *);

// Branching if positive
uint8_t op_bpl(CPU *);

// Branching if overflow bit clear
uint8_t op_bvc(CPU *);

// Branching if overflow bit set
uint8_t op_bvs(CPU *);


// status bit manipulation

// Clear carry bit
uint8_t op_clc(CPU *);

// Clear decimal mode bit
uint8_t op_cld(CPU *);

// Clear disable interrupts bit
uint8_t op_cli(CPU *);

// Clear overflow bit
uint8_t op_clv(CPU *);

// Set carry bit
uint8_t op_sec(CPU *);

// Set decimal mode bit
uint8_t op_sed(CPU *);

// Set disable interrupts bit
uint8_t op_sei(CPU *);


// interrupts

// Program-sourced interrupt
uint8_t op_brk(CPU *);

// Interrupt request
void op_irq(CPU *);

// Non-maskable interrupt
void op_nmi(CPU *);

// Return from interrupt
uint8_t op_rti(CPU *);

// Return from subroutine
uint8_t op_rts(CPU *);


// pushing/popping

// Push accumulator to stack
uint8_t op_pha(CPU *);

// Push status to stack
uint8_t op_php(CPU *);

// Pop accumulator from stack
uint8_t op_pla(CPU *);

// Pop status from stack
uint8_t op_plp(CPU *);


// arithmetic

// Add with carry
uint8_t op_adc(CPU *);

// Decrement value at location
uint8_t op_dec(CPU *);

// Decrement X register
uint8_t op_dex(CPU *);

// Decrement Y register
uint8_t op_dey(CPU *);

// Increment value at location
uint8_t op_inc(CPU *);

// Increment X register
uint8_t op_inx(CPU *);

// Increment Y register
uint8_t op_iny(CPU *);

// Subtract with carry
uint8_t op_sbc(CPU *);


// bitwise

// Bitwise AND
uint8_t op_and(CPU *);

// Arithmetical shift left
uint8_t op_asl(CPU *);

// Logical shift right
uint8_t op_lsr(CPU *);

// Exclusive OR
uint8_t op_eor(CPU *);

// Bitwise OR
uint8_t op_ora(CPU *);

// Rotate left
uint8_t op_rol(CPU *);

// Rotate right
uint8_t op_ror(CPU *);


// comparison

// bit test
uint8_t op_bit(CPU *);

// Compare accumulator
uint8_t op_cmp(CPU *);

// Compare X register
uint8_t op_cpx(CPU *);

// Compare Y register
uint8_t op_cpy(CPU *);


// jumping

// jump to location
uint8_t op_jmp(CPU *);

// jump to subroutine
uint8_t op_jsr(CPU *);


// loading

// load accumulator
uint8_t op_lda(CPU *);

// load X register
uint8_t op_ldx(CPU *);

// load Y register
uint8_t op_ldy(CPU *);


// storing

// store accumulator at address
uint8_t op_sta(CPU *);

// store X register at address
uint8_t op_stx(CPU *);

// store Y register at address
uint8_t op_sty(CPU *);


// transferring

// transfer accumulator to X register
uint8_t op_tax(CPU *);

// transfer accumulator to Y register
uint8_t op_tay(CPU *);

// transfer stack pointer to X register
uint8_t op_tsx(CPU *);

// transfer X register to accumulator
uint8_t op_txa(CPU *);

// transfer X register to stack pointer
uint8_t op_txs(CPU *);

// transfer Y register to accumulator
uint8_t op_tya(CPU *);


// illegal opcodes

// AND + LSR
uint8_t op_alr(CPU *);

// AND + carry set
uint8_t op_anc(CPU *);

// cpu_magic AND X AND oper
uint8_t op_ane(CPU *);

// AND + ROR
uint8_t op_arr(CPU *);

// DEC + CMP
uint8_t op_dcp(CPU *);

// INC + SBC
uint8_t op_isc(CPU *);

// Jams the CPU into a freeze, requiring a reset
uint8_t op_jam(CPU *);

// LDA / TSX oper
uint8_t op_las(CPU *);

// LDA + LDX
uint8_t op_lax(CPU *);

// store cpu_magic AND oper in accumulator and X
uint8_t op_lxa(CPU *);

// no operation
uint8_t op_nop(CPU *);

// ROL + AND
uint8_t op_rla(CPU *);

// ROR + ADC
uint8_t op_rra(CPU *);

// store accumulator AND X
uint8_t op_sax(CPU *);

// (X = accumulator AND X) + SBC
uint8_t op_sbx(CPU *);

// store accumulator AND X AND hi-addr + 1
uint8_t op_sha(CPU *);

// store X register AND hi-addr + 1
uint8_t op_shx(CPU *);

// store Y register AND hi-addr + 1
uint8_t op_shy(CPU *);

// store ASL + ORA
uint8_t op_slo(CPU *);

// store LSR + EOR
uint8_t op_sre(CPU *);

// transfer accumulator AND X to stack pointer
uint8_t op_tas(CPU *);

// SBC + NOP
uint8_t op_usbc(CPU *);

#endif // _INSTRUCTIONS_H
