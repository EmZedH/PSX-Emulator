#pragma once
#include "common.h"
#include "definedclasses.h"
#include "interconnect.h"
#include "instruction.h"
class CPU
{
    u32 pc;
    u32 next_pc;
    u32 sr; // Status Register
    u32 current_pc;
    u32 cause;
    u32 epc;
    u32 hi;
    u32 lo;
    bool branch;
    bool delay_slot;
    u32 registers[32] = {0xdeadbeef};
    u32 load_del_regs[32] = {0xdeadbeef};
    RegTuple load;
    Interconnect interconnect;
public:
    CPU() = default;
    CPU(Interconnect interconnect);
    // register functions
    //------------------------------------------------------------------------------------
    u32 getRegisters(RegisterIndex regIndex);

    void setRegisters(RegisterIndex regIndex, u32 value);
    //------------------------------------------------------------------------------------

    // cpu functions
    //------------------------------------------------------------------------------------
    void run_next_instruction();

    u32 load32(u32 addr);
    u32 load16(u32 addr);
    u8 load8(u32 addr);
    void store32(u32 addr, u32 value);

    void store16(u32 addr, u16 value);
    void store8(u32 addr, u8 value);
    void decode_and_execute(u32 instruction);
    // MISCELLANEOUS
    //------------------------------------------------------------------------------------
    bool add_overflow(u32 value, u32 integer);
    bool sub_overflow(u32 value, u32 integer);
    void branch_fn(u32 offset);
    void exception(ExceptionType ex);
    //------------------------------------------------------------------------------------

    // instruction sets
    //------------------------------------------------------------------------------------

    void op_syscall(Instruction instruction);
    void op_lui(Instruction instruction);
    void op_ori(Instruction instruction);
    void op_andi(Instruction instruction);
    void op_sw(Instruction instruction);
    void op_lw(Instruction instruction);
    void op_sll(Instruction instruction);
    void op_addiu(Instruction instruction);
    void op_j(Instruction instruction);
    void op_or(Instruction instruction);
    void op_and(Instruction instruction);
    void op_cop0(Instruction instruction);
    void op_cop1(Instruction instruction);
    void op_cop2(Instruction instruction);
    void op_cop3(Instruction instruction);
    void op_mfc0(Instruction instruction);
    void op_mtc0(Instruction instruction);
    void op_bne(Instruction instruction);
    void op_addi(Instruction instruction);
    void op_sltu(Instruction instruction);
    void op_addu(Instruction instruction);
    void op_sh(Instruction instruction);
    void op_jal(Instruction instruction);
    void op_sb(Instruction instruction);
    void op_jr(Instruction instruction);
    void op_lb(Instruction instruction);
    void op_beq(Instruction instruction);
    void op_add(Instruction instruction);
    void op_bgtz(Instruction instruction);
    void op_blez(Instruction instruction);
    void op_lbu(Instruction instruction);
    void op_jalr(Instruction instruction);
    void op_bxx(Instruction instruction);
    void op_slti(Instruction instruction);
    void op_subu(Instruction instruction);
    void op_sra(Instruction instruction);
    void op_div(Instruction instruction);
    void op_mflo(Instruction instruction);
    void op_srl(Instruction instruction);
    void op_sltiu(Instruction instruction);
    void op_divu(Instruction instruction);
    void op_mfhi(Instruction instruction);
    void op_mtlo(Instruction Instruction);
    void op_mthi(Instruction Instruction);
    void op_slt(Instruction instruction);
    void op_rfe(Instruction Instruction);
    void op_lhu(Instruction instruction);
    void op_sllv(Instruction instruction);
    void op_lh(Instruction instruction);
    void op_nor(Instruction instruction);
    void op_srav(Instruction instruction);
    void op_srlv(Instruction instruction);
    void op_multu(Instruction instruction);
    void op_xor(Instruction instruction);
    void op_break(Instruction instruction);
    void op_mult(Instruction instruction);
    void op_sub(Instruction instruction);
    void op_xori(Instruction instruction);
    void op_lwl(Instruction instruction);
    void op_lwr(Instruction instruction);
    void op_swl(Instruction instruction);
    void op_swr(Instruction instruction);
    void op_lwc0(Instruction instruction);
    void op_lwc1(Instruction instruction);
    void op_lwc2(Instruction instruction);
    void op_lwc3(Instruction instruction);
    void op_swc0(Instruction instruction);
    void op_swc1(Instruction instruction);
    void op_swc2(Instruction instruction);
    void op_swc3(Instruction instruction);
    void op_illegal(Instruction instruction);
    //------------------------------------------------------------------------------------
};