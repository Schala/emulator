#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

#include "devices.h"

#define IRQ_ADDR 0xFFFE
#define NMI_ADDR 0xFFFA

// None of these should be inlined, as we need them to be addressable.
// All except no_op and interrupts return the number of additional cycles
// possibly needed.

void no_op();


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


// interrupts

// Program-sourced interrupt
uint8_t op_brk(CPU *);

// Interrupt request
void op_irq(CPU *);

// Non-maskable interrupt
void op_nmi(CPU *);

// Restore from interrupt
uint8_t op_rti(CPU *);

// accumulator register manipulation

// Push accumulator to stack
uint8_t op_pha(CPU *);

// Pop accumulator from stack
uint8_t op_pla(CPU *);


// arithmetic / bitwise

// Add with carry
uint8_t op_adc(CPU *);

// Bitwise AND
uint8_t op_and(CPU *);

// Arithmetical shift left
uint8_t op_asl(CPU *);

// Subtract with carry
uint8_t op_sdc(CPU *);

#endif // _INSTRUCTIONS_H
