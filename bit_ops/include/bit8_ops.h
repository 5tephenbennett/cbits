/**
 * @file bit8_ops.h
 * @brief 8-bit integer bit manipulation operations
 *
 * Copyright 2009-2010 Stephen M Bennett. All Rights Reserved.
 */

#ifndef BIT8_OPS_H_
#define BIT8_OPS_H_

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

/** @addtogroup BIT8
 * @{
 */

/** @brief Top bit position for 8-bit values (bit 7) */
#define BIT8_TOP_BIT (7)

/** @brief Number of bits in an 8-bit value */
#define BIT8_BITS (8)

/** @brief Size of string buffer for bitstring representation (8 bits + null terminator) */
#define BIT8_STRING_SIZE (BIT8_BITS + 1)

/** @brief Bitmask for the last element */
#define BIT8_BITMASK_LAST_ELEMENT (BIT8_BITS)

/** @brief Check if bit position is greater than 7 */
#define BIT8_MORETHAN_TOPBIT(bit) (0 != ((bit)&0xf8)) /* bit > 7 */

/** @brief Check if bit position is less than or equal to 7 */
#define BIT8_LESSTHAN_OR_EQUAL_TOPBIT(bit) (0 == ((bit)&0xf8)) /* bit <= 7 */

/** @brief Convenience macro to set a specific bit */
#define BIT8(bit) BIT8_Set(bit)

/**
 * @brief Set a specific bit position
 * @param bit Bit position to set (0-7)
 * @return Value with only the specified bit set
 * @note This function ensures that wrapping does not occur when trying to set a
 *       particular bit (e.g. "1<<32 != 1").
 *
 * @code
 * BIT8_Set(3) == 8   == 0000 1000
 * BIT8_Set(7) == 128 == 1000 0000
 * @endcode
 */
uint8_t BIT8_Set(uint8_t bit);

/**
 * @brief Create a right-aligned bitmask
 * @param left_bit Leftmost bit position
 * @return Bitmask with bits 0 to left_bit set
 * @see BIT8_LBitmask
 *
 * @code
 *                       ____bit6
 *                      |  __bit4
 *                      | |
 * BIT8_RBitmask(4) == 0001 1111
 * BIT8_RBitmask(6) == 0111 1111
 * @endcode
 */
uint8_t BIT8_RBitmask(uint8_t left_bit);

/**
 * @brief Create a left-aligned bitmask
 * @param right_bit Rightmost bit position
 * @return Bitmask with bits from right_bit to 7 set
 * @see BIT8_RBitmask
 *
 * @code
 *                       ___bit5
 *                      | __bit4
 *                      ||
 * BIT_LBitmask(4) == 1111 0000
 * BIT_LBitmask(5) == 1110 0000
 * @endcode
 */
uint8_t BIT8_LBitmask(uint8_t right_bit);

/**
 * @brief Create a bitmask for a range of bits
 * @param start_bit Lowest (rightmost) bit to set.
 * @param end_bit Highest (leftmost) bit to set.
 * @return Bitmask with bits from start_bit to end_bit set
 *
 * @code
 *                         ____bit5
 *                        | ___bit4
 *                        ||  _bit3
 *                        || |
 * BIT8_Bitmask(4,5) == 0011 0000;
 * BIT8_Bitmask(5,4) == 0000 0000;  // note: wrong order of bits
 * BIT8_Bitmask(3,3) == 0000 1000;
 * @endcode
 */
uint8_t BIT8_Bitmask(uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Create a bitmask for n consecutive bits
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param bit_num Number of consecutive bits
 * @return Bitmask with bit_num bits set starting from start_bit
 *
 * @code
 * BIT8_nBitmask(3, 2) == 0x18 (0001 1000 - 2 bits set starting at bit 3)
 * BIT8_nBitmask(4, 0) == 0x00 (0000 0000 - 0 bits set)
 * BIT8_nBitmask(5, 3) == 0xE0 (1110 0000 - 3 bits set starting at bit 5)
 * @endcode
 */
uint8_t BIT8_nBitmask(uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Check if a signed integer is negative
 * @param var Signed 8-bit value to test
 * @return TRUE if negative, FALSE if positive or zero
 * @note Should only be called for signed int8_t.
 * @see BIT8_Get_Sign
 *
 * @code
 * BIT8_isNeg(-5) == TRUE
 * BIT8_isNeg(0) == FALSE
 * BIT8_isNeg(2) == FALSE
 * @endcode
 */
BOOL BIT8_isNeg(int8_t var);

/**
 * @brief Get the sign of a signed integer
 * @param var Signed 8-bit value
 * @return -1 if negative, 1 if positive or zero
 * @note Should only be called for signed int8_t.
 * @see BIT8_isNeg
 *
 * @code
 * BIT8_Get_Sign(10) == 1
 * BIT8_Get_Sign(0) == 1
 * BIT8_Get_Sign(-5) == -1
 * @endcode
 */
int8_t BIT8_Get_Sign(int8_t var);

/**
 * @brief Count the number of set bits
 * @param var Value to count bits in
 * @return Number of bits set to 1
 * @note Based on http://graphics.stanford.edu/~seander/bithacks.html
 *
 * @code
 * BIT8_Count_Bits_Set(6) == 2 (0000 0110 has 2 bits set)
 * @endcode
 */
uint8_t BIT8_Count_Bits_Set(uint8_t var);

/**
 * @brief Check if value contains a zero byte
 * @param var Value to check
 * @return TRUE if var is 0x00
 *
 * @code
 * BIT8_Has_Zero_Byte(0x00) == TRUE
 * @endcode
 *
 * @note This function is somewhat trivial for 8-bit values as it simply checks if
 *    the value is zero. It is included for API consistency with larger bit-width
 *    functions.
 */
BOOL BIT8_Has_Zero_Byte(uint8_t var);

/**
 * @brief Find the position of the first set bit
 * @param var Value to search
 * @return Position of the first (rightmost) set bit
 *
 * @code
 * BIT8_Get_First_Bit_Set(12) == 2 (0000 1100, first set bit at position 2)
 * @endcode
 *
 * @note Based on http://graphics.stanford.edu/~seander/bithacks.html
 */
uint8_t BIT8_Get_First_Bit_Set(uint8_t var);

/**
 * @brief Sign-extend a value from a specific bit width
 * @param var Value to sign-extend
 * @param bit_num Width of the original value in bits
 * @return Sign-extended 8-bit value
 * @note Based on http://graphics.stanford.edu/~seander/bithacks.html
 *
 * @code
 * // top bit is 1 so if we do sign extension we get negative
 * BIT8_Sign_Extend(2, 2) [10] == -2 [1111 1110]
 *
 * // top bit is 0 so if we do sign extension we remain positive
 * BIT8_Sign_Extend(2, 3) [010] == 2 [0000 0010]
 * @endcode
 */
int8_t BIT8_Sign_Extend(uint8_t var, uint8_t bit_num);

/**
 * @brief Reverse the order of bits
 * @param var Value to reverse
 * @return Value with bits in reverse order
 *
 * @code
 * BIT8_Reverse(7 [0000 0111]) == 0xE0 [1110 0000]
 * @endcode
 */
uint8_t BIT8_Reverse(uint8_t var);

/**
 * @brief Extract a single bit as a masked value
 * @param var Source value
 * @param bit Bit position to extract
 * @return Value with only the specified bit preserved
 *
 * @code
 *                                _bit1
 *                               |
 *                        0000 0011  // 3
 *                        0000 00X0  // bitmask
 * BIT8_Mask_Bit(3, 1) == 0000 0010  // 2
 * @endcode
 */
uint8_t BIT8_Mask_Bit(uint8_t var, uint8_t bit);

/**
 * @brief Extract a range of bits as a masked value
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param end_bit Highest (leftmost) bit to set indexed from zero.
 * @return Value with only the specified bit range preserved
 *
 * @code
 *                                    __bit2
 *                                   | _bit1
 *                                   ||
 *                             0000 1011  // 11
 *                             0000 0XX0  // bitmask
 * BIT8_Mask_Bits(11, 1, 3) == 0000 0010  // 2
 * @endcode
 */
uint8_t BIT8_Mask_Bits(uint8_t var, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Extract n consecutive bits as a masked value
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param bit_num Number of consecutive bits
 * @return Value with only the specified bits preserved
 *
 * @code
 *                                    _bit2
 *                                   |
 *                             0001 0111  // 23
 *                             000X XX00  // bitmask (3 bits)
 * BIT8_Mask_nBits(7, 2, 3) == 0001 0100  // 20
 * @endcode
 */
uint8_t BIT8_Mask_nBits(uint8_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Get the value of a single bit
 * @param var Source value
 * @param bit Bit position to return status of
 * @return TRUE if bit is set, FALSE otherwise
 *
 * @code
 *                       0000 0010  // 2
 *                       0000 00X0  // bitmask
 *                                  // >> shifted
 * BIT8_Get_Bit(2, 1) == 0000 0001  // TRUE
 * @endcode
 */
BOOL BIT8_Get_Bit(uint8_t var, uint8_t bit);

/**
 * @brief Extract a range of bits as a right-aligned value
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to get indexed from zero.
 * @param end_bit Highest (leftmost) bit to get indexed from zero.
 * @return Extracted bits shifted to the right
 *
 * @code
 *                           0000 0110  // 6
 *                           0000 0XX0  // bitmask
 *                                      // >> shifted
 * BIT8_Get_Bits(6, 1, 2) == 0000 0011  // 3
 * @endcode
 */
uint8_t BIT8_Get_Bits(uint8_t var, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Extract n consecutive bits as a right-aligned value
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to get indexed from zero.
 * @param bit_num Number of consecutive bits
 * @return Extracted bits shifted to the right
 *
 * @code
 *                            0001 0111  // 23
 *                            000X XX00  // bitmask (3 bits)
 *                                       // >> shifted
 * BIT8_Get_nBits(7, 2, 3) == 0000 0101  // 5
 * @endcode
 */
uint8_t BIT8_Get_nBits(uint8_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Extract a range of bits as a sign-extended value
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to get indexed from zero.
 * @param end_bit Highest (leftmost) bit to get indexed from zero.
 * @return Sign-extended extracted bits
 *
 * @code
 *                                  0000 0110  // 6
 *                                  0000 0XX0  // bitmask
 *                                             // >> shifted
 *                                  XXXX XX11  // top-bit is set so sign extend
 * BIT8_Get_Bits_Signed(6, 1, 2) == 1111 1111  // -1
 * @endcode
 */
int8_t BIT8_Get_Bits_Signed(uint8_t var, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Extract n consecutive bits as a sign-extended value
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to get indexed from zero.
 * @param bit_num Number of consecutive bits
 * @return Sign-extended extracted bits
 *
 * @code
 *                                   0001 0111  // 23
 *                                   000X XX00  // bitmask (3 bits)
 *                                              // >> shifted
 *                                   XXXX X101  // top-bit set so sign extend
 * BIT8_Get_nBits_Signed(7, 2, 3) == 1111 1101  // -3
 * @endcode
 */
int8_t BIT8_Get_nBits_Signed(uint8_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Set a single bit
 * @param var Source value
 * @param bit Bit position to set
 * @return Value with the specified bit set
 *
 * @code
 *                              _bit1
 *                             |
 *                      0000 0100  // 4
 *                      0000 0010  // OR(|)
 * BIT8_Set_Bit(4,1) == 0000 0110  // 6
 * @endcode
 */
uint8_t BIT8_Set_Bit(uint8_t var, uint8_t bit);

/**
 * @brief Set a range of bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param end_bit Highest (leftmost) bit to set indexed from zero.
 * @return Value with the specified bit range set
 *
 * @code
 *                                     _bit0
 *                                    |
 *                            0000 0100  // 4
 *                            0000 0011  // OR(|)
 *  BIT8_Set_Bits(4, 0, 1) == 0000 0111  // 7
 * @endcode
 */
uint8_t BIT8_Set_Bits(uint8_t var, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Set n consecutive bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to set indexed from zero.
 * @param bit_num Number of consecutive bits to set
 * @return Value with the specified bits set
 *
 * @code
 *                                     _bit0
 *                                    |
 *                            0000 0100  // 4
 *                            0000 0001  // OR(|), 1 bit
 * BIT8_Set_nBits(4, 0, 1) == 0000 0101  // 5
 * @endcode
 */
uint8_t BIT8_Set_nBits(uint8_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Clear a single bit
 * @param var Source value
 * @param bit Bit position to clear
 * @return Value with the specified bit cleared
 *
 * @code
 *                                  _bit1
 *                                 |
 *                          0000 0110  // 6
 *                          1111 1101  // AND(&)
 * BIT8_Clear_Bit(6, 1) ==  0000 0100  // 4
 * @endcode
 */
uint8_t BIT8_Clear_Bit(uint8_t var, uint8_t bit);

/**
 * @brief Clear a range of bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to clear indexed from zero.
 * @param end_bit Highest (leftmost) bit to clear indexed from zero.
 * @return Value with the specified bit range cleared
 *
 * @code
 *                                       _bit0
 *                                      |
 *                              0000 0111  // 7
 *                              1111 1100  // AND(&)
 * BIT8_Clear_Bits(7, 0, 1) ==  0000 0100  // 4
 * @endcode
 */
uint8_t BIT8_Clear_Bits(uint8_t var, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Clear n consecutive bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to clear indexed from zero.
 * @param bit_num Number of consecutive bits to clear
 * @return Value with the specified bits cleared
 *
 * @code
 *                                        _bit0
 *                                       |
 *                               0000 0111  // 7
 *                               1111 1110  // AND(&)
 * BIT8_Clear_nBits(7, 0, 1) ==  0000 0110  // 6
 * @endcode
 */
uint8_t BIT8_Clear_nBits(uint8_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Toggle a single bit
 * @param var Source value
 * @param bit Bit position to toggle
 * @return Value with the specified bit toggled
 *
 * @code
 *                                 __bit2
 *                                |
 *                          0000 0101  // 5
 *                          0000 0100  // XOR(^)
 * BIT8_Toggle_Bit(5, 2) == 0000 0001  // 1
 * @endcode
 */
uint8_t BIT8_Toggle_Bit(uint8_t var, uint8_t bit);

/**
 * @brief Toggle a range of bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to toggle indexed from zero.
 * @param end_bit Highest (leftmost) bit to toggle indexed from zero.
 * @return Value with the specified bit range toggled
 *
 * @code
 *                                       __bit0
 *                                      |
 *                              0000 0101  // 5
 *                              0000 0111  // XOR(^)
 * BIT8_Toggle_Bits(5, 0, 2) == 0000 0010  // 2
 * @endcode
 */
uint8_t BIT8_Toggle_Bits(uint8_t var, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Toggle n consecutive bits
 * @param var Source value
 * @param start_bit Lowest (rightmost) bit to toggle indexed from zero.
 * @param bit_num Number of consecutive bits to toggle
 * @return Value with the specified bits toggled
 *
 * @code
 *                                        __bit0
 *                                       |
 *                               0000 0101  // 5
 *                               0000 0011  // XOR(^)
 * BIT8_Toggle_nBits(5, 0, 2) == 0000 0110  // 6
 * @endcode
 */
uint8_t BIT8_Toggle_nBits(uint8_t var, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Insert a single bit value
 * @param var Source value
 * @param new_bit New bit value (TRUE or FALSE)
 * @param bit Bit position to modify
 * @return Value with the specified bit set to new_bit
 *
 * @code
 *                                     __bit1
 *                                    |
 *                             0000 1000  // 8
 *                                    1
 * BIT8_Insert_Bit(8, 1, 1) == 0000 1010  // 1
 * @endcode
 */
uint8_t BIT8_Insert_Bit(uint8_t var, BOOL new_bit, uint8_t bit);

/**
 * @brief Insert a value into a range of bits
 * @param var Source value
 * @param new_bits New bits to insert
 * @param start_bit Lowest (rightmost) bit to insert indexed from zero.
 * @param end_bit Highest (leftmost) bit to insert indexed from zero.
 * @return Value with the specified bit range replaced
 *
 * @code
 *                                         __bit1
 *                                        |
 *                                 0000 1000  // 8
 *                                       11
 * BIT8_Insert_Bits(8, 3, 1, 2) == 0000 1110  // 14
 *
 *                                         __bit1
 *                                        |
 *                                 0000 1000  // 8
 *                                      011
 * BIT8_Insert_Bits(8, 3, 1, 3) == 0000 0110  // 6
 * @endcode
 */
uint8_t BIT8_Insert_Bits(uint8_t var, uint8_t new_bits, uint8_t start_bit, uint8_t end_bit);

/**
 * @brief Insert a value into n consecutive bits
 * @param var Source value
 * @param new_bits New bits to insert
 * @param start_bit Lowest (rightmost) bit to insert indexed from zero.
 * @param bit_num Number of consecutive bits
 * @return Value with the specified bits replaced
 *
 * @code
 *                                             __bit2
 *                                            |
 *                                      0000 1000  // 8
 *                                           11
 *     BIT8_Insert_nBits(8, 3, 2, 2) == 0000 1100  // 12
 *
 *                                              __bit2
 *                                             |
 *                                       0001 1000  // 24
 *                                          0 11
 *     BIT8_Insert_nBits(24, 3, 2, 2) == 0000 1100  // 12
 * @endcode
 */
uint8_t BIT8_Insert_nBits(uint8_t var, uint8_t new_bits, uint8_t start_bit, uint8_t bit_num);

/**
 * @brief Create a binary string representation with leading zeros
 * @param var Value to convert
 * @param bitstring Output buffer (must be at least BIT8_STRING_SIZE chars)
 * @return Pointer to the bitstring buffer
 *
 * Always produces fixed-width output with leading zeros for consistent formatting.
 * This is optimized for performance and predictable output length.
 *
 * @code
 * BIT8_Bitstring(6, string) == "00000110"
 * BIT8_Bitstring(255, string) == "11111111"
 * @endcode
 */
char *BIT8_Bitstring(uint8_t var, char bitstring[BIT8_STRING_SIZE]);

/**
 * @brief Create a brief binary string representation without leading zeros
 * @param var Value to convert
 * @param bitstring Output buffer (must be at least BIT8_STRING_SIZE chars)
 * @return Pointer to the bitstring buffer
 *
 * Produces minimal output without unnecessary leading zeros.
 * Output length varies based on the highest set bit.
 *
 * @code
 * BIT8_BitstringBrief(6, string) == "110"
 * BIT8_BitstringBrief(0, string) == "0"
 * @endcode
 */
char *BIT8_BitstringBrief(uint8_t var, char bitstring[BIT8_STRING_SIZE]);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* of file BIT8_OPS_H_ */

/******************************************************************************
 * End of file
 ******************************************************************************/
