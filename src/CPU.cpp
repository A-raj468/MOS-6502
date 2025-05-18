#include <CPU.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>

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

BYTE CPU::fetch_opcode() {
    BYTE opcode = memory[pc];
    pc++;
    return opcode;
}

void CPU::decode(BYTE opcode) {
    std::stringstream ss;

    ss << "0x" << std::uppercase << std::setfill('0') << std::setw(2)
       << std::hex << static_cast<int>(opcode) << std::dec;

    std::cout << ss.str() << "\n";
}
