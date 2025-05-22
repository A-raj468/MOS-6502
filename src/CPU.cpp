#include <CPU.hpp>
#include <Types.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace mos6502;

CPU::CPU(mem_t &memory) : memory(memory) {
    init_lookup_table();
    init_dispatch_table();
}

void CPU::reset() {
    pc = memory[0xfffc] | (memory[0xfffd] << 8);
    n = v = b = d = z = c = 0;
    u = i = 1;
    sp = 0xfd;
    a = 0;
    x = y = 0;
}

void CPU::init_lookup_table() {
    lookup_table = {
        Instruction_info{INSTRUCTION::BRK, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::ORA, ADDRESSING_MODE::INDIRECT_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::ORA, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::ASL, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::PHP, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::ORA, ADDRESSING_MODE::IMMEDIATE},
        Instruction_info{INSTRUCTION::ASL, ADDRESSING_MODE::ACCUMULATOR},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::ORA, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::ASL, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::BPL, ADDRESSING_MODE::RELATIVE},
        Instruction_info{INSTRUCTION::ORA, ADDRESSING_MODE::INDIRECT_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::ORA, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::ASL, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::CLC, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::ORA, ADDRESSING_MODE::ABSOLUTE_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::ORA, ADDRESSING_MODE::ABSOLUTE_X},
        Instruction_info{INSTRUCTION::ASL, ADDRESSING_MODE::ABSOLUTE_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::JSR, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::AND, ADDRESSING_MODE::INDIRECT_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::BIT, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::AND, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::ROL, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::PLP, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::AND, ADDRESSING_MODE::IMMEDIATE},
        Instruction_info{INSTRUCTION::ROL, ADDRESSING_MODE::ACCUMULATOR},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::BIT, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::AND, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::ROL, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::BMI, ADDRESSING_MODE::RELATIVE},
        Instruction_info{INSTRUCTION::AND, ADDRESSING_MODE::INDIRECT_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::AND, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::ROL, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::SEC, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::AND, ADDRESSING_MODE::ABSOLUTE_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::AND, ADDRESSING_MODE::ABSOLUTE_X},
        Instruction_info{INSTRUCTION::ROL, ADDRESSING_MODE::ABSOLUTE_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::RTI, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::EOR, ADDRESSING_MODE::INDIRECT_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::EOR, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::LSR, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::PHA, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::EOR, ADDRESSING_MODE::IMMEDIATE},
        Instruction_info{INSTRUCTION::LSR, ADDRESSING_MODE::ACCUMULATOR},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::JMP, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::EOR, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::LSR, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::BVC, ADDRESSING_MODE::RELATIVE},
        Instruction_info{INSTRUCTION::EOR, ADDRESSING_MODE::INDIRECT_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::EOR, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::LSR, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::CLI, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::EOR, ADDRESSING_MODE::ABSOLUTE_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::EOR, ADDRESSING_MODE::ABSOLUTE_X},
        Instruction_info{INSTRUCTION::LSR, ADDRESSING_MODE::ABSOLUTE_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::RTS, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::ADC, ADDRESSING_MODE::INDIRECT_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::ADC, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::ROR, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::PLA, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::ADC, ADDRESSING_MODE::IMMEDIATE},
        Instruction_info{INSTRUCTION::ROR, ADDRESSING_MODE::ACCUMULATOR},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::JMP, ADDRESSING_MODE::INDIRECT},
        Instruction_info{INSTRUCTION::ADC, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::ROR, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::BVS, ADDRESSING_MODE::RELATIVE},
        Instruction_info{INSTRUCTION::ADC, ADDRESSING_MODE::INDIRECT_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::ADC, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::ROR, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::SEI, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::ADC, ADDRESSING_MODE::ABSOLUTE_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::ADC, ADDRESSING_MODE::ABSOLUTE_X},
        Instruction_info{INSTRUCTION::ROR, ADDRESSING_MODE::ABSOLUTE_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::STA, ADDRESSING_MODE::INDIRECT_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::STY, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::STA, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::STX, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::DEY, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::TXA, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::STY, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::STA, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::STX, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::BCC, ADDRESSING_MODE::RELATIVE},
        Instruction_info{INSTRUCTION::STA, ADDRESSING_MODE::INDIRECT_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::STY, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::STA, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::STX, ADDRESSING_MODE::ZEROPAGE_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::TYA, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::STA, ADDRESSING_MODE::ABSOLUTE_Y},
        Instruction_info{INSTRUCTION::TXS, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::STA, ADDRESSING_MODE::ABSOLUTE_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::LDY, ADDRESSING_MODE::IMMEDIATE},
        Instruction_info{INSTRUCTION::LDA, ADDRESSING_MODE::INDIRECT_X},
        Instruction_info{INSTRUCTION::LDX, ADDRESSING_MODE::IMMEDIATE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::LDY, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::LDA, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::LDX, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::TAY, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::LDA, ADDRESSING_MODE::IMMEDIATE},
        Instruction_info{INSTRUCTION::TAX, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::LDY, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::LDA, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::LDX, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::BCS, ADDRESSING_MODE::RELATIVE},
        Instruction_info{INSTRUCTION::LDA, ADDRESSING_MODE::INDIRECT_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::LDY, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::LDA, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::LDX, ADDRESSING_MODE::ZEROPAGE_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::CLV, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::LDA, ADDRESSING_MODE::ABSOLUTE_Y},
        Instruction_info{INSTRUCTION::TSX, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::LDY, ADDRESSING_MODE::ABSOLUTE_X},
        Instruction_info{INSTRUCTION::LDA, ADDRESSING_MODE::ABSOLUTE_X},
        Instruction_info{INSTRUCTION::LDX, ADDRESSING_MODE::ABSOLUTE_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::CPY, ADDRESSING_MODE::IMMEDIATE},
        Instruction_info{INSTRUCTION::CMP, ADDRESSING_MODE::INDIRECT_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::CPY, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::CMP, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::DEC, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INY, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::CMP, ADDRESSING_MODE::IMMEDIATE},
        Instruction_info{INSTRUCTION::DEX, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::CPY, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::CMP, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::DEC, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::BNE, ADDRESSING_MODE::RELATIVE},
        Instruction_info{INSTRUCTION::CMP, ADDRESSING_MODE::INDIRECT_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::CMP, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::DEC, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::CLD, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::CMP, ADDRESSING_MODE::ABSOLUTE_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::CMP, ADDRESSING_MODE::ABSOLUTE_X},
        Instruction_info{INSTRUCTION::DEC, ADDRESSING_MODE::ABSOLUTE_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::CPX, ADDRESSING_MODE::IMMEDIATE},
        Instruction_info{INSTRUCTION::SBC, ADDRESSING_MODE::INDIRECT_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::CPX, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::SBC, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::INC, ADDRESSING_MODE::ZEROPAGE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INX, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::SBC, ADDRESSING_MODE::IMMEDIATE},
        Instruction_info{INSTRUCTION::NOP, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::CPX, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::SBC, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::INC, ADDRESSING_MODE::ABSOLUTE},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::BEQ, ADDRESSING_MODE::RELATIVE},
        Instruction_info{INSTRUCTION::SBC, ADDRESSING_MODE::INDIRECT_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::SBC, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::INC, ADDRESSING_MODE::ZEROPAGE_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::SED, ADDRESSING_MODE::IMPLICIT},
        Instruction_info{INSTRUCTION::SBC, ADDRESSING_MODE::ABSOLUTE_Y},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
        Instruction_info{INSTRUCTION::SBC, ADDRESSING_MODE::ABSOLUTE_X},
        Instruction_info{INSTRUCTION::INC, ADDRESSING_MODE::ABSOLUTE_X},
        Instruction_info{INSTRUCTION::INVALID, ADDRESSING_MODE::INVALID},
    };
}

void CPU::init_dispatch_table() {
    dispatch_table[INSTRUCTION::ADC] = [this](ADDRESSING_MODE m, WORD o) {
        ADC(m, o);
    };
    dispatch_table[INSTRUCTION::AND] = [this](ADDRESSING_MODE m, WORD o) {
        AND(m, o);
    };
    dispatch_table[INSTRUCTION::ASL] = [this](ADDRESSING_MODE m, WORD o) {
        ASL(m, o);
    };
    dispatch_table[INSTRUCTION::BCC] = [this](ADDRESSING_MODE m, WORD o) {
        BCC(m, o);
    };
    dispatch_table[INSTRUCTION::BCS] = [this](ADDRESSING_MODE m, WORD o) {
        BCS(m, o);
    };
    dispatch_table[INSTRUCTION::BEQ] = [this](ADDRESSING_MODE m, WORD o) {
        BEQ(m, o);
    };
    dispatch_table[INSTRUCTION::BIT] = [this](ADDRESSING_MODE m, WORD o) {
        BIT(m, o);
    };
    dispatch_table[INSTRUCTION::BMI] = [this](ADDRESSING_MODE m, WORD o) {
        BMI(m, o);
    };
    dispatch_table[INSTRUCTION::BNE] = [this](ADDRESSING_MODE m, WORD o) {
        BNE(m, o);
    };
    dispatch_table[INSTRUCTION::BPL] = [this](ADDRESSING_MODE m, WORD o) {
        BPL(m, o);
    };
    dispatch_table[INSTRUCTION::BRK] = [this](ADDRESSING_MODE m, WORD o) {
        BRK(m, o);
    };
    dispatch_table[INSTRUCTION::BVC] = [this](ADDRESSING_MODE m, WORD o) {
        BVC(m, o);
    };
    dispatch_table[INSTRUCTION::BVS] = [this](ADDRESSING_MODE m, WORD o) {
        BVS(m, o);
    };
    dispatch_table[INSTRUCTION::CLC] = [this](ADDRESSING_MODE m, WORD o) {
        CLC(m, o);
    };
    dispatch_table[INSTRUCTION::CLD] = [this](ADDRESSING_MODE m, WORD o) {
        CLD(m, o);
    };
    dispatch_table[INSTRUCTION::CLI] = [this](ADDRESSING_MODE m, WORD o) {
        CLI(m, o);
    };
    dispatch_table[INSTRUCTION::CLV] = [this](ADDRESSING_MODE m, WORD o) {
        CLV(m, o);
    };
    dispatch_table[INSTRUCTION::CMP] = [this](ADDRESSING_MODE m, WORD o) {
        CMP(m, o);
    };
    dispatch_table[INSTRUCTION::CPX] = [this](ADDRESSING_MODE m, WORD o) {
        CPX(m, o);
    };
    dispatch_table[INSTRUCTION::CPY] = [this](ADDRESSING_MODE m, WORD o) {
        CPY(m, o);
    };
    dispatch_table[INSTRUCTION::DEC] = [this](ADDRESSING_MODE m, WORD o) {
        DEC(m, o);
    };
    dispatch_table[INSTRUCTION::DEX] = [this](ADDRESSING_MODE m, WORD o) {
        DEX(m, o);
    };
    dispatch_table[INSTRUCTION::DEY] = [this](ADDRESSING_MODE m, WORD o) {
        DEY(m, o);
    };
    dispatch_table[INSTRUCTION::EOR] = [this](ADDRESSING_MODE m, WORD o) {
        EOR(m, o);
    };
    dispatch_table[INSTRUCTION::INC] = [this](ADDRESSING_MODE m, WORD o) {
        INC(m, o);
    };
    dispatch_table[INSTRUCTION::INX] = [this](ADDRESSING_MODE m, WORD o) {
        INX(m, o);
    };
    dispatch_table[INSTRUCTION::INY] = [this](ADDRESSING_MODE m, WORD o) {
        INY(m, o);
    };
    dispatch_table[INSTRUCTION::JMP] = [this](ADDRESSING_MODE m, WORD o) {
        JMP(m, o);
    };
    dispatch_table[INSTRUCTION::JSR] = [this](ADDRESSING_MODE m, WORD o) {
        JSR(m, o);
    };
    dispatch_table[INSTRUCTION::LDA] = [this](ADDRESSING_MODE m, WORD o) {
        LDA(m, o);
    };
    dispatch_table[INSTRUCTION::LDX] = [this](ADDRESSING_MODE m, WORD o) {
        LDX(m, o);
    };
    dispatch_table[INSTRUCTION::LDY] = [this](ADDRESSING_MODE m, WORD o) {
        LDY(m, o);
    };
    dispatch_table[INSTRUCTION::LSR] = [this](ADDRESSING_MODE m, WORD o) {
        LSR(m, o);
    };
    dispatch_table[INSTRUCTION::NOP] = [this](ADDRESSING_MODE m, WORD o) {
        NOP(m, o);
    };
    dispatch_table[INSTRUCTION::ORA] = [this](ADDRESSING_MODE m, WORD o) {
        ORA(m, o);
    };
    dispatch_table[INSTRUCTION::PHA] = [this](ADDRESSING_MODE m, WORD o) {
        PHA(m, o);
    };
    dispatch_table[INSTRUCTION::PHP] = [this](ADDRESSING_MODE m, WORD o) {
        PHP(m, o);
    };
    dispatch_table[INSTRUCTION::PLA] = [this](ADDRESSING_MODE m, WORD o) {
        PLA(m, o);
    };
    dispatch_table[INSTRUCTION::PLP] = [this](ADDRESSING_MODE m, WORD o) {
        PLP(m, o);
    };
    dispatch_table[INSTRUCTION::ROL] = [this](ADDRESSING_MODE m, WORD o) {
        ROL(m, o);
    };
    dispatch_table[INSTRUCTION::ROR] = [this](ADDRESSING_MODE m, WORD o) {
        ROR(m, o);
    };
    dispatch_table[INSTRUCTION::RTI] = [this](ADDRESSING_MODE m, WORD o) {
        RTI(m, o);
    };
    dispatch_table[INSTRUCTION::RTS] = [this](ADDRESSING_MODE m, WORD o) {
        RTS(m, o);
    };
    dispatch_table[INSTRUCTION::SBC] = [this](ADDRESSING_MODE m, WORD o) {
        SBC(m, o);
    };
    dispatch_table[INSTRUCTION::SEC] = [this](ADDRESSING_MODE m, WORD o) {
        SEC(m, o);
    };
    dispatch_table[INSTRUCTION::SED] = [this](ADDRESSING_MODE m, WORD o) {
        SED(m, o);
    };
    dispatch_table[INSTRUCTION::SEI] = [this](ADDRESSING_MODE m, WORD o) {
        SEI(m, o);
    };
    dispatch_table[INSTRUCTION::STA] = [this](ADDRESSING_MODE m, WORD o) {
        STA(m, o);
    };
    dispatch_table[INSTRUCTION::STX] = [this](ADDRESSING_MODE m, WORD o) {
        STX(m, o);
    };
    dispatch_table[INSTRUCTION::STY] = [this](ADDRESSING_MODE m, WORD o) {
        STY(m, o);
    };
    dispatch_table[INSTRUCTION::TAX] = [this](ADDRESSING_MODE m, WORD o) {
        TAX(m, o);
    };
    dispatch_table[INSTRUCTION::TAY] = [this](ADDRESSING_MODE m, WORD o) {
        TAY(m, o);
    };
    dispatch_table[INSTRUCTION::TSX] = [this](ADDRESSING_MODE m, WORD o) {
        TSX(m, o);
    };
    dispatch_table[INSTRUCTION::TXA] = [this](ADDRESSING_MODE m, WORD o) {
        TXA(m, o);
    };
    dispatch_table[INSTRUCTION::TXS] = [this](ADDRESSING_MODE m, WORD o) {
        TXS(m, o);
    };
    dispatch_table[INSTRUCTION::TYA] = [this](ADDRESSING_MODE m, WORD o) {
        TYA(m, o);
    };
    dispatch_table[INSTRUCTION::INVALID] = [this](ADDRESSING_MODE m, WORD o) {
        INVALID(m, o);
    };
}

BYTE CPU::get_p() {
    BYTE p = (n << 7) | (v << 6) | (u << 5) | (b << 4) | (d << 3) | (i << 2) |
             (z << 1) | c;
    return p;
}

BYTE CPU::fetch_opcode() {
    BYTE opcode = memory[pc];
    pc++;
    return opcode;
}

Instruction_info CPU::decode(BYTE opcode) { return lookup_table[opcode]; }

WORD CPU::fetch_operands(ADDRESSING_MODE mode) {
    WORD operand = 0x0000;
    switch (mode) {
    case ADDRESSING_MODE::IMPLICIT: {
    } break;
    case ADDRESSING_MODE::ACCUMULATOR: {
    } break;
    case ADDRESSING_MODE::IMMEDIATE: {
        operand = memory[pc++];
    } break;
    case ADDRESSING_MODE::ZEROPAGE: {
        operand = memory[pc++];
    } break;
    case ADDRESSING_MODE::ZEROPAGE_X: {
        operand = memory[pc++];
    } break;
    case ADDRESSING_MODE::ZEROPAGE_Y: {
        operand = memory[pc++];
    } break;
    case ADDRESSING_MODE::RELATIVE: {
        operand = memory[pc++];
    } break;
    case ADDRESSING_MODE::ABSOLUTE: {
        BYTE lo = memory[pc++];
        BYTE hi = memory[pc++];
        operand = (hi << 8) | lo;
    } break;
    case ADDRESSING_MODE::ABSOLUTE_X: {
        BYTE lo = memory[pc++];
        BYTE hi = memory[pc++];
        operand = (hi << 8) | lo;
    } break;
    case ADDRESSING_MODE::ABSOLUTE_Y: {
        BYTE lo = memory[pc++];
        BYTE hi = memory[pc++];
        operand = (hi << 8) | lo;
    } break;
    case ADDRESSING_MODE::INDIRECT: {
        BYTE lo = memory[pc++];
        BYTE hi = memory[pc++];
        operand = (hi << 8) | lo;
    } break;
    case ADDRESSING_MODE::INDIRECT_X: {
        operand = memory[pc++];
    } break;
    case ADDRESSING_MODE::INDIRECT_Y: {
        operand = memory[pc++];
    } break;
    case ADDRESSING_MODE::INVALID: {
    } break;
    default:
        break;
    }
    return operand;
}

static std::string print_info(INSTRUCTION ins, ADDRESSING_MODE mode) {
    std::string i = "INVALID";
    switch (ins) {
    case INSTRUCTION::ADC: {
        i = "ADC";
    } break;
    case INSTRUCTION::AND: {
        i = "AND";
    } break;
    case INSTRUCTION::ASL: {
        i = "ASL";
    } break;
    case INSTRUCTION::BCC: {
        i = "BCC";
    } break;
    case INSTRUCTION::BCS: {
        i = "BCS";
    } break;
    case INSTRUCTION::BEQ: {
        i = "BEQ";
    } break;
    case INSTRUCTION::BIT: {
        i = "BIT";
    } break;
    case INSTRUCTION::BMI: {
        i = "BMI";
    } break;
    case INSTRUCTION::BNE: {
        i = "BNE";
    } break;
    case INSTRUCTION::BPL: {
        i = "BPL";
    } break;
    case INSTRUCTION::BRK: {
        i = "BRK";
    } break;
    case INSTRUCTION::BVC: {
        i = "BVC";
    } break;
    case INSTRUCTION::BVS: {
        i = "BVS";
    } break;
    case INSTRUCTION::CLC: {
        i = "CLC";
    } break;
    case INSTRUCTION::CLD: {
        i = "CLD";
    } break;
    case INSTRUCTION::CLI: {
        i = "CLI";
    } break;
    case INSTRUCTION::CLV: {
        i = "CLV";
    } break;
    case INSTRUCTION::CMP: {
        i = "CMP";
    } break;
    case INSTRUCTION::CPX: {
        i = "CPX";
    } break;
    case INSTRUCTION::CPY: {
        i = "CPY";
    } break;
    case INSTRUCTION::DEC: {
        i = "DEC";
    } break;
    case INSTRUCTION::DEX: {
        i = "DEX";
    } break;
    case INSTRUCTION::DEY: {
        i = "DEY";
    } break;
    case INSTRUCTION::EOR: {
        i = "EOR";
    } break;
    case INSTRUCTION::INC: {
        i = "INC";
    } break;
    case INSTRUCTION::INX: {
        i = "INX";
    } break;
    case INSTRUCTION::INY: {
        i = "INY";
    } break;
    case INSTRUCTION::JMP: {
        i = "JMP";
    } break;
    case INSTRUCTION::JSR: {
        i = "JSR";
    } break;
    case INSTRUCTION::LDA: {
        i = "LDA";
    } break;
    case INSTRUCTION::LDX: {
        i = "LDX";
    } break;
    case INSTRUCTION::LDY: {
        i = "LDY";
    } break;
    case INSTRUCTION::LSR: {
        i = "LSR";
    } break;
    case INSTRUCTION::NOP: {
        i = "NOP";
    } break;
    case INSTRUCTION::ORA: {
        i = "ORA";
    } break;
    case INSTRUCTION::PHA: {
        i = "PHA";
    } break;
    case INSTRUCTION::PHP: {
        i = "PHP";
    } break;
    case INSTRUCTION::PLA: {
        i = "PLA";
    } break;
    case INSTRUCTION::PLP: {
        i = "PLP";
    } break;
    case INSTRUCTION::ROL: {
        i = "ROL";
    } break;
    case INSTRUCTION::ROR: {
        i = "ROR";
    } break;
    case INSTRUCTION::RTI: {
        i = "RTI";
    } break;
    case INSTRUCTION::RTS: {
        i = "RTS";
    } break;
    case INSTRUCTION::SBC: {
        i = "SBC";
    } break;
    case INSTRUCTION::SEC: {
        i = "SEC";
    } break;
    case INSTRUCTION::SED: {
        i = "SED";
    } break;
    case INSTRUCTION::SEI: {
        i = "SEI";
    } break;
    case INSTRUCTION::STA: {
        i = "STA";
    } break;
    case INSTRUCTION::STX: {
        i = "STX";
    } break;
    case INSTRUCTION::STY: {
        i = "STY";
    } break;
    case INSTRUCTION::TAX: {
        i = "TAX";
    } break;
    case INSTRUCTION::TAY: {
        i = "TAY";
    } break;
    case INSTRUCTION::TSX: {
        i = "TSX";
    } break;
    case INSTRUCTION::TXA: {
        i = "TXA";
    } break;
    case INSTRUCTION::TXS: {
        i = "TXS";
    } break;
    case INSTRUCTION::TYA: {
        i = "TYA";
    } break;
    case INSTRUCTION::INVALID:
        break;
    }
    std::string m = "INVALID";
    switch (mode) {
    case ADDRESSING_MODE::IMPLICIT: {
        m = "IMPLICIT";
    } break;
    case ADDRESSING_MODE::ACCUMULATOR: {
        m = "ACCUMULATOR";
    } break;
    case ADDRESSING_MODE::IMMEDIATE: {
        m = "IMMEDIATE";
    } break;
    case ADDRESSING_MODE::ZEROPAGE: {
        m = "ZEROPAGE";
    } break;
    case ADDRESSING_MODE::ZEROPAGE_X: {
        m = "ZEROPAGE_X";
    } break;
    case ADDRESSING_MODE::ZEROPAGE_Y: {
        m = "ZEROPAGE_Y";
    } break;
    case ADDRESSING_MODE::RELATIVE: {
        m = "RELATIVE";
    } break;
    case ADDRESSING_MODE::ABSOLUTE: {
        m = "ABSOLUTE";
    } break;
    case ADDRESSING_MODE::ABSOLUTE_X: {
        m = "ABSOLUTE_X";
    } break;
    case ADDRESSING_MODE::ABSOLUTE_Y: {
        m = "ABSOLUTE_Y";
    } break;
    case ADDRESSING_MODE::INDIRECT: {
        m = "INDIRECT";
    } break;
    case ADDRESSING_MODE::INDIRECT_X: {
        m = "INDIRECT_X";
    } break;
    case ADDRESSING_MODE::INDIRECT_Y: {
        m = "INDIRECT_Y";
    } break;
    case ADDRESSING_MODE::INVALID: {
        m = "INVALID";
    } break;
        break;
    }
    return i + "\t" + m;
}

void CPU::execute(BYTE opcode) {
    WORD orig_pc = pc - 1;

    auto [ins, mode] = decode(opcode);
    WORD operand = fetch_operands(mode);

    instruction_t to_execute = dispatch_table[ins];

    to_execute(mode, operand);

    std::stringstream ss;
    ss << "0x" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << static_cast<int>(orig_pc) << std::dec << ":\t";
    ss << "0x" << std::setw(2) << std::setfill('0') << std::hex
       << static_cast<int>(opcode) << std::dec << "\t";
    ss << "0x" << std::setw(4) << std::setfill('0') << std::hex
       << static_cast<int>(operand) << std::dec << "\t";
    ss << print_info(ins, mode);
    // std::cerr << ss.str() << "\n";
}

void CPU::ADC(ADDRESSING_MODE mode, WORD operand) {
    BYTE rhs = 0x00;
    switch (mode) {
    case ADDRESSING_MODE::IMMEDIATE: {
        rhs = operand & 0xff;
    } break;
    case ADDRESSING_MODE::ZEROPAGE: {
        rhs = memory[operand];
    } break;
    case ADDRESSING_MODE::ZEROPAGE_X: {
        WORD addr = (operand + x) & 0xff;
        rhs = memory[addr];
    } break;
    case ADDRESSING_MODE::ABSOLUTE: {
        rhs = memory[operand];
    } break;
    case ADDRESSING_MODE::ABSOLUTE_X: {
        rhs = memory[operand + x];
    } break;
    case ADDRESSING_MODE::ABSOLUTE_Y: {
        rhs = memory[operand + y];
    } break;
    case ADDRESSING_MODE::INDIRECT_X: {
        WORD addr = (operand + x) & 0xff;
        addr = memory[addr] | (memory[addr + 1] << 8);
        rhs = memory[addr];
    } break;
    case ADDRESSING_MODE::INDIRECT_Y: {
        WORD addr = operand;
        addr = memory[addr] | (memory[addr + 1] << 8);
        addr = addr + y;
        rhs = memory[addr];
    } break;
    case ADDRESSING_MODE::ACCUMULATOR:
    case ADDRESSING_MODE::IMPLICIT:
    case ADDRESSING_MODE::RELATIVE:
    case ADDRESSING_MODE::INVALID: {
        // TODO: add assert
    } break;
    default:
        break;
    }
    WORD result = rhs + a + c;
    BYTE new_a = result & 0xff;

    c = (result > 0xff) ? 1 : 0;
    z = new_a == 0 ? 1 : 0;
    v = (((a ^ new_a) & (rhs ^ new_a)) & 0x80) >> 7;
    n = (new_a >> 7) & 0x1;

    a = new_a;
}

void CPU::AND(ADDRESSING_MODE mode, WORD operand) {
    BYTE rhs = 0x00;
    switch (mode) {
    case ADDRESSING_MODE::IMMEDIATE: {
        rhs = operand & 0xff;
    } break;
    case ADDRESSING_MODE::ZEROPAGE: {
        rhs = memory[operand];
    } break;
    case ADDRESSING_MODE::ZEROPAGE_X: {
        WORD addr = (operand + x) & 0xff;
        rhs = memory[addr];
    } break;
    case ADDRESSING_MODE::ABSOLUTE: {
        rhs = memory[operand];
    } break;
    case ADDRESSING_MODE::ABSOLUTE_X: {
        rhs = memory[operand + x];
    } break;
    case ADDRESSING_MODE::ABSOLUTE_Y: {
        rhs = memory[operand + y];
    } break;
    case ADDRESSING_MODE::INDIRECT_X: {
        WORD addr = (operand + x) & 0xff;
        addr = memory[addr] | (memory[addr + 1] << 8);
        rhs = memory[addr];
    } break;
    case ADDRESSING_MODE::INDIRECT_Y: {
        WORD addr = operand;
        addr = memory[addr] | (memory[addr + 1] << 8);
        addr = addr + y;
        rhs = memory[addr];
    } break;
    case ADDRESSING_MODE::ACCUMULATOR:
    case ADDRESSING_MODE::IMPLICIT:
    case ADDRESSING_MODE::RELATIVE:
    case ADDRESSING_MODE::INVALID: {
        // TODO: add assert
    } break;
    default:
        break;
    }

    BYTE new_a = rhs & a;

    z = new_a == 0 ? 1 : 0;
    n = (new_a >> 7) & 0x1;

    a = new_a;
}

void CPU::ASL(ADDRESSING_MODE mode, WORD operand) {
    BYTE *rhs = nullptr;
    switch (mode) {
    case ADDRESSING_MODE::ACCUMULATOR: {
        rhs = &a;
    } break;
    case ADDRESSING_MODE::ZEROPAGE: {
        rhs = &memory[operand];
    } break;
    case ADDRESSING_MODE::ZEROPAGE_X: {
        WORD addr = (operand + x) & 0xff;
        rhs = &memory[addr];
    } break;
    case ADDRESSING_MODE::ABSOLUTE: {
        rhs = &memory[operand];
    } break;
    case ADDRESSING_MODE::ABSOLUTE_X: {
        rhs = &memory[operand + x];
    } break;
    case ADDRESSING_MODE::IMMEDIATE:
    case ADDRESSING_MODE::ABSOLUTE_Y:
    case ADDRESSING_MODE::INDIRECT_X:
    case ADDRESSING_MODE::INDIRECT_Y:
    case ADDRESSING_MODE::IMPLICIT:
    case ADDRESSING_MODE::RELATIVE:
    case ADDRESSING_MODE::INVALID: {
        // TODO: add assert
    } break;
    default:
        break;
    }

    if (rhs != nullptr) {
        c = (*rhs >> 7) & 0x1;
        *rhs = *rhs << 1;
        z = (*rhs == 0) ? 1 : 0;
        n = (*rhs >> 7) & 0x1;
    }
}

void CPU::BCC(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::BCS(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::BEQ(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::BIT(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::BMI(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::BNE(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::BPL(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::BRK(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::BVC(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::BVS(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::CLC(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::CLD(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::CLI(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::CLV(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::CMP(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::CPX(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::CPY(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::DEC(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::DEX(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::DEY(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::EOR(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::INC(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::INX(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::INY(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::JMP(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::JSR(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::LDA(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::LDX(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::LDY(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::LSR(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::NOP(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::ORA(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::PHA(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::PHP(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::PLA(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::PLP(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::ROL(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::ROR(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::RTI(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::RTS(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::SBC(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::SEC(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::SED(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::SEI(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::STA(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::STX(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::STY(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::TAX(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::TAY(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::TSX(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::TXA(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::TXS(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::TYA(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}

void CPU::INVALID(ADDRESSING_MODE mode, WORD operand) {
    std::cerr << "Not Implemented\n";
}
