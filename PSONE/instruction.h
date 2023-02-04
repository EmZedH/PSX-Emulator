#pragma once
#include "common.h"
#include "definedclasses.h"
class Instruction
{
public:
    u32 instruction;
    Instruction() = default;
    Instruction(u32 instruction);
    u32 function();
    RegisterIndex target();
    u32 immediateVal();
    RegisterIndex source();
    u32 immediateVal_se();
    RegisterIndex subRegIndex();
    u32 subfunction();
    u32 shift();
    u32 immediateJump();
    u32 cop_opcode();
};