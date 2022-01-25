#ifndef _MIPS_ISA_H
#define _MIPS_ISA_H

#include "dev.h"

// MIPS instruction data
typedef struct _MIPS_ISA
{
	// Every type of instruction is stored as 32 bits
	union
	{
		struct
		{
			uint32_t
				op : 6, // opcode
				rs : 5, // source register #
				rt : 5, // target register # or branch condition
				imm : 16; // immediate value, branch displacement or address displacement
		} imm; // immediate

		struct
		{
			uint32_t
				op : 6, // opcode
				t : 26; // target address
		} jmp; // jump

		struct
		{
			uint32_t
				op : 6, // opcode
				rs : 5, // source register #
				rt : 5, // target register # or branch condition
				rd : 5, // destination register #
				sa : 5, // shift amount
				f : 6; // function
		} reg; // register
	};

	void (*op)(MIPS_CPU *);
} MIPS_ISA;

// add
void mips_add(MIPS_CPU *);

// add immediate
void mips_addi(MIPS_CPU *);

// add immediate unsigned
void mips_addiu(MIPS_CPU *);

// add unsigned
void mips_addu(MIPS_CPU *);

// and
void mips_and(MIPS_CPU *);

// and immediate
void mips_andi(MIPS_CPU *);

// branch on equal
void mips_beq(MIPS_CPU *);

// branch on equal likely
void mips_beql(MIPS_CPU *);

// branch on >= 0
void mips_bgez(MIPS_CPU *);

// branch on >= 0 and link likely
void mips_bgezall(MIPS_CPU *);

// branch on > 0
void mips_bgtz(MIPS_CPU *);

// branch on > 0 likely
void mips_bgtzl(MIPS_CPU *);

// branch on <= 0
void mips_blez(MIPS_CPU *);

// branch on <= 0 likely
void mips_blezl(MIPS_CPU *);

// branch on < 0
void mips_bltz(MIPS_CPU *);

// branch on < 0 and link
void mips_bltzal(MIPS_CPU *);

// branch on < 0 and link likely
void mips_bltzall(MIPS_CPU *);

// branch on < 0 likely
void mips_bltzl(MIPS_CPU *);

// branch on not equal
void mips_bne(MIPS_CPU *);

// branch on not equal likely
void mips_bnel(MIPS_CPU *);

// breakpoint
void mips_break(MIPS_CPU *);

// double word add
void mips_dadd(MIPS_CPU *);

// double word add immediate
void mips_daddi(MIPS_CPU *);

// double word add immediate unsigned
void mips_daddiu(MIPS_CPU *);

// double word add unsigned
void mips_daddu(MIPS_CPU *);

// double word divide
void mips_ddiv(MIPS_CPU *);

// double word divide unsigned
void mips_ddivu(MIPS_CPU *);

// divide
void mips_div(MIPS_CPU *);

// divide unsigned
void mips_divu(MIPS_CPU *);

// double word move from system control coprocessor
void mips_dmfc0(MIPS_CPU *);

// double word move to system control coprocessor
void mips_dmtc0(MIPS_CPU *);

// double word multiply
void mips_dmult(MIPS_CPU *);

// double word multiply unsigned
void mips_dmultu(MIPS_CPU *);

// double word shift left logical
void mips_dsll(MIPS_CPU *);

// double word shift left logical + 32
void mips_dsll32(MIPS_CPU *);

// double word shift left logical variable
void mips_dsllv(MIPS_CPU *);

// double word shift right arithmetic
void mips_dsra(MIPS_CPU *);

// double word shift right arithmetic + 32
void mips_dsra32(MIPS_CPU *);

// double word shift right arithmetic variable
void mips_dsrav(MIPS_CPU *);

// double word shift right logical
void mips_dsrl(MIPS_CPU *);

// double word shift right logical + 32
void mips_dsrl32(MIPS_CPU *);

// double word shift right logical variable
void mips_dsrlv(MIPS_CPU *);

// double word subtract
void mips_dsub(MIPS_CPU *);

// double word subtract unsigned
void mips_dsubu(MIPS_CPU *);

// return from exception
void mips_eret(MIPS_CPU *);

// jump
void mips_j(MIPS_CPU *);

// jump and link
void mips_jal(MIPS_CPU *);

// jump and link register
void mips_jalr(MIPS_CPU *);

// jump register
void mips_jr(MIPS_CPU *);

// load byte
void mips_lb(MIPS_CPU *);

// load byte unsigned
void mips_lbu(MIPS_CPU *);

// load double word
void mips_ld(MIPS_CPU *);

// load double word left
void mips_ldl(MIPS_CPU *);

// load double word right
void mips_ldr(MIPS_CPU *);

// load half word
void mips_lh(MIPS_CPU *);

// load half word unsigned
void mips_lhu(MIPS_CPU *);

// load linked
void mips_ll(MIPS_CPU *);

// load linked double word
void mips_lld(MIPS_CPU *);

// load upper immediate
void mips_lui(MIPS_CPU *);

// load word
void mips_lw(MIPS_CPU *);

// load word left
void mips_lwl(MIPS_CPU *);

// load word right
void mips_lwr(MIPS_CPU *);

// load word unsigned
void mips_lwu(MIPS_CPU *);

// move from system control coprocessor
void mips_mfc0(MIPS_CPU *);

// move from hi
void mips_mfhi(MIPS_CPU *);

// move from lo
void mips_mflo(MIPS_CPU *);

// move to system control coprocessor
void mips_mtc0(MIPS_CPU *);

// move to hi
void mips_mthi(MIPS_CPU *);

// move to lo
void mips_mtlo(MIPS_CPU *);

// multiply
void mips_mult(MIPS_CPU *);

// multiply unsigned
void mips_multu(MIPS_CPU *);

void mips_nor(MIPS_CPU *);

// or
void mips_or(MIPS_CPU *);

// or immediate
void mips_ori(MIPS_CPU *);

// store byte
void mips_sb(MIPS_CPU *);

// store conditional
void mips_sc(MIPS_CPU *);

// store conditional double word
void mips_scd(MIPS_CPU *);

// store double word
void mips_sd(MIPS_CPU *);

// store double word left
void mips_sdl(MIPS_CPU *);

// store double word right
void mips_sdr(MIPS_CPU *);

// store half word
void mips_sh(MIPS_CPU *);

// shift left logical
void mips_sll(MIPS_CPU *);

// shift left logical variable
void mips_sllv(MIPS_CPU *);

// set on less than
void mips_slt(MIPS_CPU *);

// set on less than immediate
void mips_slti(MIPS_CPU *);

// set on less than immediate unsigned
void mips_sltiu(MIPS_CPU *);

// set on less than unsigned
void mips_sltu(MIPS_CPU *);

// shift right arithmetic
void mips_sra(MIPS_CPU *);

// shift right arithmetic variable
void mips_srav(MIPS_CPU *);

// shift right logical
void mips_srl(MIPS_CPU *);

// shift right logical variable
void mips_srlv(MIPS_CPU *);

// subtract
void mips_sub(MIPS_CPU *);

// subtract unsigned
void mips_subu(MIPS_CPU *);

// store word
void mips_sw(MIPS_CPU *);

// store word left
void mips_swl(MIPS_CPU *);

// store word right
void mips_swr(MIPS_CPU *);

// synchronise
void mips_sync(MIPS_CPU *);

// system call
void mips_syscall(MIPS_CPU *);

// trap if equal
void mips_teq(MIPS_CPU *);

// trap if equal immediate
void mips_teqi(MIPS_CPU *);

// trap if >=
void mips_tge(MIPS_CPU *);

// trap if >= immediate
void mips_tgei(MIPS_CPU *);

// trap if >= immediate unsigned
void mips_tgeiu(MIPS_CPU *);

// trap if >= unsigned
void mips_tgeu(MIPS_CPU *);

// probe TLB for matching entry
void mips_tlbp(MIPS_CPU *);

// read indexed TLB entry
void mips_tlbr(MIPS_CPU *);

// write indexed TLB entry
void mips_tlbwi(MIPS_CPU *);

// write random TLB entry
void mips_tlbwr(MIPS_CPU *);

// trap if <
void mips_tlt(MIPS_CPU *);

// trap if < immediate
void mips_tlti(MIPS_CPU *);

// trap if < immediate unsigned
void mips_tltiu(MIPS_CPU *);

// trap if < unsigned
void mips_tltu(MIPS_CPU *);

// trap if not equal
void mips_tne(MIPS_CPU *);

// trap if not equal immediate
void mips_tnei(MIPS_CPU *);

// exclusive or
void mips_xor(MIPS_CPU *);

// exclusive or immediate
void mips_xori(MIPS_CPU *);

#endif // _MIPS_ISA_H
