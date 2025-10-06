/**
 * @file bit32_gtest.cpp
 * @brief Google Test for 32-bit operations.
 */
#include <cstdint>
#include <gtest/gtest.h>

extern "C" {
#include "bit32_ops.h"
#include "bit_ops.h"
}

// Test fixture for BIT32 operations
class Bit32Test : public ::testing::Test {
  protected:
    void SetUp() override {
        // Test setup
    }

    void TearDown() override {
        // Test cleanup
    }
};

// Test BIT32_Set function
TEST_F(Bit32Test, BIT32_Set) {
    EXPECT_EQ(BIT32_Set(0), 1);            // 2^0 = 1
    EXPECT_EQ(BIT32_Set(1), 2);            // 2^1 = 2
    EXPECT_EQ(BIT32_Set(16), 65536);       // 2^16 = 65536
    EXPECT_EQ(BIT32_Set(31), 2147483648U); // 2^31 = 2147483648
}

// Test BIT32_Count_Bits_Set function
TEST_F(Bit32Test, BIT32_Count_Bits_Set) {
    EXPECT_EQ(BIT32_Count_Bits_Set(0), 0);           // All zeros -> 0 bits
    EXPECT_EQ(BIT32_Count_Bits_Set(1), 1);           // 1 bit set
    EXPECT_EQ(BIT32_Count_Bits_Set(3), 2);           // 2 bits set
    EXPECT_EQ(BIT32_Count_Bits_Set(0xFF), 8);        // 8 bits set
    EXPECT_EQ(BIT32_Count_Bits_Set(0xFFFF), 16);     // 16 bits set
    EXPECT_EQ(BIT32_Count_Bits_Set(0xFFFFFFFF), 32); // All 32 bits set
}

// Test BIT32_Get_Bit function
TEST_F(Bit32Test, BIT32_Get_Bit) {
    uint32_t value = 0xAAAAAAAA; // 10101010101010101010101010101010

    EXPECT_EQ(BIT32_Get_Bit(value, 0), 0);  // bit 0 is 0
    EXPECT_EQ(BIT32_Get_Bit(value, 1), 1);  // bit 1 is 1
    EXPECT_EQ(BIT32_Get_Bit(value, 16), 0); // bit 16 is 0
    EXPECT_EQ(BIT32_Get_Bit(value, 17), 1); // bit 17 is 1
    EXPECT_EQ(BIT32_Get_Bit(value, 31), 1); // bit 31 is 1
}

// Test BIT32_Bitmask function
TEST_F(Bit32Test, BIT32_Bitmask) {
    EXPECT_EQ(BIT32_Bitmask(0, 7), 0xFF);         // bits 0-7
    EXPECT_EQ(BIT32_Bitmask(8, 15), 0xFF00);      // bits 8-15
    EXPECT_EQ(BIT32_Bitmask(16, 23), 0xFF0000);   // bits 16-23
    EXPECT_EQ(BIT32_Bitmask(24, 31), 0xFF000000); // bits 24-31
}

// Test BIT32_Get_Bits function
TEST_F(Bit32Test, BIT32_Get_Bits) {
    uint32_t value = 0x12345678;

    EXPECT_EQ(BIT32_Get_Bits(value, 0, 7), 0x78);   // bottom byte
    EXPECT_EQ(BIT32_Get_Bits(value, 8, 15), 0x56);  // second byte
    EXPECT_EQ(BIT32_Get_Bits(value, 16, 23), 0x34); // third byte
    EXPECT_EQ(BIT32_Get_Bits(value, 24, 31), 0x12); // top byte
}

// Test BIT32_Has_Zero_Byte function
TEST_F(Bit32Test, BIT32_Has_Zero_Byte) {
    EXPECT_TRUE(BIT32_Has_Zero_Byte(0x12000034));  // Second byte is zero
    EXPECT_TRUE(BIT32_Has_Zero_Byte(0x00123456));  // First byte is zero
    EXPECT_TRUE(BIT32_Has_Zero_Byte(0x12345600));  // Last byte is zero
    EXPECT_FALSE(BIT32_Has_Zero_Byte(0x12345678)); // No zero bytes
    EXPECT_FALSE(BIT32_Has_Zero_Byte(0xFFFFFFFF)); // All bytes 0xFF
}

TEST_F(Bit32Test, BIT32_Clear_Bit) {
    std::uint32_t value = 0xFFFFFFFF;                  // All bits set
    EXPECT_EQ(BIT32_Clear_Bit(value, 0), 0xFFFFFFFE);  // clear bit 0
    EXPECT_EQ(BIT32_Clear_Bit(value, 16), 0xFFFEFFFF); // clear bit 16
    EXPECT_EQ(BIT32_Clear_Bit(value, 31), 0x7FFFFFFF); // clear bit 31
}

TEST_F(Bit32Test, BIT32_Clear_Bits) {
    std::uint32_t value = 0xFFFFFFFF;                       // All bits set
    EXPECT_EQ(BIT32_Clear_Bits(value, 0, 7), 0xFFFFFF00);   // clear bits 0-7
    EXPECT_EQ(BIT32_Clear_Bits(value, 8, 15), 0xFFFF00FF);  // clear bits 8-15
    EXPECT_EQ(BIT32_Clear_Bits(value, 16, 23), 0xFF00FFFF); // clear bits 16-23
    EXPECT_EQ(BIT32_Clear_Bits(value, 24, 31), 0x00FFFFFF); // clear bits 24-31
}

TEST_F(Bit32Test, BIT32_Clear_nBits) {
    std::uint32_t value = 0xFFFFFFFF;                       // All bits set
    EXPECT_EQ(BIT32_Clear_nBits(value, 0, 8), 0xFFFFFF00);  // clear 8 bits from pos 0
    EXPECT_EQ(BIT32_Clear_nBits(value, 8, 8), 0xFFFF00FF);  // clear 8 bits from pos 8
    EXPECT_EQ(BIT32_Clear_nBits(value, 16, 8), 0xFF00FFFF); // clear 8 bits from pos 16
    EXPECT_EQ(BIT32_Clear_nBits(value, 24, 8), 0x00FFFFFF); // clear 8 bits from pos 24
}

TEST_F(Bit32Test, BIT32_Convert_Grey_Code) {
    EXPECT_EQ(BIT32_Convert_Grey_Code(0), 0); // 0 -> 0
    EXPECT_EQ(BIT32_Convert_Grey_Code(1), 1); // 1 -> 1
    EXPECT_EQ(BIT32_Convert_Grey_Code(3), 2); // 11 -> 10
    EXPECT_EQ(BIT32_Convert_Grey_Code(2), 3); // 10 -> 11
}

TEST_F(Bit32Test, BIT32_Get_Bits_Signed) {
    std::int32_t value = -1431655766;                        // 0xAAAAAAAA as signed
    EXPECT_EQ(BIT32_Get_Bits_Signed(value, 0, 15), -21846);  // bits 0-15: 0xAAAA (unsigned part)
    EXPECT_EQ(BIT32_Get_Bits_Signed(value, 16, 31), -21846); // bits 16-31: 0xAAAA (unsigned part)

    // Test error condition: end_bit < start_bit should return 0
    EXPECT_EQ(BIT32_Get_Bits_Signed(value, 15, 0), 0);  // Invalid: end_bit < start_bit
    EXPECT_EQ(BIT32_Get_Bits_Signed(value, 31, 16), 0); // Invalid: end_bit < start_bit
}

TEST_F(Bit32Test, BIT32_Get_First_Bit_Set) {
    EXPECT_EQ(BIT32_Get_First_Bit_Set(0x00000001), 0);  // bit 0
    EXPECT_EQ(BIT32_Get_First_Bit_Set(0x00000002), 1);  // bit 1
    EXPECT_EQ(BIT32_Get_First_Bit_Set(0x00010000), 16); // bit 16
    EXPECT_EQ(BIT32_Get_First_Bit_Set(0x80000000), 31); // bit 31
    EXPECT_EQ(BIT32_Get_First_Bit_Set(0x00000006), 1);  // first set bit is bit 1
}

TEST_F(Bit32Test, BIT32_Get_nBits) {
    std::uint32_t value = 0x12345678;
    EXPECT_EQ(BIT32_Get_nBits(value, 0, 8), 0x78);  // 8 bits from pos 0
    EXPECT_EQ(BIT32_Get_nBits(value, 8, 8), 0x56);  // 8 bits from pos 8
    EXPECT_EQ(BIT32_Get_nBits(value, 16, 8), 0x34); // 8 bits from pos 16
    EXPECT_EQ(BIT32_Get_nBits(value, 24, 8), 0x12); // 8 bits from pos 24
}

TEST_F(Bit32Test, BIT32_Get_nBits_Signed) {
    std::int32_t value = -1431655766; // 0xAAAAAAAA as signed
    // 0xFFFFAAAA == -21846
    EXPECT_EQ(BIT32_Get_nBits_Signed(value, 0, 16),
              -21846); // 16 bits from pos 0: 0xAAAA (unsigned part)
    EXPECT_EQ(BIT32_Get_nBits_Signed(value, 16, 16),
              -21846); // 16 bits from pos 16: 0xAAAA (unsigned part)
}

TEST_F(Bit32Test, BIT32_Get_Sign) {
    EXPECT_EQ(BIT32_Get_Sign(2147483647), 1);   // positive: 0x7FFFFFFF -> +1
    EXPECT_EQ(BIT32_Get_Sign(-2147483648), -1); // negative: 0x80000000 -> -1
    EXPECT_EQ(BIT32_Get_Sign(-1), -1);          // negative: 0xFFFFFFFF -> -1
    EXPECT_EQ(BIT32_Get_Sign(0), 1);            // zero: 0x00000000 -> +1
}

TEST_F(Bit32Test, BIT32_Insert_Bit) {
    std::uint32_t value = 0x0000FFFF;                      // 0000000000000000 1111111111111111
    EXPECT_EQ(BIT32_Insert_Bit(value, 1, 16), 0x0001FFFF); // insert 1 at pos 16
    EXPECT_EQ(BIT32_Insert_Bit(value, 0, 16), 0x0000FFFF); // insert 0 at pos 16
}

TEST_F(Bit32Test, BIT32_Insert_Bits) {
    std::uint32_t value = 0x0000FFFF; // 0000000000000000 1111111111111111
    EXPECT_EQ(BIT32_Insert_Bits(value, 0xABCD, 16, 31), 0xABCDFFFF); // insert 0xABCD at bits 16-31
}

TEST_F(Bit32Test, BIT32_Insert_nBits) {
    std::uint32_t value = 0x0000FFFF; // 0000000000000000 1111111111111111
    EXPECT_EQ(BIT32_Insert_nBits(value, 0xABCD, 16, 16), 0xABCDFFFF); // insert 16 bits at pos 16
}

TEST_F(Bit32Test, BIT32_isNeg) {
    EXPECT_EQ(BIT32_isNeg(2147483647), 0);  // positive: 0x7FFFFFFF
    EXPECT_EQ(BIT32_isNeg(-2147483648), 1); // negative: 0x80000000
    EXPECT_EQ(BIT32_isNeg(-1), 1);          // negative: 0xFFFFFFFF
    EXPECT_EQ(BIT32_isNeg(0), 0);           // zero: 0x00000000
}

TEST_F(Bit32Test, BIT32_LBitmask) {
    EXPECT_EQ(BIT32_LBitmask(0), 0xFFFFFFFF);  // (-1 << 0) = all bits
    EXPECT_EQ(BIT32_LBitmask(7), 0xFFFFFF80);  // (-1 << 7) = bits 7-31
    EXPECT_EQ(BIT32_LBitmask(15), 0xFFFF8000); // (-1 << 15) = bits 15-31
    EXPECT_EQ(BIT32_LBitmask(31), 0x80000000); // (-1 << 31) = bit 31 only
}

TEST_F(Bit32Test, BIT32_Mask_Bit) {
    std::uint32_t value = 0xAAAAAAAA;                 // 10101010101010101010101010101010
    EXPECT_EQ(BIT32_Mask_Bit(value, 0), 0x00000000);  // mask bit 0: bit not set
    EXPECT_EQ(BIT32_Mask_Bit(value, 1), 0x00000002);  // mask bit 1: bit is set
    EXPECT_EQ(BIT32_Mask_Bit(value, 16), 0x00000000); // mask bit 16: bit not set
    EXPECT_EQ(BIT32_Mask_Bit(value, 17), 0x00020000); // mask bit 17: bit is set
}

TEST_F(Bit32Test, BIT32_Mask_Bits) {
    std::uint32_t value = 0x12345678;
    EXPECT_EQ(BIT32_Mask_Bits(value, 0, 7), 0x00000078);   // mask bits 0-7
    EXPECT_EQ(BIT32_Mask_Bits(value, 8, 15), 0x00005600);  // mask bits 8-15
    EXPECT_EQ(BIT32_Mask_Bits(value, 16, 23), 0x00340000); // mask bits 16-23
    EXPECT_EQ(BIT32_Mask_Bits(value, 24, 31), 0x12000000); // mask bits 24-31
}

TEST_F(Bit32Test, BIT32_Mask_nBits) {
    std::uint32_t value = 0x12345678;
    EXPECT_EQ(BIT32_Mask_nBits(value, 0, 8), 0x00000078);  // mask 8 bits from pos 0
    EXPECT_EQ(BIT32_Mask_nBits(value, 8, 8), 0x00005600);  // mask 8 bits from pos 8
    EXPECT_EQ(BIT32_Mask_nBits(value, 16, 8), 0x00340000); // mask 8 bits from pos 16
    EXPECT_EQ(BIT32_Mask_nBits(value, 24, 8), 0x12000000); // mask 8 bits from pos 24
}

TEST_F(Bit32Test, BIT32_nBitmask) {
    EXPECT_EQ(BIT32_nBitmask(0, 8), 0x000000FF);  // 8 bits from pos 0
    EXPECT_EQ(BIT32_nBitmask(8, 8), 0x0000FF00);  // 8 bits from pos 8
    EXPECT_EQ(BIT32_nBitmask(16, 8), 0x00FF0000); // 8 bits from pos 16
    EXPECT_EQ(BIT32_nBitmask(24, 8), 0xFF000000); // 8 bits from pos 24

    // Test error conditions that return 0
    EXPECT_EQ(BIT32_nBitmask(32, 1), 0); // start_bit > BIT32_TOP_BIT (31)
    EXPECT_EQ(BIT32_nBitmask(33, 8), 0); // start_bit > BIT32_TOP_BIT (31)
    EXPECT_EQ(BIT32_nBitmask(0, 0), 0);  // bit_num == 0
    EXPECT_EQ(BIT32_nBitmask(8, 0), 0);  // bit_num == 0
    EXPECT_EQ(BIT32_nBitmask(30, 3), 0); // start_bit + bit_num (33) > BIT32_BITS (32)
    EXPECT_EQ(BIT32_nBitmask(25, 8), 0); // start_bit + bit_num (33) > BIT32_BITS (32)
}

TEST_F(Bit32Test, BIT32_RBitmask) {
    EXPECT_EQ(BIT32_RBitmask(0), 0x00000001);  // right mask from bit 0
    EXPECT_EQ(BIT32_RBitmask(7), 0x000000FF);  // right mask from bit 7
    EXPECT_EQ(BIT32_RBitmask(15), 0x0000FFFF); // right mask from bit 15
    EXPECT_EQ(BIT32_RBitmask(31), 0xFFFFFFFF); // right mask from bit 31
}

TEST_F(Bit32Test, BIT32_Reverse) {
    EXPECT_EQ(BIT32_Reverse(0x00000001), 0x80000000); // reverse bit pattern
    EXPECT_EQ(BIT32_Reverse(0x000000FF), 0xFF000000); // reverse byte pattern
    EXPECT_EQ(BIT32_Reverse(0x12345678), 0x1E6A2C48); // bit reversal of 0x12345678
}

TEST_F(Bit32Test, BIT32_Set_Bit) {
    std::uint32_t value = 0x00000000;
    EXPECT_EQ(BIT32_Set_Bit(value, 0), 0x00000001);  // set bit 0
    EXPECT_EQ(BIT32_Set_Bit(value, 16), 0x00010000); // set bit 16
    EXPECT_EQ(BIT32_Set_Bit(value, 31), 0x80000000); // set bit 31
}

TEST_F(Bit32Test, BIT32_Set_Bits) {
    std::uint32_t value = 0x00000000;
    EXPECT_EQ(BIT32_Set_Bits(value, 0, 7), 0x000000FF);   // set bits 0-7
    EXPECT_EQ(BIT32_Set_Bits(value, 8, 15), 0x0000FF00);  // set bits 8-15
    EXPECT_EQ(BIT32_Set_Bits(value, 16, 23), 0x00FF0000); // set bits 16-23
    EXPECT_EQ(BIT32_Set_Bits(value, 24, 31), 0xFF000000); // set bits 24-31
}

TEST_F(Bit32Test, BIT32_Set_nBits) {
    std::uint32_t value = 0x00000000;
    EXPECT_EQ(BIT32_Set_nBits(value, 0, 8), 0x000000FF);  // set 8 bits from pos 0
    EXPECT_EQ(BIT32_Set_nBits(value, 8, 8), 0x0000FF00);  // set 8 bits from pos 8
    EXPECT_EQ(BIT32_Set_nBits(value, 16, 8), 0x00FF0000); // set 8 bits from pos 16
    EXPECT_EQ(BIT32_Set_nBits(value, 24, 8), 0xFF000000); // set 8 bits from pos 24
}

TEST_F(Bit32Test, BIT32_Sign_Extend) {
    // Test more cases to improve coverage of switch statement
    EXPECT_EQ(BIT32_Sign_Extend(0x1, 1), -1); // 1 -> -1 (sign bit set)
    EXPECT_EQ(BIT32_Sign_Extend(0x0, 1), 0);  // 0 -> 0 (sign bit clear)

    // 2-bit cases
    EXPECT_EQ(BIT32_Sign_Extend(0x3, 2), -1); // 11 -> -1
    EXPECT_EQ(BIT32_Sign_Extend(0x2, 2), -2); // 10 -> -2
    EXPECT_EQ(BIT32_Sign_Extend(0x1, 2), 1);  // 01 -> 1
    EXPECT_EQ(BIT32_Sign_Extend(0x0, 2), 0);  // 00 -> 0

    // 4-bit cases
    EXPECT_EQ(BIT32_Sign_Extend(0xF, 4), -1); // 1111 -> -1
    EXPECT_EQ(BIT32_Sign_Extend(0x8, 4), -8); // 1000 -> -8
    EXPECT_EQ(BIT32_Sign_Extend(0x7, 4), 7);  // 0111 -> 7
    EXPECT_EQ(BIT32_Sign_Extend(0x0, 4), 0);  // 0000 -> 0

    // 8-bit cases
    EXPECT_EQ(BIT32_Sign_Extend(0xFF, 8), -1);   // 11111111 -> -1
    EXPECT_EQ(BIT32_Sign_Extend(0x80, 8), -128); // 10000000 -> -128
    EXPECT_EQ(BIT32_Sign_Extend(0x7F, 8), 127);  // 01111111 -> 127
    EXPECT_EQ(BIT32_Sign_Extend(0x01, 8), 1);    // 00000001 -> 1

    // 16-bit cases
    EXPECT_EQ(BIT32_Sign_Extend(0xFFFF, 16), -1);     // All bits set
    EXPECT_EQ(BIT32_Sign_Extend(0x8000, 16), -32768); // Sign bit set
    EXPECT_EQ(BIT32_Sign_Extend(0x7FFF, 16), 32767);  // Max positive
    EXPECT_EQ(BIT32_Sign_Extend(0x0001, 16), 1);      // Small positive

    // Additional switch cases to improve coverage
    EXPECT_EQ(BIT32_Sign_Extend(0x7, 3), -1); // case 3: 111 -> -1
    EXPECT_EQ(BIT32_Sign_Extend(0x3, 3), 3);  // case 3: 011 -> 3

    EXPECT_EQ(BIT32_Sign_Extend(0x1F, 5), -1); // case 5: 11111 -> -1
    EXPECT_EQ(BIT32_Sign_Extend(0xF, 5), 15);  // case 5: 01111 -> 15

    EXPECT_EQ(BIT32_Sign_Extend(0x3F, 6), -1); // case 6: 111111 -> -1
    EXPECT_EQ(BIT32_Sign_Extend(0x1F, 6), 31); // case 6: 011111 -> 31

    EXPECT_EQ(BIT32_Sign_Extend(0x7F, 7), -1); // case 7: 1111111 -> -1
    EXPECT_EQ(BIT32_Sign_Extend(0x3F, 7), 63); // case 7: 0111111 -> 63

    // Add more cases to cover higher switch cases
    EXPECT_EQ(BIT32_Sign_Extend(0x3FF, 10), -1);  // case 10: all bits set -> -1
    EXPECT_EQ(BIT32_Sign_Extend(0x1FF, 10), 511); // case 10: 0111111111 -> 511

    EXPECT_EQ(BIT32_Sign_Extend(0x7FF, 11), -1);   // case 11: all bits set -> -1
    EXPECT_EQ(BIT32_Sign_Extend(0x3FF, 11), 1023); // case 11: 01111111111 -> 1023

    EXPECT_EQ(BIT32_Sign_Extend(0xFFF, 12), -1);   // case 12: all bits set -> -1
    EXPECT_EQ(BIT32_Sign_Extend(0x7FF, 12), 2047); // case 12: 011111111111 -> 2047

    EXPECT_EQ(BIT32_Sign_Extend(0x3FFF, 14), -1);   // case 14: all bits set -> -1
    EXPECT_EQ(BIT32_Sign_Extend(0x1FFF, 14), 8191); // case 14: 01111111111111 -> 8191

    EXPECT_EQ(BIT32_Sign_Extend(0x7FFF, 15), -1);    // case 15: all bits set -> -1
    EXPECT_EQ(BIT32_Sign_Extend(0x3FFF, 15), 16383); // case 15: 011111111111111 -> 16383

    EXPECT_EQ(BIT32_Sign_Extend(0x1FFFF, 17), -1);   // case 17: all bits set -> -1
    EXPECT_EQ(BIT32_Sign_Extend(0xFFFF, 17), 65535); // case 17: 01111111111111111 -> 65535

    EXPECT_EQ(BIT32_Sign_Extend(0x3FFFF, 18), -1);     // case 18: all bits set -> -1
    EXPECT_EQ(BIT32_Sign_Extend(0x1FFFF, 18), 131071); // case 18: 011111111111111111 -> 131071

    // Test 31-bit values (case 31) - these use SIGNED_SHIFTS path, not switch statement
    EXPECT_EQ(BIT32_Sign_Extend(0x7FFFFFFF, 31), -1); // 31 bits with sign bit set -> -1
    EXPECT_EQ(BIT32_Sign_Extend(0x3FFFFFFF, 31),
              1073741823); // 31 bits without sign bit -> positive

    // Test case 32 (full width) - should return var directly
    EXPECT_EQ(BIT32_Sign_Extend(0xFFFFFFFF, 32), 0xFFFFFFFF); // Full width value
    EXPECT_EQ(BIT32_Sign_Extend(0x12345678, 32), 0x12345678); // Another full width value
}

TEST_F(Bit32Test, BIT32_Toggle_Bit) {
    std::uint32_t value = 0xAAAAAAAA;                  // 10101010101010101010101010101010
    EXPECT_EQ(BIT32_Toggle_Bit(value, 0), 0xAAAAAAAB); // toggle bit 0
    EXPECT_EQ(BIT32_Toggle_Bit(value, 1), 0xAAAAAAA8); // toggle bit 1
}

TEST_F(Bit32Test, BIT32_Toggle_Bits) {
    std::uint32_t value = 0xAAAAAAAA;                      // 10101010101010101010101010101010
    EXPECT_EQ(BIT32_Toggle_Bits(value, 0, 7), 0xAAAAAA55); // toggle bits 0-7
}

TEST_F(Bit32Test, BIT32_Toggle_nBits) {
    std::uint32_t value = 0xAAAAAAAA;                       // 10101010101010101010101010101010
    EXPECT_EQ(BIT32_Toggle_nBits(value, 0, 8), 0xAAAAAA55); // toggle 8 bits from pos 0
}

TEST_F(Bit32Test, BIT32_Bitstring) {
    char bitstring[BIT32_STRING_SIZE]; // Buffer for 32 bits + null terminator

    // Test with leading zeros (full 32-bit representation)
    char *result = BIT32_Bitstring(6, bitstring);
    EXPECT_STREQ(result, "00000000000000000000000000000110"); // Example from documentation
    EXPECT_EQ(result, bitstring);                             // Should return the same buffer

    // Test without leading zeros (minimal representation)
    result = BIT32_BitstringBrief(7, bitstring);
    EXPECT_STREQ(result, "111"); // Example from documentation

    // Test zero value
    result = BIT32_Bitstring(0, bitstring);
    EXPECT_STREQ(result, "00000000000000000000000000000000"); // All zeros with leading zeros

    result = BIT32_BitstringBrief(0, bitstring);
    EXPECT_STREQ(result, "0"); // Single zero without leading zeros

    // Test maximum value (all bits set)
    result = BIT32_Bitstring(0xFFFFFFFF, bitstring);
    EXPECT_STREQ(result, "11111111111111111111111111111111"); // All ones

    result = BIT32_BitstringBrief(0xFFFFFFFF, bitstring);
    EXPECT_STREQ(result, "11111111111111111111111111111111"); // Same, no leading zeros to remove

    // Test single bit values
    result = BIT32_Bitstring(1, bitstring);
    EXPECT_STREQ(result, "00000000000000000000000000000001");

    result = BIT32_BitstringBrief(1, bitstring);
    EXPECT_STREQ(result, "1");

    // Test high bit set
    result = BIT32_Bitstring(0x80000000, bitstring);
    EXPECT_STREQ(result, "10000000000000000000000000000000");

    result = BIT32_BitstringBrief(0x80000000, bitstring);
    EXPECT_STREQ(result, "10000000000000000000000000000000"); // No leading zeros to remove

    // Test pattern value
    result = BIT32_BitstringBrief(0xAAAAAAAA, bitstring);
    EXPECT_STREQ(result, "10101010101010101010101010101010");
}