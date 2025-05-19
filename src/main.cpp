#include <CPU.hpp>
#include <Types.hpp>

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <rom_file>\n";
        return 1;
    }

    std::string rom_path = argv[1];

    std::cout << "Rom path: " << rom_path << "\n";

    std::ifstream rom(rom_path, std::ios::binary);

    if (!rom.is_open()) {
        std::cerr << "Failed to open rom: " << rom_path << "\n";
        return 1;
    }

    mos6502::mem_t memory = {0};
    rom.read(reinterpret_cast<char *>(&memory[0]), 0x10000);

    mos6502::CPU cpu(memory);
    cpu.reset();
    cpu.init_opcode_table();

    for (int i = 0; i < 12; i++) {
        mos6502::BYTE opcode = cpu.fetch_opcode();
        cpu.execute(opcode);
    }

    rom.close();

    return 9;
}
