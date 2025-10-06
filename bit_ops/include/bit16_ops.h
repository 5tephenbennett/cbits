/**
 * @file bit16_ops.h
 * @brief 16-bit integer bit manipulation operations
 *
 * Copyright 2009-2010 Stephen M Bennett. All Rights Reserved.
 */

#ifndef BIT16_OPS_H_
#define BIT16_OPS_H_

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

/** @addtogroup BIT16
 * @{
 */

/** @brief Top bit position for 16-bit values (bit 15) */
#define BIT16_TOP_BIT (15)

/** @brief Number of bits in a 16-bit value */
#define BIT16_BITS (16)

/** @brief Size of string buffer for bitstring representation (16 bits + null terminator) */
#define BIT16_STRING_SIZE (BIT16_BITS + 1)

/** @brief Bitmask for the last element */
#define BIT16_BITMASK_LAST_ELEMENT (BIT16_BITS)

/** @brief Check if bit position is greater than 15 */
#define BIT16_MORETHAN_TOPBIT(bit) (0 != ((bit)&0xf0)) /* bit > 15  */

/** @brief Check if bit position is less than or equal to 15 */
#define BIT16_LESSTHAN_OR_EQUAL_TOPBIT(bit) (0 == ((bit)&0xf0)) /* bit <= 15 */

/** @brief Swap byte order (endianness) of a 16-bit value */
#define BIT16_SWAP_ENDIAN(x) ((x) = (((x) << 8) & 0xff00) | (((x) >> 8) & 0x00ff))

/** @brief Fill both bytes with the same 8-bit value */
#define BIT16_FILL_BYTES(b) ((b)*0x0101)

/** @brief Check if the value contains any zero bytes */
#define BIT16_HAS_ZERO_BYTE(x) (((x)-0x0101) & ~(x)&0x8080)

/** @brief Convenience macro to set a specific bit */
#define BIT16(bit) BIT16_Set(bit)

/**
 * @brief Set a specific bit position
 * @param bit Bit position to set (0-15)
 * @return Value with only the specified bit set
 * @note This function ensures that wrapping does not occur when trying to set a
 *       particular bit (e.g. "1<<32 != 1").
 *
 * @code
 * BIT16_Set(3) returns 8 (0000 0000 0000 1000)
 * BIT16_Set(15) returns 32768 (1000 0000 0000 0000)
 * @endcode
 */
uint16_t BIT16_Set(uint8_t bit);

/**
 * @brief Create a right-aligned bitmask
 * @param left_bit Leftmost bit position
 * @return Bitmask with bits 0 to left_bit set
 * @see BIT16_LBitmask
 *
 * @code
 *                        ____bit6
 *                       |  __bit4
 *                       | |
 * BIT16_RBitmask(4) == 0001 1111
 * BIT16_RBitmask(6) == 0111 1111
 * @endcode
 */
uint16_t BIT16_RBitmask(uint8_t left_bit);

/**
 * @brief Create a left-aligned bitmask
 * @param right_bit Rightmost bit position
 * @return Bitmask with bits from right_bit to 15 set
 * @see BIT16_RBitmask
 *
 * @code
 *  E.g.
 *                           ___bit5
 *                          | __bit4
 *                          ||
 *     BIT_LBitmask(4) == 1111 0000
 *     BIT_LBitmask(5) == 1110 0000
 * @endcode
 *
 * @code
 * BIT16_LBitmask(4) returns 0xFFF0 (1111 1111 1111 0000)
 * BIT16_LBitmask(5) returns 0xFFE0 (1111 1111 1110 0000)
 * @endcode
 */
uint16_t BIT16_LBitmask(uint8_t right_bit);

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
 * BIT16_Bitmask(4,5) == 0011 0000;
 * BIT16_Bitmask(5,4) == 0000 0000;  // note: wrong order of bits
 * BIT16_Bitmask(3,3) == 0000 1000;
 * @endcode
 */
uint16_t BIT16_Bitmask(uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Create a bitmask for n consecutive bits
 * @param start_bit Starting bit position
 * @param bit_num Number of consecutive bits
 * @return Bitmask with bit_num bits set starting from start_bit
 *
 * @code
 * BIT16_nBitmask(3, 2) == 0x18 (0001 1000 - 2 bits set starting at bit 3)
 * BIT16_nBitmask(4, 0) == 0x00 (0000 0000 - 0 bits set)
 * BIT16_nBitmask(5, 3) == 0xE0 (1110 0000 - 3 bits set starting at bit 5)
 * @endcode
 */
uint16_t BIT16_nBitmask(uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Check if a signed integer is negative
 * @param var Signed 16-bit value to test
 * @return TRUE if negative, FALSE if positive or zero
 * @note Should only be called for signed int16_t.
 * @see BIT16_Get_Sign
 *
 * @code
 * BIT16_isNeg(-5) == TRUE
 * BIT16_isNeg(0) == FALSE
 * BIT16_isNeg(2) == FALSE
 * @endcode
 */
BOOL BIT16_isNeg(int16_t var);

/**
 * @brief Get the sign of a signed integer
 * @param var Signed 16-bit value
 * @return -1 if negative, 1 if positive or zero
 * @note Should only be called for signed int16_t.
 * @see BIT16_isNeg
 *
 * @code
 * BIT16_Get_Sign(10) == 1
 * BIT16_Get_Sign(0) == 1
 * BIT16_Get_Sign(-5) == -1
 * @endcode
 */
int16_t BIT16_Get_Sign(int16_t var);

/**
 * @brief Count the number of set bits
 * @param var Value to count bits in
 * @return Number of bits set to 1
 * @note Based on http://graphics.stanford.edu/~seander/bithacks.html
 *
 * @code
 * BIT16_Count_Bits_Set(6) returns 2 (0000 0000 0000 0110 has 2 bits set)
 * @endcode
 */
uint8_t BIT16_Count_Bits_Set(uint16_t var);

/**
 * @brief Check if value contains a zero byte
 * @param var Value to check
 * @return TRUE if any byte is 0x00
 *
 * @code
 * BIT16_Has_Zero_Byte(0x0600) == TRUE
 * @endcode
 *
 * @note This function checks if any byte within the 16-bit value is zero.
 *    It is included for API consistency with larger bit-width functions.
 */
BOOL BIT16_Has_Zero_Byte(uint16_t var);

/**
 * @brief Find the position of the first set bit
 * @param var Value to search
 * @return Position of the first (rightmost) set bit
 * @note From http://graphics.stanford.edu/~seander/bithacks.html
 *
 * @code
 * BIT16_Get_First_Bit_Set(12) returns 2 (0000 0000 0000 1100, first set bit at position 2)
 * @endcode
 */
uint8_t BIT16_Get_First_Bit_Set(uint16_t var);

/**
 * @brief Sign-extend a value from a specific bit width
 * @param var Value to sign-extend
 * @param bit_num Width of the original value in bits
 * @return Sign-extended 16-bit value
 * @note Based on http://graphics.stanford.edu/~seander/bithacks.html
 *
 * @code
 * // top bit is 1 so if we do sign extension we get negative
 * BIT16_Sign_Extend(2, 2) [10] == -2 [1111 1111 1111 1110]
 *
 * // top bit is 0 so if we do sign extension we remain positive
 * BIT16_Sign_Extend(2, 3) [010] == 2 [0000 0000 0000 0010]
 * @endcode
 */
int16_t BIT16_Sign_Extend(uint16_t var, uint8_t bit_num);

/**
 * @brief Reverse the order of bits
 * @param var Value to reverse
 * @return Value with bits in reverse order
 * @note Derived from http://graphics.stanford.edu/~seander/bithacks.html
 *
 * @code
 * BIT16_Reverse(7) returns 0xE000 (0000 0000 0000 0111 becomes 1110 0000 0000 0000)
 * @endcode
 */
uint16_t BIT16_Reverse(uint16_t var);

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
 * BIT16_Mask_Bit(3, 1) == 0000 0010  // 2
 * @endcode
 */
uint16_t BIT16_Mask_Bit(uint16_t var, uint8_t bit);

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
 * BIT16_Mask_Bits(11, 1, 3) == 0000 1010  // 10
 * @endcode
 */
uint16_t BIT16_Mask_Bits(uint16_t var, uint8_t start_bit, uint8_t end_bit);

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
 * BIT16_Mask_nBits(23, 2, 3) == 0001 0100  // 20
 * @endcode
 */
uint16_t BIT16_Mask_nBits(uint16_t var, uint8_t start_bit, uint8_t bit_num);

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
 * BIT16_Get_Bit(2, 1) == 0000 0001  // TRUE
 * @endcode
 */
BOOL BIT16_Get_Bit(uint16_t var, uint8_t bit);

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
 * BIT16_Get_Bits(6, 1, 2) == 0000 0011  // 3
 * @endcode
 */
uint16_t BIT16_Get_Bits(uint16_t var, uint8_t start_bit, uint8_t end_bit);

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
 * BIT16_Get_nBits(23, 2, 3) == 0000 0101  // 5
 * @endcode
 */
uint16_t BIT16_Get_nBits(uint16_t var, uint8_t start_bit, uint8_t bit_num);

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
 * BIT16_Get_Bits_Signed(6, 1, 2) == 1111 1111  // -1
 * @endcode
 */
int16_t BIT16_Get_Bits_Signed(uint16_t var, uint8_t start_bit, uint8_t end_bit);

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
 * BIT16_Get_nBits_Signed(23, 2, 3) == 1111 1101  // -3
 * @endcode
 */
int16_t BIT16_Get_nBits_Signed(uint16_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Set a single bit
 * @param var Source value
 * @param bit Bit position to set
 * @return Value with the specified bit set
 *
 * @code
 * BIT16_Set_Bit(4, 1) returns 6 (0000 0000 0000 0100 | 0000 0000 0000 0010 = 0000 0000 0000 0110)
 * @endcode
 */
uint16_t BIT16_Set_Bit(uint16_t var, uint8_t bit);

/**
 * @brief Set a range of bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param end_bit Highest (leftmost) bit to set indexed from zero.
 * @return Value with the specified bit range set
 *
 * @code
 * BIT16_Set_Bits(4, 0, 1) returns 7 (sets bits 0 and 1)
 * @endcode
 */
uint16_t BIT16_Set_Bits(uint16_t var, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Set n consecutive bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param bit_num Number of consecutive bits to set
 * @return Value with the specified bits set
 *
 * @code
 * BIT16_Set_nBits(4, 0, 1) returns 5 (sets 1 bit starting from bit 0)
 * @endcode
 */
uint16_t BIT16_Set_nBits(uint16_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Clear a single bit
 * @param var Source value
 * @param bit Bit position to clear
 * @return Value with the specified bit cleared
 *
 * @code
 * BIT16_Clear_Bit(6, 1) returns 4 (0000 0000 0000 0110 & ~0000 0000 0000 0010 = 0000 0000 0000
 * 0100)
 * @endcode
 */
uint16_t BIT16_Clear_Bit(uint16_t var, uint8_t bit);

/**
 * @brief Clear a range of bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param end_bit Highest (leftmost) bit to set indexed from zero.
 * @return Value with the specified bit range cleared
 *
 * @code
 * BIT16_Clear_Bits(7, 0, 1) returns 4 (clears bits 0 and 1)
 * @endcode
 */
uint16_t BIT16_Clear_Bits(uint16_t var, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Clear n consecutive bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param bit_num Number of consecutive bits to clear
 * @return Value with the specified bits cleared
 *
 * @code
 * BIT16_Clear_nBits(7, 0, 1) returns 6 (clears 1 bit starting from bit 0)
 * @endcode
 */
uint16_t BIT16_Clear_nBits(uint16_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Toggle a single bit
 * @param var Source value
 * @param bit Bit position to toggle
 * @return Value with the specified bit toggled
 *
 * @code
 * BIT16_Toggle_Bit(5, 2) returns 1 (0000 0000 0000 0101 ^ 0000 0000 0000 0100 = 0000 0000 0000
 * 0001)
 * @endcode
 */
uint16_t BIT16_Toggle_Bit(uint16_t var, uint8_t bit);

/**
 * @brief Toggle a range of bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param end_bit Highest (leftmost) bit to set indexed from zero.
 * @return Value with the specified bit range toggled
 *
 * @code
 * BIT16_Toggle_Bits(5, 0, 2) returns 2 (toggles bits 0, 1, and 2)
 * @endcode
 */
uint16_t BIT16_Toggle_Bits(uint16_t var, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Toggle n consecutive bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to toggle indexed from zero.
 * @param bit_num Number of consecutive bits to toggle
 * @return Value with the specified bits toggled
 *
 * @code
 * BIT16_Toggle_nBits(5, 0, 2) returns 6 (toggles 2 bits starting from bit 0)
 * @endcode
 */
uint16_t BIT16_Toggle_nBits(uint16_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Insert a single bit value
 * @param var Source value
 * @param new_bit New bit value (TRUE or FALSE)
 * @param bit Bit position to modify
 * @return Value with the specified bit set to new_bit
 *
 * @code
 * BIT16_Insert_Bit(8, 1, 1) returns 10 (sets bit 1)
 * @endcode
 */
uint16_t BIT16_Insert_Bit(uint16_t var, BOOL new_bit, uint8_t bit);

/**
 * @brief Insert a value into a range of bits
 * @param var Source value
 * @param new_bits New bits to insert
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param end_bit Highest (leftmost) bit to set indexed from zero.
 * @return Value with the specified bit range replaced
 *
 * @code
 * BIT16_Insert_Bits(8, 3, 1, 2) returns 14 (inserts 11 into bits 1-2)
 * @endcode
 */
uint16_t BIT16_Insert_Bits(uint16_t var, uint16_t new_bits, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Insert a value into n consecutive bits
 * @param var Source value
 * @param new_bits New bits to insert
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param bit_num Number of consecutive bits
 * @return Value with the specified bits replaced
 *
 * @code
 * BIT16_Insert_nBits(8, 3, 2, 2) returns 12 (inserts 11 into 2 bits starting at bit 2)
 * @endcode
 */
uint16_t BIT16_Insert_nBits(uint16_t var, uint16_t new_bits, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Create a binary string representation
 * @param var Value to convert
 * @param bitstring Output buffer (must be at least BIT16_STRING_SIZE chars)
 * @return Pointer to the bitstring buffer
 *
 * @code
 * BIT16_Bitstring(6, string) returns "0000000000000110"
 * BIT16_BitstringBrief(7, string) returns "111"
 * @endcode
 */
char *BIT16_Bitstring(uint16_t var, char bitstring[BIT16_STRING_SIZE]);

/**
 * @brief Convert 16-bit value to binary string without leading zeros
 * @param var The 16-bit value to convert
 * @param bitstring Output buffer (must be at least BIT16_STRING_SIZE chars)
 * @return Pointer to the bitstring buffer
 */
char *BIT16_BitstringBrief(uint16_t var, char bitstring[BIT16_STRING_SIZE]);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* of file BIT16_OPS_H_ */

/******************************************************************************
 * End of file
 ******************************************************************************/
