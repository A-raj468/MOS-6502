// Types.hpp
#pragma once

#include <array>
#include <cstdint>

namespace mos6502 {
using mem_t = std::array<uint8_t, 0x10000>;

using BYTE = uint8_t;
using WORD = uint16_t;

enum class ADDRESSING_MODE {
    IMPLICIT,
    ACCUMULATOR,
    IMMEDIATE,
    ZEROPAGE,
    ZEROPAGE_X,
    ZEROPAGE_Y,
    RELATIVE,
    ABSOLUTE,
    ABSOLUTE_X,
    ABSOLUTE_Y,
    INDIRECT,
    INDIRECT_X,
    INDIRECT_Y,
    INVALID,
};

enum class INSTRUCTION {
    ADC,
    AND,
    ASL,
    BCC,
    BCS,
    BEQ,
    BIT,
    BMI,
    BNE,
    BPL,
    BRK,
    BVC,
    BVS,
    CLC,
    CLD,
    CLI,
    CLV,
    CMP,
    CPX,
    CPY,
    DEC,
    DEX,
    DEY,
    EOR,
    INC,
    INX,
    INY,
    JMP,
    JSR,
    LDA,
    LDX,
    LDY,
    LSR,
    NOP,
    ORA,
    PHA,
    PHP,
    PLA,
    PLP,
    ROL,
    ROR,
    RTI,
    RTS,
    SBC,
    SEC,
    SED,
    SEI,
    STA,
    STX,
    STY,
    TAX,
    TAY,
    TSX,
    TXA,
    TXS,
    TYA,
    INVALID,
};

struct Instruction_info {
    INSTRUCTION ins;
    ADDRESSING_MODE mode;
};
} // namespace mos6502
