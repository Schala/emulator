#ifndef _68K_ISA_H
#define _68K_ISA_H

#include "dev.h"

// Add decimal with extend
void m68k_abcd(M68K_CPU *);

// Add binary
void m68k_add(M68K_CPU *);

// Add address
void m68k_adda(M68K_CPU *);

// Add immediate
void m68k_addi(M68K_CPU *);

// Add quick
void m68k_addq(M68K_CPU *);

// Add extended
void m68k_addx(M68K_CPU *);

// And logical
void m68k_and(M68K_CPU *);

// And immediate
void m68k_andi(M68K_CPU *);

// Arithmetic shift left
void m68k_asl(M68K_CPU *);

// Arithmetic shift right
void m68k_asr(M68K_CPU *);

// Branch on carry clear
void m68k_bcc(M68K_CPU *);

// Test bit and change
void m68k_bchg(M68K_CPU *);

// Test bit and clear
void m68k_bclr(M68K_CPU *);

// Branch on carry set
void m68k_bcs(M68K_CPU *);

// Branch on equal
void m68k_beq(M68K_CPU *);

// Branch on >=
void m68k_bge(M68K_CPU *);

// Branch on >
void m68k_bgt(M68K_CPU *);

// Branch on higher
void m68k_bhi(M68K_CPU *);

// Branch on <=
void m68k_ble(M68K_CPU *);

// Branch on lower or same
void m68k_bls(M68K_CPU *);

// Branch on <
void m68k_blt(M68K_CPU *);

// Branch on negative
void m68k_bmi(M68K_CPU *);

// Branch on not equal
void m68k_bne(M68K_CPU *);

// Branch on positive
void m68k_bpl(M68K_CPU *);

// Branch always
void m68k_bra(M68K_CPU *);

// Test bit and set
void m68k_bset(M68K_CPU *);

// Branch to subroutine
void m68k_bsr(M68K_CPU *);

// Test bit
void m68k_btst(M68K_CPU *);

// Branch on overflow clear
void m68k_bvc(M68K_CPU *);

// Branch on overflow set
void m68k_bvs(M68K_CPU *);

// Check register against bounds
void m68k_chk(M68K_CPU *);

// Clear operand
void m68k_clr(M68K_CPU *);

// Compare
void m68k_cmp(M68K_CPU *);

// Compare address
void m68k_cmpa(M68K_CPU *);

// Compare immediate
void m68k_cmpi(M68K_CPU *);

// Compare memory
void m68k_cmpm(M68K_CPU *);

// Test equal, decrement and branch
void m68k_dbeq(M68K_CPU *);

// Signed divide
void m68k_divs(M68K_CPU *);

// Unsigned divide
void m68k_divu(M68K_CPU *);

// Exclusive or logical
void m68k_eor(M68K_CPU *);

// Exclusive or immediate
void m68k_eori(M68K_CPU *);

// Exchange registers
void m68k_exg(M68K_CPU *);

// Sign-extend data register
void m68k_ext(M68K_CPU *);

// Illegal instruction
void m68k_illegal(M68K_CPU *);

// Unconditional jump
void m68k_jmp(M68K_CPU *);

// Jump to subroutine
void m68k_jsr(M68K_CPU *);

// Load effective address
void m68k_lea(M68K_CPU *);

// Link and allocate
void m68k_link(M68K_CPU *);

// Logical shift left
void m68k_lsl(M68K_CPU *);

// Logical shift right
void m68k_lsr(M68K_CPU *);

// Copy data from source to destination
void m68k_move(M68K_CPU *);

// Move address
void m68k_movea(M68K_CPU *);

// Move multiple registers
void m68k_movem(M68K_CPU *);

// Move peripheral data
void m68k_movep(M68K_CPU *);

// Move quick
void m68k_moveq(M68K_CPU *);

// Signed multiply
void m68k_muls(M68K_CPU *);

// Unsigned multiply
void m68k_mulu(M68K_CPU *);

// Negate decimal with sign extend
void m68k_nbcd(M68K_CPU *);

// Negate
void m68k_neg(M68K_CPU *);

// Negate with extend
void m68k_negx(M68K_CPU *);

// No operation
void m68k_nop(M68K_CPU *);

// Logical compliment
void m68k_not(M68K_CPU *);

// Or logical
void m68k_or(M68K_CPU *);

// Or immediate
void m68k_ori(M68K_CPU *);

// Push effective address
void m68k_pea(M68K_CPU *);

// Reset external devices
void m68k_reset(M68K_CPU *);

// Rotate left
void m68k_rol(M68K_CPU *);

// Rotate right
void m68k_ror(M68K_CPU *);

// Rotate left with extend
void m68k_roxl(M68K_CPU *);

// Rotate right with extend
void m68k_roxr(M68K_CPU *);

// Return from exception
void m68k_rte(M68K_CPU *);

// Return and restore condition codes
void m68k_rtr(M68K_CPU *);

// Return from subroutine
void m68k_rts(M68K_CPU *);

// Subtract decimal with extend
void m68k_sbcd(M68K_CPU *);

// Set on carry clear
void m68k_scc(M68K_CPU *);

// Set on carry set
void m68k_scs(M68K_CPU *);

// Set on equal
void m68k_seq(M68K_CPU *);

// Set on false
void m68k_sf(M68K_CPU *);

// Set on >=
void m68k_sge(M68K_CPU *);

// Set on >
void m68k_sgt(M68K_CPU *);

// Set on higher
void m68k_shi(M68K_CPU *);

// Set on <=
void m68k_sle(M68K_CPU *);

// Set on lower or same
void m68k_sls(M68K_CPU *);

// Set on <
void m68k_slt(M68K_CPU *);

// Set on negative
void m68k_smi(M68K_CPU *);

// Set on not equal
void m68k_sne(M68K_CPU *);

// Set on positive
void m68k_spl(M68K_CPU *);

// Set to subroutine
void m68k_ssr(M68K_CPU *);

// Set if true
void m68k_st(M68K_CPU *);

// Load status register and stop
void m68k_stop(M68K_CPU *);

// Subtract binary
void m68k_sub(M68K_CPU *);

// Subtract address
void m68k_suba(M68K_CPU *);

// Subtract immediate
void m68k_subi(M68K_CPU *);

// Subtract quick
void m68k_subq(M68K_CPU *);

// Subtract extended
void m68k_subx(M68K_CPU *);

// Set on overflow clear
void m68k_svc(M68K_CPU *);

// Set on overflow set
void m68k_svs(M68K_CPU *);

// Swap register halves
void m68k_swap(M68K_CPU *);

// Test and set operand
void m68k_tas(M68K_CPU *);

// Trap
void m68k_trap(M68K_CPU *);

// Trap on overflow
void m68k_trapv(M68K_CPU *);

// Test operand
void m68k_tst(M68K_CPU *);

// Unlink
void m68k_unlk(M68K_CPU *);

#endif // _68K_ISA_H
