#include <CPU.hpp>
#include <Types.hpp>

#include <array>
#include <gtest/gtest.h>

using namespace mos6502;

TEST(TEST_INSTRUCTIONS, ADC) {
    mem_t memory;

    WORD start = 0x8000;
    memory[0xfffc] = start & 0xff;
    memory[0xfffd] = (start >> 8) & 0xff;

    WORD pc = start;

    /* Assembly to be tested
LDX #$04
LDY #$a0

ADC #$05
ADC #$FB
ADC #$01
ADC #$7F
ADC #$80
ADC #$01
ADC #$FF

; Setup: memory[0x0010] = 0x02
STA $10
ADC $10

; Setup: memory[0x0014] = 0x05
STA $14
ADC $10,X

; Setup: memory[$1234] = 0x0a
STA $1234
ADC $1234

; Setup: memory[$1238] = 0x14
STA $1238
ADC $1234,X

; Setup: memory[$12d4] = 0x28
STA $12d4
ADC $1234,Y

; Setup: ($20,X) → [$50, $50] = $5050 → memory[$5050] = 0x50
STA $24
STA $25
STA $5050
ADC ($20,X)

; Setup: ($20),Y → [$a0, $a0] = $a0a0 → memory[$a0a0 + Y] = 0xa0
STA $20
STA $21
STA $a140
ADC ($20),Y
     */

    // ADC #$05
    memory[pc++] = 0x69;
    memory[pc++] = 0x05;
    // ADC #$FB
    memory[pc++] = 0x69;
    memory[pc++] = 0xFB;
    // ADC #$01
    memory[pc++] = 0x69;
    memory[pc++] = 0x01;
    // ADC #$7F
    memory[pc++] = 0x69;
    memory[pc++] = 0x7F;
    // ADC #$80
    memory[pc++] = 0x69;
    memory[pc++] = 0x80;
    // ADC #$01
    memory[pc++] = 0x69;
    memory[pc++] = 0x01;
    // ADC #$FF
    memory[pc++] = 0x69;
    memory[pc++] = 0xFF;
    // ADC $10
    memory[0x0010] = 0x02;
    memory[pc++] = 0x65;
    memory[pc++] = 0x10;
    // ADC $10,X
    memory[0x0014] = 0x05;
    memory[pc++] = 0x75;
    memory[pc++] = 0x10;
    // ADC $1234
    memory[0x1234] = 0x0a;
    memory[pc++] = 0x6D;
    memory[pc++] = 0x34;
    memory[pc++] = 0x12;
    // ADC $1234,X
    memory[0x1238] = 0x14;
    memory[pc++] = 0x7D;
    memory[pc++] = 0x34;
    memory[pc++] = 0x12;
    // ADC $1234,Y
    memory[0x12d4] = 0x28;
    memory[pc++] = 0x79;
    memory[pc++] = 0x34;
    memory[pc++] = 0x12;
    // ADC ($20,X)
    memory[0x0024] = 0x50;
    memory[0x0025] = 0x50;
    memory[0x5050] = 0x50;
    memory[pc++] = 0x61;
    memory[pc++] = 0x20;
    // ADC ($20),Y
    memory[0x0020] = 0xa0;
    memory[0x0021] = 0xa0;
    memory[0xa140] = 0xa0;
    memory[pc++] = 0x71;
    memory[pc++] = 0x20;

    // Results
    std::array<BYTE, 14> A = {0x05, 0x00, 0x02, 0x81, 0x01, 0x03, 0x02,
                              0x05, 0x0a, 0x14, 0x28, 0x50, 0xa0, 0x40};
    std::array<BYTE, 14> P = {0x30, 0x33, 0x30, 0xf0, 0x71, 0x30, 0x31,
                              0x30, 0x30, 0x30, 0x30, 0x30, 0xf0, 0x71};

    CPU cpu(memory);
    cpu.reset();

    cpu.x = 0x04;
    cpu.y = 0xa0;

    int N = 14;
    for (int i = 0; i < N; i++) {
        BYTE opcode = cpu.fetch_opcode();
        cpu.execute(opcode);
        EXPECT_EQ(cpu.a, A[i]);
        BYTE p = cpu.get_p();
        EXPECT_EQ(p & 0xc3, P[i] & 0xc3);
    }
}

TEST(TEST_INSTRUCTIONS, AND) {
    mem_t memory;

    WORD start = 0x8000;
    memory[0xfffc] = start & 0xff;
    memory[0xfffd] = (start >> 8) & 0xff;

    WORD pc = start;

    /* Assembly to be tested
LDX #$04
LDY #$a0
LDA #$FF

AND #$AF
AND #$4D
AND #$02

LDA #$ff

; Setup: memory[0x0010] = 0x7f
LDA #$7f
STA $10
LDA #$ff
AND $10

; Setup: memory[0x0014] = 0x3f
LDA #$3f
STA $14
LDA #$7f
AND $10,X

; Setup: memory[$1234] = 0x1f
LDA #$1f
STA $1234
LDA #$3f
AND $1234

; Setup: memory[$1238] = 0x0f
LDA #$0f
STA $1238
LDA #$1f
AND $1234,X

; Setup: memory[$12d4] = 0x07
LDA #$07
STA $12d4
LDA #$0f
AND $1234,Y

; Setup: ($20,X) → [$07, $07] = $0707 → memory[$0707] = 0x03
STA $24
STA $25
LDA #$03
STA $0707
LDA #$07
AND ($20,X)

; Setup: ($20),Y → [$03, $03] = $0303 → memory[$0303 + Y] = 0x01
STA $20
STA $21
LDA #$01
STA $03a3
LDA #$03
AND ($20),Y
     */

    // AND #$AF
    memory[pc++] = 0x29;
    memory[pc++] = 0xaf;
    // AND #$4D
    memory[pc++] = 0x29;
    memory[pc++] = 0x4d;
    // AND #$02
    memory[pc++] = 0x29;
    memory[pc++] = 0x02;

    // AND $10
    memory[0x0010] = 0x7f;
    memory[pc++] = 0x25;
    memory[pc++] = 0x10;
    // AND $10,X
    memory[0x0014] = 0x3f;
    memory[pc++] = 0x35;
    memory[pc++] = 0x10;
    // AND $1234
    memory[0x1234] = 0x1f;
    memory[pc++] = 0x2d;
    memory[pc++] = 0x34;
    memory[pc++] = 0x12;
    // AND $1234,X
    memory[0x1238] = 0x0f;
    memory[pc++] = 0x3d;
    memory[pc++] = 0x34;
    memory[pc++] = 0x12;
    // AND $1234,Y
    memory[0x12d4] = 0x07;
    memory[pc++] = 0x39;
    memory[pc++] = 0x34;
    memory[pc++] = 0x12;
    // AND ($20,X)
    memory[0x0024] = 0x07;
    memory[0x0025] = 0x07;
    memory[0x0707] = 0x03;
    memory[pc++] = 0x21;
    memory[pc++] = 0x20;
    // AND ($20),Y
    memory[0x0020] = 0x03;
    memory[0x0021] = 0x03;
    memory[0x03a3] = 0x01;
    memory[pc++] = 0x31;
    memory[pc++] = 0x20;

    // Results
    std::array<BYTE, 10> A = {0xaf, 0x0d, 0x00, 0x7f, 0x3f,
                              0x1f, 0x0f, 0x07, 0x03, 0x01};
    std::array<BYTE, 10> P = {0xb0, 0x30, 0x32, 0x30, 0x30,
                              0x30, 0x30, 0x30, 0x30, 0x30};

    CPU cpu(memory);
    cpu.reset();

    cpu.x = 0x04;
    cpu.y = 0xa0;
    cpu.a = 0xff;

    for (int i = 0; i < 3; i++) {
        BYTE opcode = cpu.fetch_opcode();
        cpu.execute(opcode);
        EXPECT_EQ(cpu.a, A[i]);
        BYTE p = cpu.get_p();
        EXPECT_EQ(p & 0x82, P[i] & 0x82);
    }
    cpu.a = 0xff;
    int N = 7;
    for (int i = 3; i < 3 + N; i++) {
        BYTE opcode = cpu.fetch_opcode();
        cpu.execute(opcode);
        EXPECT_EQ(cpu.a, A[i]);
        BYTE p = cpu.get_p();
        EXPECT_EQ(p & 0x82, P[i] & 0x82);
    }
}

TEST(TEST_INSTRUCTIONS, ASL) {
    mem_t memory;

    WORD start = 0x8000;
    memory[0xfffc] = start & 0xff;
    memory[0xfffd] = (start >> 8) & 0xff;

    WORD pc = start;

    /* Assembly to be tested
LDX #$04

LDA #$40

ASL A
ASL A

; Setup: memory[0x0010] = 0x55
LDA #$55
STA $10
ASL $10
LDA $10

; Setup: memory[0x0014] = 0xaa
STA $10,X
ASL $10,X
LDA $10,X

; Setup: memory[$1234] = 0x54
STA $1234
ASL $1234
LDA $1234

; Setup: memory[$1238] = 0xa8
STA $1234,X
ASL $1234,X
LDA $1234,X
     */

    // AND #$AF
    memory[pc++] = 0x29;
    memory[pc++] = 0xaf;
    // AND #$4D
    memory[pc++] = 0x29;
    memory[pc++] = 0x4d;
    // AND #$02
    memory[pc++] = 0x29;
    memory[pc++] = 0x02;

    // AND $10
    memory[0x0010] = 0x7f;
    memory[pc++] = 0x25;
    memory[pc++] = 0x10;
    // AND $10,X
    memory[0x0014] = 0x3f;
    memory[pc++] = 0x35;
    memory[pc++] = 0x10;
    // AND $1234
    memory[0x1234] = 0x1f;
    memory[pc++] = 0x2d;
    memory[pc++] = 0x34;
    memory[pc++] = 0x12;
    // AND $1234,X
    memory[0x1238] = 0x0f;
    memory[pc++] = 0x3d;
    memory[pc++] = 0x34;
    memory[pc++] = 0x12;
    // AND $1234,Y
    memory[0x12d4] = 0x07;
    memory[pc++] = 0x39;
    memory[pc++] = 0x34;
    memory[pc++] = 0x12;
    // AND ($20,X)
    memory[0x0024] = 0x07;
    memory[0x0025] = 0x07;
    memory[0x0707] = 0x03;
    memory[pc++] = 0x21;
    memory[pc++] = 0x20;
    // AND ($20),Y
    memory[0x0020] = 0x03;
    memory[0x0021] = 0x03;
    memory[0x03a3] = 0x01;
    memory[pc++] = 0x31;
    memory[pc++] = 0x20;

    // Results
    std::array<BYTE, 10> A = {0xaf, 0x0d, 0x00, 0x7f, 0x3f,
                              0x1f, 0x0f, 0x07, 0x03, 0x01};
    std::array<BYTE, 10> P = {0xb0, 0x30, 0x32, 0x30, 0x30,
                              0x30, 0x30, 0x30, 0x30, 0x30};

    CPU cpu(memory);
    cpu.reset();

    cpu.x = 0x04;
    cpu.y = 0xa0;
    cpu.a = 0xff;

    for (int i = 0; i < 3; i++) {
        BYTE opcode = cpu.fetch_opcode();
        cpu.execute(opcode);
        EXPECT_EQ(cpu.a, A[i]);
        BYTE p = cpu.get_p();
        EXPECT_EQ(p & 0x82, P[i] & 0x82);
    }
    cpu.a = 0xff;
    int N = 7;
    for (int i = 3; i < 3 + N; i++) {
        BYTE opcode = cpu.fetch_opcode();
        cpu.execute(opcode);
        EXPECT_EQ(cpu.a, A[i]);
        BYTE p = cpu.get_p();
        EXPECT_EQ(p & 0x82, P[i] & 0x82);
    }
}
