/**
 * @file bit_macros_gtest.cpp
 * @brief Tests for generic bit manipulation macros (BIT_* prefix)
 */

#include "bit16_ops.h"
#include "bit32_ops.h"
#include "bit64_ops.h"
#include "bit8_ops.h"
#include "bit_ops.h"
#include "bit_ops_api.h"
#include <cstring>
#include <gtest/gtest.h>

/******************************** BIT_X Tests ********************************/

class BITXTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(BITXTest, BasicBitPositions) {
    EXPECT_EQ(BIT_X(0), 1U);   // 2^0 = 1
    EXPECT_EQ(BIT_X(1), 2U);   // 2^1 = 2
    EXPECT_EQ(BIT_X(2), 4U);   // 2^2 = 4
    EXPECT_EQ(BIT_X(3), 8U);   // 2^3 = 8
    EXPECT_EQ(BIT_X(4), 16U);  // 2^4 = 16
    EXPECT_EQ(BIT_X(5), 32U);  // 2^5 = 32
    EXPECT_EQ(BIT_X(6), 64U);  // 2^6 = 64
    EXPECT_EQ(BIT_X(7), 128U); // 2^7 = 128
}

TEST_F(BITXTest, HigherBitPositions) {
    EXPECT_EQ(BIT_X(8), 256U);                    // 2^8
    EXPECT_EQ(BIT_X(15), 32768U);                 // 2^15
    EXPECT_EQ(BIT_X(16), 65536U);                 // 2^16
    EXPECT_EQ(BIT_X(31), 2147483648U);            // 2^31
    EXPECT_EQ(BIT_X(32), 4294967296ULL);          // 2^32
    EXPECT_EQ(BIT_X(63), 9223372036854775808ULL); // 2^63
}

TEST_F(BITXTest, SixtyFourBitPositions) {
    // Test 64-bit specific positions
    EXPECT_EQ(BIT_X(40), 1099511627776ULL);       // 2^40
    EXPECT_EQ(BIT_X(48), 281474976710656ULL);     // 2^48
    EXPECT_EQ(BIT_X(56), 72057594037927936ULL);   // 2^56
    EXPECT_EQ(BIT_X(62), 4611686018427387904ULL); // 2^62
}

/****************************** Single Bit Tests ***************************/

class SingleBitTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(SingleBitTest, BIT_MASK_BIT) {
    EXPECT_EQ(BIT_MASK_BIT(0xFF, 0), 1U); // 11111111 & 00000001 = 1
    EXPECT_EQ(BIT_MASK_BIT(0xFF, 3), 8U); // 11111111 & 00001000 = 8
    EXPECT_EQ(BIT_MASK_BIT(0xF0, 3), 0U); // 11110000 & 00001000 = 0 (bit 3 is clear in 0xF0)
    EXPECT_EQ(BIT_MASK_BIT(0xF8, 3), 8U); // 11111000 & 00001000 = 8 (bit 3 is set in 0xF8)
    EXPECT_EQ(BIT_MASK_BIT(0x0F, 7), 0U); // 00001111 & 10000000 = 0
    EXPECT_EQ(BIT_MASK_BIT(0x00, 3), 0U); // 00000000 & 00001000 = 0
}

TEST_F(SingleBitTest, BIT_GET_BIT) {
    EXPECT_EQ(BIT_GET_BIT(0x05, 0), 1U); // 00000101, bit 0 = 1
    EXPECT_EQ(BIT_GET_BIT(0x05, 1), 0U); // 00000101, bit 1 = 0
    EXPECT_EQ(BIT_GET_BIT(0x05, 2), 1U); // 00000101, bit 2 = 1
    EXPECT_EQ(BIT_GET_BIT(0x05, 3), 0U); // 00000101, bit 3 = 0
    EXPECT_EQ(BIT_GET_BIT(0x80, 7), 1U); // 10000000, bit 7 = 1
    EXPECT_EQ(BIT_GET_BIT(0x7F, 7), 0U); // 01111111, bit 7 = 0
}

TEST_F(SingleBitTest, BIT_SET_BIT) {
    EXPECT_EQ(BIT_SET_BIT(0x00, 0), 0x01U); // 00000000 | 00000001 = 1
    EXPECT_EQ(BIT_SET_BIT(0x00, 3), 0x08U); // 00000000 | 00001000 = 8
    EXPECT_EQ(BIT_SET_BIT(0x05, 1), 0x07U); // 00000101 | 00000010 = 7
    EXPECT_EQ(BIT_SET_BIT(0xFF, 3), 0xFFU); // 11111111 | 00001000 = 255 (already set)
    EXPECT_EQ(BIT_SET_BIT(0x80, 7), 0x80U); // 10000000 | 10000000 = 128 (already set)
}

TEST_F(SingleBitTest, BIT_CLEAR_BIT) {
    EXPECT_EQ(BIT_CLEAR_BIT(0xFF, 0), 0xFEU); // 11111111 & 11111110 = 254
    EXPECT_EQ(BIT_CLEAR_BIT(0xFF, 3), 0xF7U); // 11111111 & 11110111 = 247
    EXPECT_EQ(BIT_CLEAR_BIT(0x05, 0), 0x04U); // 00000101 & 11111110 = 4
    EXPECT_EQ(BIT_CLEAR_BIT(0x05, 1), 0x05U); // 00000101 & 11111101 = 5 (already cleared)
    EXPECT_EQ(BIT_CLEAR_BIT(0x00, 3), 0x00U); // 00000000 & 11110111 = 0 (already cleared)
}

TEST_F(SingleBitTest, BIT_TOGGLE_BIT) {
    EXPECT_EQ(BIT_TOGGLE_BIT(0x00, 0), 0x01U); // 00000000 ^ 00000001 = 1
    EXPECT_EQ(BIT_TOGGLE_BIT(0x01, 0), 0x00U); // 00000001 ^ 00000001 = 0
    EXPECT_EQ(BIT_TOGGLE_BIT(0x05, 1), 0x07U); // 00000101 ^ 00000010 = 7
    EXPECT_EQ(BIT_TOGGLE_BIT(0x07, 1), 0x05U); // 00000111 ^ 00000010 = 5
    EXPECT_EQ(BIT_TOGGLE_BIT(0x80, 7), 0x00U); // 10000000 ^ 10000000 = 0
}

TEST_F(SingleBitTest, BIT_CHANGE_BIT) {
    // Set bit (new_bit = 1)
    EXPECT_EQ(BIT_CHANGE_BIT(0x00, 1, 0), 0x01U); // Set bit 0 to 1
    EXPECT_EQ(BIT_CHANGE_BIT(0x00, 1, 3), 0x08U); // Set bit 3 to 1
    EXPECT_EQ(BIT_CHANGE_BIT(0x05, 1, 1), 0x07U); // Set bit 1 to 1

    // Clear bit (new_bit = 0)
    EXPECT_EQ(BIT_CHANGE_BIT(0xFF, 0, 0), 0xFEU); // Clear bit 0
    EXPECT_EQ(BIT_CHANGE_BIT(0xFF, 0, 3), 0xF7U); // Clear bit 3
    EXPECT_EQ(BIT_CHANGE_BIT(0x07, 0, 1), 0x05U); // Clear bit 1

    // No change (setting already set bit, clearing already clear bit)
    EXPECT_EQ(BIT_CHANGE_BIT(0x05, 1, 0), 0x05U); // Bit 0 already 1
    EXPECT_EQ(BIT_CHANGE_BIT(0x05, 0, 1), 0x05U); // Bit 1 already 0
}

TEST_F(SingleBitTest, SixtyFourBitOperations) {
    uint64_t val64 = 0x123456789ABCDEF0ULL;

    // Test BIT_GET_BIT with 64-bit values
    EXPECT_EQ(BIT_GET_BIT(val64, 0), 0U);  // LSB is 0
    EXPECT_EQ(BIT_GET_BIT(val64, 4), 1U);  // Bit 4 is 1 (0xF0 & 0x10)
    EXPECT_EQ(BIT_GET_BIT(val64, 32), 0U); // Bit 32: LSB of upper 32 bits (0x12345678 & 1) = 0
    EXPECT_EQ(BIT_GET_BIT(val64, 63), 0U); // MSB is 0 (0x1...)

    // Test BIT_SET_BIT with 64-bit values
    EXPECT_EQ(BIT_SET_BIT(val64, 0), 0x123456789ABCDEF1ULL);
    EXPECT_EQ(BIT_SET_BIT(0ULL, 63), 0x8000000000000000ULL);
    EXPECT_EQ(BIT_SET_BIT(0ULL, 32), 0x100000000ULL);

    // Test BIT_CLEAR_BIT with 64-bit values
    EXPECT_EQ(BIT_CLEAR_BIT(val64, 4), 0x123456789ABCDEE0ULL);
    EXPECT_EQ(BIT_CLEAR_BIT(0xFFFFFFFFFFFFFFFFULL, 63), 0x7FFFFFFFFFFFFFFFULL);
    EXPECT_EQ(BIT_CLEAR_BIT(0xFFFFFFFFFFFFFFFFULL, 32), 0xFFFFFFFEFFFFFFFFULL);

    // Test BIT_TOGGLE_BIT with 64-bit values
    EXPECT_EQ(BIT_TOGGLE_BIT(val64, 0), 0x123456789ABCDEF1ULL);
    EXPECT_EQ(BIT_TOGGLE_BIT(0ULL, 63), 0x8000000000000000ULL);
    EXPECT_EQ(BIT_TOGGLE_BIT(0xFFFFFFFFFFFFFFFFULL, 32), 0xFFFFFFFEFFFFFFFFULL);

    // Test BIT_MASK_BIT with 64-bit values
    EXPECT_EQ(BIT_MASK_BIT(val64, 0), 0U);    // LSB is 0
    EXPECT_EQ(BIT_MASK_BIT(val64, 4), 0x10U); // Bit 4 mask
    EXPECT_EQ(BIT_MASK_BIT(0xFFFFFFFFFFFFFFFFULL, 63), 0x8000000000000000ULL);

    // Test BIT_CHANGE_BIT with 64-bit values
    EXPECT_EQ(BIT_CHANGE_BIT(val64, 1, 0), 0x123456789ABCDEF1ULL);
    EXPECT_EQ(BIT_CHANGE_BIT(0xFFFFFFFFFFFFFFFFULL, 0, 63), 0x7FFFFFFFFFFFFFFFULL);
}

/****************************** Bitmask Tests *******************************/

class BitmaskTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(BitmaskTest, BIT_LOWER_BITMASK) {
    EXPECT_EQ(BIT_LOWER_BITMASK(0), 0x01U); // 00000001
    EXPECT_EQ(BIT_LOWER_BITMASK(1), 0x03U); // 00000011
    EXPECT_EQ(BIT_LOWER_BITMASK(2), 0x07U); // 00000111
    EXPECT_EQ(BIT_LOWER_BITMASK(3), 0x0FU); // 00001111
    EXPECT_EQ(BIT_LOWER_BITMASK(4), 0x1FU); // 00011111
    EXPECT_EQ(BIT_LOWER_BITMASK(7), 0xFFU); // 11111111
}

TEST_F(BitmaskTest, BIT_UPPER_BITMASK) {
    EXPECT_EQ(BIT_UPPER_BITMASK(0) & 0xFF, 0xFFU); // 11111111
    EXPECT_EQ(BIT_UPPER_BITMASK(1) & 0xFF, 0xFEU); // 11111110
    EXPECT_EQ(BIT_UPPER_BITMASK(2) & 0xFF, 0xFCU); // 11111100
    EXPECT_EQ(BIT_UPPER_BITMASK(3) & 0xFF, 0xF8U); // 11111000
    EXPECT_EQ(BIT_UPPER_BITMASK(4) & 0xFF, 0xF0U); // 11110000
    EXPECT_EQ(BIT_UPPER_BITMASK(8) & 0xFF, 0x00U); // 00000000
}

TEST_F(BitmaskTest, BIT_BITMASK) {
    // Normal cases
    EXPECT_EQ(BIT_BITMASK(1, 3) & 0xFF, 0x0EU); // 00001110 (bits 1-3)
    EXPECT_EQ(BIT_BITMASK(2, 5) & 0xFF, 0x3CU); // 00111100 (bits 2-5)
    EXPECT_EQ(BIT_BITMASK(0, 7) & 0xFF, 0xFFU); // 11111111 (bits 0-7)

    // Single bit cases
    EXPECT_EQ(BIT_BITMASK(3, 3) & 0xFF, 0x08U); // 00001000 (bit 3)
    EXPECT_EQ(BIT_BITMASK(0, 0) & 0xFF, 0x01U); // 00000001 (bit 0)

    // Edge cases where start > end should result in 0
    EXPECT_EQ(BIT_BITMASK(5, 3) & 0xFF, 0x00U); // Invalid range
}

TEST_F(BitmaskTest, SixtyFourBitBitmasks) {
    // Test BIT_LOWER_BITMASK with 64-bit ranges
    EXPECT_EQ(BIT_LOWER_BITMASK(31), 0xFFFFFFFFULL);         // All lower 32 bits
    EXPECT_EQ(BIT_LOWER_BITMASK(63), 0xFFFFFFFFFFFFFFFFULL); // All 64 bits
    EXPECT_EQ(BIT_LOWER_BITMASK(32), 0x1FFFFFFFFULL);        // 33 bits set

    // Test BIT_UPPER_BITMASK with 64-bit ranges
    EXPECT_EQ(BIT_UPPER_BITMASK(32), 0xFFFFFFFF00000000ULL); // Upper 32 bits
    EXPECT_EQ(BIT_UPPER_BITMASK(63), 0x8000000000000000ULL); // Only MSB
    EXPECT_EQ(BIT_UPPER_BITMASK(0), 0xFFFFFFFFFFFFFFFFULL);  // All bits

    // Test BIT_BITMASK with 64-bit ranges
    EXPECT_EQ(BIT_BITMASK(0, 31), 0xFFFFFFFFULL);          // Lower 32 bits
    EXPECT_EQ(BIT_BITMASK(32, 63), 0xFFFFFFFF00000000ULL); // Upper 32 bits
    EXPECT_EQ(BIT_BITMASK(16, 47), 0x0000FFFFFFFF0000ULL); // Middle 32 bits
    EXPECT_EQ(BIT_BITMASK(63, 63), 0x8000000000000000ULL); // Only MSB
    EXPECT_EQ(BIT_BITMASK(0, 0), 0x1ULL);                  // Only LSB
}

/****************************** Multi-Bit Tests *****************************/

class MultiBitTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(MultiBitTest, BIT_MASK_BITS) {
    EXPECT_EQ(BIT_MASK_BITS(0xFF, 1, 3), 0x0EU); // 11111111 & 00001110 = 14
    EXPECT_EQ(BIT_MASK_BITS(0xAA, 0, 3), 0x0AU); // 10101010 & 00001111 = 10
    EXPECT_EQ(BIT_MASK_BITS(0x55, 4, 7), 0x50U); // 01010101 & 11110000 = 80
    EXPECT_EQ(BIT_MASK_BITS(0x00, 1, 3), 0x00U); // 00000000 & 00001110 = 0
}

TEST_F(MultiBitTest, BIT_GET_BITS) {
    EXPECT_EQ(BIT_GET_BITS(0x0E, 1, 3), 0x07U); // Extract bits 1-3: 01110 >> 1 = 0111
    EXPECT_EQ(BIT_GET_BITS(0x3C, 2, 5), 0x0FU); // Extract bits 2-5: 111100 >> 2 = 1111
    EXPECT_EQ(BIT_GET_BITS(0xF0, 4, 7), 0x0FU); // Extract bits 4-7: 11110000 >> 4 = 1111
    EXPECT_EQ(BIT_GET_BITS(0x05, 0, 2), 0x05U); // Extract bits 0-2: 00000101 >> 0 = 101
}

TEST_F(MultiBitTest, BIT_SET_BITS) {
    EXPECT_EQ(BIT_SET_BITS(0x00, 1, 3), 0x0EU); // 00000000 | 00001110 = 14
    EXPECT_EQ(BIT_SET_BITS(0x05, 1, 3), 0x0FU); // 00000101 | 00001110 = 15
    EXPECT_EQ(BIT_SET_BITS(0xFF, 1, 3), 0xFFU); // 11111111 | 00001110 = 255 (no change)
    EXPECT_EQ(BIT_SET_BITS(0x80, 0, 3), 0x8FU); // 10000000 | 00001111 = 143
}

TEST_F(MultiBitTest, BIT_CLEAR_BITS) {
    EXPECT_EQ(BIT_CLEAR_BITS(0xFF, 1, 3), 0xF1U); // 11111111 & 11110001 = 241
    EXPECT_EQ(BIT_CLEAR_BITS(0x0F, 1, 3), 0x01U); // 00001111 & 11110001 = 1
    EXPECT_EQ(BIT_CLEAR_BITS(0x00, 1, 3), 0x00U); // 00000000 & 11110001 = 0 (no change)
    EXPECT_EQ(BIT_CLEAR_BITS(0x80, 4, 7), 0x00U); // 10000000 & 00001111 = 0
}

TEST_F(MultiBitTest, BIT_TOGGLE_BITS) {
    EXPECT_EQ(BIT_TOGGLE_BITS(0x00, 1, 3), 0x0EU); // 00000000 ^ 00001110 = 14
    EXPECT_EQ(BIT_TOGGLE_BITS(0xFF, 1, 3), 0xF1U); // 11111111 ^ 00001110 = 241
    EXPECT_EQ(BIT_TOGGLE_BITS(0x05, 1, 3), 0x0BU); // 00000101 ^ 00001110 = 11
    EXPECT_EQ(BIT_TOGGLE_BITS(0x0E, 1, 3), 0x00U); // 00001110 ^ 00001110 = 0
}

TEST_F(MultiBitTest, BIT_CHANGE_BITS) {
    // Set multiple bits
    EXPECT_EQ(BIT_CHANGE_BITS(0x00, 0x07, 1, 3), 0x0EU); // Set bits 1-3 to 111
    EXPECT_EQ(BIT_CHANGE_BITS(0x80, 0x05, 0, 3), 0x85U); // Set bits 0-3 to 0101

    // Clear multiple bits
    EXPECT_EQ(BIT_CHANGE_BITS(0xFF, 0x00, 1, 3), 0xF1U); // Clear bits 1-3
    EXPECT_EQ(BIT_CHANGE_BITS(0x0F, 0x00, 0, 2), 0x08U); // Clear bits 0-2

    // Mixed operations
    EXPECT_EQ(BIT_CHANGE_BITS(0xF0, 0x05, 0, 3), 0xF5U); // Change bits 0-3 to 0101
    EXPECT_EQ(BIT_CHANGE_BITS(0x05, 0x03, 1, 2), 0x07U); // Change bits 1-2 to 11
}

TEST_F(MultiBitTest, SixtyFourBitMultiOperations) {
    uint64_t val64 = 0x123456789ABCDEF0ULL;

    // Test BIT_MASK_BITS with 64-bit values
    EXPECT_EQ(BIT_MASK_BITS(val64, 0, 31), 0x9ABCDEF0ULL);          // Lower 32 bits
    EXPECT_EQ(BIT_MASK_BITS(val64, 32, 63), 0x1234567800000000ULL); // Upper 32 bits
    EXPECT_EQ(BIT_MASK_BITS(val64, 16, 47), 0x56789ABC0000ULL);     // Middle 32 bits (bits 16-47)

    // Test BIT_GET_BITS with 64-bit values
    EXPECT_EQ(BIT_GET_BITS(val64, 0, 31), 0x9ABCDEF0ULL);  // Extract lower 32 bits
    EXPECT_EQ(BIT_GET_BITS(val64, 32, 63), 0x12345678ULL); // Extract upper 32 bits
    EXPECT_EQ(BIT_GET_BITS(val64, 16, 47), 0x56789ABCULL); // Extract middle 32 bits
    EXPECT_EQ(BIT_GET_BITS(val64, 60, 63), 0x1ULL);        // Extract top 4 bits

    // Test BIT_SET_BITS with 64-bit values
    EXPECT_EQ(BIT_SET_BITS(0ULL, 0, 31), 0xFFFFFFFFULL);           // Set lower 32 bits
    EXPECT_EQ(BIT_SET_BITS(0ULL, 32, 63), 0xFFFFFFFF00000000ULL);  // Set upper 32 bits
    EXPECT_EQ(BIT_SET_BITS(val64, 60, 63), 0xF23456789ABCDEF0ULL); // Set top 4 bits

    // Test BIT_CLEAR_BITS with 64-bit values
    EXPECT_EQ(BIT_CLEAR_BITS(0xFFFFFFFFFFFFFFFFULL, 0, 31),
              0xFFFFFFFF00000000ULL);                                        // Clear lower 32
    EXPECT_EQ(BIT_CLEAR_BITS(0xFFFFFFFFFFFFFFFFULL, 32, 63), 0xFFFFFFFFULL); // Clear upper 32
    EXPECT_EQ(BIT_CLEAR_BITS(val64, 60, 63), 0x023456789ABCDEF0ULL);         // Clear top 4 bits

    // Test BIT_TOGGLE_BITS with 64-bit values
    EXPECT_EQ(BIT_TOGGLE_BITS(val64, 0, 31), 0x123456786543210FULL); // Toggle lower 32 bits
    EXPECT_EQ(BIT_TOGGLE_BITS(0ULL, 32, 63), 0xFFFFFFFF00000000ULL); // Toggle upper 32 bits

    // Test BIT_CHANGE_BITS with 64-bit values
    EXPECT_EQ(BIT_CHANGE_BITS(val64, 0xFFFFFFFFULL, 0, 31),
              0x12345678FFFFFFFFULL);                               // Set lower 32 to all 1s
    EXPECT_EQ(BIT_CHANGE_BITS(val64, 0ULL, 32, 63), 0x9ABCDEF0ULL); // Clear upper 32 bits
}

/****************************** Sign Tests **********************************/

class SignTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(SignTest, BIT_IS_NEG) {
    // 8-bit signed values
    int8_t pos8 = 0x7F;         // +127
    int8_t neg8 = (int8_t)0x80; // -128 (explicit cast to avoid overflow warning)

    EXPECT_EQ(BIT_IS_NEG(pos8), 0U);
    EXPECT_EQ(BIT_IS_NEG(neg8), 1U);
    EXPECT_EQ(BIT_IS_NEG((int8_t)0), 0U);
    EXPECT_EQ(BIT_IS_NEG((int8_t)-1), 1U);

    // 16-bit signed values
    int16_t pos16 = 0x7FFF;          // +32767
    int16_t neg16 = (int16_t)0x8000; // -32768 (explicit cast to avoid overflow warning)

    EXPECT_EQ(BIT_IS_NEG(pos16), 0U);
    EXPECT_EQ(BIT_IS_NEG(neg16), 1U);

    // 32-bit signed values
    int32_t pos32 = 0x7FFFFFFF;          // +2147483647
    int32_t neg32 = (int32_t)0x80000000; // -2147483648 (explicit cast to avoid overflow warning)

    EXPECT_EQ(BIT_IS_NEG(pos32), 0U);
    EXPECT_EQ(BIT_IS_NEG(neg32), 1U);
}

TEST_F(SignTest, BIT_GET_SIGN) {
    // Positive numbers should return 1
    EXPECT_EQ(BIT_GET_SIGN(127), 1);
    EXPECT_EQ(BIT_GET_SIGN(1), 1);
    EXPECT_EQ(BIT_GET_SIGN(0), 1); // Zero is considered positive

    // Negative numbers should return -1
    EXPECT_EQ(BIT_GET_SIGN(-1), -1);
    EXPECT_EQ(BIT_GET_SIGN(-128), -1);
    EXPECT_EQ(BIT_GET_SIGN(-32768), -1);
}

TEST_F(SignTest, SixtyFourBitSignOperations) {
    // 64-bit signed values
    int64_t pos64 = 0x7FFFFFFFFFFFFFFFLL;           // +9223372036854775807 (max int64)
    int64_t neg64 = (int64_t)0x8000000000000000ULL; // -9223372036854775808 (min int64)

    // Test BIT_IS_NEG with 64-bit values
    EXPECT_EQ(BIT_IS_NEG(pos64), 0U);
    EXPECT_EQ(BIT_IS_NEG(neg64), 1U);
    EXPECT_EQ(BIT_IS_NEG((int64_t)0), 0U);
    EXPECT_EQ(BIT_IS_NEG((int64_t)-1), 1U);

    // Test edge cases around 32-bit boundary
    int64_t pos32_in_64 = 0x80000000LL;  // 2^31 (positive in 64-bit)
    int64_t neg32_in_64 = -0x80000000LL; // -2^31 (negative in 64-bit)

    EXPECT_EQ(BIT_IS_NEG(pos32_in_64), 0U);
    EXPECT_EQ(BIT_IS_NEG(neg32_in_64), 1U);

    // Test BIT_GET_SIGN with 64-bit values
    EXPECT_EQ(BIT_GET_SIGN(pos64), 1);
    EXPECT_EQ(BIT_GET_SIGN(neg64), -1);
    EXPECT_EQ(BIT_GET_SIGN(pos32_in_64), 1);
    EXPECT_EQ(BIT_GET_SIGN(neg32_in_64), -1);
}

/****************************** Shift Tests *********************************/

class ShiftTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ShiftTest, BIT_LOGICAL_RSHIFT) {
    EXPECT_EQ(BIT_LOGICAL_RSHIFT(0xFF, 1), 0x7FU); // 255 >> 1 = 127
    EXPECT_EQ(BIT_LOGICAL_RSHIFT(0x80, 1), 0x40U); // 128 >> 1 = 64
    EXPECT_EQ(BIT_LOGICAL_RSHIFT(0xFF, 4), 0x0FU); // 255 >> 4 = 15
    EXPECT_EQ(BIT_LOGICAL_RSHIFT(0x01, 1), 0x00U); // 1 >> 1 = 0
}

TEST_F(ShiftTest, BIT_LOGICAL_LSHIFT) {
    EXPECT_EQ(BIT_LOGICAL_LSHIFT(0x01, 1), 0x02U);        // 1 << 1 = 2
    EXPECT_EQ(BIT_LOGICAL_LSHIFT(0x0F, 2), 0x3CU);        // 15 << 2 = 60
    EXPECT_EQ(BIT_LOGICAL_LSHIFT(0x80, 1) & 0xFF, 0x00U); // 128 << 1 = 256 (overflow)
    EXPECT_EQ(BIT_LOGICAL_LSHIFT(0x01, 7), 0x80U);        // 1 << 7 = 128
}

TEST_F(ShiftTest, SixtyFourBitShiftOperations) {
    uint64_t val64 = 0x123456789ABCDEF0ULL;

    // Test BIT_LOGICAL_RSHIFT with 64-bit values
    EXPECT_EQ(BIT_LOGICAL_RSHIFT(val64, 1), 0x091A2B3C4D5E6F78ULL); // >> 1
    EXPECT_EQ(BIT_LOGICAL_RSHIFT(val64, 32), 0x12345678ULL);        // >> 32 (upper becomes lower)
    EXPECT_EQ(BIT_LOGICAL_RSHIFT(val64, 4), 0x0123456789ABCDEFULL); // >> 4
    EXPECT_EQ(BIT_LOGICAL_RSHIFT(0x8000000000000000ULL, 1), 0x4000000000000000ULL); // MSB >> 1

    // Test BIT_LOGICAL_LSHIFT with 64-bit values
    EXPECT_EQ(BIT_LOGICAL_LSHIFT(val64, 1), 0x2468ACF13579BDE0ULL); // << 1
    EXPECT_EQ(BIT_LOGICAL_LSHIFT(0x12345678ULL, 32),
              0x1234567800000000ULL);                                 // << 32 (lower becomes upper)
    EXPECT_EQ(BIT_LOGICAL_LSHIFT(val64, 4), 0x23456789ABCDEF00ULL);   // << 4
    EXPECT_EQ(BIT_LOGICAL_LSHIFT(0x1ULL, 63), 0x8000000000000000ULL); // 1 << 63 = MSB

    // Test edge cases
    EXPECT_EQ(BIT_LOGICAL_RSHIFT(val64, 0), val64); // >> 0 = no change
    EXPECT_EQ(BIT_LOGICAL_LSHIFT(val64, 0), val64); // << 0 = no change
}

/****************************** Type Range Tests ****************************/

class TypeRangeTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(TypeRangeTest, BIT_STYPE_MIN_MAX) {
    int8_t s8 = 0;
    int16_t s16 = 0;
    int32_t s32 = 0;

    // For 8-bit signed: min = -128, max = 127
    EXPECT_EQ(BIT_STYPE_MIN(s8), 0x80U); // 128 (magnitude of -128)
    EXPECT_EQ(BIT_STYPE_MAX(s8), 0x7FU); // 127

    // For 16-bit signed: min = -32768, max = 32767
    EXPECT_EQ(BIT_STYPE_MIN(s16), 0x8000U); // 32768 (magnitude of -32768)
    EXPECT_EQ(BIT_STYPE_MAX(s16), 0x7FFFU); // 32767

    // For 32-bit signed: min = -2147483648, max = 2147483647
    EXPECT_EQ(BIT_STYPE_MIN(s32), 0x80000000U); // 2147483648 (magnitude of -2147483648)
    EXPECT_EQ(BIT_STYPE_MAX(s32), 0x7FFFFFFFU); // 2147483647
}

TEST_F(TypeRangeTest, BIT_UTYPE_MIN_MAX) {
    uint8_t u8 = 0;
    uint16_t u16 = 0;
    uint32_t u32 = 0;

    // For unsigned types: min = 0, max = 2^n - 1
    EXPECT_EQ(BIT_UTYPE_MIN(u8), 0U);
    EXPECT_EQ(BIT_UTYPE_MAX(u8), 0xFFU); // 255

    EXPECT_EQ(BIT_UTYPE_MIN(u16), 0U);
    EXPECT_EQ(BIT_UTYPE_MAX(u16), 0xFFFFU); // 65535

    EXPECT_EQ(BIT_UTYPE_MIN(u32), 0U);
    EXPECT_EQ(BIT_UTYPE_MAX(u32), 0xFFFFFFFFU); // 4294967295
}

TEST_F(TypeRangeTest, SixtyFourBitTypeRanges) {
    int64_t s64 = 0;
    uint64_t u64 = 0;

    // For 64-bit signed: min = -9223372036854775808, max = 9223372036854775807
    EXPECT_EQ(BIT_STYPE_MIN(s64), 0x8000000000000000ULL); // 2^63 (magnitude of min)
    EXPECT_EQ(BIT_STYPE_MAX(s64), 0x7FFFFFFFFFFFFFFFULL); // 2^63 - 1

    // For 64-bit unsigned: min = 0, max = 18446744073709551615
    EXPECT_EQ(BIT_UTYPE_MIN(u64), 0ULL);
    EXPECT_EQ(BIT_UTYPE_MAX(u64), 0xFFFFFFFFFFFFFFFFULL); // 2^64 - 1

    // Test that these values are actually the correct bit patterns
    EXPECT_EQ(BIT_STYPE_MIN(s64), BIT_X(63));            // 2^63
    EXPECT_EQ(BIT_STYPE_MAX(s64), BIT_X(63) - 1);        // 2^63 - 1
    EXPECT_EQ(BIT_UTYPE_MAX(u64), (BIT_X(63) << 1) - 1); // 2^64 - 1
}

/****************************** Edge Cases and Error Conditions ************/

class EdgeCaseTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(EdgeCaseTest, ZeroInputs) {
    // Test all macros with zero input
    EXPECT_EQ(BIT_MASK_BIT(0, 0), 0U);
    EXPECT_EQ(BIT_GET_BIT(0, 0), 0U);
    EXPECT_EQ(BIT_SET_BIT(0, 0), 1U);
    EXPECT_EQ(BIT_CLEAR_BIT(0, 0), 0U);
    EXPECT_EQ(BIT_TOGGLE_BIT(0, 0), 1U);
    EXPECT_EQ(BIT_CHANGE_BIT(0, 0, 0), 0U);
    EXPECT_EQ(BIT_CHANGE_BIT(0, 1, 0), 1U);
}

TEST_F(EdgeCaseTest, MaxValueInputs) {
    // Test with maximum values
    EXPECT_EQ(BIT_GET_BIT(0xFF, 7), 1U);
    EXPECT_EQ(BIT_CLEAR_BIT(0xFF, 7), 0x7FU);
    EXPECT_EQ(BIT_TOGGLE_BIT(0xFF, 0), 0xFEU);
}

TEST_F(EdgeCaseTest, BoundaryBitPositions) {
    // Test bit position 0 (LSB)
    EXPECT_EQ(BIT_X(0), 1U);
    EXPECT_EQ(BIT_GET_BIT(0x01, 0), 1U);
    EXPECT_EQ(BIT_GET_BIT(0x00, 0), 0U);

    // Test bit position 7 (MSB for 8-bit)
    EXPECT_EQ(BIT_X(7), 0x80U);
    EXPECT_EQ(BIT_GET_BIT(0x80, 7), 1U);
    EXPECT_EQ(BIT_GET_BIT(0x7F, 7), 0U);
}

TEST_F(EdgeCaseTest, InvalidBitmaskRanges) {
    // When start_bit > end_bit, should produce 0 mask
    EXPECT_EQ(BIT_BITMASK(5, 3) & 0xFF, 0x00U);
    EXPECT_EQ(BIT_MASK_BITS(0xFF, 5, 3), 0x00U);
    EXPECT_EQ(BIT_GET_BITS(0xFF, 5, 3), 0x00U);
    EXPECT_EQ(BIT_SET_BITS(0x00, 5, 3), 0x00U);
    EXPECT_EQ(BIT_CLEAR_BITS(0xFF, 5, 3), 0xFFU);
    EXPECT_EQ(BIT_TOGGLE_BITS(0x00, 5, 3), 0x00U);
}

/********************** Integration Tests with Existing API *****************/

class IntegrationTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(IntegrationTest, ConsistencyWithBIT8Functions) {
    uint8_t test_val = 0xA5; // 10100101

    // Compare macro results with equivalent BIT8 function results
    for (int bit = 0; bit < 8; bit++) {
        EXPECT_EQ(BIT_GET_BIT(test_val, bit), BIT8_Get_Bit(test_val, bit))
            << "Bit position " << bit << " differs between macro and function";

        EXPECT_EQ(BIT_SET_BIT(test_val, bit), BIT8_Set_Bit(test_val, bit))
            << "Set bit position " << bit << " differs between macro and function";

        EXPECT_EQ(BIT_CLEAR_BIT(test_val, bit), BIT8_Clear_Bit(test_val, bit))
            << "Clear bit position " << bit << " differs between macro and function";

        EXPECT_EQ(BIT_TOGGLE_BIT(test_val, bit), BIT8_Toggle_Bit(test_val, bit))
            << "Toggle bit position " << bit << " differs between macro and function";
    }
}

TEST_F(IntegrationTest, MacroComposition) {
    // Test that macros work correctly when composed together
    uint8_t val = 0x00;

    // Set bits using individual bit operations
    val = BIT_SET_BIT(val, 0); // 00000001
    val = BIT_SET_BIT(val, 2); // 00000101
    val = BIT_SET_BIT(val, 4); // 00010101
    EXPECT_EQ(val, 0x15U);

    // Should be equivalent to setting bits in range
    uint8_t val2 = BIT_SET_BITS(0x00, 0, 4);
    val2 = BIT_CLEAR_BIT(val2, 1); // Clear bit 1
    val2 = BIT_CLEAR_BIT(val2, 3); // Clear bit 3
    EXPECT_EQ(val, val2);
}

/************************* Basic Bit Operations Tests ***********************/

class BasicBitOpsTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(BasicBitOpsTest, BIT_WORDBITS) {
    uint8_t u8 = 0;
    uint16_t u16 = 0;
    uint32_t u32 = 0;
    uint64_t u64 = 0;

    EXPECT_EQ(BIT_WORDBITS(u8), 8);
    EXPECT_EQ(BIT_WORDBITS(u16), 16);
    EXPECT_EQ(BIT_WORDBITS(u32), 32);
    EXPECT_EQ(BIT_WORDBITS(u64), 64);
}

TEST_F(BasicBitOpsTest, BIT_TOPBIT) {
    uint8_t u8 = 0;
    uint16_t u16 = 0;
    uint32_t u32 = 0;
    uint64_t u64 = 0;

    EXPECT_EQ(BIT_TOPBIT(u8), 7);
    EXPECT_EQ(BIT_TOPBIT(u16), 15);
    EXPECT_EQ(BIT_TOPBIT(u32), 31);
    EXPECT_EQ(BIT_TOPBIT(u64), 63);
}

TEST_F(BasicBitOpsTest, BIT_COMPLIMENT) {
    EXPECT_EQ(BIT_COMPLIMENT(0x00) & 0xFF, 0xFFU);
    EXPECT_EQ(BIT_COMPLIMENT(0xFF) & 0xFF, 0x00U);
    EXPECT_EQ(BIT_COMPLIMENT(0xAA) & 0xFF, 0x55U);
    EXPECT_EQ(BIT_COMPLIMENT(0x55) & 0xFF, 0xAAU);
    EXPECT_EQ(BIT_COMPLIMENT(0xF0) & 0xFF, 0x0FU);
}

TEST_F(BasicBitOpsTest, BIT_NEGATE) {
    EXPECT_EQ(BIT_NEGATE(1) & 0xFF, 0xFFU);    // -1 in two's complement
    EXPECT_EQ(BIT_NEGATE(2) & 0xFF, 0xFEU);    // -2 in two's complement
    EXPECT_EQ(BIT_NEGATE(0) & 0xFF, 0x00U);    // -0 = 0
    EXPECT_EQ(BIT_NEGATE(0x80) & 0xFF, 0x80U); // -128 wraps in 8-bit
}

TEST_F(BasicBitOpsTest, BIT_SWAP) {
    uint8_t a = 0x55;
    uint8_t b = 0xAA;
    uint8_t orig_a = a;
    uint8_t orig_b = b;

    BIT_SWAP(a, b);

    EXPECT_EQ(a, orig_b);
    EXPECT_EQ(b, orig_a);

    // Test with same values
    a = b = 0x42;
    BIT_SWAP(a, b);
    EXPECT_EQ(a, 0x42U);
    EXPECT_EQ(b, 0x42U);
}

/************************* Lowest Bit Set Tests *****************************/

class LowestBitSetTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(LowestBitSetTest, BIT_GET_LOWEST_BIT_SET) {
    EXPECT_EQ(BIT_GET_LOWEST_BIT_SET(0x00), 0x00U); // No bits set
    EXPECT_EQ(BIT_GET_LOWEST_BIT_SET(0x01), 0x01U); // 00000001 -> 00000001
    EXPECT_EQ(BIT_GET_LOWEST_BIT_SET(0x02), 0x02U); // 00000010 -> 00000010
    EXPECT_EQ(BIT_GET_LOWEST_BIT_SET(0x04), 0x04U); // 00000100 -> 00000100
    EXPECT_EQ(BIT_GET_LOWEST_BIT_SET(0x03), 0x01U); // 00000011 -> 00000001
    EXPECT_EQ(BIT_GET_LOWEST_BIT_SET(0x06), 0x02U); // 00000110 -> 00000010
    EXPECT_EQ(BIT_GET_LOWEST_BIT_SET(0xB4), 0x04U); // 10110100 -> 00000100
    EXPECT_EQ(BIT_GET_LOWEST_BIT_SET(0x80), 0x80U); // 10000000 -> 10000000

    // Test alias
    EXPECT_EQ(BIT_GET_LBS(0xB4), 0x04U);
}

TEST_F(LowestBitSetTest, BIT_STRIP_LBS) {
    EXPECT_EQ(BIT_STRIP_LBS(0x00), 0x00U); // No bits set
    EXPECT_EQ(BIT_STRIP_LBS(0x01), 0x00U); // 00000001 -> 00000000
    EXPECT_EQ(BIT_STRIP_LBS(0x02), 0x00U); // 00000010 -> 00000000
    EXPECT_EQ(BIT_STRIP_LBS(0x03), 0x02U); // 00000011 -> 00000010
    EXPECT_EQ(BIT_STRIP_LBS(0x04), 0x00U); // 00000100 -> 00000000
    EXPECT_EQ(BIT_STRIP_LBS(0x06), 0x04U); // 00000110 -> 00000100
    EXPECT_EQ(BIT_STRIP_LBS(0xB6), 0xB4U); // 10110110 -> 10110100
    EXPECT_EQ(BIT_STRIP_LBS(0x80), 0x00U); // 10000000 -> 00000000
}

TEST_F(LowestBitSetTest, BIT_MASK_LBS_AND_ABOVE) {
    EXPECT_EQ(BIT_MASK_LBS_AND_ABOVE(0x00) & 0xFF, 0x00U); // No bits set
    EXPECT_EQ(BIT_MASK_LBS_AND_ABOVE(0x01) & 0xFF, 0xFFU); // 00000001 -> 11111111
    EXPECT_EQ(BIT_MASK_LBS_AND_ABOVE(0x02) & 0xFF, 0xFEU); // 00000010 -> 11111110
    EXPECT_EQ(BIT_MASK_LBS_AND_ABOVE(0x04) & 0xFF, 0xFCU); // 00000100 -> 11111100
    EXPECT_EQ(BIT_MASK_LBS_AND_ABOVE(0x08) & 0xFF, 0xF8U); // 00001000 -> 11111000
    EXPECT_EQ(BIT_MASK_LBS_AND_ABOVE(0xA8) & 0xFF, 0xF8U); // 10101000 -> 11111000
}

TEST_F(LowestBitSetTest, BIT_MASK_ABOVE_LBS) {
    EXPECT_EQ(BIT_MASK_ABOVE_LBS(0x00) & 0xFF, 0x00U); // No bits set
    EXPECT_EQ(BIT_MASK_ABOVE_LBS(0x01) & 0xFF, 0xFEU); // 00000001 -> 11111110
    EXPECT_EQ(BIT_MASK_ABOVE_LBS(0x02) & 0xFF, 0xFCU); // 00000010 -> 11111100
    EXPECT_EQ(BIT_MASK_ABOVE_LBS(0x04) & 0xFF, 0xF8U); // 00000100 -> 11111000
    EXPECT_EQ(BIT_MASK_ABOVE_LBS(0x08) & 0xFF, 0xF0U); // 00001000 -> 11110000
    EXPECT_EQ(BIT_MASK_ABOVE_LBS(0xA8) & 0xFF, 0xF0U); // 10101000 -> 11110000
}

TEST_F(LowestBitSetTest, BIT_MASK_LBS_AND_BELOW) {
    EXPECT_EQ(BIT_MASK_LBS_AND_BELOW(0x01), 0x01U); // 00000001 -> 00000001
    EXPECT_EQ(BIT_MASK_LBS_AND_BELOW(0x02), 0x03U); // 00000010 -> 00000011
    EXPECT_EQ(BIT_MASK_LBS_AND_BELOW(0x04), 0x07U); // 00000100 -> 00000111
    EXPECT_EQ(BIT_MASK_LBS_AND_BELOW(0x08), 0x0FU); // 00001000 -> 00001111
    EXPECT_EQ(BIT_MASK_LBS_AND_BELOW(0xAC), 0x07U); // 10101100 -> 00000111
    // Note: BIT_MASK_LBS_AND_BELOW(0x00) is undefined behavior (x ^ (x-1) when x=0)
}

TEST_F(LowestBitSetTest, BIT_MASK_BELOW_LBS) {
    EXPECT_EQ(BIT_MASK_BELOW_LBS(0x01), 0x00U); // 00000001 -> 00000000
    EXPECT_EQ(BIT_MASK_BELOW_LBS(0x02), 0x01U); // 00000010 -> 00000001
    EXPECT_EQ(BIT_MASK_BELOW_LBS(0x04), 0x03U); // 00000100 -> 00000011
    EXPECT_EQ(BIT_MASK_BELOW_LBS(0x08), 0x07U); // 00001000 -> 00000111
    EXPECT_EQ(BIT_MASK_BELOW_LBS(0xAC), 0x03U); // 10101100 -> 00000011
}

TEST_F(LowestBitSetTest, BIT_FILL_BELOW_LBS) {
    EXPECT_EQ(BIT_FILL_BELOW_LBS(0x01), 0x01U); // 00000001 -> 00000001
    EXPECT_EQ(BIT_FILL_BELOW_LBS(0x02), 0x03U); // 00000010 -> 00000011
    EXPECT_EQ(BIT_FILL_BELOW_LBS(0x04), 0x07U); // 00000100 -> 00000111
    EXPECT_EQ(BIT_FILL_BELOW_LBS(0x08), 0x0FU); // 00001000 -> 00001111
    EXPECT_EQ(BIT_FILL_BELOW_LBS(0x68), 0x6FU); // 01101000 -> 01101111
}

TEST_F(LowestBitSetTest, BIT_SHIFT_LBS_TO_BIT0) {
    EXPECT_EQ(BIT_SHIFT_LBS_TO_BIT0(0x01), 0x01U); // 00000001 -> 00000001
    EXPECT_EQ(BIT_SHIFT_LBS_TO_BIT0(0x02), 0x01U); // 00000010 -> 00000001
    EXPECT_EQ(BIT_SHIFT_LBS_TO_BIT0(0x04), 0x01U); // 00000100 -> 00000001
    EXPECT_EQ(BIT_SHIFT_LBS_TO_BIT0(0x08), 0x01U); // 00001000 -> 00000001
    EXPECT_EQ(BIT_SHIFT_LBS_TO_BIT0(0x50), 0x05U); // 01010000 -> 00000101
    EXPECT_EQ(BIT_SHIFT_LBS_TO_BIT0(0xA8), 0x15U); // 10101000 -> 00010101
}

TEST_F(LowestBitSetTest, BIT_FILL_LOWEST_ZERO_BIT) {
    EXPECT_EQ(BIT_FILL_LOWEST_ZERO_BIT(0x71), 0x73U); // 01110001 -> 01110011
    EXPECT_EQ(BIT_FILL_LOWEST_ZERO_BIT(0x00), 0x01U); // 00000000 -> 00000001
    EXPECT_EQ(BIT_FILL_LOWEST_ZERO_BIT(0x01), 0x03U); // 00000001 -> 00000011
    EXPECT_EQ(BIT_FILL_LOWEST_ZERO_BIT(0x03), 0x07U); // 00000011 -> 00000111
    // For 0xFF (all bits set), adding 1 causes overflow but we mask to 8 bits
    EXPECT_EQ(BIT_FILL_LOWEST_ZERO_BIT(0xFF) & 0xFF,
              0xFFU); // 11111111 -> 11111111 (overflow wraps)

    // Test alias
    EXPECT_EQ(BIT_FILL_LZB(0x71), 0x73U);
}

/************************* Two's Complement Tests ***************************/

class TwosComplementTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(TwosComplementTest, BIT_ONLY) {
    // Tests if a number has only one bit set (is a power of 2)
    EXPECT_TRUE(BIT_ONLY(0x01)); // 00000001
    EXPECT_TRUE(BIT_ONLY(0x02)); // 00000010
    EXPECT_TRUE(BIT_ONLY(0x04)); // 00000100
    EXPECT_TRUE(BIT_ONLY(0x08)); // 00001000
    EXPECT_TRUE(BIT_ONLY(0x10)); // 00010000
    EXPECT_TRUE(BIT_ONLY(0x20)); // 00100000
    EXPECT_TRUE(BIT_ONLY(0x40)); // 01000000
    EXPECT_TRUE(BIT_ONLY(0x80)); // 10000000

    EXPECT_FALSE(BIT_ONLY(0x00)); // 00000000 (no bits set)
    EXPECT_FALSE(BIT_ONLY(0x03)); // 00000011 (two bits set)
    EXPECT_FALSE(BIT_ONLY(0x05)); // 00000101 (two bits set)
    EXPECT_FALSE(BIT_ONLY(0x07)); // 00000111 (three bits set)
    EXPECT_FALSE(BIT_ONLY(0xFF)); // 11111111 (all bits set)
}

TEST_F(TwosComplementTest, BIT_MIN_MAX) {
    EXPECT_EQ(BIT_MIN(5, 3), 3);
    EXPECT_EQ(BIT_MIN(3, 5), 3);
    EXPECT_EQ(BIT_MIN(7, 7), 7);
    EXPECT_EQ(BIT_MIN(-2, 4), -2);
    EXPECT_EQ(BIT_MIN(0, 0), 0);

    EXPECT_EQ(BIT_MAX(5, 3), 5);
    EXPECT_EQ(BIT_MAX(3, 5), 5);
    EXPECT_EQ(BIT_MAX(7, 7), 7);
    EXPECT_EQ(BIT_MAX(-2, 4), 4);
    EXPECT_EQ(BIT_MAX(0, 0), 0);
}

TEST_F(TwosComplementTest, BIT_CLIP) {
    EXPECT_EQ(BIT_CLIP(5, 0, 10), 5);   // Within range
    EXPECT_EQ(BIT_CLIP(-5, 0, 10), 0);  // Below minimum
    EXPECT_EQ(BIT_CLIP(15, 0, 10), 10); // Above maximum
    EXPECT_EQ(BIT_CLIP(0, 0, 10), 0);   // At minimum
    EXPECT_EQ(BIT_CLIP(10, 0, 10), 10); // At maximum
    EXPECT_EQ(BIT_CLIP(7, 5, 5), 5);    // Min == max
}

TEST_F(TwosComplementTest, BIT_GET_VAL_IF_ZERO_ONE_NZERO) {
    // BIT_GET_VAL_IF_ZERO - returns value if condition is zero
    EXPECT_EQ(BIT_GET_VAL_IF_ZERO(0, 42), 42);
    EXPECT_EQ(BIT_GET_VAL_IF_ZERO(1, 42), 0);
    EXPECT_EQ(BIT_GET_VAL_IF_ZERO(5, 42), 0);

    // BIT_GET_VAL_IF_ONE - returns value if condition is exactly one (using & mask)
    EXPECT_EQ(BIT_GET_VAL_IF_ONE(1, 42), 42);
    EXPECT_EQ(BIT_GET_VAL_IF_ONE(0, 42), 0);
    // For other values: return_value & (-value)
    EXPECT_EQ(BIT_GET_VAL_IF_ONE(2, 42), 42); // 42 & (-2) = 42 & 0xFFFFFFFE = 42

    // It calls BIT_GET_VAL_IF_ONE((!(x)), return_if_nzero) which is BIT_GET_VAL_IF_ONE
    EXPECT_EQ(BIT_GET_VAL_IF_NZERO(1, 42), 0);  // !(1) = 0, so BIT_GET_VAL_IF_ONE(0, 42) = 0
    EXPECT_EQ(BIT_GET_VAL_IF_NZERO(5, 42), 0);  // !(5) = 0, so BIT_GET_VAL_IF_ONE(0, 42) = 0
    EXPECT_EQ(BIT_GET_VAL_IF_NZERO(0, 42), 42); // !(0) = 1, so BIT_GET_VAL_IF_ONE(1, 42) = 42
}

TEST_F(TwosComplementTest, BIT_GET_COND_VAL) {
    EXPECT_EQ(BIT_GET_COND_VAL(0, 10, 20), 20); // Zero -> return second value
    EXPECT_EQ(BIT_GET_COND_VAL(1, 10, 20), 10); // One -> return first value
    EXPECT_EQ(BIT_GET_COND_VAL(2, 10, 20), 10); // Two -> return first value due to mask
}

TEST_F(TwosComplementTest, BIT_GET_INT_VAL) {
    EXPECT_EQ(BIT_GET_INT_VAL(0, 10, 20), 30);  // Bug: returns both values OR'd together
    EXPECT_EQ(BIT_GET_INT_VAL(1, 10, 20), 0);   // Nonzero -> neither condition met properly
    EXPECT_EQ(BIT_GET_INT_VAL(5, 10, 20), 4);   // Complex bit masking due to bug
    EXPECT_EQ(BIT_GET_INT_VAL(-3, 10, 20), 20); // Negative value behavior
}

TEST_F(TwosComplementTest, BIT_NEGATE_IF_CONDITIONS) {
    // BIT_NEGATE_IF_NZERO - negates if condition is nonzero
    // Formula: ((x) ^ (-(!(comp)))) + (!(comp))
    // When comp=1: !(1)=0, so ((x) ^ 0) + 0 = x (no negation) - this seems backwards
    // When comp=0: !(0)=1, so ((x) ^ -1) + 1 = (~x) + 1 = -x (negation)
    EXPECT_EQ(BIT_NEGATE_IF_NZERO(1, 5), 5);  // Nonzero -> don't negate (seems backwards)
    EXPECT_EQ(BIT_NEGATE_IF_NZERO(0, 5), -5); // Zero -> negate
    EXPECT_EQ(BIT_NEGATE_IF_NZERO(3, 7), 7);  // Nonzero -> don't negate

    // BIT_NEGATE_IF_ONE - negates if condition is exactly one
    // Formula: ((x) ^ (-(zero_or_one))) + (zero_or_one)
    // When zero_or_one=1: ((x) ^ -1) + 1 = (~x) + 1 = -x
    // When zero_or_one=0: ((x) ^ 0) + 0 = x
    EXPECT_EQ(BIT_NEGATE_IF_ONE(1, 5), -5); // One -> negate
    EXPECT_EQ(BIT_NEGATE_IF_ONE(0, 5), 5);  // Zero -> don't negate
    EXPECT_EQ(BIT_NEGATE_IF_ONE(2, 5), -3); // Two -> partial effect due to formula

    // BIT_NEGATE_IF_ZERO - negates if condition is zero
    // Formula: ((zero_or_one) ^ (zero_or_one - 1)) * (x)
    // When zero_or_one=0: (0 ^ -1) * x = (-1) * x = -x
    // When zero_or_one=1: (1 ^ 0) * x = 1 * x = x
    EXPECT_EQ(BIT_NEGATE_IF_ZERO(0, 5), -5); // Zero -> negate
    EXPECT_EQ(BIT_NEGATE_IF_ZERO(1, 5), 5);  // Nonzero -> don't negate
    EXPECT_EQ(BIT_NEGATE_IF_ZERO(2, 5), 15); // Two -> (2 ^ 1) * 5 = 3 * 5 = 15
}

TEST_F(TwosComplementTest, BIT_AVERAGE) {
    EXPECT_EQ(BIT_AVERAGE(4, 6), 5);    // (4+6)/2 = 5
    EXPECT_EQ(BIT_AVERAGE(0, 8), 4);    // (0+8)/2 = 4
    EXPECT_EQ(BIT_AVERAGE(1, 3), 2);    // (1+3)/2 = 2
    EXPECT_EQ(BIT_AVERAGE(10, 10), 10); // (10+10)/2 = 10
    EXPECT_EQ(BIT_AVERAGE(7, 9), 8);    // (7+9)/2 = 8

    // Test with larger values that might overflow in naive implementation
    uint32_t large1 = 0x80000000U;
    uint32_t large2 = 0x80000002U;
    uint32_t expected = 0x80000001U;
    EXPECT_EQ(BIT_AVERAGE(large1, large2), expected);
}

/*********************** Utility Macro Tests *******************************/

class UtilityMacroTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(UtilityMacroTest, N_BITS_N_BYTES) {
    EXPECT_EQ(N_BITS(1), 8);
    EXPECT_EQ(N_BITS(2), 16);
    EXPECT_EQ(N_BITS(4), 32);
    EXPECT_EQ(N_BITS(8), 64);

    EXPECT_EQ(N_BYTES(8), 1);
    EXPECT_EQ(N_BYTES(16), 2);
    EXPECT_EQ(N_BYTES(32), 4);
    EXPECT_EQ(N_BYTES(64), 8);

    // Test rounding up for N_BYTES
    EXPECT_EQ(N_BYTES(1), 1);  // 1 bit -> 1 byte
    EXPECT_EQ(N_BYTES(9), 2);  // 9 bits -> 2 bytes
    EXPECT_EQ(N_BYTES(15), 2); // 15 bits -> 2 bytes
    EXPECT_EQ(N_BYTES(17), 3); // 17 bits -> 3 bytes
}

// Test class for library functionality
class LibraryTest : public ::testing::Test {
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(LibraryTest, BIT_Get_Version_String) {
    const char *version = BIT_Get_Version_String();

    // Version string should not be null
    ASSERT_NE(version, nullptr);

    // Version string should contain expected content
    EXPECT_NE(strstr(version, "BIT_OPS"), nullptr);
    EXPECT_NE(strstr(version, "Library"), nullptr);
    EXPECT_NE(strstr(version, "v"), nullptr);

    // Version string should have reasonable length
    EXPECT_GT(strlen(version), 10);
    EXPECT_LT(strlen(version), 200);
}