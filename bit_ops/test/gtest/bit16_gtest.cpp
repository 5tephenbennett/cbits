/**
 * @file bit16_gtest.cpp
 * @brief Google Test for 16-bit operations.
 */
#include <cstdint>
#include <gtest/gtest.h>

extern "C" {
#include "bit16_ops.h"
#include "bit_ops.h"
}

// Test fixture for BIT16 operations
class Bit16Test : public ::testing::Test {
  protected:
    void SetUp() override {
        // Test setup
    }

    void TearDown() override {
        // Test cleanup
    }
};

// Test BIT16_Set function
TEST_F(Bit16Test, BIT16_Set) {
    EXPECT_EQ(BIT16_Set(0), 1);      // 2^0 = 1
    EXPECT_EQ(BIT16_Set(1), 2);      // 2^1 = 2
    EXPECT_EQ(BIT16_Set(8), 256);    // 2^8 = 256
    EXPECT_EQ(BIT16_Set(15), 32768); // 2^15 = 32768
}

// Test BIT16_Count_Bits_Set function
TEST_F(Bit16Test, BIT16_Count_Bits_Set) {
    EXPECT_EQ(BIT16_Count_Bits_Set(0), 0);      // 0000000000000000 -> 0 bits
    EXPECT_EQ(BIT16_Count_Bits_Set(1), 1);      // 0000000000000001 -> 1 bit
    EXPECT_EQ(BIT16_Count_Bits_Set(3), 2);      // 0000000000000011 -> 2 bits
    EXPECT_EQ(BIT16_Count_Bits_Set(255), 8);    // 0000000011111111 -> 8 bits
    EXPECT_EQ(BIT16_Count_Bits_Set(65535), 16); // 1111111111111111 -> 16 bits
}

// Test BIT16_Get_Bit function
TEST_F(Bit16Test, BIT16_Get_Bit) {
    uint16_t value = 0b1010101010101010; // 43690

    EXPECT_EQ(BIT16_Get_Bit(value, 0), 0);  // bit 0 is 0
    EXPECT_EQ(BIT16_Get_Bit(value, 1), 1);  // bit 1 is 1
    EXPECT_EQ(BIT16_Get_Bit(value, 8), 0);  // bit 8 is 0
    EXPECT_EQ(BIT16_Get_Bit(value, 9), 1);  // bit 9 is 1
    EXPECT_EQ(BIT16_Get_Bit(value, 15), 1); // bit 15 is 1
}

// Test BIT16_Bitmask function
TEST_F(Bit16Test, BIT16_Bitmask) {
    EXPECT_EQ(BIT16_Bitmask(0, 3), 15);     // bits 0-3: 0000000000001111
    EXPECT_EQ(BIT16_Bitmask(4, 7), 240);    // bits 4-7: 0000000011110000
    EXPECT_EQ(BIT16_Bitmask(8, 15), 65280); // bits 8-15: 1111111100000000
}

// Test BIT16_Get_Bits function
TEST_F(Bit16Test, BIT16_Get_Bits) {
    uint16_t value = 0x1234; // 0001001000110100

    EXPECT_EQ(BIT16_Get_Bits(value, 0, 3), 4);   // bottom 4 bits: 0100 = 4
    EXPECT_EQ(BIT16_Get_Bits(value, 4, 7), 3);   // next 4 bits: 0011 = 3
    EXPECT_EQ(BIT16_Get_Bits(value, 8, 11), 2);  // next 4 bits: 0010 = 2
    EXPECT_EQ(BIT16_Get_Bits(value, 12, 15), 1); // top 4 bits: 0001 = 1
}

TEST_F(Bit16Test, BIT16_Clear_Bit) {
    std::uint16_t value = 0xFFFF;                  // 1111111111111111
    EXPECT_EQ(BIT16_Clear_Bit(value, 0), 0xFFFE);  // clear bit 0
    EXPECT_EQ(BIT16_Clear_Bit(value, 8), 0xFEFF);  // clear bit 8
    EXPECT_EQ(BIT16_Clear_Bit(value, 15), 0x7FFF); // clear bit 15
}

TEST_F(Bit16Test, BIT16_Clear_Bits) {
    std::uint16_t value = 0xFFFF;                      // 1111111111111111
    EXPECT_EQ(BIT16_Clear_Bits(value, 0, 7), 0xFF00);  // clear bits 0-7
    EXPECT_EQ(BIT16_Clear_Bits(value, 8, 15), 0x00FF); // clear bits 8-15
    EXPECT_EQ(BIT16_Clear_Bits(value, 4, 11), 0xF00F); // clear bits 4-11
}

TEST_F(Bit16Test, BIT16_Clear_nBits) {
    std::uint16_t value = 0xFFFF;                      // 1111111111111111
    EXPECT_EQ(BIT16_Clear_nBits(value, 0, 8), 0xFF00); // clear 8 bits from pos 0
    EXPECT_EQ(BIT16_Clear_nBits(value, 8, 8), 0x00FF); // clear 8 bits from pos 8
    EXPECT_EQ(BIT16_Clear_nBits(value, 4, 8), 0xF00F); // clear 8 bits from pos 4
}

TEST_F(Bit16Test, BIT16_Get_Bits_Signed) {
    std::int16_t value = -21846; // 1010101010101010 (same as 0xAAAA but signed)
    EXPECT_EQ(BIT16_Get_Bits_Signed(value, 0, 7), -86);  // bits 0-7: 10101010 (signed)
    EXPECT_EQ(BIT16_Get_Bits_Signed(value, 8, 15), -86); // bits 8-15: 10101010 (signed)

    // Test error condition: end_bit < start_bit
    EXPECT_EQ(BIT16_Get_Bits_Signed(value, 7, 0), 0);  // Invalid: end_bit < start_bit
    EXPECT_EQ(BIT16_Get_Bits_Signed(value, 15, 8), 0); // Invalid: end_bit < start_bit
}

TEST_F(Bit16Test, BIT16_Get_First_Bit_Set) {
    EXPECT_EQ(BIT16_Get_First_Bit_Set(0x0001), 0);  // 0000000000000001
    EXPECT_EQ(BIT16_Get_First_Bit_Set(0x0002), 1);  // 0000000000000010
    EXPECT_EQ(BIT16_Get_First_Bit_Set(0x0100), 8);  // 0000000100000000
    EXPECT_EQ(BIT16_Get_First_Bit_Set(0x8000), 15); // 1000000000000000
    EXPECT_EQ(BIT16_Get_First_Bit_Set(0x0006), 1);  // 0000000000000110 (first set bit is bit 1)
}

TEST_F(Bit16Test, BIT16_Get_nBits) {
    std::uint16_t value = 0x1234;                   // 0001001000110100
    EXPECT_EQ(BIT16_Get_nBits(value, 0, 4), 0x04);  // 4 bits from pos 0: 0100
    EXPECT_EQ(BIT16_Get_nBits(value, 4, 4), 0x03);  // 4 bits from pos 4: 0011
    EXPECT_EQ(BIT16_Get_nBits(value, 8, 4), 0x02);  // 4 bits from pos 8: 0010
    EXPECT_EQ(BIT16_Get_nBits(value, 12, 4), 0x01); // 4 bits from pos 12: 0001
}

TEST_F(Bit16Test, BIT16_Get_nBits_Signed) {
    std::int16_t value = -21846;                         // 1010101010101010
    EXPECT_EQ(BIT16_Get_nBits_Signed(value, 0, 8), -86); // 8 bits from pos 0: 10101010 (signed)
    EXPECT_EQ(BIT16_Get_nBits_Signed(value, 8, 8), -86); // 8 bits from pos 8: 10101010 (signed)
}

TEST_F(Bit16Test, BIT16_Get_Sign) {
    EXPECT_EQ(BIT16_Get_Sign(32767), 1);   // positive: 0111111111111111 -> +1
    EXPECT_EQ(BIT16_Get_Sign(-32768), -1); // negative: 1000000000000000 -> -1
    EXPECT_EQ(BIT16_Get_Sign(-1), -1);     // negative: 1111111111111111 -> -1
    EXPECT_EQ(BIT16_Get_Sign(0), 1);       // zero: 0000000000000000 -> +1
}

TEST_F(Bit16Test, BIT16_Has_Zero_Byte) {
    EXPECT_EQ(BIT16_Has_Zero_Byte(0x0000), 1); // has zero bytes
    EXPECT_EQ(BIT16_Has_Zero_Byte(0xFF00), 1); // has zero byte in lower half
    EXPECT_EQ(BIT16_Has_Zero_Byte(0x00FF), 1); // has zero byte in upper half
    EXPECT_EQ(BIT16_Has_Zero_Byte(0xFFFF), 0); // no zero bytes
}

TEST_F(Bit16Test, BIT16_Insert_Bit) {
    std::uint16_t value = 0x00FF;                     // 0000000011111111
    EXPECT_EQ(BIT16_Insert_Bit(value, 1, 8), 0x01FF); // insert 1 at pos 8
    EXPECT_EQ(BIT16_Insert_Bit(value, 0, 8), 0x00FF); // insert 0 at pos 8
}

TEST_F(Bit16Test, BIT16_Insert_Bits) {
    std::uint16_t value = 0x00FF;                             // 0000000011111111
    EXPECT_EQ(BIT16_Insert_Bits(value, 0xAB, 8, 15), 0xABFF); // insert 0xAB at bits 8-15
}

TEST_F(Bit16Test, BIT16_Insert_nBits) {
    std::uint16_t value = 0x00FF;                             // 0000000011111111
    EXPECT_EQ(BIT16_Insert_nBits(value, 0xAB, 8, 8), 0xABFF); // insert 8 bits (0xAB) at pos 8
}

TEST_F(Bit16Test, BIT16_isNeg) {
    EXPECT_EQ(BIT16_isNeg(32767), 0);  // positive: 0111111111111111
    EXPECT_EQ(BIT16_isNeg(-32768), 1); // negative: 1000000000000000
    EXPECT_EQ(BIT16_isNeg(-1), 1);     // negative: 1111111111111111
    EXPECT_EQ(BIT16_isNeg(0), 0);      // zero: 0000000000000000
}

TEST_F(Bit16Test, BIT16_LBitmask) {
    EXPECT_EQ(BIT16_LBitmask(0), 0xFFFF);  // (-1 << 0) = 1111111111111111
    EXPECT_EQ(BIT16_LBitmask(7), 0xFF80);  // (-1 << 7) = 1111111110000000
    EXPECT_EQ(BIT16_LBitmask(15), 0x8000); // (-1 << 15) = 1000000000000000
}

TEST_F(Bit16Test, BIT16_Mask_Bit) {
    std::uint16_t value = 0xAAAA;                // 1010101010101010
    EXPECT_EQ(BIT16_Mask_Bit(value, 0), 0x0000); // mask bit 0: bit not set
    EXPECT_EQ(BIT16_Mask_Bit(value, 1), 0x0002); // mask bit 1: bit is set
    EXPECT_EQ(BIT16_Mask_Bit(value, 8), 0x0000); // mask bit 8: bit not set
    EXPECT_EQ(BIT16_Mask_Bit(value, 9), 0x0200); // mask bit 9: bit is set
}

TEST_F(Bit16Test, BIT16_Mask_Bits) {
    std::uint16_t value = 0x1234;                     // 0001001000110100
    EXPECT_EQ(BIT16_Mask_Bits(value, 0, 7), 0x0034);  // mask bits 0-7
    EXPECT_EQ(BIT16_Mask_Bits(value, 8, 15), 0x1200); // mask bits 8-15
}

TEST_F(Bit16Test, BIT16_Mask_nBits) {
    std::uint16_t value = 0x1234;                     // 0001001000110100
    EXPECT_EQ(BIT16_Mask_nBits(value, 0, 8), 0x0034); // mask 8 bits from pos 0
    EXPECT_EQ(BIT16_Mask_nBits(value, 8, 8), 0x1200); // mask 8 bits from pos 8
}

TEST_F(Bit16Test, BIT16_nBitmask) {
    EXPECT_EQ(BIT16_nBitmask(0, 8), 0x00FF); // 8 bits from pos 0
    EXPECT_EQ(BIT16_nBitmask(8, 8), 0xFF00); // 8 bits from pos 8
    EXPECT_EQ(BIT16_nBitmask(4, 8), 0x0FF0); // 8 bits from pos 4
}

TEST_F(Bit16Test, BIT16_RBitmask) {
    EXPECT_EQ(BIT16_RBitmask(0), 0x0001);  // right mask from bit 0
    EXPECT_EQ(BIT16_RBitmask(7), 0x00FF);  // right mask from bit 7
    EXPECT_EQ(BIT16_RBitmask(15), 0xFFFF); // right mask from bit 15
}

TEST_F(Bit16Test, BIT16_Reverse) {
    EXPECT_EQ(BIT16_Reverse(0x0001), 0x8000); // 0000000000000001 -> 1000000000000000
    EXPECT_EQ(BIT16_Reverse(0x00FF), 0xFF00); // 0000000011111111 -> 1111111100000000
    EXPECT_EQ(BIT16_Reverse(0x1234), 0x2C48); // bit reversal of 0x1234
}

TEST_F(Bit16Test, BIT16_Set_Bit) {
    std::uint16_t value = 0x0000;                // 0000000000000000
    EXPECT_EQ(BIT16_Set_Bit(value, 0), 0x0001);  // set bit 0
    EXPECT_EQ(BIT16_Set_Bit(value, 8), 0x0100);  // set bit 8
    EXPECT_EQ(BIT16_Set_Bit(value, 15), 0x8000); // set bit 15
}

TEST_F(Bit16Test, BIT16_Set_Bits) {
    std::uint16_t value = 0x0000;                    // 0000000000000000
    EXPECT_EQ(BIT16_Set_Bits(value, 0, 7), 0x00FF);  // set bits 0-7
    EXPECT_EQ(BIT16_Set_Bits(value, 8, 15), 0xFF00); // set bits 8-15
}

TEST_F(Bit16Test, BIT16_Set_nBits) {
    std::uint16_t value = 0x0000;                    // 0000000000000000
    EXPECT_EQ(BIT16_Set_nBits(value, 0, 8), 0x00FF); // set 8 bits from pos 0
    EXPECT_EQ(BIT16_Set_nBits(value, 8, 8), 0xFF00); // set 8 bits from pos 8
}

TEST_F(Bit16Test, BIT16_Sign_Extend) {
    // Test sign extension for various bit widths covering more switch cases

    // Test 1-bit values (case 1)
    EXPECT_EQ(BIT16_Sign_Extend(0x1, 1), -1); // 1 -> -1 (sign bit set)
    EXPECT_EQ(BIT16_Sign_Extend(0x0, 1), 0);  // 0 -> 0 (sign bit clear)

    // Test 2-bit values (case 2)
    EXPECT_EQ(BIT16_Sign_Extend(0x3, 2), -1); // 11 -> -1
    EXPECT_EQ(BIT16_Sign_Extend(0x2, 2), -2); // 10 -> -2
    EXPECT_EQ(BIT16_Sign_Extend(0x1, 2), 1);  // 01 -> 1
    EXPECT_EQ(BIT16_Sign_Extend(0x0, 2), 0);  // 00 -> 0

    // Test 4-bit values (case 4)
    EXPECT_EQ(BIT16_Sign_Extend(0xF, 4), -1); // 1111 -> -1
    EXPECT_EQ(BIT16_Sign_Extend(0x8, 4), -8); // 1000 -> -8
    EXPECT_EQ(BIT16_Sign_Extend(0x7, 4), 7);  // 0111 -> 7
    EXPECT_EQ(BIT16_Sign_Extend(0x0, 4), 0);  // 0000 -> 0

    // Test 8-bit values (case 8)
    EXPECT_EQ(BIT16_Sign_Extend(0xFF, 8), -1);   // 11111111 -> -1
    EXPECT_EQ(BIT16_Sign_Extend(0x80, 8), -128); // 10000000 -> -128
    EXPECT_EQ(BIT16_Sign_Extend(0x7F, 8), 127);  // 01111111 -> 127
    EXPECT_EQ(BIT16_Sign_Extend(0x01, 8), 1);    // 00000001 -> 1

    // Test other bit widths to cover more switch cases
    EXPECT_EQ(BIT16_Sign_Extend(0x7, 3), -1); // case 3: 111 -> -1
    EXPECT_EQ(BIT16_Sign_Extend(0x3, 3), 3);  // case 3: 011 -> 3

    EXPECT_EQ(BIT16_Sign_Extend(0x1F, 5), -1); // case 5: 11111 -> -1
    EXPECT_EQ(BIT16_Sign_Extend(0x0F, 5), 15); // case 5: 01111 -> 15

    EXPECT_EQ(BIT16_Sign_Extend(0x3F, 6), -1); // case 6: 111111 -> -1
    EXPECT_EQ(BIT16_Sign_Extend(0x1F, 6), 31); // case 6: 011111 -> 31

    EXPECT_EQ(BIT16_Sign_Extend(0x7F, 7), -1); // case 7: 1111111 -> -1
    EXPECT_EQ(BIT16_Sign_Extend(0x3F, 7), 63); // case 7: 0111111 -> 63

    // Test mid-range values
    EXPECT_EQ(BIT16_Sign_Extend(0x1FF, 9), -1);  // case 9: 111111111 -> -1
    EXPECT_EQ(BIT16_Sign_Extend(0x0FF, 9), 255); // case 9: 011111111 -> 255

    EXPECT_EQ(BIT16_Sign_Extend(0x3FF, 10), -1);  // case 10: 1111111111 -> -1
    EXPECT_EQ(BIT16_Sign_Extend(0x1FF, 10), 511); // case 10: 0111111111 -> 511

    EXPECT_EQ(BIT16_Sign_Extend(0x1FFF, 13), -1);   // case 13: 1111111111111 -> -1
    EXPECT_EQ(BIT16_Sign_Extend(0x0FFF, 13), 4095); // case 13: 0111111111111 -> 4095

    EXPECT_EQ(BIT16_Sign_Extend(0x7FF, 11), -1);   // case 11: 11111111111 -> -1
    EXPECT_EQ(BIT16_Sign_Extend(0x3FF, 11), 1023); // case 11: 01111111111 -> 1023

    EXPECT_EQ(BIT16_Sign_Extend(0xFFF, 12), -1);   // case 12: 111111111111 -> -1
    EXPECT_EQ(BIT16_Sign_Extend(0x7FF, 12), 2047); // case 12: 011111111111 -> 2047

    // Test larger bit widths
    EXPECT_EQ(BIT16_Sign_Extend(0x3FFF, 14), -1);   // case 14
    EXPECT_EQ(BIT16_Sign_Extend(0x1FFF, 14), 8191); // case 14

    // Test 15-bit values (case 15) - original test cases
    EXPECT_EQ(BIT16_Sign_Extend(0x7FFF, 15), -1); // sign extend from bit 15
    EXPECT_EQ(BIT16_Sign_Extend(0x8000, 15), 0);  // sign extend from bit 15

    // Test 16-bit values (case 16) - covers return var; path
    EXPECT_EQ(BIT16_Sign_Extend(0xFFFF, 16), (int16_t)0xFFFF); // 16-bit: all bits set -> -1
    EXPECT_EQ(BIT16_Sign_Extend(0x8000, 16), (int16_t)0x8000); // 16-bit: sign bit set -> -32768
    EXPECT_EQ(BIT16_Sign_Extend(0x7FFF, 16), (int16_t)0x7FFF); // 16-bit: max positive -> 32767
    EXPECT_EQ(BIT16_Sign_Extend(0x0000, 16), (int16_t)0x0000); // 16-bit: zero -> 0

    // Test invalid bit numbers (default case) - covers default: return 0; path
    EXPECT_EQ(BIT16_Sign_Extend(0xFFFF, 0), 0);   // Invalid bit number 0 -> default case
    EXPECT_EQ(BIT16_Sign_Extend(0x1234, 17), 0);  // Invalid bit number 17 -> default case
    EXPECT_EQ(BIT16_Sign_Extend(0xABCD, 255), 0); // Invalid bit number 255 -> default case
}

TEST_F(Bit16Test, BIT16_Toggle_Bit) {
    std::uint16_t value = 0xAAAA;                  // 1010101010101010
    EXPECT_EQ(BIT16_Toggle_Bit(value, 0), 0xAAAB); // toggle bit 0
    EXPECT_EQ(BIT16_Toggle_Bit(value, 1), 0xAAA8); // toggle bit 1
}

TEST_F(Bit16Test, BIT16_Toggle_Bits) {
    std::uint16_t value = 0xAAAA;                      // 1010101010101010
    EXPECT_EQ(BIT16_Toggle_Bits(value, 0, 7), 0xAA55); // toggle bits 0-7
}

TEST_F(Bit16Test, BIT16_Toggle_nBits) {
    std::uint16_t value = 0xAAAA;                       // 1010101010101010
    EXPECT_EQ(BIT16_Toggle_nBits(value, 0, 8), 0xAA55); // toggle 8 bits from pos 0
}

// Test BIT16_Bitstring function
// Test BIT16_Bitstring function
TEST_F(Bit16Test, BIT16_Bitstring) {
    char bitstring[BIT16_STRING_SIZE];
    char *result;

    // Test standard bitstring (always with leading zeros)
    result = BIT16_Bitstring(0b1010101010101010, bitstring);
    EXPECT_STREQ(result, "1010101010101010");
    EXPECT_STREQ(bitstring, "1010101010101010");

    result = BIT16_Bitstring(0b0000000000000001, bitstring);
    EXPECT_STREQ(result, "0000000000000001");
    EXPECT_STREQ(bitstring, "0000000000000001");

    result = BIT16_Bitstring(0x0000, bitstring);
    EXPECT_STREQ(result, "0000000000000000");
    EXPECT_STREQ(bitstring, "0000000000000000");

    result = BIT16_Bitstring(0xFFFF, bitstring);
    EXPECT_STREQ(result, "1111111111111111");
    EXPECT_STREQ(bitstring, "1111111111111111");
}

// Test BIT16_BitstringBrief function
TEST_F(Bit16Test, BIT16_BitstringBrief) {
    char bitstring[BIT16_STRING_SIZE];
    char *result;

    // Test brief bitstring (no leading zeros)
    result = BIT16_BitstringBrief(0b1010101010101010, bitstring);
    EXPECT_STREQ(result, "1010101010101010");
    EXPECT_STREQ(bitstring, "1010101010101010");

    result = BIT16_BitstringBrief(0b0000000000000001, bitstring);
    EXPECT_STREQ(result, "1");
    EXPECT_STREQ(bitstring, "1");

    result = BIT16_BitstringBrief(0b0000000000001000, bitstring);
    EXPECT_STREQ(result, "1000");
    EXPECT_STREQ(bitstring, "1000");

    result = BIT16_BitstringBrief(0x0000, bitstring);
    EXPECT_STREQ(result, "0");
    EXPECT_STREQ(bitstring, "0");

    result = BIT16_BitstringBrief(0xFFFF, bitstring);
    EXPECT_STREQ(result, "1111111111111111");
    EXPECT_STREQ(bitstring, "1111111111111111");

    result = BIT16_BitstringBrief(0x8000, bitstring);
    EXPECT_STREQ(result, "1000000000000000");
    EXPECT_STREQ(bitstring, "1000000000000000");
}