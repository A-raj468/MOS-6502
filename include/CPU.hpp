// CPU.hpp
#pragma once

#include <array>
#include <cstdint>

namespace mos6502 {

using mem_t = std::array<uint8_t, 0x10000>;

using BYTE = uint8_t;
using WORD = uint16_t;

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

  public:
    CPU(mem_t &memory);

    void reset();

    BYTE fetch_opcode();

    void decode(BYTE opcode);

    void fetch_operands();

    void execute();
};
} // namespace mos6502
