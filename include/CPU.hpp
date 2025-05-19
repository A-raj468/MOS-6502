// CPU.hpp
#pragma once

#include <Types.hpp>

#include <array>
#include <unordered_map>

namespace mos6502 {
class CPU {
  public:
    // A: Accumulator
    BYTE a;

    // P: Flag register
    BYTE n : 1;
    BYTE v : 1;
    BYTE u : 1;
    BYTE b : 1;
    BYTE d : 1;
    BYTE i : 1;
    BYTE z : 1;
    BYTE c : 1;

    // PC: Program counter
    WORD pc;

    // SP: Stack pointer
    BYTE sp;

    // X, Y: Index registers
    BYTE x;
    BYTE y;

  private:
    // Ref to memory
    mem_t &memory;

    // Opcode lookup table
    std::array<Instruction_info, 0x100> lookup_table;

    // Instruction Dispatch table
    std::unordered_map<INSTRUCTION, instruction_t> dispatch_table;

    void ADC(ADDRESSING_MODE mode, WORD operand);
    void AND(ADDRESSING_MODE mode, WORD operand);
    void ASL(ADDRESSING_MODE mode, WORD operand);
    void BCC(ADDRESSING_MODE mode, WORD operand);
    void BCS(ADDRESSING_MODE mode, WORD operand);
    void BEQ(ADDRESSING_MODE mode, WORD operand);
    void BIT(ADDRESSING_MODE mode, WORD operand);
    void BMI(ADDRESSING_MODE mode, WORD operand);
    void BNE(ADDRESSING_MODE mode, WORD operand);
    void BPL(ADDRESSING_MODE mode, WORD operand);
    void BRK(ADDRESSING_MODE mode, WORD operand);
    void BVC(ADDRESSING_MODE mode, WORD operand);
    void BVS(ADDRESSING_MODE mode, WORD operand);
    void CLC(ADDRESSING_MODE mode, WORD operand);
    void CLD(ADDRESSING_MODE mode, WORD operand);
    void CLI(ADDRESSING_MODE mode, WORD operand);
    void CLV(ADDRESSING_MODE mode, WORD operand);
    void CMP(ADDRESSING_MODE mode, WORD operand);
    void CPX(ADDRESSING_MODE mode, WORD operand);
    void CPY(ADDRESSING_MODE mode, WORD operand);
    void DEC(ADDRESSING_MODE mode, WORD operand);
    void DEX(ADDRESSING_MODE mode, WORD operand);
    void DEY(ADDRESSING_MODE mode, WORD operand);
    void EOR(ADDRESSING_MODE mode, WORD operand);
    void INC(ADDRESSING_MODE mode, WORD operand);
    void INX(ADDRESSING_MODE mode, WORD operand);
    void INY(ADDRESSING_MODE mode, WORD operand);
    void JMP(ADDRESSING_MODE mode, WORD operand);
    void JSR(ADDRESSING_MODE mode, WORD operand);
    void LDA(ADDRESSING_MODE mode, WORD operand);
    void LDX(ADDRESSING_MODE mode, WORD operand);
    void LDY(ADDRESSING_MODE mode, WORD operand);
    void LSR(ADDRESSING_MODE mode, WORD operand);
    void NOP(ADDRESSING_MODE mode, WORD operand);
    void ORA(ADDRESSING_MODE mode, WORD operand);
    void PHA(ADDRESSING_MODE mode, WORD operand);
    void PHP(ADDRESSING_MODE mode, WORD operand);
    void PLA(ADDRESSING_MODE mode, WORD operand);
    void PLP(ADDRESSING_MODE mode, WORD operand);
    void ROL(ADDRESSING_MODE mode, WORD operand);
    void ROR(ADDRESSING_MODE mode, WORD operand);
    void RTI(ADDRESSING_MODE mode, WORD operand);
    void RTS(ADDRESSING_MODE mode, WORD operand);
    void SBC(ADDRESSING_MODE mode, WORD operand);
    void SEC(ADDRESSING_MODE mode, WORD operand);
    void SED(ADDRESSING_MODE mode, WORD operand);
    void SEI(ADDRESSING_MODE mode, WORD operand);
    void STA(ADDRESSING_MODE mode, WORD operand);
    void STX(ADDRESSING_MODE mode, WORD operand);
    void STY(ADDRESSING_MODE mode, WORD operand);
    void TAX(ADDRESSING_MODE mode, WORD operand);
    void TAY(ADDRESSING_MODE mode, WORD operand);
    void TSX(ADDRESSING_MODE mode, WORD operand);
    void TXA(ADDRESSING_MODE mode, WORD operand);
    void TXS(ADDRESSING_MODE mode, WORD operand);
    void TYA(ADDRESSING_MODE mode, WORD operand);
    void INVALID(ADDRESSING_MODE mode, WORD operand);

    void init_lookup_table();
    void init_dispatch_table();

  public:
    CPU(mem_t &memory);

    void reset();

    BYTE get_p();

    BYTE fetch_opcode();
    Instruction_info decode(BYTE opcode);
    WORD fetch_operands(ADDRESSING_MODE mode);

    void execute(BYTE opcode);
};
} // namespace mos6502
