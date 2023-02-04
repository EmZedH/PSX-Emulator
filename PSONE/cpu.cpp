#include "cpu.h"
CPU::CPU(Interconnect interconnect)
{
    this->pc = 0xbfc00000;
    this->next_pc = 0xbfc00004;
    this->current_pc = 0x0;
    this->sr = 0x0;
    this->hi = 0xdeadbeef;
    this->lo = 0xdeadbeef;
    this->branch = false;
    this->delay_slot = false;
    this->registers[0] = 0x0;
    this->interconnect = interconnect;
    // this->nextInstruction = Instruction(0x0);
    this->load = RegTuple(RegisterIndex(0), 0);
}
// register functions
//------------------------------------------------------------------------------------
u32 CPU::getRegisters(RegisterIndex regIndex)
{
    return this->registers[regIndex.index];
}

void CPU::setRegisters(RegisterIndex regIndex, u32 value)
{
    this->load_del_regs[regIndex.index] = value;
    this->load_del_regs[0] = 0;
}
//------------------------------------------------------------------------------------

// cpu functions
//------------------------------------------------------------------------------------
void CPU::run_next_instruction()
{
    Instruction instruction(this->load32(this->pc));
    this->delay_slot = this->branch;
    this->branch = false;
    this->current_pc = this->pc;
    if (current_pc % 4 != 0)
    {
        this->exception(ExceptionType::LoadAddressError);
        return;
    }
    this->pc = this->next_pc;
    this->next_pc += 4;

    this->setRegisters(this->load.index, this->load.value);
    // std::cout << std::endl
    //      << "Instruction: "
    //      << std::hex << instruction.instruction << " PC: " << this->pc - 4 << std::endl;
    this->load = RegTuple(RegisterIndex(0), 0);
    decode_and_execute(instruction.instruction);
    for (int i = 0; i < 32; i++)
    {
        this->registers[i] = this->load_del_regs[i];
    }
    //extra
    // this->interconnect.printRam(instruction.instruction,this->pc,instruction.source(),this->getRegisters(instruction.source()));
    //
}

u32 CPU::load32(u32 addr)
{
    return this->interconnect.load32(addr);
}
u32 CPU::load16(u32 addr)
{
    return this->interconnect.load16(addr);
}
u8 CPU::load8(u32 addr)
{
    return this->interconnect.load8(addr);
}
void CPU::store32(u32 addr, u32 value)
{
    this->interconnect.store32(addr, value);
}

void CPU::store16(u32 addr, u16 value)
{
    this->interconnect.store16(addr, value);
}
void CPU::store8(u32 addr, u8 value)
{
    this->interconnect.store8(addr, value);
}
void CPU::decode_and_execute(u32 instruction)
{
    Instruction instruct(instruction);
    switch (instruct.function())
    {
    case 0b000000:
        switch (instruct.subfunction())
        {
        case 0b000000:
            // std::cout << "SLL" << std::endl;
            op_sll(instruct);
            break;
        case 0b100101:
            // std::cout << "OR" << std::endl;
            op_or(instruct);
            break;
        case 0b101011:
            // std::cout << "SLTU" << std::endl;
            op_sltu(instruct);
            break;
        case 0b100001:
            // std::cout << "ADDU" << std::endl;
            op_addu(instruct);
            break;
        case 0b001000:
            // std::cout << "JR" << std::endl;
            op_jr(instruct);
            break;
        case 0b100100:
            // std::cout << "AND" << std::endl;
            op_and(instruct);
            break;
        case 0b100000:
            // std::cout << "ADD" << std::endl;
            op_add(instruct);
            break;
        case 0b001001:
            // std::cout << "JALR" << std::endl;
            op_jalr(instruct);
            break;
        case 0b100011:
            // std::cout << "SUBU" << std::endl;
            op_subu(instruct);
            break;
        case 0b000011:
            // std::cout << "SRA" << std::endl;
            op_sra(instruct);
            break;
        case 0b011010:
            // std::cout << "DIV" << std::endl;
            op_div(instruct);
            break;
        case 0b010010:
            // std::cout << "MFLO" << std::endl;
            op_mflo(instruct);
            break;
        case 0b000010:
            // std::cout << "SRL" << std::endl;
            op_srl(instruct);
            break;
        case 0b010000:
            // std::cout << "MFHI" << std::endl;
            op_mfhi(instruct);
            break;
        case 0b011011:
            // std::cout << "DIVU" << std::endl;
            op_divu(instruct);
            break;
        case 0b101010:
            // std::cout << "SLT" << std::endl;
            op_slt(instruct);
            break;
        case 0b001100:
            // std::cout << "SYSCALL" << std::endl;
            op_syscall(instruct);
            break;
        case 0b010011:
            // std::cout << "MTLO" << std::endl;
            op_mtlo(instruct);
            break;
        case 0b010001:
            // std::cout << "MTHI" << std::endl;
            op_mthi(instruct);
            break;
        case 0b000100:
            // std::cout << "SLLV" << std::endl;
            op_sllv(instruct);
            break;
        case 0b100111:
            // std::cout << "NOR" << std::endl;
            op_nor(instruct);
            break;
        case 0b000111:
            // std::cout << "SRAV" << std::endl;
            op_srav(instruct);
            break;
        case 0b000110:
            // std::cout << "SRLV" << std::endl;
            op_srlv(instruct);
            break;
        case 0b011001:
            // std::cout << "MULTU" << std::endl;
            op_multu(instruct);
            break;
        case 0b001101:
            // std::cout << "BREAK" << std::endl;
            op_break(instruct);
            break;
        case 0b011000:
            // std::cout << "MULT" << std::endl;
            op_mult(instruct);
            break;
        case 0b100010:
            // std::cout << "SUB" << std::endl;
            op_sub(instruct);
            break;
        case 0b001110:
            // std::cout << "XORI" << std::endl;
            op_xori(instruct);
            break;
        case 0b100110:
            // std::cout << "XOR" << std::endl;
            op_xor(instruct);
            break;
        default:
            op_illegal(instruct);
            // std::cout << "Unhandled instruction ";
            // std::cout << std::hex << instruction;
            // std::cout << "\n";
            // exit(0);
        }
        break;
    case 0b001111:
        // std::cout << "LUI" << std::endl;
        op_lui(instruct);
        break;
    case 0b001101:
        // std::cout << "ORI" << std::endl;
        op_ori(instruct);
        break;
    case 0b101011:
        // std::cout << "SW" << std::endl;
        op_sw(instruct);
        break;
    case 0b001001:
        // std::cout << "ADDIU" << std::endl;
        op_addiu(instruct);
        break;
    case 0b000010:
        // std::cout << "J" << std::endl;
        op_j(instruct);
        break;
    case 0b010000:
        // std::cout << "COP0" << std::endl;
        op_cop0(instruct);
        break;
    case 0b010001:
        // std::cout << "COP1" << std::endl;
        op_cop1(instruct);
        break;
    case 0b010010:
        // std::cout << "COP2" << std::endl;
        op_cop2(instruct);
        break;
    case 0b010011:
        // std::cout << "COP3" << std::endl;
        op_cop3(instruct);
        break;
    case 0b000101:
        // std::cout << "BNE" << std::endl;
        op_bne(instruct);
        break;
    case 0b001000:
        // std::cout << "ADDI" << std::endl;
        op_addi(instruct);
        break;
    case 0b100011:
        // std::cout << "LW" << std::endl;
        op_lw(instruct);
        break;
    case 0b101001:
        // std::cout << "SH" << std::endl;
        op_sh(instruct);
        break;
    case 0b000011:
        // std::cout << "JAL" << std::endl;
        op_jal(instruct);
        break;
    case 0b001100:
        // std::cout << "ANDI" << std::endl;
        op_andi(instruct);
        break;
    case 0b101000:
        // std::cout << "SB" << std::endl;
        op_sb(instruct);
        break;
    case 0b100000:
        // std::cout << "LB" << std::endl;
        op_lb(instruct);
        break;
    case 0b000100:
        // std::cout << "BEQ" << std::endl;
        op_beq(instruct);
        break;
    case 0b000111:
        // std::cout << "BGTZ" << std::endl;
        op_bgtz(instruct);
        break;
    case 0b000110:
        // std::cout << "BLEZ" << std::endl;
        op_blez(instruct);
        break;
    case 0b100100:
        // std::cout << "LBU" << std::endl;
        op_lbu(instruct);
        break;
    case 0b000001:
        // std::cout << "BXX" << std::endl;
        op_bxx(instruct);
        break;
    case 0b001010:
        // std::cout << "SLTI" << std::endl;
        op_slti(instruct);
        break;
    case 0b001011:
        // std::cout << "SLTIU" << std::endl;
        op_sltiu(instruct);
        break;
    case 0b100101:
        // std::cout << "LHU" << std::endl;
        op_lhu(instruct);
        break;
    case 0b100001:
        // std::cout << "LH" << std::endl;
        op_lh(instruct);
        break;
    case 0b100010:
        // std::cout << "LWL" << std::endl;
        op_lwl(instruct);
        break;
    case 0b100110:
        // std::cout << "LWR" << std::endl;
        op_lwr(instruct);
        break;
    case 0b101010:
        // std::cout << "SWL" << std::endl;
        op_swl(instruct);
        break;
    case 0b101110:
        // std::cout << "SWR" << std::endl;
        op_swr(instruct);
        break;
    case 0b110000:
        // std::cout << "LWC0" << std::endl;
        op_lwc0(instruct);
        break;
    case 0b110001:
        // std::cout << "LWC1" << std::endl;
        op_lwc1(instruct);
        break;
    case 0b110010:
        // std::cout << "LWC2" << std::endl;
        op_lwc2(instruct);
        break;
    case 0b110011:
        // std::cout << "LWC3" << std::endl;
        op_lwc3(instruct);
        break;
    case 0b111000:
        // std::cout << "SWC0" << std::endl;
        op_swc0(instruct);
        break;
    case 0b111001:
        // std::cout << "SWC1" << std::endl;
        op_swc1(instruct);
        break;
    case 0b111010:
        // std::cout << "SWC2" << std::endl;
        op_swc2(instruct);
        break;
    case 0b111011:
        // std::cout << "SWC3" << std::endl;
        op_swc3(instruct);
        break;
    default:
        op_illegal(instruct);
        // std::cout << "Unhandled instruction ";
        // std::cout << std::hex << instruction;
        // std::cout << "\n";
        // exit(0);
    }
}

// MISCELLANEOUS
//------------------------------------------------------------------------------------
bool CPU::add_overflow(u32 value, u32 integer)
{
    return ((((value + integer) ^ value) & ((value + integer) ^ integer)) & 0x80000000) != 0;
}
bool CPU::sub_overflow(u32 value, u32 integer)
{
    return ((((value - integer) ^ value) & ((value - integer) ^ integer)) & 0x80000000) != 0;
}
void CPU::branch_fn(u32 offset)
{
    // this->pc += ((offset << 2) - 4);
    this->next_pc = this->pc + (offset << 2);
    this->branch = true;
}
void CPU::exception(ExceptionType ex)
{
    u32 handler = ((this->sr & (1 << 22)) != 0) ? 0xbfc00180 : 0x80000080;
    u32 mode = this->sr & 0x3f;
    this->sr &= ~(0x3f);
    this->sr |= (mode << 2) & 0x3f;
    this->cause = (u32)(ex) << 2;
    this->epc = this->current_pc;
    if (delay_slot)
    {
        this->epc = this->epc - 4;
        this->cause |= 1 << 31;
    }
    this->pc = handler;
    this->next_pc = this->pc + 4;
}
//------------------------------------------------------------------------------------

// instruction sets
//------------------------------------------------------------------------------------

void CPU::op_syscall(Instruction instruction)
{
    this->exception(ExceptionType::Syscall);
}
void CPU::op_lui(Instruction instruction) // LUI instruction
{
    this->setRegisters(instruction.target(), instruction.immediateVal() << 16);
}

void CPU::op_ori(Instruction instruction) // ORI instruction
{
    this->setRegisters(instruction.target(), this->getRegisters(instruction.source()) | instruction.immediateVal());
}
void CPU::op_andi(Instruction instruction) // ANDI instruction
{
    this->setRegisters(instruction.target(), this->getRegisters(instruction.source()) & instruction.immediateVal());
}

void CPU::op_sw(Instruction instruction) // SW instruction
{
    if ((this->sr & 0x10000) != 0)
    {
        std::cout << "Ignoring store while cache is isolated " << std::hex << instruction.instruction << std::endl;
        return;
    }
    u32 addr = this->getRegisters(instruction.source()) + instruction.immediateVal_se();
    if (addr % 4 == 0)
    {
        this->store32(addr, this->getRegisters(instruction.target()));
    }
    else
    {
        this->exception(ExceptionType::StoreAddressError);
    }
}

void CPU::op_lw(Instruction instruction) // SW instruction
{
    if ((this->sr & 0x10000) != 0)
    {
        std::cout << "Ignoring load while cache is isolated " << std::endl;
        return;
    }
    u32 addr = this->getRegisters(instruction.source()) + instruction.immediateVal_se();
    if (addr % 4 == 0)
    {
        this->load = RegTuple(instruction.target(), this->load32(addr));
    }
    else
    {
        this->exception(ExceptionType::LoadAddressError);
    }
}

void CPU::op_sll(Instruction instruction)
{

    this->setRegisters(instruction.subRegIndex(), this->getRegisters(instruction.target()) << instruction.shift());
}
void CPU::op_addiu(Instruction instruction)
{
    this->setRegisters(instruction.target(), this->getRegisters(instruction.source()) + instruction.immediateVal_se());
}
void CPU::op_j(Instruction instruction)
{
    this->next_pc = ((this->next_pc & 0xf0000000) | (instruction.immediateJump() << 2));
    this->branch = true;
}
void CPU::op_or(Instruction instruction)
{
    this->setRegisters(instruction.subRegIndex(), (this->getRegisters(instruction.source()) | this->getRegisters(instruction.target())));
}
void CPU::op_and(Instruction instruction)
{
    this->setRegisters(instruction.subRegIndex(), (this->getRegisters(instruction.source()) & this->getRegisters(instruction.target())));
}
void CPU::op_cop0(Instruction instruction)
{
    switch (instruction.cop_opcode())
    {
    case (0b00100):
        this->op_mtc0(instruction);
        break;
    case (0b00000):
        this->op_mfc0(instruction);
        break;
    case (0b10000):
        this->op_rfe(instruction);
        break;
    default:
        std::cout << "Unhandled cop0 instruction " << std::hex << instruction.instruction;
        exit(0);
    }
}
void CPU::op_cop1(Instruction instruction)
{
    this->exception(ExceptionType::CoprocessorError);
}
void CPU::op_cop2(Instruction instruction)
{
    std::cout << "Unhandled GTE instruction " << instruction.instruction << std::endl;
    exit(0);
}
void CPU::op_cop3(Instruction instruction)
{
    this->exception(ExceptionType::CoprocessorError);
}
void CPU::op_mfc0(Instruction instruction)
{
    switch (instruction.subRegIndex().index)
    {
    case (12):
        this->load = RegTuple(instruction.target(), this->sr);
        break;
    case (13):
        this->load = RegTuple(instruction.target(), this->cause);
        break;
    case (14):
        this->load = RegTuple(instruction.target(), this->epc);
        break;
    default:
        std::cout << "Unhandled read from COP Register " << instruction.subRegIndex().index << std::endl;
        exit(0);
    }
}
void CPU::op_mtc0(Instruction instruction)
{
    u32 value = this->getRegisters(instruction.target());
    switch (instruction.subRegIndex().index)
    {
    case (3):
    case (5):
    case (6):
    case (7):
    case (9):
    case (11):
        if (value != 0)
        {
            std::cout << "Unhandled write to COP_0" << instruction.subRegIndex().index << std::endl;
        }
        break;
    case (12):
        this->sr = value;
        break;
    case (13):
        if (value != 0)
            std::cout << "Unhandled write to CAUSE register" << std::endl;
        break;
    default:
        std::cout << "Unhandled write to COP Register " << instruction.subRegIndex().index << std::endl;
        exit(0);
    }
}
void CPU::op_bne(Instruction instruction)
{
    if (this->getRegisters(instruction.source()) != this->getRegisters(instruction.target()))
    {
        this->branch_fn(instruction.immediateVal_se());
    }
}
void CPU::op_addi(Instruction instruction)
{
    if (this->add_overflow(this->getRegisters(instruction.source()), instruction.immediateVal_se()))
    {
        // std::cout << "ADDI Exception Triggered at instruction " << std::hex << instruction.instruction << std::endl;
        // exit(0);
        this->exception(ExceptionType::Overflow);
    }
    else
    {
        this->setRegisters(instruction.target(), instruction.immediateVal_se() + this->getRegisters(instruction.source()));
    }
}
void CPU::op_sltu(Instruction instruction)
{
    bool value = (this->getRegisters(instruction.source())) < (this->getRegisters(instruction.target()));
    this->setRegisters(instruction.subRegIndex(), value ? 0x1 : 0x0);
}
void CPU::op_addu(Instruction instruction)
{
    this->setRegisters(instruction.subRegIndex(), this->getRegisters(instruction.source()) + this->getRegisters(instruction.target()));
}
void CPU::op_sh(Instruction instruction)
{
    if ((this->sr & 0x10000) != 0)
    {
        std::cout << "Ignoring store while cache is isolated " << std::endl;
    }
    u32 addr = this->getRegisters(instruction.source()) + instruction.immediateVal_se();
    if (addr % 2 == 0)
    {
        this->store16(addr, (u16)this->getRegisters(instruction.target()));
    }
    else
    {
        this->exception(ExceptionType::StoreAddressError);
    }
}
void CPU::op_jal(Instruction instruction)
{
    this->setRegisters(RegisterIndex(31), this->next_pc);
    this->op_j(instruction);
}
void CPU::op_sb(Instruction instruction)
{
    if ((this->sr & 0x10000) != 0)
    {
        std::cout << "Ignoring store while cache is isolated " << std::endl;
    }
    this->store8(this->getRegisters(instruction.source()) + instruction.immediateVal_se(), (u8)this->getRegisters(instruction.target()));
}
void CPU::op_jr(Instruction instruction)
{
    this->next_pc = this->getRegisters(instruction.source());
    this->branch = true;
}
void CPU::op_lb(Instruction instruction)
{
    this->load = RegTuple(instruction.target(), (u32)(s8)this->load8(this->getRegisters(instruction.source()) + instruction.immediateVal_se()));
}
void CPU::op_beq(Instruction instruction)
{
    if (this->getRegisters(instruction.source()) == this->getRegisters(instruction.target()))
    {
        this->branch_fn(instruction.immediateVal_se());
    }
}
void CPU::op_add(Instruction instruction)
{
    if (this->add_overflow(this->getRegisters(instruction.source()), this->getRegisters(instruction.target())))
    {
        // std::cout << "ADD Exception Triggered at instruction " << std::hex << instruction.instruction << std::endl;
        // exit(0);
        this->exception(ExceptionType::Overflow);
    }
    else
    {
        this->setRegisters(instruction.subRegIndex(), this->getRegisters(instruction.target()) + this->getRegisters(instruction.source()));
    }
}
void CPU::op_bgtz(Instruction instruction)
{
    if ((s32)this->getRegisters(instruction.source()) > 0)
    {
        this->branch_fn(instruction.immediateVal_se());
    }
}
void CPU::op_blez(Instruction instruction)
{
    if ((s32)this->getRegisters(instruction.source()) <= 0)
    {
        this->branch_fn(instruction.immediateVal_se());
    }
}
void CPU::op_lbu(Instruction instruction)
{
    this->load = RegTuple(RegisterIndex(instruction.target()), (u32)this->load8(this->getRegisters(instruction.source()) + instruction.immediateVal_se()));
}
void CPU::op_jalr(Instruction instruction)
{
    this->setRegisters(instruction.subRegIndex(), this->next_pc);
    this->next_pc = this->getRegisters(instruction.source());
    this->branch = true;
}
void CPU::op_bxx(Instruction instruction)
{
    bool test = ((s32)(this->getRegisters(instruction.source())) < 0);
    test = test ^ ((instruction.instruction >> 16) & 0x1);
    if (test != 0)
    {
        if ((((instruction.instruction >> 20) & 0x1) != 0))
        {
            this->setRegisters(RegisterIndex(31), this->next_pc);
        }
        this->branch_fn(instruction.immediateVal_se());
    }
}
void CPU::op_slti(Instruction instruction)
{
    bool value = ((s32)this->getRegisters(instruction.source())) < instruction.immediateVal_se();
    this->setRegisters(instruction.target(), value ? 0x1 : 0x0);
}
void CPU::op_subu(Instruction instruction)
{
    this->setRegisters(instruction.subRegIndex(), this->getRegisters(instruction.source()) - this->getRegisters(instruction.target()));
}
void CPU::op_sra(Instruction instruction)
{
    this->setRegisters(instruction.subRegIndex(), (u32)(((s32)this->getRegisters(instruction.target())) >> (s32)instruction.shift()));
}
void CPU::op_div(Instruction instruction)
{
    s32 num = (s32)this->getRegisters(instruction.source());
    s32 den = (s32)this->getRegisters(instruction.target());
    if (den == 0)
    {
        this->hi = (u32)num;
        if (num >= 0)
        {
            this->lo = 0xffffffff;
        }
        else
        {
            this->lo = 1;
        }
    }
    else if ((u32)num == 0x80000000 && den == -1)
    {
        this->hi = 0;
        this->lo = 0x80000000;
    }
    else
    {
        this->hi = (u32)(num % den);
        this->lo = (u32)(num / den);
    }
}
void CPU::op_mflo(Instruction instruction)
{
    this->setRegisters(instruction.subRegIndex(), this->lo);
}
void CPU::op_srl(Instruction instruction)
{
    this->setRegisters(instruction.subRegIndex(), this->getRegisters(instruction.target()) >> (s32)instruction.shift());
}
void CPU::op_sltiu(Instruction instruction)
{
    bool value = (this->getRegisters(instruction.source()) < instruction.immediateVal_se());
    this->setRegisters(instruction.target(), value ? 0x1 : 0x0);
}
void CPU::op_divu(Instruction instruction)
{
    u32 num = this->getRegisters(instruction.source());
    u32 den = this->getRegisters(instruction.target());
    if (den == 0)
    {
        this->hi = num;
        this->lo = 0xffffffff;
    }
    else
    {
        this->hi = num % den;
        this->lo = num / den;
    }
}
void CPU::op_mfhi(Instruction instruction)
{
    this->setRegisters(instruction.subRegIndex(), this->hi);
}
void CPU::op_mtlo(Instruction Instruction)
{
    this->lo = this->getRegisters(Instruction.source());
}
void CPU::op_mthi(Instruction Instruction)
{
    this->hi = this->getRegisters(Instruction.source());
}
void CPU::op_slt(Instruction instruction)
{
    bool value = ((s32)this->getRegisters(instruction.source()) < (s32)this->getRegisters(instruction.target()));
    this->setRegisters(instruction.subRegIndex(), value ? 0x1 : 0x0);
}
void CPU::op_rfe(Instruction Instruction)
{
    if (Instruction.instruction & 0x3f != 0b010000)
    {
        std::cout << "Invalid COP0 instruction " << Instruction.instruction << std::endl;
        exit(0);
    }
    u32 mode = this->sr & 0x3f;
    this->sr &= !0x3f;
    this->sr |= mode >> 2;
}
void CPU::op_lhu(Instruction instruction)
{
    u32 addr = this->getRegisters(instruction.source()) + instruction.immediateVal_se();
    if (addr % 2 == 0)
    {
        this->load = RegTuple(instruction.target(), (u32)this->load16(addr));
    }
    else
    {
        this->exception(ExceptionType::LoadAddressError);
    }
}
void CPU::op_sllv(Instruction instruction)
{
    this->setRegisters(instruction.subRegIndex(), this->getRegisters(instruction.target()) << (this->getRegisters(instruction.source()) & 0x1f));
}
void CPU::op_lh(Instruction instruction)
{
    this->load = RegTuple(instruction.target(), (u32)(s16)this->load16(this->getRegisters(instruction.source()) + instruction.immediateVal_se()));
}
void CPU::op_nor(Instruction instruction)
{
    this->setRegisters(instruction.subRegIndex(), !(this->getRegisters(instruction.source()) | this->getRegisters(instruction.target())));
}
void CPU::op_srav(Instruction instruction)
{
    this->setRegisters(instruction.subRegIndex(), (u32)(((s16)this->getRegisters(instruction.target())) >> (this->getRegisters(instruction.source()) & 0x1f)));
}
void CPU::op_srlv(Instruction instruction)
{
    this->setRegisters(instruction.subRegIndex(), this->getRegisters(instruction.target()) >> (this->getRegisters(instruction.source()) & 0x1f));
}
void CPU::op_multu(Instruction instruction)
{
    u64 v = (u64)this->getRegisters(instruction.source()) * (u64)this->getRegisters(instruction.target());
    this->hi = (u32)(v >> 32);
    this->lo = (u32)v;
}
void CPU::op_xor(Instruction instruction)
{
    this->setRegisters(instruction.subRegIndex(), this->getRegisters(instruction.source()) ^ this->getRegisters(instruction.target()));
}
void CPU::op_break(Instruction instruction)
{
    this->exception(ExceptionType::Break);
}
void CPU::op_mult(Instruction instruction)
{
    u64 v = (s64)(s16)this->getRegisters(instruction.source()) * (s64)(s16)this->getRegisters(instruction.target());
    this->hi = (u32)(v >> 32);
    this->lo = (u32)v;
}
void CPU::op_sub(Instruction instruction)
{
    if (this->sub_overflow(this->getRegisters(instruction.source()), this->getRegisters(instruction.target())))
    {
        // std::cout << "ADD Exception Triggered at instruction " << std::hex << instruction.instruction << std::endl;
        // exit(0);
        this->exception(ExceptionType::Overflow);
    }
    else
    {
        this->setRegisters(instruction.subRegIndex(), (u32)(this->getRegisters(instruction.source()) - this->getRegisters(instruction.target())));
    }
}
void CPU::op_xori(Instruction instruction)
{
    this->setRegisters(this->getRegisters(instruction.target()), this->getRegisters(instruction.source()) ^ instruction.immediateVal());
}
void CPU::op_lwl(Instruction instruction)
{
    u32 addr = this->getRegisters(instruction.source()) + instruction.immediateVal_se();
    // u32 cur_v = this->load_del_regs[instruction.target()];
    // u32 aligned_addr = addr & !3;
    u32 aligned_word = this->load32(addr & !3);
    switch (addr & 3)
    {
    case 0:
        this->load = RegTuple(instruction.target(), (this->load_del_regs[instruction.target().index] & 0x00ffffff) | (aligned_word << 24));
        break;
    case 1:
        this->load = RegTuple(instruction.target(), (this->load_del_regs[instruction.target().index] & 0x0000ffff) | (aligned_word << 16));
        break;
    case 2:
        this->load = RegTuple(instruction.target(), (this->load_del_regs[instruction.target().index] & 0x000000ff) | (aligned_word << 8));
        break;
    case 3:
        this->load = RegTuple(instruction.target(), (this->load_del_regs[instruction.target().index] & 0x00000000) | (aligned_word << 0));
        break;
    default:
        std::cout << "Unreachable case in LWL " << instruction.instruction << std::endl;
    }
}
void CPU::op_lwr(Instruction instruction)
{
    u32 addr = this->getRegisters(instruction.source()) + instruction.immediateVal_se();
    // u32 cur_v = this->load_del_regs[instruction.target()];
    // u32 aligned_addr = addr & !3;
    u32 aligned_word = this->load32(addr & !3);
    switch (addr & 3)
    {
    case 0:
        this->load = RegTuple(instruction.target(), (this->load_del_regs[instruction.target().index] & 0x00000000) | (aligned_word << 0));
        break;
    case 1:
        this->load = RegTuple(instruction.target(), (this->load_del_regs[instruction.target().index] & 0xff000000) | (aligned_word << 8));
        break;
    case 2:
        this->load = RegTuple(instruction.target(), (this->load_del_regs[instruction.target().index] & 0xffff0000) | (aligned_word << 16));
        break;
    case 3:
        this->load = RegTuple(instruction.target(), (this->load_del_regs[instruction.target().index] & 0xffffff00) | (aligned_word << 24));
        break;
    default:
        std::cout << "Unreachable case in LWR " << instruction.instruction << std::endl;
    }
}
void CPU::op_swl(Instruction instruction)
{
    u32 addr = this->getRegisters(instruction.source()) + instruction.immediateVal_se();
    switch (addr & 3)
    {
    case 0:
        this->store32(addr, (this->load32(addr & !3) & 0xffffff00) | (this->getRegisters(instruction.target()) >> 24));
        break;
    case 1:
        this->store32(addr, (this->load32(addr & !3) & 0xffff0000) | (this->getRegisters(instruction.target()) >> 16));
        break;
    case 2:
        this->store32(addr, (this->load32(addr & !3) & 0xff000000) | (this->getRegisters(instruction.target()) >> 8));
        break;
    case 3:
        this->store32(addr, (this->load32(addr & !3) & 0x00000000) | (this->getRegisters(instruction.target()) >> 0));
        break;
    default:
        std::cout << "Unreachable case in SWL " << instruction.instruction << std::endl;
    }
}
void CPU::op_swr(Instruction instruction)
{
    u32 addr = this->getRegisters(instruction.source()) + instruction.immediateVal_se();
    switch (addr & 3)
    {
    case 0:
        this->store32(addr, (this->load32(addr & !3) & 0x00000000) | (this->getRegisters(instruction.target()) >> 0));
        break;
    case 1:
        this->store32(addr, (this->load32(addr & !3) & 0xff000000) | (this->getRegisters(instruction.target()) >> 8));
        break;
    case 2:
        this->store32(addr, (this->load32(addr & !3) & 0xffff0000) | (this->getRegisters(instruction.target()) >> 16));
        break;
    case 3:
        this->store32(addr, (this->load32(addr & !3) & 0xffffff00) | (this->getRegisters(instruction.target()) >> 24));
        break;
    default:
        std::cout << "Unreachable case in SWL " << instruction.instruction << std::endl;
    }
}
void CPU::op_lwc0(Instruction instruction)
{
    this->exception(ExceptionType::CoprocessorError);
}
void CPU::op_lwc1(Instruction instruction)
{
    this->exception(ExceptionType::CoprocessorError);
}
void CPU::op_lwc2(Instruction instruction)
{
    std::cout << "Unhandled GTE LWC " << instruction.instruction << std::endl;
    exit(0);
}
void CPU::op_lwc3(Instruction instruction)
{
    this->exception(ExceptionType::CoprocessorError);
}
void CPU::op_swc0(Instruction instruction)
{
    this->exception(ExceptionType::CoprocessorError);
}
void CPU::op_swc1(Instruction instruction)
{
    this->exception(ExceptionType::CoprocessorError);
}
void CPU::op_swc2(Instruction instruction)
{
    std::cout << "Unhandled GTE SWC " << instruction.instruction << std::endl;
    exit(0);
}
void CPU::op_swc3(Instruction instruction)
{
    this->exception(ExceptionType::CoprocessorError);
}
void CPU::op_illegal(Instruction instruction)
{
    std::cout << "Illegal instruction " << instruction.instruction << std::endl;
    this->exception(ExceptionType::IllegalInstruction);
}
//------------------------------------------------------------------------------------
