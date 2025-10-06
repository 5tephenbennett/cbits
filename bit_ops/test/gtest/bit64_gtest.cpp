/**
 * @file bit64_gtest.cpp
 * @brief Google Test for 64-bit operations.
 */
#include <cstdint>
#include <gtest/gtest.h>

extern "C" {
#include "bit64_ops.h"
#include "bit_ops.h"
}

// Test fixture for BIT64 operations
class Bit64Test : public ::testing::Test {
  protected:
    void SetUp() override {
        // Test setup
    }

    void TearDown() override {
        // Test cleanup
    }
};

// Test BIT64_Set function
TEST_F(Bit64Test, BIT64_Set) {
    EXPECT_EQ(BIT64_Set(0), 1ULL);                    // 2^0 = 1
    EXPECT_EQ(BIT64_Set(1), 2ULL);                    // 2^1 = 2
    EXPECT_EQ(BIT64_Set(32), 4294967296ULL);          // 2^32
    EXPECT_EQ(BIT64_Set(63), 9223372036854775808ULL); // 2^63
}

// Test BIT64_Count_Bits_Set function
TEST_F(Bit64Test, BIT64_Count_Bits_Set) {
    EXPECT_EQ(BIT64_Count_Bits_Set(0), 0);                      // All zeros -> 0 bits
    EXPECT_EQ(BIT64_Count_Bits_Set(1), 1);                      // 1 bit set
    EXPECT_EQ(BIT64_Count_Bits_Set(3), 2);                      // 2 bits set
    EXPECT_EQ(BIT64_Count_Bits_Set(0xFF), 8);                   // 8 bits set
    EXPECT_EQ(BIT64_Count_Bits_Set(0xFFFF), 16);                // 16 bits set
    EXPECT_EQ(BIT64_Count_Bits_Set(0xFFFFFFFF), 32);            // 32 bits set
    EXPECT_EQ(BIT64_Count_Bits_Set(0xFFFFFFFFFFFFFFFFULL), 64); // All 64 bits set
}

// Test BIT64_Get_Bit function
TEST_F(Bit64Test, BIT64_Get_Bit) {
    uint64_t value = 0xAAAAAAAAAAAAAAAAULL; // Alternating pattern

    EXPECT_EQ(BIT64_Get_Bit(value, 0), 0);  // bit 0 is 0
    EXPECT_EQ(BIT64_Get_Bit(value, 1), 1);  // bit 1 is 1
    EXPECT_EQ(BIT64_Get_Bit(value, 32), 0); // bit 32 is 0
    EXPECT_EQ(BIT64_Get_Bit(value, 33), 1); // bit 33 is 1
    EXPECT_EQ(BIT64_Get_Bit(value, 63), 1); // bit 63 is 1
}

// Test BIT64_Bitmask function
TEST_F(Bit64Test, BIT64_Bitmask) {
    EXPECT_EQ(BIT64_Bitmask(0, 7), 0xFFULL);                 // bits 0-7
    EXPECT_EQ(BIT64_Bitmask(8, 15), 0xFF00ULL);              // bits 8-15
    EXPECT_EQ(BIT64_Bitmask(32, 39), 0xFF00000000ULL);       // bits 32-39
    EXPECT_EQ(BIT64_Bitmask(56, 63), 0xFF00000000000000ULL); // bits 56-63
}

// Test BIT64_Get_Bits function
TEST_F(Bit64Test, BIT64_Get_Bits) {
    uint64_t value = 0x123456789ABCDEF0ULL;

    EXPECT_EQ(BIT64_Get_Bits(value, 0, 7), 0xF0ULL);   // bottom byte
    EXPECT_EQ(BIT64_Get_Bits(value, 8, 15), 0xDEULL);  // second byte
    EXPECT_EQ(BIT64_Get_Bits(value, 32, 39), 0x78ULL); // fifth byte
    EXPECT_EQ(BIT64_Get_Bits(value, 56, 63), 0x12ULL); // top byte
}

// Test BIT64_Has_Zero_Byte function
TEST_F(Bit64Test, BIT64_Has_Zero_Byte) {
    EXPECT_TRUE(BIT64_Has_Zero_Byte(0x1200345678ABCDEFULL));  // Second byte is zero
    EXPECT_TRUE(BIT64_Has_Zero_Byte(0x0023456789ABCDEFULL));  // First byte is zero
    EXPECT_TRUE(BIT64_Has_Zero_Byte(0x123456789ABCDE00ULL));  // Last byte is zero
    EXPECT_FALSE(BIT64_Has_Zero_Byte(0x123456789ABCDEFULL));  // No zero bytes
    EXPECT_FALSE(BIT64_Has_Zero_Byte(0xFFFFFFFFFFFFFFFFULL)); // All bytes 0xFF
}

// Test BIT64_isNeg function - optimized version to avoid long runtime
TEST_F(Bit64Test, BIT64_isNeg) {
    // Test some specific values instead of a massive loop
    EXPECT_FALSE(BIT64_isNeg(0));                             // Zero is not negative
    EXPECT_FALSE(BIT64_isNeg(1));                             // Positive numbers
    EXPECT_FALSE(BIT64_isNeg(0x7FFFFFFFFFFFFFFFLL));          // Max positive int64_t
    EXPECT_TRUE(BIT64_isNeg(-1));                             // -1 is negative
    EXPECT_TRUE(BIT64_isNeg(-42));                            // Negative numbers
    EXPECT_TRUE(BIT64_isNeg((int64_t)0x8000000000000000ULL)); // Min negative int64_t
}

TEST_F(Bit64Test, BIT64_Clear_Bit) {
    std::uint64_t value = 0xFFFFFFFFFFFFFFFFULL;                  // All bits set
    EXPECT_EQ(BIT64_Clear_Bit(value, 0), 0xFFFFFFFFFFFFFFFEULL);  // clear bit 0
    EXPECT_EQ(BIT64_Clear_Bit(value, 32), 0xFFFFFFFEFFFFFFFFULL); // clear bit 32
    EXPECT_EQ(BIT64_Clear_Bit(value, 63), 0x7FFFFFFFFFFFFFFFULL); // clear bit 63
}

TEST_F(Bit64Test, BIT64_Clear_Bits) {
    std::uint64_t value = 0xFFFFFFFFFFFFFFFFULL;                       // All bits set
    EXPECT_EQ(BIT64_Clear_Bits(value, 0, 7), 0xFFFFFFFFFFFFFF00ULL);   // clear bits 0-7
    EXPECT_EQ(BIT64_Clear_Bits(value, 8, 15), 0xFFFFFFFFFFFF00FFULL);  // clear bits 8-15
    EXPECT_EQ(BIT64_Clear_Bits(value, 32, 39), 0xFFFFFF00FFFFFFFFULL); // clear bits 32-39
    EXPECT_EQ(BIT64_Clear_Bits(value, 56, 63), 0x00FFFFFFFFFFFFFFULL); // clear bits 56-63
}

TEST_F(Bit64Test, BIT64_Clear_nBits) {
    std::uint64_t value = 0xFFFFFFFFFFFFFFFFULL;                       // All bits set
    EXPECT_EQ(BIT64_Clear_nBits(value, 0, 8), 0xFFFFFFFFFFFFFF00ULL);  // clear 8 bits from pos 0
    EXPECT_EQ(BIT64_Clear_nBits(value, 8, 8), 0xFFFFFFFFFFFF00FFULL);  // clear 8 bits from pos 8
    EXPECT_EQ(BIT64_Clear_nBits(value, 32, 8), 0xFFFFFF00FFFFFFFFULL); // clear 8 bits from pos 32
    EXPECT_EQ(BIT64_Clear_nBits(value, 56, 8), 0x00FFFFFFFFFFFFFFULL); // clear 8 bits from pos 56
}

TEST_F(Bit64Test, BIT64_Get_Bits_Signed) {
    std::int64_t value = -6148914691236517206LL;                   // 0xAAAAAAAAAAAAAAAA as signed
    EXPECT_EQ(BIT64_Get_Bits_Signed(value, 0, 31), -1431655766LL); // bits 0-31: 0xAAAAAAAA (signed)
    EXPECT_EQ(BIT64_Get_Bits_Signed(value, 32, 63),
              -1431655766LL); // bits 32-63: 0xAAAAAAAA (signed)

    // Test error condition - end_bit < start_bit (covers return 0; path)
    EXPECT_EQ(BIT64_Get_Bits_Signed(value, 10, 5), 0); // Invalid: end_bit < start_bit -> 0
    EXPECT_EQ(BIT64_Get_Bits_Signed(value, 63, 0), 0); // Invalid: end_bit < start_bit -> 0
}

TEST_F(Bit64Test, BIT64_Get_First_Bit_Set) {
    EXPECT_EQ(BIT64_Get_First_Bit_Set(0x0000000000000001ULL), 0);  // bit 0
    EXPECT_EQ(BIT64_Get_First_Bit_Set(0x0000000000000002ULL), 1);  // bit 1
    EXPECT_EQ(BIT64_Get_First_Bit_Set(0x0000000100000000ULL), 32); // bit 32
    EXPECT_EQ(BIT64_Get_First_Bit_Set(0x8000000000000000ULL), 63); // bit 63
    EXPECT_EQ(BIT64_Get_First_Bit_Set(0x0000000000000006ULL), 1);  // first set bit is bit 1
}

TEST_F(Bit64Test, BIT64_Get_nBits) {
    std::uint64_t value = 0x123456789ABCDEF0ULL;
    EXPECT_EQ(BIT64_Get_nBits(value, 0, 8), 0xF0ULL);  // 8 bits from pos 0
    EXPECT_EQ(BIT64_Get_nBits(value, 8, 8), 0xDEULL);  // 8 bits from pos 8
    EXPECT_EQ(BIT64_Get_nBits(value, 32, 8), 0x78ULL); // 8 bits from pos 32
    EXPECT_EQ(BIT64_Get_nBits(value, 56, 8), 0x12ULL); // 8 bits from pos 56
}

TEST_F(Bit64Test, BIT64_Get_nBits_Signed) {
    std::int64_t value = -6148914691236517206LL; // 0xAAAAAAAAAAAAAAAA as signed
    EXPECT_EQ(BIT64_Get_nBits_Signed(value, 0, 32),
              -1431655766LL); // 32 bits from pos 0: 0xAAAAAAAA (signed)
    EXPECT_EQ(BIT64_Get_nBits_Signed(value, 32, 32),
              -1431655766LL); // 32 bits from pos 32: 0xAAAAAAAA (signed)
}

TEST_F(Bit64Test, BIT64_Get_Sign) {
    EXPECT_EQ(BIT64_Get_Sign(9223372036854775807LL), 1); // positive: 0x7FFFFFFFFFFFFFFF -> +1
    EXPECT_EQ(BIT64_Get_Sign(static_cast<int64_t>(0x8000000000000000ULL)),
              -1);                       // negative: 0x8000000000000000 -> -1
    EXPECT_EQ(BIT64_Get_Sign(-1LL), -1); // negative: 0xFFFFFFFFFFFFFFFF -> -1
    EXPECT_EQ(BIT64_Get_Sign(0LL), 1);   // zero: 0x0000000000000000 -> +1
}

TEST_F(Bit64Test, BIT64_Insert_Bit) {
    uint64_t value = 0xFFFFFFFFULL; // 32 bits set
    // Insert bit 1 at position 32
    EXPECT_EQ(BIT64_Insert_Bit(value, 1, 32), 0x1FFFFFFFFULL);
    // Insert bit 0 at position 16 - clears bit 16
    EXPECT_EQ(BIT64_Insert_Bit(value, 0, 16), 0xFFFEFFFFULL);
}

TEST_F(Bit64Test, BIT64_Insert_Bits) {
    std::uint64_t value = 0x00000000FFFFFFFFULL; // Lower 32 bits set
    EXPECT_EQ(BIT64_Insert_Bits(value, 0xABCDEF12ULL, 32, 63),
              0xABCDEF12FFFFFFFFULL); // insert at bits 32-63
}

TEST_F(Bit64Test, BIT64_Insert_nBits) {
    std::uint64_t value = 0x00000000FFFFFFFFULL; // Lower 32 bits set
    EXPECT_EQ(BIT64_Insert_nBits(value, 0xABCDEF12ULL, 32, 32),
              0xABCDEF12FFFFFFFFULL); // insert 32 bits at pos 32
}

TEST_F(Bit64Test, BIT64_LBitmask) {
    EXPECT_EQ(BIT64_LBitmask(0), 0xFFFFFFFFFFFFFFFFULL);  // (-1 << 0) = all bits
    EXPECT_EQ(BIT64_LBitmask(7), 0xFFFFFFFFFFFFFF80ULL);  // (-1 << 7) = bits 7-63
    EXPECT_EQ(BIT64_LBitmask(31), 0xFFFFFFFF80000000ULL); // (-1 << 31) = bits 31-63
    EXPECT_EQ(BIT64_LBitmask(63), 0x8000000000000000ULL); // (-1 << 63) = bit 63 only

    // Test error conditions - invalid bit positions > 63
    EXPECT_EQ(BIT64_LBitmask(64), 0x0000000000000000ULL);  // Invalid bit position -> 0
    EXPECT_EQ(BIT64_LBitmask(255), 0x0000000000000000ULL); // Invalid bit position -> 0
}

TEST_F(Bit64Test, BIT64_Mask_Bit) {
    std::uint64_t value = 0xAAAAAAAAAAAAAAAAULL;                 // Alternating pattern
    EXPECT_EQ(BIT64_Mask_Bit(value, 0), 0x0000000000000000ULL);  // mask bit 0: bit not set
    EXPECT_EQ(BIT64_Mask_Bit(value, 1), 0x0000000000000002ULL);  // mask bit 1: bit is set
    EXPECT_EQ(BIT64_Mask_Bit(value, 32), 0x0000000000000000ULL); // mask bit 32: bit not set
    EXPECT_EQ(BIT64_Mask_Bit(value, 33), 0x0000000200000000ULL); // mask bit 33: bit is set
}

TEST_F(Bit64Test, BIT64_Mask_Bits) {
    std::uint64_t value = 0x123456789ABCDEF0ULL;
    EXPECT_EQ(BIT64_Mask_Bits(value, 0, 7), 0x00000000000000F0ULL);   // mask bits 0-7
    EXPECT_EQ(BIT64_Mask_Bits(value, 8, 15), 0x000000000000DE00ULL);  // mask bits 8-15
    EXPECT_EQ(BIT64_Mask_Bits(value, 32, 39), 0x0000007800000000ULL); // mask bits 32-39
    EXPECT_EQ(BIT64_Mask_Bits(value, 56, 63), 0x1200000000000000ULL); // mask bits 56-63
}

TEST_F(Bit64Test, BIT64_Mask_nBits) {
    std::uint64_t value = 0x123456789ABCDEF0ULL;
    EXPECT_EQ(BIT64_Mask_nBits(value, 0, 8), 0x00000000000000F0ULL);  // mask 8 bits from pos 0
    EXPECT_EQ(BIT64_Mask_nBits(value, 8, 8), 0x000000000000DE00ULL);  // mask 8 bits from pos 8
    EXPECT_EQ(BIT64_Mask_nBits(value, 32, 8), 0x0000007800000000ULL); // mask 8 bits from pos 32
    EXPECT_EQ(BIT64_Mask_nBits(value, 56, 8), 0x1200000000000000ULL); // mask 8 bits from pos 56
}

TEST_F(Bit64Test, BIT64_nBitmask) {
    EXPECT_EQ(BIT64_nBitmask(0, 8), 0x00000000000000FFULL);  // 8 bits from pos 0
    EXPECT_EQ(BIT64_nBitmask(8, 8), 0x000000000000FF00ULL);  // 8 bits from pos 8
    EXPECT_EQ(BIT64_nBitmask(32, 8), 0x000000FF00000000ULL); // 8 bits from pos 32
    EXPECT_EQ(BIT64_nBitmask(56, 8), 0xFF00000000000000ULL); // 8 bits from pos 56

    // Test error conditions
    EXPECT_EQ(BIT64_nBitmask(64, 8), 0x0000000000000000ULL);  // Invalid start_bit > 63 -> 0
    EXPECT_EQ(BIT64_nBitmask(255, 4), 0x0000000000000000ULL); // Invalid start_bit > 63 -> 0
    EXPECT_EQ(BIT64_nBitmask(0, 65),
              0xFFFFFFFFFFFFFFFFULL); // Invalid bit_num > 64 -> all 1s shifted
    EXPECT_EQ(BIT64_nBitmask(4, 100),
              0xFFFFFFFFFFFFFFF0ULL); // Invalid bit_num > 64 -> all 1s shifted from pos 4
}

TEST_F(Bit64Test, BIT64_RBitmask) {
    EXPECT_EQ(BIT64_RBitmask(0), 0x0000000000000001ULL);  // right mask from bit 0
    EXPECT_EQ(BIT64_RBitmask(7), 0x00000000000000FFULL);  // right mask from bit 7
    EXPECT_EQ(BIT64_RBitmask(31), 0x00000000FFFFFFFFULL); // right mask from bit 31
    EXPECT_EQ(BIT64_RBitmask(63), 0xFFFFFFFFFFFFFFFFULL); // right mask from bit 63

    // Test error conditions - invalid bit positions > 63
    EXPECT_EQ(BIT64_RBitmask(64), 0xFFFFFFFFFFFFFFFFULL);  // Invalid bit position -> all 1s
    EXPECT_EQ(BIT64_RBitmask(255), 0xFFFFFFFFFFFFFFFFULL); // Invalid bit position -> all 1s
}

TEST_F(Bit64Test, BIT64_Reverse) {
    EXPECT_EQ(BIT64_Reverse(0x0000000000000001ULL), 0x8000000000000000ULL); // reverse bit pattern
    EXPECT_EQ(BIT64_Reverse(0x00000000000000FFULL), 0xFF00000000000000ULL); // reverse byte pattern
    EXPECT_EQ(BIT64_Reverse(0x123456789ABCDEF0ULL), 0x0F7B3D591E6A2C48ULL); // bit reversal
}

TEST_F(Bit64Test, BIT64_Set_Bit) {
    std::uint64_t value = 0x0000000000000000ULL;
    EXPECT_EQ(BIT64_Set_Bit(value, 0), 0x0000000000000001ULL);  // set bit 0
    EXPECT_EQ(BIT64_Set_Bit(value, 32), 0x0000000100000000ULL); // set bit 32
    EXPECT_EQ(BIT64_Set_Bit(value, 63), 0x8000000000000000ULL); // set bit 63
}

TEST_F(Bit64Test, BIT64_Set_Bits) {
    std::uint64_t value = 0x0000000000000000ULL;
    EXPECT_EQ(BIT64_Set_Bits(value, 0, 7), 0x00000000000000FFULL);   // set bits 0-7
    EXPECT_EQ(BIT64_Set_Bits(value, 8, 15), 0x000000000000FF00ULL);  // set bits 8-15
    EXPECT_EQ(BIT64_Set_Bits(value, 32, 39), 0x000000FF00000000ULL); // set bits 32-39
    EXPECT_EQ(BIT64_Set_Bits(value, 56, 63), 0xFF00000000000000ULL); // set bits 56-63
}

TEST_F(Bit64Test, BIT64_Set_nBits) {
    std::uint64_t value = 0x0000000000000000ULL;
    EXPECT_EQ(BIT64_Set_nBits(value, 0, 8), 0x00000000000000FFULL);  // set 8 bits from pos 0
    EXPECT_EQ(BIT64_Set_nBits(value, 8, 8), 0x000000000000FF00ULL);  // set 8 bits from pos 8
    EXPECT_EQ(BIT64_Set_nBits(value, 32, 8), 0x000000FF00000000ULL); // set 8 bits from pos 32
    EXPECT_EQ(BIT64_Set_nBits(value, 56, 8), 0xFF00000000000000ULL); // set 8 bits from pos 56
}

TEST_F(Bit64Test, BIT64_Sign_Extend) {
    // Test sign extension for various bit widths

    // Test 1-bit values (case 1)
    EXPECT_EQ(BIT64_Sign_Extend(0x1, 1), -1); // 1 -> -1 (sign bit set)
    EXPECT_EQ(BIT64_Sign_Extend(0x0, 1), 0);  // 0 -> 0 (sign bit clear)

    // Test 2-bit values (case 2)
    EXPECT_EQ(BIT64_Sign_Extend(0x3, 2), -1); // 11 -> -1 (sign bit set)
    EXPECT_EQ(BIT64_Sign_Extend(0x2, 2), -2); // 10 -> -2 (sign bit set)
    EXPECT_EQ(BIT64_Sign_Extend(0x1, 2), 1);  // 01 -> 1 (sign bit clear)
    EXPECT_EQ(BIT64_Sign_Extend(0x0, 2), 0);  // 00 -> 0 (sign bit clear)

    // Test 4-bit values (case 4)
    EXPECT_EQ(BIT64_Sign_Extend(0xF, 4), -1); // 1111 -> -1
    EXPECT_EQ(BIT64_Sign_Extend(0x8, 4), -8); // 1000 -> -8
    EXPECT_EQ(BIT64_Sign_Extend(0x7, 4), 7);  // 0111 -> 7
    EXPECT_EQ(BIT64_Sign_Extend(0x0, 4), 0);  // 0000 -> 0

    // Test 8-bit values (case 8)
    EXPECT_EQ(BIT64_Sign_Extend(0xFF, 8), -1);   // 11111111 -> -1
    EXPECT_EQ(BIT64_Sign_Extend(0x80, 8), -128); // 10000000 -> -128
    EXPECT_EQ(BIT64_Sign_Extend(0x7F, 8), 127);  // 01111111 -> 127
    EXPECT_EQ(BIT64_Sign_Extend(0x01, 8), 1);    // 00000001 -> 1

    // Test 16-bit values (case 16)
    EXPECT_EQ(BIT64_Sign_Extend(0xFFFF, 16), -1);     // All bits set
    EXPECT_EQ(BIT64_Sign_Extend(0x8000, 16), -32768); // Sign bit set
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFF, 16), 32767);  // Max positive
    EXPECT_EQ(BIT64_Sign_Extend(0x0001, 16), 1);      // Small positive

    // Test 32-bit values (case 32)
    EXPECT_EQ(BIT64_Sign_Extend(0xFFFFFFFFULL, 32), -1);            // All bits set
    EXPECT_EQ(BIT64_Sign_Extend(0x80000000ULL, 32), -2147483648LL); // Sign bit set
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFFULL, 32), 2147483647);    // Max positive
    EXPECT_EQ(BIT64_Sign_Extend(0x00000001ULL, 32), 1);             // Small positive

    // Test other bit widths to cover more switch cases
    EXPECT_EQ(BIT64_Sign_Extend(0x7, 3), -1); // case 3: 111 -> -1
    EXPECT_EQ(BIT64_Sign_Extend(0x3, 3), 3);  // case 3: 011 -> 3

    EXPECT_EQ(BIT64_Sign_Extend(0x1F, 5), -1); // case 5: 11111 -> -1
    EXPECT_EQ(BIT64_Sign_Extend(0x0F, 5), 15); // case 5: 01111 -> 15

    EXPECT_EQ(BIT64_Sign_Extend(0x3F, 6), -1); // case 6: 111111 -> -1
    EXPECT_EQ(BIT64_Sign_Extend(0x1F, 6), 31); // case 6: 011111 -> 31

    EXPECT_EQ(BIT64_Sign_Extend(0x7F, 7), -1); // case 7: 1111111 -> -1
    EXPECT_EQ(BIT64_Sign_Extend(0x3F, 7), 63); // case 7: 0111111 -> 63

    // Test mid-range values
    EXPECT_EQ(BIT64_Sign_Extend(0x3FF, 10), -1);  // case 10: 1111111111 -> -1
    EXPECT_EQ(BIT64_Sign_Extend(0x1FF, 10), 511); // case 10: 0111111111 -> 511

    EXPECT_EQ(BIT64_Sign_Extend(0x7FF, 11), -1);   // case 11: 11111111111 -> -1
    EXPECT_EQ(BIT64_Sign_Extend(0x3FF, 11), 1023); // case 11: 01111111111 -> 1023

    EXPECT_EQ(BIT64_Sign_Extend(0xFFF, 12), -1);   // case 12: 111111111111 -> -1
    EXPECT_EQ(BIT64_Sign_Extend(0x7FF, 12), 2047); // case 12: 011111111111 -> 2047

    // Test larger bit widths
    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFULL, 14), -1);   // case 14
    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFULL, 14), 8191); // case 14

    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFULL, 15), -1);    // case 15
    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFULL, 15), 16383); // case 15

    // Add comprehensive tests for more switch cases to improve coverage
    EXPECT_EQ(BIT64_Sign_Extend(0x1FF, 9), -1); // case 9: 111111111 -> -1
    EXPECT_EQ(BIT64_Sign_Extend(0xFF, 9), 255); // case 9: 011111111 -> 255

    EXPECT_EQ(BIT64_Sign_Extend(0x1FFF, 13), -1);  // case 13: 1111111111111 -> -1
    EXPECT_EQ(BIT64_Sign_Extend(0xFFF, 13), 4095); // case 13: 0111111111111 -> 4095

    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFULL, 17), -1);   // case 17: 11111111111111111 -> -1
    EXPECT_EQ(BIT64_Sign_Extend(0xFFFFULL, 17), 65535); // case 17: 01111111111111111 -> 65535

    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFULL, 18), -1);     // case 18
    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFULL, 18), 131071); // case 18

    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFULL, 19), -1);     // case 19
    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFULL, 19), 262143); // case 19

    EXPECT_EQ(BIT64_Sign_Extend(0xFFFFFULL, 20), -1);     // case 20
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFULL, 20), 524287); // case 20

    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFULL, 21), -1);     // case 21
    EXPECT_EQ(BIT64_Sign_Extend(0xFFFFFULL, 21), 1048575); // case 21

    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFULL, 22), -1);      // case 22
    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFULL, 22), 2097151); // case 22

    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFULL, 23), -1);      // case 23
    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFULL, 23), 4194303); // case 23

    EXPECT_EQ(BIT64_Sign_Extend(0xFFFFFFULL, 24), -1);      // case 24
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFULL, 24), 8388607); // case 24

    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFULL, 25), -1);      // case 25
    EXPECT_EQ(BIT64_Sign_Extend(0xFFFFFFULL, 25), 16777215); // case 25

    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFULL, 26), -1);       // case 26
    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFULL, 26), 33554431); // case 26

    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFULL, 27), -1);       // case 27
    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFULL, 27), 67108863); // case 27

    EXPECT_EQ(BIT64_Sign_Extend(0xFFFFFFFULL, 28), -1);        // case 28
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFULL, 28), 134217727); // case 28

    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFFULL, 29), -1);       // case 29
    EXPECT_EQ(BIT64_Sign_Extend(0xFFFFFFFULL, 29), 268435455); // case 29

    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFFULL, 30), -1);        // case 30
    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFFULL, 30), 536870911); // case 30

    EXPECT_EQ(BIT64_Sign_Extend(0x80000000ULL, 32), -2147483648); // case 32 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFFULL, 32), 2147483647);  // case 32 positive

    EXPECT_EQ(BIT64_Sign_Extend(0x100000000ULL, 33), -4294967296); // case 33 negative
    EXPECT_EQ(BIT64_Sign_Extend(0xFFFFFFFFULL, 33), 4294967295);   // case 33 positive

    EXPECT_EQ(BIT64_Sign_Extend(0x200000000ULL, 34), -8589934592); // case 34 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFFFULL, 34), 8589934591);  // case 34 positive

    EXPECT_EQ(BIT64_Sign_Extend(0x400000000ULL, 35), -17179869184); // case 35 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFFFULL, 35), 17179869183);  // case 35 positive

    EXPECT_EQ(BIT64_Sign_Extend(0x800000000ULL, 36), -34359738368); // case 36 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFFFULL, 36), 34359738367);  // case 36 positive

    EXPECT_EQ(BIT64_Sign_Extend(0x1000000000ULL, 37), -68719476736); // case 37 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x0FFFFFFFFFULL, 37), 68719476735);  // case 37 positive

    EXPECT_EQ(BIT64_Sign_Extend(0x2000000000ULL, 38), -137438953472); // case 38 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFFFFULL, 38), 137438953471);  // case 38 positive

    EXPECT_EQ(BIT64_Sign_Extend(0x4000000000ULL, 39), -274877906944); // case 39 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFFFFULL, 39), 274877906943);  // case 39 positive

    EXPECT_EQ(BIT64_Sign_Extend(0x8000000000ULL, 40), -549755813888); // case 40 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFFFFULL, 40), 549755813887);  // case 40 positive

    // Test higher cases 41-64 for comprehensive coverage
    EXPECT_EQ(BIT64_Sign_Extend(0x10000000000ULL, 41), -1099511627776); // case 41 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x0FFFFFFFFFFULL, 41), 1099511627775);  // case 41 positive

    EXPECT_EQ(BIT64_Sign_Extend(0x20000000000ULL, 42), -2199023255552); // case 42 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFFFFFULL, 42), 2199023255551);  // case 42 positive

    EXPECT_EQ(BIT64_Sign_Extend(0x40000000000ULL, 43), -4398046511104); // case 43 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFFFFFULL, 43), 4398046511103);  // case 43 positive

    EXPECT_EQ(BIT64_Sign_Extend(0x80000000000ULL, 44), -8796093022208); // case 44 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFFFFFULL, 44), 8796093022207);  // case 44 positive

    EXPECT_EQ(BIT64_Sign_Extend(0x100000000000ULL, 45), -17592186044416); // case 45 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x0FFFFFFFFFFFULL, 45), 17592186044415);  // case 45 positive

    EXPECT_EQ(BIT64_Sign_Extend(0x200000000000ULL, 46), -35184372088832); // case 46 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFFFFFFULL, 46), 35184372088831);  // case 46 positive

    EXPECT_EQ(BIT64_Sign_Extend(0x400000000000ULL, 47), -70368744177664); // case 47 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFFFFFFULL, 47), 70368744177663);  // case 47 positive

    EXPECT_EQ(BIT64_Sign_Extend(0x800000000000ULL, 48), -140737488355328); // case 48 negative
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFFFFFFULL, 48), 140737488355327);  // case 48

    // Test highest bit cases 49-64
    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFFFFFFFULL, 49), -1);              // case 49
    EXPECT_EQ(BIT64_Sign_Extend(0x0FFFFFFFFFFFFULL, 49), 281474976710655); // case 49

    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFFFFFFFULL, 50), -1);              // case 50
    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFFFFFFFULL, 50), 562949953421311); // case 50

    // Test very high bit cases for comprehensive switch coverage
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFFFFFFFULL, 51), -1);               // case 51
    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFFFFFFFULL, 51), 1125899906842623); // case 51

    EXPECT_EQ(BIT64_Sign_Extend(0xFFFFFFFFFFFFFULL, 52), -1);               // case 52
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFFFFFFFULL, 52), 2251799813685247); // case 52

    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFFFFFFFFULL, 53), -1);               // case 53
    EXPECT_EQ(BIT64_Sign_Extend(0x0FFFFFFFFFFFFFULL, 53), 4503599627370495); // case 53

    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFFFFFFFFULL, 54), -1);               // case 54
    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFFFFFFFFULL, 54), 9007199254740991); // case 54

    // Test extreme high cases
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFFFFFFFFULL, 55), -1);                // case 55
    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFFFFFFFFULL, 55), 18014398509481983); // case 55

    EXPECT_EQ(BIT64_Sign_Extend(0xFFFFFFFFFFFFFFULL, 56), -1);                // case 56
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFFFFFFFFULL, 56), 36028797018963967); // case 56

    // Test highest possible cases 57-64
    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFFFFFFFFFULL, 57), -1);                // case 57
    EXPECT_EQ(BIT64_Sign_Extend(0x0FFFFFFFFFFFFFFULL, 57), 72057594037927935); // case 57

    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFFFFFFFFFULL, 58), -1);                 // case 58
    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFFFFFFFFFULL, 58), 144115188075855871); // case 58

    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFFFFFFFFFULL, 59), -1);                 // case 59
    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFFFFFFFFFULL, 59), 288230376151711743); // case 59

    EXPECT_EQ(BIT64_Sign_Extend(0xFFFFFFFFFFFFFFFULL, 60), -1);                 // case 60
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFFFFFFFFFULL, 60), 576460752303423487); // case 60

    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFFFFFFFFFFULL, 61), -1);                  // case 61
    EXPECT_EQ(BIT64_Sign_Extend(0x0FFFFFFFFFFFFFFFULL, 61), 1152921504606846975); // case 61

    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFFFFFFFFFFULL, 62), -1);                  // case 62
    EXPECT_EQ(BIT64_Sign_Extend(0x1FFFFFFFFFFFFFFFULL, 62), 2305843009213693951); // case 62

    // Test 63-bit values (case 63) - original test cases
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFFFFFFFFFFULL, 63), -1);
    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFFFFFFFFFFULL, 63), 4611686018427387903);

    // Test 31-bit values (case 31) - covers the uncovered case 31
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFFULL, 31),
              -1); // case 31: 1111111111111111111111111111111 -> -1
    EXPECT_EQ(BIT64_Sign_Extend(0x3FFFFFFFULL, 31),
              1073741823); // case 31: 0111111111111111111111111111111 -> max positive 31-bit

    // Test 64-bit values (case 64) - full width
    EXPECT_EQ(BIT64_Sign_Extend(0xFFFFFFFFFFFFFFFFULL, 64), -1); // case 64: all bits set
    EXPECT_EQ(BIT64_Sign_Extend(0x7FFFFFFFFFFFFFFFULL, 64),
              9223372036854775807); // case 64: max positive

    // Test invalid bit numbers (default case) - covers default: return 0; path
    EXPECT_EQ(BIT64_Sign_Extend(0xFFFFFFFFFFFFFFFFULL, 0),
              0); // Invalid bit number 0 -> default case
    EXPECT_EQ(BIT64_Sign_Extend(0x1234567890ABCDEFULL, 65),
              0); // Invalid bit number 65 -> default case
    EXPECT_EQ(BIT64_Sign_Extend(0xFEDCBA0987654321ULL, 255),
              0); // Invalid bit number 255 -> default case
}

TEST_F(Bit64Test, BIT64_Toggle_Bit) {
    std::uint64_t value = 0xAAAAAAAAAAAAAAAAULL;                  // Alternating pattern
    EXPECT_EQ(BIT64_Toggle_Bit(value, 0), 0xAAAAAAAAAAAAAAABULL); // toggle bit 0
    EXPECT_EQ(BIT64_Toggle_Bit(value, 1), 0xAAAAAAAAAAAAAAA8ULL); // toggle bit 1
}

TEST_F(Bit64Test, BIT64_Toggle_Bits) {
    std::uint64_t value = 0xAAAAAAAAAAAAAAAAULL;                      // Alternating pattern
    EXPECT_EQ(BIT64_Toggle_Bits(value, 0, 7), 0xAAAAAAAAAAAAAA55ULL); // toggle bits 0-7
}

TEST_F(Bit64Test, BIT64_Toggle_nBits) {
    std::uint64_t value = 0xAAAAAAAAAAAAAAAAULL;                       // Alternating pattern
    EXPECT_EQ(BIT64_Toggle_nBits(value, 0, 8), 0xAAAAAAAAAAAAAA55ULL); // toggle 8 bits from pos 0
}

// Test BIT64_Bitstring function
TEST_F(Bit64Test, BIT64_Bitstring) {
    char bitstring[BIT64_STRING_SIZE];
    char *result;

    // Test with leading zeros
    result = BIT64_Bitstring(0x0000000000000001ULL, bitstring);
    EXPECT_STREQ(result, "0000000000000000000000000000000000000000000000000000000000000001");
    EXPECT_STREQ(bitstring, "0000000000000000000000000000000000000000000000000000000000000001");

    result = BIT64_Bitstring(0xAAAAAAAAAAAAAAAAULL, bitstring);
    EXPECT_STREQ(result, "1010101010101010101010101010101010101010101010101010101010101010");
    EXPECT_STREQ(bitstring, "1010101010101010101010101010101010101010101010101010101010101010");

    result = BIT64_Bitstring(0x0000000000000000ULL, bitstring);
    EXPECT_STREQ(result, "0000000000000000000000000000000000000000000000000000000000000000");
    EXPECT_STREQ(bitstring, "0000000000000000000000000000000000000000000000000000000000000000");

    // Test without leading zeros
    result = BIT64_BitstringBrief(0x0000000000000001ULL, bitstring);
    EXPECT_STREQ(result, "1");
    EXPECT_STREQ(bitstring, "1");

    result = BIT64_BitstringBrief(0x0000000000001000ULL, bitstring);
    EXPECT_STREQ(result, "1000000000000");
    EXPECT_STREQ(bitstring, "1000000000000");

    result = BIT64_BitstringBrief(0x0000000000000000ULL, bitstring);
    EXPECT_STREQ(result, "0");
    EXPECT_STREQ(bitstring, "0");

    result = BIT64_BitstringBrief(0x8000000000000000ULL, bitstring);
    EXPECT_STREQ(result, "1000000000000000000000000000000000000000000000000000000000000000");
    EXPECT_STREQ(bitstring, "1000000000000000000000000000000000000000000000000000000000000000");

    // Test edge cases
    result = BIT64_Bitstring(0xFFFFFFFFFFFFFFFFULL, bitstring);
    EXPECT_STREQ(result, "1111111111111111111111111111111111111111111111111111111111111111");
    EXPECT_STREQ(bitstring, "1111111111111111111111111111111111111111111111111111111111111111");

    result = BIT64_BitstringBrief(0xFFFFFFFFFFFFFFFFULL, bitstring);
    EXPECT_STREQ(result, "1111111111111111111111111111111111111111111111111111111111111111");
    EXPECT_STREQ(bitstring, "1111111111111111111111111111111111111111111111111111111111111111");
}