#include <CPU.hpp>
#include <Types.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace mos6502;

CPU::CPU(mem_t &memory) : memory(memory) {}

void CPU::reset() {
    pc = memory[0xfffc] | (memory[0xfffd] << 8);
    n = v = b = d = z = c = 0;
    u = i = 1;
    sp = 0xfd;
    a = 0;
    x = y = 0;
}

void CPU::init_opcode_table() {
    table = {
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

BYTE CPU::fetch_opcode() {
    BYTE opcode = memory[pc];
    pc++;
    return opcode;
}

Instruction_info CPU::decode(BYTE opcode) { return table[opcode]; }

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

std::string print_info(INSTRUCTION ins, ADDRESSING_MODE mode) {
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
    std::stringstream ss;

    ss << "0x" << std::uppercase << std::setw(4) << std::setfill('0')
       << std::hex << static_cast<int>(pc) << std::dec << ":\t";

    auto [ins, mode] = decode(opcode);
    WORD operand = fetch_operands(mode);

    ss << "0x" << std::setw(2) << std::setfill('0') << std::hex
       << static_cast<int>(opcode) << std::dec << "\t";
    ss << "0x" << std::setw(4) << std::setfill('0') << std::hex
       << static_cast<int>(operand) << std::dec << "\t";
    ss << print_info(ins, mode);

    std::cerr << ss.str() << "\n";
}
