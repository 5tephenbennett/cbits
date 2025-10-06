/**
 * @file bit8_gtest.cpp
 * @brief Google Test for 8-bit operations.
 */
#include <cstdint>
#include <gtest/gtest.h>

extern "C" {
#include "bit8_ops.h"
#include "bit_ops.h"
}

class Bit8Test : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(Bit8Test, BIT8_Set) {
    EXPECT_EQ(BIT8_Set(0), 1);   // 2^0 = 1
    EXPECT_EQ(BIT8_Set(1), 2);   // 2^1 = 2
    EXPECT_EQ(BIT8_Set(2), 4);   // 2^2 = 4
    EXPECT_EQ(BIT8_Set(3), 8);   // 2^3 = 8
    EXPECT_EQ(BIT8_Set(7), 128); // 2^7 = 128
}

TEST_F(Bit8Test, BIT8_Count_Bits_Set) {
    EXPECT_EQ(BIT8_Count_Bits_Set(0), 0);   // 00000000 -> 0 bits
    EXPECT_EQ(BIT8_Count_Bits_Set(1), 1);   // 00000001 -> 1 bit
    EXPECT_EQ(BIT8_Count_Bits_Set(3), 2);   // 00000011 -> 2 bits
    EXPECT_EQ(BIT8_Count_Bits_Set(7), 3);   // 00000111 -> 3 bits
    EXPECT_EQ(BIT8_Count_Bits_Set(15), 4);  // 00001111 -> 4 bits
    EXPECT_EQ(BIT8_Count_Bits_Set(255), 8); // 11111111 -> 8 bits
}

TEST_F(Bit8Test, BIT8_Get_Bit) {
    std::uint8_t value = 0b10101010; // 170

    EXPECT_EQ(BIT8_Get_Bit(value, 0), 0); // bit 0 is 0
    EXPECT_EQ(BIT8_Get_Bit(value, 1), 1); // bit 1 is 1
    EXPECT_EQ(BIT8_Get_Bit(value, 2), 0); // bit 2 is 0
    EXPECT_EQ(BIT8_Get_Bit(value, 3), 1); // bit 3 is 1
    EXPECT_EQ(BIT8_Get_Bit(value, 7), 1); // bit 7 is 1
}

TEST_F(Bit8Test, BIT8_Set_Bit) {
    std::uint8_t value = 0;

    value = BIT8_Set_Bit(value, 0);
    EXPECT_EQ(value, 1); // 00000001

    value = BIT8_Set_Bit(value, 7);
    EXPECT_EQ(value, 129); // 10000001

    value = BIT8_Set_Bit(value, 3);
    EXPECT_EQ(value, 137); // 10001001
}

TEST_F(Bit8Test, BIT8_Clear_Bit) {
    std::uint8_t value = 255; // 11111111

    value = BIT8_Clear_Bit(value, 0);
    EXPECT_EQ(value, 254); // 11111110

    value = BIT8_Clear_Bit(value, 7);
    EXPECT_EQ(value, 126); // 01111110

    value = BIT8_Clear_Bit(value, 3);
    EXPECT_EQ(value, 118); // 01110110
}

TEST_F(Bit8Test, BIT8_Bitmask) {
    EXPECT_EQ(BIT8_Bitmask(0, 3), 0x0F); // bits 0-3: 00001111
    EXPECT_EQ(BIT8_Bitmask(2, 5), 0x3C); // bits 2-5: 00111100
    EXPECT_EQ(BIT8_Bitmask(4, 7), 0xF0); // bits 4-7: 11110000
    EXPECT_EQ(BIT8_Bitmask(0, 7), 0xFF); // bits 0-7: 11111111
    EXPECT_EQ(BIT8_Bitmask(3, 3), 0x08); // bit 3 only: 00001000
}

TEST_F(Bit8Test, BIT8_Clear_Bits) {
    std::uint8_t value = 0xFF;                     // 11111111
    EXPECT_EQ(BIT8_Clear_Bits(value, 0, 3), 0xF0); // clear bits 0-3: 11110000
    EXPECT_EQ(BIT8_Clear_Bits(value, 2, 5), 0xC3); // clear bits 2-5: 11000011
    EXPECT_EQ(BIT8_Clear_Bits(value, 4, 7), 0x0F); // clear bits 4-7: 00001111
}

TEST_F(Bit8Test, BIT8_Clear_nBits) {
    std::uint8_t value = 0xFF;                      // 11111111
    EXPECT_EQ(BIT8_Clear_nBits(value, 0, 4), 0xF0); // clear 4 bits from pos 0: 11110000
    EXPECT_EQ(BIT8_Clear_nBits(value, 2, 3), 0xE3); // clear 3 bits from pos 2: 11100011
    EXPECT_EQ(BIT8_Clear_nBits(value, 4, 4), 0x0F); // clear 4 bits from pos 4: 00001111
}

TEST_F(Bit8Test, BIT8_Get_Bits) {
    std::uint8_t value = 0xAB;                   // 10101011
    EXPECT_EQ(BIT8_Get_Bits(value, 0, 3), 0x0B); // bits 0-3: 1011
    EXPECT_EQ(BIT8_Get_Bits(value, 2, 5), 0x0A); // bits 2-5: 1010
    EXPECT_EQ(BIT8_Get_Bits(value, 4, 7), 0x0A); // bits 4-7: 1010
}

TEST_F(Bit8Test, BIT8_Get_Bits_Signed) {
    std::int8_t value = -85;                          // 10101011 (same as 0xAB but signed)
    EXPECT_EQ(BIT8_Get_Bits_Signed(value, 0, 3), -5); // bits 0-3: 1011 (signed interpretation)
    EXPECT_EQ(BIT8_Get_Bits_Signed(value, 4, 7), -6); // bits 4-7: 1010 (signed)

    // Test error condition - end_bit < start_bit (covers return 0; path)
    EXPECT_EQ(BIT8_Get_Bits_Signed(value, 5, 2), 0); // Invalid: end_bit < start_bit -> 0
    EXPECT_EQ(BIT8_Get_Bits_Signed(value, 7, 0), 0); // Invalid: end_bit < start_bit -> 0
}

TEST_F(Bit8Test, BIT8_Get_First_Bit_Set) {
    EXPECT_EQ(BIT8_Get_First_Bit_Set(0x01), 0); // 00000001
    EXPECT_EQ(BIT8_Get_First_Bit_Set(0x02), 1); // 00000010
    EXPECT_EQ(BIT8_Get_First_Bit_Set(0x04), 2); // 00000100
    EXPECT_EQ(BIT8_Get_First_Bit_Set(0x80), 7); // 10000000
    EXPECT_EQ(BIT8_Get_First_Bit_Set(0x06), 1); // 00000110 (first set bit is bit 1)
}

TEST_F(Bit8Test, BIT8_Get_nBits) {
    std::uint8_t value = 0xAB;                    // 10101011
    EXPECT_EQ(BIT8_Get_nBits(value, 0, 4), 0x0B); // 4 bits from pos 0: 1011
    EXPECT_EQ(BIT8_Get_nBits(value, 2, 3), 0x02); // 3 bits from pos 2: 010
    EXPECT_EQ(BIT8_Get_nBits(value, 4, 4), 0x0A); // 4 bits from pos 4: 1010
}

TEST_F(Bit8Test, BIT8_Get_nBits_Signed) {
    std::int8_t value = -85;                           // 10101011
    EXPECT_EQ(BIT8_Get_nBits_Signed(value, 0, 4), -5); // 4 bits from pos 0: 1011 (signed)
    EXPECT_EQ(BIT8_Get_nBits_Signed(value, 4, 4), -6); // 4 bits from pos 4: 1010 (signed)
}

TEST_F(Bit8Test, BIT8_Get_Sign) {
    EXPECT_EQ(BIT8_Get_Sign(127), 1);   // positive: 01111111 -> +1
    EXPECT_EQ(BIT8_Get_Sign(-128), -1); // negative: 10000000 -> -1
    EXPECT_EQ(BIT8_Get_Sign(-1), -1);   // negative: 11111111 -> -1
    EXPECT_EQ(BIT8_Get_Sign(0), 1);     // zero: 00000000 -> +1 (non-negative)
}

TEST_F(Bit8Test, BIT8_Has_Zero_Byte) {
    EXPECT_EQ(BIT8_Has_Zero_Byte(0x00), 1); // has zero byte
    EXPECT_EQ(BIT8_Has_Zero_Byte(0xFF), 0); // no zero byte
    EXPECT_EQ(BIT8_Has_Zero_Byte(0x7F), 0); // no zero byte
}

TEST_F(Bit8Test, BIT8_Insert_Bit) {
    unsigned char value = 0x07; // 00000111
    // Insert bit 1 at position 0 - bit 0 is already 1, so no change
    EXPECT_EQ(BIT8_Insert_Bit(value, 1, 0), 0x07);
    // Insert bit 0 at position 4 - clears bit 4
    EXPECT_EQ(BIT8_Insert_Bit(value, 0, 4), 0x07); // 0x07 has bit 4 = 0 already
}

TEST_F(Bit8Test, BIT8_Insert_Bits) {
    std::uint8_t value = 0x0F;                            // 00001111
    EXPECT_EQ(BIT8_Insert_Bits(value, 0x0A, 4, 7), 0xAF); // insert 1010 at bits 4-7
}

TEST_F(Bit8Test, BIT8_Insert_nBits) {
    std::uint8_t value = 0x0F;                             // 00001111
    EXPECT_EQ(BIT8_Insert_nBits(value, 0x0A, 4, 4), 0xAF); // insert 4 bits (1010) at pos 4
}

TEST_F(Bit8Test, BIT8_isNeg) {
    EXPECT_EQ(BIT8_isNeg(127), 0);  // positive: 01111111
    EXPECT_EQ(BIT8_isNeg(-128), 1); // negative: 10000000
    EXPECT_EQ(BIT8_isNeg(-1), 1);   // negative: 11111111
    EXPECT_EQ(BIT8_isNeg(0), 0);    // zero: 00000000
}

TEST_F(Bit8Test, BIT8_LBitmask) {
    EXPECT_EQ(BIT8_LBitmask(0), 0xFF); // (-1 << 0) = 11111111
    EXPECT_EQ(BIT8_LBitmask(3), 0xF8); // (-1 << 3) = 11111000
    EXPECT_EQ(BIT8_LBitmask(7), 0x80); // (-1 << 7) = 10000000
}

TEST_F(Bit8Test, BIT8_Mask_Bit) {
    std::uint8_t value = 0xAB;                // 10101011
    EXPECT_EQ(BIT8_Mask_Bit(value, 0), 0x01); // mask bit 0: 00000001
    EXPECT_EQ(BIT8_Mask_Bit(value, 1), 0x02); // mask bit 1: 00000010
    EXPECT_EQ(BIT8_Mask_Bit(value, 2), 0x00); // mask bit 2: 00000000 (bit not set)
}

TEST_F(Bit8Test, BIT8_Mask_Bits) {
    std::uint8_t value = 0xAB;                    // 10101011
    EXPECT_EQ(BIT8_Mask_Bits(value, 0, 3), 0x0B); // mask bits 0-3: 00001011
    EXPECT_EQ(BIT8_Mask_Bits(value, 4, 7), 0xA0); // mask bits 4-7: 10100000
}

TEST_F(Bit8Test, BIT8_Mask_nBits) {
    std::uint8_t value = 0xAB;                     // 10101011
    EXPECT_EQ(BIT8_Mask_nBits(value, 0, 4), 0x0B); // mask 4 bits from pos 0: 00001011
    EXPECT_EQ(BIT8_Mask_nBits(value, 4, 4), 0xA0); // mask 4 bits from pos 4: 10100000
}

TEST_F(Bit8Test, BIT8_nBitmask) {
    EXPECT_EQ(BIT8_nBitmask(0, 4), 0x0F); // 4 bits from pos 0: 00001111
    EXPECT_EQ(BIT8_nBitmask(2, 3), 0x1C); // 3 bits from pos 2: 00011100
    EXPECT_EQ(BIT8_nBitmask(4, 4), 0xF0); // 4 bits from pos 4: 11110000
}

TEST_F(Bit8Test, BIT8_RBitmask) {
    EXPECT_EQ(BIT8_RBitmask(0), 0x01); // right mask from bit 0
    EXPECT_EQ(BIT8_RBitmask(3), 0x0F); // right mask from bit 3
    EXPECT_EQ(BIT8_RBitmask(7), 0xFF); // right mask from bit 7
}

TEST_F(Bit8Test, BIT8_Reverse) {
    EXPECT_EQ(BIT8_Reverse(0x01), 0x80); // 00000001 -> 10000000
    EXPECT_EQ(BIT8_Reverse(0x0F), 0xF0); // 00001111 -> 11110000
    EXPECT_EQ(BIT8_Reverse(0xAB), 0xD5); // 10101011 -> 11010101
}

TEST_F(Bit8Test, BIT8_Set_Bits) {
    std::uint8_t value = 0x00;                   // 00000000
    EXPECT_EQ(BIT8_Set_Bits(value, 0, 3), 0x0F); // set bits 0-3: 00001111
    EXPECT_EQ(BIT8_Set_Bits(value, 4, 7), 0xF0); // set bits 4-7: 11110000
}

TEST_F(Bit8Test, BIT8_Set_nBits) {
    std::uint8_t value = 0x00;                    // 00000000
    EXPECT_EQ(BIT8_Set_nBits(value, 0, 4), 0x0F); // set 4 bits from pos 0: 00001111
    EXPECT_EQ(BIT8_Set_nBits(value, 4, 4), 0xF0); // set 4 bits from pos 4: 11110000
}

TEST_F(Bit8Test, BIT8_Sign_Extend) {
    // Test sign extension for various bit widths covering more switch cases
    // Note: BIT8_Sign_Extend only handles cases 2-8 in switch statement, default returns 0

    // Test 1-bit values (no case in switch, returns 0)
    EXPECT_EQ(BIT8_Sign_Extend(0x1, 1), -1);
    EXPECT_EQ(BIT8_Sign_Extend(0x0, 1), 0);

    // Test 2-bit values (case 2)
    EXPECT_EQ(BIT8_Sign_Extend(0x3, 2), (int8_t)0xFF); // 11 -> -1
    EXPECT_EQ(BIT8_Sign_Extend(0x2, 2), (int8_t)0xFE); // 10 -> -2
    EXPECT_EQ(BIT8_Sign_Extend(0x1, 2), (int8_t)0x01); // 01 -> 1
    EXPECT_EQ(BIT8_Sign_Extend(0x0, 2), (int8_t)0x00); // 00 -> 0

    // Test 3-bit values (case 3)
    EXPECT_EQ(BIT8_Sign_Extend(0x7, 3), (int8_t)0xFF); // 111 -> -1
    EXPECT_EQ(BIT8_Sign_Extend(0x4, 3), (int8_t)0xFC); // 100 -> -4
    EXPECT_EQ(BIT8_Sign_Extend(0x3, 3), (int8_t)0x03); // 011 -> 3
    EXPECT_EQ(BIT8_Sign_Extend(0x0, 3), (int8_t)0x00); // 000 -> 0

    // Test 4-bit values (case 4)
    EXPECT_EQ(BIT8_Sign_Extend(0xF, 4), (int8_t)0xFF); // 1111 -> -1
    EXPECT_EQ(BIT8_Sign_Extend(0x8, 4), (int8_t)0xF8); // 1000 -> -8
    EXPECT_EQ(BIT8_Sign_Extend(0x7, 4), (int8_t)0x07); // 0111 -> 7
    EXPECT_EQ(BIT8_Sign_Extend(0x0, 4), (int8_t)0x00); // 0000 -> 0

    // Test 5-bit values (case 5)
    EXPECT_EQ(BIT8_Sign_Extend(0x1F, 5), (int8_t)0xFF); // 11111 -> -1
    EXPECT_EQ(BIT8_Sign_Extend(0x10, 5), (int8_t)0xF0); // 10000 -> -16
    EXPECT_EQ(BIT8_Sign_Extend(0x0F, 5), (int8_t)0x0F); // 01111 -> 15
    EXPECT_EQ(BIT8_Sign_Extend(0x00, 5), (int8_t)0x00); // 00000 -> 0

    // Test 6-bit values (case 6)
    EXPECT_EQ(BIT8_Sign_Extend(0x3F, 6), (int8_t)0xFF); // 111111 -> -1
    EXPECT_EQ(BIT8_Sign_Extend(0x20, 6), (int8_t)0xE0); // 100000 -> -32
    EXPECT_EQ(BIT8_Sign_Extend(0x1F, 6), (int8_t)0x1F); // 011111 -> 31
    EXPECT_EQ(BIT8_Sign_Extend(0x00, 6), (int8_t)0x00); // 000000 -> 0

    // Test 7-bit values (case 7) - original test cases
    EXPECT_EQ(BIT8_Sign_Extend(0x7F, 7), (int8_t)0xFF); // -1
    EXPECT_EQ(BIT8_Sign_Extend(0x40, 7), (int8_t)0xC0); // extends to 11000000
    EXPECT_EQ(BIT8_Sign_Extend(0x3F, 7), (int8_t)0x3F); // 0111111 -> 63
    EXPECT_EQ(BIT8_Sign_Extend(0x00, 7), (int8_t)0x00); // 0000000 -> 0

    // Test 8-bit values (case 8) - covers return var; path
    EXPECT_EQ(BIT8_Sign_Extend(0xFF, 8), (int8_t)0xFF); // 8-bit: 11111111 -> -1
    EXPECT_EQ(BIT8_Sign_Extend(0x80, 8), (int8_t)0x80); // 8-bit: 10000000 -> -128
    EXPECT_EQ(BIT8_Sign_Extend(0x7F, 8), (int8_t)0x7F); // 8-bit: 01111111 -> 127
    EXPECT_EQ(BIT8_Sign_Extend(0x00, 8), (int8_t)0x00); // 8-bit: 00000000 -> 0

    // Test invalid bit numbers (default case) - covers default: return 0; path
    EXPECT_EQ(BIT8_Sign_Extend(0xFF, 0), 0);   // Invalid bit number 0 -> default case
    EXPECT_EQ(BIT8_Sign_Extend(0x12, 9), 0);   // Invalid bit number 9 -> default case
    EXPECT_EQ(BIT8_Sign_Extend(0xAB, 255), 0); // Invalid bit number 255 -> default case
}

// Test BIT8_Bitstring function
TEST_F(Bit8Test, BIT8_Bitstring) {
    char bitstring[BIT8_STRING_SIZE];
    char *result;

    // Test standard bitstring (always with leading zeros)
    result = BIT8_Bitstring(0b10101010, bitstring);
    EXPECT_STREQ(result, "10101010");
    EXPECT_STREQ(bitstring, "10101010");

    result = BIT8_Bitstring(0b00000001, bitstring);
    EXPECT_STREQ(result, "00000001");
    EXPECT_STREQ(bitstring, "00000001");

    result = BIT8_Bitstring(0x00, bitstring);
    EXPECT_STREQ(result, "00000000");
    EXPECT_STREQ(bitstring, "00000000");

    result = BIT8_Bitstring(0xFF, bitstring);
    EXPECT_STREQ(result, "11111111");
    EXPECT_STREQ(bitstring, "11111111");
}

// Test BIT8_BitstringBrief function
TEST_F(Bit8Test, BIT8_BitstringBrief) {
    char bitstring[BIT8_STRING_SIZE];
    char *result;

    // Test brief bitstring (no leading zeros)
    result = BIT8_BitstringBrief(0b10101010, bitstring);
    EXPECT_STREQ(result, "10101010");
    EXPECT_STREQ(bitstring, "10101010");

    result = BIT8_BitstringBrief(0b00000001, bitstring);
    EXPECT_STREQ(result, "1");
    EXPECT_STREQ(bitstring, "1");

    result = BIT8_BitstringBrief(0b00000000, bitstring);
    EXPECT_STREQ(result, "0");
    EXPECT_STREQ(bitstring, "0");

    result = BIT8_BitstringBrief(0b00001111, bitstring);
    EXPECT_STREQ(result, "1111");
    EXPECT_STREQ(bitstring, "1111");

    result = BIT8_BitstringBrief(0b00000110, bitstring);
    EXPECT_STREQ(result, "110");
    EXPECT_STREQ(bitstring, "110");
}

TEST_F(Bit8Test, BIT8_Toggle_Bit) {
    std::uint8_t value = 0xAA;                  // 10101010
    EXPECT_EQ(BIT8_Toggle_Bit(value, 0), 0xAB); // toggle bit 0: 10101011
    EXPECT_EQ(BIT8_Toggle_Bit(value, 1), 0xA8); // toggle bit 1: 10101000
}

TEST_F(Bit8Test, BIT8_Toggle_Bits) {
    std::uint8_t value = 0xAA;                      // 10101010
    EXPECT_EQ(BIT8_Toggle_Bits(value, 0, 3), 0xA5); // toggle bits 0-3: 10100101
}

TEST_F(Bit8Test, BIT8_Toggle_nBits) {
    std::uint8_t value = 0xAA;                       // 10101010
    EXPECT_EQ(BIT8_Toggle_nBits(value, 0, 4), 0xA5); // toggle 4 bits from pos 0: 10100101
}
