#ifndef _6502_ISA_H
#define _6502_ISA_H

#include "cpu.h"

#define IRQ_ADDR 0xFFFE
#define NMI_ADDR 0xFFFA

// None of these should be inlined, as we need them to be addressable.
// All except interrupts return the number of additional cycles possibly needed.


// address modes

// Absolute address mode
uint8_t am6502_abs(CPU_6502 *);

// Absolute address mode with X register offset
uint8_t am6502_abx(CPU_6502 *);

// Absolute address mode with Y register offset
uint8_t am6502_aby(CPU_6502 *);

// Immediate address mode
uint8_t am6502_imm(CPU_6502 *);

// Implied address mode
uint8_t am6502_imp(CPU_6502 *);

// Indirect address mode (pointer access)
uint8_t am6502_ind(CPU_6502 *);

// Indirect address mode of zero-page with X register offset
uint8_t am6502_izx(CPU_6502 *);

// Indirect address mode of zero-page with Y register offset
uint8_t am6502_izy(CPU_6502 *);

// Relative address mode (branching)
uint8_t am6502_rel(CPU_6502 *);

// Zero-page address mode
uint8_t am6502_zp(CPU_6502 *);

// Zero-page address mode with X register offset
uint8_t am6502_zpx(CPU_6502 *);

// Zero-page address mode with Y register offset
uint8_t am6502_zpy(CPU_6502 *);


// branching

// Branching if carry bit clear
uint8_t op6502_bcc(CPU_6502 *);

// Branching if carry bit set
uint8_t op6502_bcs(CPU_6502 *);

// Branching if equal
uint8_t op6502_beq(CPU_6502 *);

// Branching if negative
uint8_t op6502_bmi(CPU_6502 *);

// Branching if not equal
uint8_t op6502_bne(CPU_6502 *);

// Branching if positive
uint8_t op6502_bpl(CPU_6502 *);

// Branching if overflow bit clear
uint8_t op6502_bvc(CPU_6502 *);

// Branching if overflow bit set
uint8_t op6502_bvs(CPU_6502 *);


// status bit manipulation

// Clear carry bit
uint8_t op6502_clc(CPU_6502 *);

// Clear decimal mode bit
uint8_t op6502_cld(CPU_6502 *);

// Clear disable interrupts bit
uint8_t op6502_cli(CPU_6502 *);

// Clear overflow bit
uint8_t op6502_clv(CPU_6502 *);

// Set carry bit
uint8_t op6502_sec(CPU_6502 *);

// Set decimal mode bit
uint8_t op6502_sed(CPU_6502 *);

// Set disable interrupts bit
uint8_t op6502_sei(CPU_6502 *);


// interrupts

// Program-sourced interrupt
uint8_t op6502_brk(CPU_6502 *);

// Interrupt request
void op6502_irq(CPU_6502 *);

// Non-maskable interrupt
void op6502_nmi(CPU_6502 *);

// Return from interrupt
uint8_t op6502_rti(CPU_6502 *);

// Return from subroutine
uint8_t op6502_rts(CPU_6502 *);


// pushing/popping

// Push accumulator to stack
uint8_t op6502_pha(CPU_6502 *);

// Push status to stack
uint8_t op6502_php(CPU_6502 *);

// Pop accumulator from stack
uint8_t op6502_pla(CPU_6502 *);

// Pop status from stack
uint8_t op6502_plp(CPU_6502 *);


// arithmetic

// Add with carry
uint8_t op6502_adc(CPU_6502 *);

// Decrement value at location
uint8_t op6502_dec(CPU_6502 *);

// Decrement X register
uint8_t op6502_dex(CPU_6502 *);

// Decrement Y register
uint8_t op6502_dey(CPU_6502 *);

// Increment value at location
uint8_t op6502_inc(CPU_6502 *);

// Increment X register
uint8_t op6502_inx(CPU_6502 *);

// Increment Y register
uint8_t op6502_iny(CPU_6502 *);

// Subtract with carry
uint8_t op6502_sbc(CPU_6502 *);


// bitwise

// Bitwise AND
uint8_t op6502_and(CPU_6502 *);

// Arithmetical shift left
uint8_t op6502_asl(CPU_6502 *);

// Logical shift right
uint8_t op6502_lsr(CPU_6502 *);

// Exclusive OR
uint8_t op6502_eor(CPU_6502 *);

// Bitwise OR
uint8_t op6502_ora(CPU_6502 *);

// Rotate left
uint8_t op6502_rol(CPU_6502 *);

// Rotate right
uint8_t op6502_ror(CPU_6502 *);


// comparison

// bit test
uint8_t op6502_bit(CPU_6502 *);

// Compare accumulator
uint8_t op6502_cmp(CPU_6502 *);

// Compare X register
uint8_t op6502_cpx(CPU_6502 *);

// Compare Y register
uint8_t op6502_cpy(CPU_6502 *);


// jumping

// jump to location
uint8_t op6502_jmp(CPU_6502 *);

// jump to subroutine
uint8_t op6502_jsr(CPU_6502 *);


// loading

// load accumulator
uint8_t op6502_lda(CPU_6502 *);

// load X register
uint8_t op6502_ldx(CPU_6502 *);

// load Y register
uint8_t op6502_ldy(CPU_6502 *);


// storing

// store accumulator at address
uint8_t op6502_sta(CPU_6502 *);

// store X register at address
uint8_t op6502_stx(CPU_6502 *);

// store Y register at address
uint8_t op6502_sty(CPU_6502 *);


// transferring

// transfer accumulator to X register
uint8_t op6502_tax(CPU_6502 *);

// transfer accumulator to Y register
uint8_t op6502_tay(CPU_6502 *);

// transfer stack pointer to X register
uint8_t op6502_tsx(CPU_6502 *);

// transfer X register to accumulator
uint8_t op6502_txa(CPU_6502 *);

// transfer X register to stack pointer
uint8_t op6502_txs(CPU_6502 *);

// transfer Y register to accumulator
uint8_t op6502_tya(CPU_6502 *);


// illegal opcodes

// AND + LSR
uint8_t op6502_alr(CPU_6502 *);

// AND + carry set
uint8_t op6502_anc(CPU_6502 *);

// cpu_magic AND X AND oper
uint8_t op6502_ane(CPU_6502 *);

// AND + ROR
uint8_t op6502_arr(CPU_6502 *);

// DEC + CMP
uint8_t op6502_dcp(CPU_6502 *);

// INC + SBC
uint8_t op6502_isc(CPU_6502 *);

// Jams the CPU_6502 into a freeze, requiring a reset
uint8_t op6502_jam(CPU_6502 *);

// LDA / TSX oper
uint8_t op6502_las(CPU_6502 *);

// LDA + LDX
uint8_t op6502_lax(CPU_6502 *);

// store cpu_magic AND oper in accumulator and X
uint8_t op6502_lxa(CPU_6502 *);

// no operation
uint8_t op6502_nop(CPU_6502 *);

// ROL + AND
uint8_t op6502_rla(CPU_6502 *);

// ROR + ADC
uint8_t op6502_rra(CPU_6502 *);

// store accumulator AND X
uint8_t op6502_sax(CPU_6502 *);

// (X = accumulator AND X) + SBC
uint8_t op6502_sbx(CPU_6502 *);

// store accumulator AND X AND hi-addr + 1
uint8_t op6502_sha(CPU_6502 *);

// store X register AND hi-addr + 1
uint8_t op6502_shx(CPU_6502 *);

// store Y register AND hi-addr + 1
uint8_t op6502_shy(CPU_6502 *);

// store ASL + ORA
uint8_t op6502_slo(CPU_6502 *);

// store LSR + EOR
uint8_t op6502_sre(CPU_6502 *);

// transfer accumulator AND X to stack pointer
uint8_t op6502_tas(CPU_6502 *);

// SBC + NOP
uint8_t op6502_usbc(CPU_6502 *);

#endif // _6502_ISA_H
