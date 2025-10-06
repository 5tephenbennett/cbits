/**
 * @file bit32_ops.h
 * @brief 32-bit integer bit manipulation operations
 *
 * Copyright 2009-2010 Stephen M Bennett. All Rights Reserved.
 */

#ifndef BIT32_OPS_H_
#define BIT32_OPS_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "bit_ops.h"

/******************************** CONSTANTS **********************************/

/***************************** BASIC DATA TYPES ******************************/

/******************************* ENUMERATIONS ********************************/

/************************ STRUCTURE/UNION DATA TYPES *************************/

/********************************** MACROS ***********************************/

/*************************** FUNCTIONS PROTOTYPES ****************************/

/** @addtogroup BIT32
 * @{
 */

/** @brief Top bit position for 32-bit values (bit 31) */
#define BIT32_TOP_BIT (31)

/** @brief Number of bits in a 32-bit value */
#define BIT32_BITS (32)

/** @brief Size of string buffer for bitstring representation (32 bits + null terminator) */
#define BIT32_STRING_SIZE (BIT32_BITS + 1)

/** @brief Bitmask for the last element */
#define BIT32_BITMASK_LAST_ELEMENT (BIT32_BITS)

/** @brief Check if bit position is greater than 31 */
#define BIT32_MORETHAN_TOPBIT(bit) (0 != ((bit)&0xe0)) /* bit > 31  */

/** @brief Check if bit position is less than or equal to 31 */
#define BIT32_LESSTHAN_OR_EQUAL_TOPBIT(bit) (0 == ((bit)&0xe0)) /* bit <= 31 */

/** @brief Swap byte order (endianness) of a 32-bit value */
#define BIT32_SWAP_ENDIAN(x)                                                                       \
    ((x) = (((x) << 24) & 0xff000000) | (((x) >> 24) & 0x000000ff) | (((x) << 8) & 0x00ff0000) |   \
           (((x) >> 8) & 0x0000ff00))

/** @brief Fill all bytes with the same 8-bit value */
#define BIT32_FILL_BYTES(b) ((b)*0x01010101)

/** @brief Check if the value contains any zero bytes */
#define BIT32_HAS_ZERO_BYTE(x) (((x)-0x01010101UL) & ~(x)&0x80808080UL)

/** @brief Convenience macro to set a specific bit */
#define BIT32(bit) BIT32_Set(bit)

/**
 * @brief Set a specific bit position
 * @param bit Bit position to set (0-31)
 * @return Value with only the specified bit set
 * @note This function ensures that wrapping does not occur when trying to set a
 *       particular bit (e.g. "1<<32 != 1").
 *
 * @code
 * - BIT32_Set(3) returns 8 (0000 0000 0000 0000 0000 0000 0000 1000)
 * - BIT32_Set(31) returns 2147483648 (1000 0000 0000 0000 0000 0000 0000 0000)
 * @endcode
 */
uint32_t BIT32_Set(uint8_t bit);

/**
 * @brief Create a right-aligned bitmask
 * @param left_bit Leftmost bit position
 * @return Bitmask with bits 0 to left_bit set
 * @see BIT32_LBitmask
 *
 * @code
 *                        ____bit6
 *                       |  __bit4
 *                       | |
 * BIT32_RBitmask(4) == 0001 1111
 * BIT32_RBitmask(6) == 0111 1111
 * @endcode
 */
uint32_t BIT32_RBitmask(uint8_t left_bit);

/**
 * @brief Create a left-aligned bitmask
 * @param right_bit Rightmost bit position
 * @return Bitmask with bits from right_bit to 31 set
 * @see BIT32_RBitmask
 *
 * @code
 *                         ___bit5
 *                        | __bit4
 *                        ||
 * BIT32_LBitmask(4) == 1111 0000
 * BIT32_LBitmask(5) == 1110 0000
 * @endcode
 */
uint32_t BIT32_LBitmask(uint8_t right_bit);

/**
 * @brief Create a bitmask for a range of bits
 * @param start_bit Lowest (rightmost) bit to set.
 * @param end_bit Highest (leftmost) bit to set.
 * @return Bitmask with bits from start_bit to end_bit set
 *
 * @code
 *                          ____bit5
 *                         | ___bit4
 *                         ||  _bit3
 *                         || |
 * BIT32_Bitmask(4,5) == 0011 0000;
 * BIT32_Bitmask(5,4) == 0000 0000;  // note: wrong order of bits
 * BIT32_Bitmask(3,3) == 0000 1000;
 * @endcode
 */
uint32_t BIT32_Bitmask(uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Create a bitmask for n consecutive bits
 * @param start_bit Starting bit position
 * @param bit_num Number of consecutive bits
 * @return Bitmask with bit_num bits set starting from start_bit
 *
 * @code
 * BIT32_nBitmask(3, 2) == 0x18 (0001 1000 - 2 bits set starting at bit 3)
 * BIT32_nBitmask(4, 0) == 0x00 (0000 0000 - 0 bits set)
 * BIT32_nBitmask(5, 3) == 0xE0 (1110 0000 - 3 bits set starting at bit 5)
 * @endcode
 */
uint32_t BIT32_nBitmask(uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Check if a signed integer is negative
 * @param var Signed 32-bit value to test
 * @return TRUE if negative, FALSE if positive or zero
 * @note Should only be called for signed int32_t.
 * @see BIT32_Get_Sign
 *
 * @code
 * BIT32_isNeg(-5) == TRUE
 * BIT32_isNeg(0) == FALSE
 * BIT32_isNeg(2) == FALSE
 * @endcode
 */
BOOL BIT32_isNeg(int32_t var);

/**
 * @brief Get the sign of a signed integer
 * @param var Signed 32-bit value
 * @return -1 if negative, 1 if positive or zero
 * @note Should only be called for signed int32_t.
 * @see BIT32_isNeg
 *
 * @code
 * BIT32_Get_Sign(10) == 1
 * BIT32_Get_Sign(0) == 1
 * BIT32_Get_Sign(-5) == -1
 * @endcode
 */
int32_t BIT32_Get_Sign(int32_t var);

/**
 * @brief Count the number of set bits
 * @param var Value to count bits in
 * @return Number of bits set to 1
 * @note Based on http://graphics.stanford.edu/~seander/bithacks.html
 *
 * @code
 * BIT32_Count_Bits_Set(6) returns 2 (binary 110 has 2 bits set)
 * @endcode
 */
uint8_t BIT32_Count_Bits_Set(uint32_t var);

/**
 * @brief Check if value contains a zero byte
 * @param var Value to check
 * @return TRUE if any byte is 0x00
 *
 * @code
 * BIT32_Has_Zero_Byte(0x00FF0000) == TRUE
 * @endcode
 *
 * @note This function checks if any byte within the 32-bit value is zero.
 *    Uses efficient bit manipulation to detect zero bytes.
 */
BOOL BIT32_Has_Zero_Byte(uint32_t var);

/**
 * @brief Find the position of the first set bit
 * @param var Value to search
 * @return Position of the first (rightmost) set bit
 *
 * @code
 * BIT32_Get_First_Bit_Set(12) returns 2 (binary 1100, first set bit at position 2)
 * @endcode
 */
uint8_t BIT32_Get_First_Bit_Set(uint32_t var);

/**
 * @brief Sign-extend a value from a specific bit width
 * @param var Value to sign-extend
 * @param bit_num Width of the original value in bits
 * @return Sign-extended 32-bit value
 * @note Based on http://graphics.stanford.edu/~seander/bithacks.html
 *
 * @code
 * // top bit is 1 so if we do sign extension we get negative
 * BIT32_Sign_Extend(2, 2) [10] == -2 [1111 1111 1111 1111 1111 1111 1111 1110]
 *
 * // top bit is 0 so if we do sign extension we remain positive
 * BIT32_Sign_Extend(2, 3) [010] == 2 [0000 0000 0000 0000 0000 0000 0000 0010]
 * @endcode
 */
int32_t BIT32_Sign_Extend(uint32_t var, uint8_t bit_num);

/**
 * @brief Convert Gray code to binary
 * @param gray_code Gray code value to convert
 * @return Binary representation of the Gray code
 * @note From http://aggregate.org/MAGIC/
 */
uint32_t BIT32_Convert_Grey_Code(uint32_t gray_code);

/**
 * @brief Reverse the order of bits
 * @param var Value to reverse
 * @return Value with bits in reverse order
 *
 * @code
 * BIT32_Reverse(7) returns 0xE0000000 (0...0111 becomes 1110...0)
 * @endcode
 */
uint32_t BIT32_Reverse(uint32_t var);

/**
 * @brief Extract a single bit as a masked value
 * @param var Source value
 * @param bit Bit position to extract
 * @return Value with only the specified bit preserved
 *
 * @code
 *                                 _bit1
 *                                |
 *                         0000 0011  // 3
 *                         0000 00X0  // bitmask
 * BIT32_Mask_Bit(3, 1) == 0000 0010  // 2
 * @endcode
 */
uint32_t BIT32_Mask_Bit(uint32_t var, uint8_t bit);

/**
 * @brief Extract a range of bits as a masked value
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to get indexed from zero.
 * @param end_bit Highest (leftmost) bit to get indexed from zero.
 * @return Value with only the specified bit range preserved
 *
 * @code
 *                                     ___bit3
 *                                    |  _bit1
 *                                    | |
 *                              0000 1011  // 11
 *                              0000 XXX0  // bitmask
 * BIT32_Mask_Bits(11, 1, 3) == 0000 1010  // 10
 * @endcode
 */
uint32_t BIT32_Mask_Bits(uint32_t var, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Extract n consecutive bits as a masked value
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to get indexed from zero.
 * @param bit_num Number of consecutive bits
 * @return Value with only the specified bits preserved
 *
 * @code
 *                                      _bit2
 *                                     |
 *                               0001 0111  // 23
 *                               000X XX00  // bitmask (3 bits)
 * BIT32_Mask_nBits(23, 2, 3) == 0001 0100  // 20
 * @endcode
 */
uint32_t BIT32_Mask_nBits(uint32_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Get the value of a single bit
 * @param var Source value
 * @param bit Bit position to test
 * @return TRUE if bit is set, FALSE otherwise
 *
 * @code
 *                        0000 0010  // 2
 *                        0000 00X0  // bitmask
 *                                   // >> shifted
 * BIT32_Get_Bit(2, 1) == 0000 0001  // TRUE
 * @endcode
 */
BOOL BIT32_Get_Bit(uint32_t var, uint8_t bit);

/**
 * @brief Extract a range of bits as a right-aligned value
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to get indexed from zero.
 * @param end_bit Highest (leftmost) bit to get indexed from zero.
 * @return Extracted bits shifted to the right
 *
 * @code
 *                            0000 0110  // 6
 *                            0000 0XX0  // bitmask
 *                                       // >> shifted
 * BIT32_Get_Bits(6, 1, 2) == 0000 0011  // 3
 * @endcode
 */
uint32_t BIT32_Get_Bits(uint32_t var, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Extract n consecutive bits as a right-aligned value
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to get indexed from zero.
 * @param bit_num Number of consecutive bits
 * @return Extracted bits shifted to the right
 *
 * @code
 *                              0001 0111  // 23
 *                              000X XX00  // bitmask (3 bits)
 *                                         // >> shifted
 * BIT32_Get_nBits(23, 2, 3) == 0000 0101  // 5
 * @endcode
 */
uint32_t BIT32_Get_nBits(uint32_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Extract a range of bits as a sign-extended value
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to get indexed from zero.
 * @param end_bit Highest (leftmost) bit to get indexed from zero.
 * @return Sign-extended extracted bits
 *
 * @code
 *                                   0000 0110  // 6
 *                                   0000 0XX0  // bitmask
 *                                              // >> shifted
 *                                   XXXX XX11  // top-bit is set so sign extend
 * BIT32_Get_Bits_Signed(6, 1, 2) == 1111 1111  // -1
 * @endcode
 */
int32_t BIT32_Get_Bits_Signed(uint32_t var, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Extract n consecutive bits as a sign-extended value
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to get indexed from zero.
 * @param bit_num Number of consecutive bits
 * @return Sign-extended extracted bits
 *
 * @code
 *                                     0001 0111  // 23
 *                                     000X XX00  // bitmask (3 bits)
 *                                                // >> shifted
 *                                     XXXX X101  // top-bit set so sign extend
 * BIT32_Get_nBits_Signed(23, 2, 3) == 1111 1101  // -3
 * @endcode
 */
int32_t BIT32_Get_nBits_Signed(uint32_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Set a single bit
 * @param var Source value
 * @param bit Bit position to set
 * @return Value with the specified bit set
 *
 * @code
 * BIT32_Set_Bit(4, 1) returns 6 (0100 | 0010 = 0110)
 * @endcode
 */
uint32_t BIT32_Set_Bit(uint32_t var, uint8_t bit);

/**
 * @brief Set a range of bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param end_bit Highest (leftmost) bit to set indexed from zero.
 * @return Value with the specified bit range set
 *
 * @code
 * BIT32_Set_Bits(4, 0, 1) returns 7 (sets bits 0 and 1)
 * @endcode
 */
uint32_t BIT32_Set_Bits(uint32_t var, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Set n consecutive bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param bit_num Number of consecutive bits to set
 * @return Value with the specified bits set
 *
 * @code
 * BIT32_Set_nBits(4, 0, 1) returns 5 (sets 1 bit starting from bit 0)
 * @endcode
 */
uint32_t BIT32_Set_nBits(uint32_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Clear a single bit
 * @param var Source value
 * @param bit Bit position to clear
 * @return Value with the specified bit cleared
 *
 * @code
 * BIT32_Clear_Bit(6, 1) returns 4 (0110 & ~0010 = 0100)
 * @endcode
 */
uint32_t BIT32_Clear_Bit(uint32_t var, uint8_t bit);

/**
 * @brief Clear a range of bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param end_bit Highest (leftmost) bit to set indexed from zero.
 * @return Value with the specified bit range cleared
 *
 * @code
 * BIT32_Clear_Bits(7, 0, 1) returns 4 (clears bits 0 and 1)
 * @endcode
 */
uint32_t BIT32_Clear_Bits(uint32_t var, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Clear n consecutive bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param bit_num Number of consecutive bits to clear
 * @return Value with the specified bits cleared
 *
 * @code
 * BIT32_Clear_nBits(7, 0, 1) returns 6 (clears 1 bit starting from bit 0)
 * @endcode
 */
uint32_t BIT32_Clear_nBits(uint32_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Toggle a single bit
 * @param var Source value
 * @param bit Bit position to toggle
 * @return Value with the specified bit toggled
 *
 * @code
 * BIT32_Toggle_Bit(5, 2) returns 1 (0101 ^ 0100 = 0001)
 * @endcode
 */
uint32_t BIT32_Toggle_Bit(uint32_t var, uint8_t bit);

/**
 * @brief Toggle a range of bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param end_bit Highest (leftmost) bit to set indexed from zero.
 * @return Value with the specified bit range toggled
 *
 * @code
 * BIT32_Toggle_Bits(5, 0, 2) returns 2 (toggles bits 0, 1, and 2)
 * @endcode
 */
uint32_t BIT32_Toggle_Bits(uint32_t var, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Toggle n consecutive bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to toggle indexed from zero.
 * @param bit_num Number of consecutive bits to toggle
 * @return Value with the specified bits toggled
 *
 * @code
 * BIT32_Toggle_nBits(5, 0, 2) returns 6 (toggles 2 bits starting from bit 0)
 * @endcode
 */
uint32_t BIT32_Toggle_nBits(uint32_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Insert a single bit value
 * @param var Source value
 * @param new_bit New bit value (TRUE or FALSE)
 * @param bit Bit position to modify
 * @return Value with the specified bit set to new_bit
 *
 * @code
 * BIT32_Insert_Bit(8, 1, 1) returns 10 (sets bit 1)
 * @endcode
 */
uint32_t BIT32_Insert_Bit(uint32_t var, BOOL new_bit, uint8_t bit);

/**
 * @brief Insert a value into a range of bits
 * @param var Source value
 * @param new_bits New bits to insert
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param end_bit Highest (leftmost) bit to set indexed from zero.
 * @return Value with the specified bit range replaced
 *
 * @code
 * BIT32_Insert_Bits(8, 3, 1, 2) returns 14 (inserts 11 into bits 1-2)
 * @endcode
 */
uint32_t BIT32_Insert_Bits(uint32_t var, uint32_t new_bits, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Insert a value into n consecutive bits
 * @param var Source value
 * @param new_bits New bits to insert
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param bit_num Number of consecutive bits
 * @return Value with the specified bits replaced
 *
 * @code
 * BIT32_Insert_nBits(8, 3, 2, 2) returns 12 (inserts 11 into 2 bits starting at bit 2)
 * @endcode
 */
uint32_t BIT32_Insert_nBits(uint32_t var, uint32_t new_bits, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Create a binary string representation
 * @param var Value to convert
 * @param bitstring Output buffer (must be at least BIT32_STRING_SIZE chars)
 * @return Pointer to the bitstring buffer
 *
 * @code
 * - BIT32_Bitstring(6, string) returns "00000000000000000000000000000110"
 * - BIT32_BitstringBrief(7, string) returns "111"
 * @endcode
 */
char *BIT32_Bitstring(uint32_t var, char bitstring[BIT32_STRING_SIZE]);

/**
 * @brief Convert 32-bit value to binary string without leading zeros
 * @param var The 32-bit value to convert
 * @param bitstring Output buffer (must be at least BIT32_STRING_SIZE chars)
 * @return Pointer to the bitstring buffer
 */
char *BIT32_BitstringBrief(uint32_t var, char bitstring[BIT32_STRING_SIZE]);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* of file BIT32_OPS_H_ */

/******************************************************************************
 * End of file
 ******************************************************************************/
