#include <CPU.hpp>
#include <Types.hpp>

#include <array>
#include <gtest/gtest.h>

using namespace mos6502;

// A dummy test to verify that the testing framework is working
TEST(TEST_ADC, ADC_TESTS) {
    mem_t memory;

    WORD start = 0x8000;
    memory[0xfffc] = start & 0xff;
    memory[0xfffd] = (start >> 8) & 0xff;

    WORD pc = start;

    // ADC #$05 ; A = 0x00 + 0x05 = 0x05
    memory[pc++] = 0x69;
    memory[pc++] = 0x05;
    // ADC #$FB ; A = 0x05 + 0xFB = 0x00 (carry set)
    memory[pc++] = 0x69;
    memory[pc++] = 0xFB;
    // ADC #$01 ; A = 0x00 + 0x01 + 1(carry) = 0x02
    memory[pc++] = 0x69;
    memory[pc++] = 0x01;
    // ADC #$7F ; A = 0x02 + 0x7F = 0x81 (overflow)
    memory[pc++] = 0x69;
    memory[pc++] = 0x7F;
    // ADC #$80 ; A = 0x80 + 0x80 = 0x00 (overflow + carry)
    memory[pc++] = 0x69;
    memory[pc++] = 0x80;
    // ADC #$01 ; A = 0x00 + 0x01 + carry = 0x02
    memory[pc++] = 0x69;
    memory[pc++] = 0x01;
    // ADC #$FF ; A = 0x02 + 0xFF = 0x01, carry set
    memory[pc++] = 0x69;
    memory[pc++] = 0xFF;

    // Results
    std::array<BYTE, 7> A = {0x05, 0x00, 0x02, 0x81, 0x01, 0x03, 0x02};
    std::array<BYTE, 7> P = {0b00110000, 0b00110011, 0b00110000, 0b11110000,
                             0b01110001, 0b00110000, 0b00110001};

    CPU cpu(memory);
    cpu.reset();

    for (int i = 0; i < 7; i++) {
        BYTE opcode = cpu.fetch_opcode();
        cpu.execute(opcode);
        EXPECT_EQ(cpu.a, A[i]);
        BYTE p = cpu.get_p();
        EXPECT_EQ(p & 0b11000011, P[i] & 0b11000011);
    }
}
