#include "instruction.h"

Instruction::Instruction(u32 instruction)
    {
        this->instruction = instruction;
    }
u32 Instruction::function()
    {
        return (instruction >> 26);
    }

RegisterIndex Instruction::target()
    {
        return RegisterIndex((instruction >> 16) & 0x1f);
    }

u32 Instruction::immediateVal()
    {
        return (instruction & 0xffff);
    }

RegisterIndex Instruction::source()
    {
        return RegisterIndex(((instruction >> 21) & 0x1f));
    }
    
u32 Instruction::immediateVal_se()
    {
        return ((u32)(s16)(instruction & 0xffff));
    }
    
RegisterIndex Instruction::subRegIndex() // d
    {
        return RegisterIndex(((instruction >> 11) & 0x1f));
    }
    
u32 Instruction::subfunction()
    {
        return (instruction & 0x3f);
    }
    
u32 Instruction::shift()
    {
        return ((instruction >> 6) & 0x1f);
    }
    
u32 Instruction::immediateJump()
    {
        return (instruction & 0x3ffffff);
    }
    
u32 Instruction::cop_opcode()
    {
        return ((instruction >> 21) & 0x1f);
    }