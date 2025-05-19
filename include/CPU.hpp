// CPU.hpp
#pragma once

#include <Types.hpp>

#include <array>

namespace mos6502 {
class CPU {
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

    // Ref to memory
    mem_t &memory;

    // Opcode lookup table
    std::array<Instruction_info, 0x100> table;

    void LDA(ADDRESSING_MODE mode, WORD operand);

  public:
    CPU(mem_t &memory);

    void reset();
    void init_opcode_table();

    BYTE fetch_opcode();
    Instruction_info decode(BYTE opcode);
    WORD fetch_operands(ADDRESSING_MODE mode);

    void execute(BYTE opcode);
};
} // namespace mos6502
