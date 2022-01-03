#ifndef _65C816_ISA_H
#define _65C816_ISA_H

#include "devices.h"

// address modes

// absolute indirect
uint8_t am65c816_abi(CPU_65C816 *);

// absolute long
uint8_t am65c816_abl(CPU_65C816 *);

// absolute
uint8_t am65c816_abs(CPU_65C816 *);

// absolute indirect long
uint8_t am65c816_ail(CPU_65C816 *);

// absolute indexed with X
uint8_t am65c816_aix(CPU_65C816 *);

// absolute indexed with Y
uint8_t am65c816_aiy(CPU_65C816 *);

// absolute indexed indirect
uint8_t am65c816_axi(CPU_65C816 *);

// absolute indexed long
uint8_t am65c816_axl(CPU_65C816 *);

// direct indirect long
uint8_t am65c816_dil(CPU_65C816 *);

// direct indirect
uint8_t am65c816_din(CPU_65C816 *);

// direct
uint8_t am65c816_dir(CPU_65C816 *);

// direct indexed with X
uint8_t am65c816_dix(CPU_65C816 *);

// direct indexed with Y
uint8_t am65c816_diy(CPU_65C816 *);

// direct indexed indirect
uint8_t am65c816_dxi(CPU_65C816 *);

// direct indirect indexed long
uint8_t am65c816_dxl(CPU_65C816 *);

// direct indirect indexed
uint8_t am65c816_dyi(CPU_65C816 *);

// immediate
uint8_t am65c816_imm(CPU_65C816 *);

// implied
uint8_t am65c816_imp(CPU_65C816 *);

// block move
uint8_t am65c816_mov(CPU_65C816 *);

// relative
uint8_t am65c816_rel(CPU_65C816 *);

// relative long
uint8_t am65c816_rln(CPU_65C816 *);

// stack relative
uint8_t am65c816_sr(CPU_65C816 *);

// stack relative indirect indexed
uint8_t am65c816_sri(CPU_65C816 *);


// operations

// add with carry
uint8_t op65c816_adc(CPU_65C816 *);

// and accumulator with memory
uint8_t op65c816_and(CPU_65C816 *);

// arithmetic shift left
uint8_t op65c816_asl(CPU_65C816 *);

// branch if carry clear
uint8_t op65c816_bcc(CPU_65C816 *);

// branch if carry set
uint8_t op65c816_bcs(CPU_65C816 *);

// branch if equal
uint8_t op65c816_beq(CPU_65C816 *);

// test bit
uint8_t op65c816_bit(CPU_65C816 *);

// branch if negative
uint8_t op65c816_bmi(CPU_65C816 *);

// branch if not equal
uint8_t op65c816_bne(CPU_65C816 *);

// branch if positive
uint8_t op65c816_bpl(CPU_65C816 *);

// branch always
uint8_t op65c816_bra(CPU_65C816 *);

// program sourced interrupt
uint8_t op65c816_brk(CPU_65C816 *);

// branch long always
uint8_t op65c816_brl(CPU_65C816 *);

// branch if overflow clear
uint8_t op65c816_bvc(CPU_65C816 *);

// clear carry
uint8_t op65c816_clc(CPU_65C816 *);

// clear decimal
uint8_t op65c816_cld(CPU_65C816 *);

// clear interrupt disable
uint8_t op65c816_cli(CPU_65C816 *);

// clear overflow
uint8_t op65c816_clv(CPU_65C816 *);

// compare accumulator with memory
uint8_t op65c816_cmp(CPU_65C816 *);

// co-processor
uint8_t op65c816_cop(CPU_65C816 *);

// compare X index register with memory
uint8_t op65c816_cpx(CPU_65C816 *);

// compare Y index register with memory
uint8_t op65c816_cpy(CPU_65C816 *);

// decrement
uint8_t op65c816_dec(CPU_65C816 *);

// decrement X index register
uint8_t op65c816_dex(CPU_65C816 *);

// decrement Y index register
uint8_t op65c816_dey(CPU_65C816 *);

// exclusive OR accumulator with memory
uint8_t op65c816_eor(CPU_65C816 *);

// increment
uint8_t op65c816_inc(CPU_65C816 *);

// increment X index register
uint8_t op65c816_inx(CPU_65C816 *);

// increment Y index register
uint8_t op65c816_iny(CPU_65C816 *);

// jump long
uint8_t op65c816_jml(CPU_65C816 *);

// jump
uint8_t op65c816_jmp(CPU_65C816 *);

// jump to subroutine long
uint8_t op65c816_jsl(CPU_65C816 *);

// jump to subroutine
uint8_t op65c816_jsr(CPU_65C816 *);

// load accumulator
uint8_t op65c816_lda(CPU_65C816 *);

// load X index register
uint8_t op65c816_ldx(CPU_65C816 *);

// load Y index register
uint8_t op65c816_ldy(CPU_65C816 *);

// logical shift right
uint8_t op65c816_lsr(CPU_65C816 *);

// block move negative
uint8_t op65c816_mvn(CPU_65C816 *);

// block move positive
uint8_t op65c816_mvp(CPU_65C816 *);

// no operation
uint8_t op65c816_nop(CPU_65C816 *);

// or accumulator with memory
uint8_t op65c816_ora(CPU_65C816 *);

// push effective absolute address
uint8_t op65c816_pea(CPU_65C816 *);

// push effective indirect address
uint8_t op65c816_pei(CPU_65C816 *);

// push effective relative indirect address
uint8_t op65c816_per(CPU_65C816 *);

// push accumulator
uint8_t op65c816_pha(CPU_65C816 *);

// push data bank register
uint8_t op65c816_phb(CPU_65C816 *);

// push direct page register
uint8_t op65c816_phd(CPU_65C816 *);

// push program bank register
uint8_t op65c816_phk(CPU_65C816 *);

// push status register
uint8_t op65c816_php(CPU_65C816 *);

// push X index register
uint8_t op65c816_phx(CPU_65C816 *);

// push Y index register
uint8_t op65c816_phy(CPU_65C816 *);

// pull accumulator
uint8_t op65c816_pla(CPU_65C816 *);

// pull data bank register
uint8_t op65c816_plb(CPU_65C816 *);

// pull direct page register
uint8_t op65c816_pld(CPU_65C816 *);

// pull status register
uint8_t op65c816_plp(CPU_65C816 *);

// pull X index register
uint8_t op65c816_plx(CPU_65C816 *);

// pull Y index register
uint8_t op65c816_ply(CPU_65C816 *);

// reset status register
uint8_t op65c816_rep(CPU_65C816 *);

// rotate left
uint8_t op65c816_rol(CPU_65C816 *);

// rotate right
uint8_t op65c816_ror(CPU_65C816 *);

// return from interrupt
uint8_t op65c816_rti(CPU_65C816 *);

// return from subroutine long
uint8_t op65c816_rtl(CPU_65C816 *);

// return from subrutine
uint8_t op65c816_rts(CPU_65C816 *);

// subtract with borrow from accumulator
uint8_t op65c816_sbc(CPU_65C816 *);

// set carry flag
uint8_t op65c816_sec(CPU_65C816 *);

// set decimal flag
uint8_t op65c816_sed(CPU_65C816 *);

// set interrupt disable flag
uint8_t op65c816_sei(CPU_65C816 *);

// set status
uint8_t op65c816_sep(CPU_65C816 *);

// store accumulator to memory
uint8_t op65c816_sta(CPU_65C816 *);

// store processor
uint8_t op65c816_stp(CPU_65C816 *);

// store X index register to memory
uint8_t op65c816_stx(CPU_65C816 *);

// store Y index register to memory
uint8_t op65c816_sty(CPU_65C816 *);

// store 0 to memory
uint8_t op65c816_stz(CPU_65C816 *);

// transfer accumulator to X index register
uint8_t op65c816_tax(CPU_65C816 *);

// transfer accumulator to Y index register
uint8_t op65c816_tay(CPU_65C816 *);

// transfer 16-bit accumulator to direct page register
uint8_t op65c816_tcd(CPU_65C816 *);

// transfer accumulator to stack pointer
uint8_t op65c816_tcs(CPU_65C816 *);

// transfer direct page register to 16-bit accumulator
uint8_t op65c816_tdc(CPU_65C816 *);

// test and reset bits against accumulator
uint8_t op65c816_trb(CPU_65C816 *);

// test and set bits against accumulator
uint8_t op65c816_tsb(CPU_65C816 *);

// transfer stack pointer to 16-bit accumulator
uint8_t op65c816_tsc(CPU_65C816 *);

// transfer stack pointer to X index register
uint8_t op65c816_tsx(CPU_65C816 *);

// transfer X index register to accumulator
uint8_t op65c816_txa(CPU_65C816 *);

// transfer X index register to stack pointer
uint8_t op65c816_txs(CPU_65C816 *);

// transfer X index register to Y index register
uint8_t op65c816_txy(CPU_65C816 *);

// transfer Y index register to accumulator
uint8_t op65c816_tya(CPU_65C816 *);

// transfer Y index register to X index register
uint8_t op65c816_tyx(CPU_65C816 *);

// wait for interrupt
uint8_t op65c816_wai(CPU_65C816 *);

// exchange B and A accumulators
uint8_t op65c816_xba(CPU_65C816 *);

// exchange carry and emulation flags
uint8_t op65c816_xce(CPU_65C816 *);

#endif // _65C816_ISA_H
