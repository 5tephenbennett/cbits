/**
 * @file bit_ops.h
 * @brief General bit manipulation operations
 *
 * Copyright 2009-2010 Stephen M Bennett. All Rights Reserved.
 */

#ifndef BIT_OPS_H_
#define BIT_OPS_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <stdint.h>

/**
 * @brief Conversion reference table for decimal, octal, hexadecimal, and binary
 *
 * This lookup table provides quick reference for number base conversions
 * commonly used in bit manipulation operations.
 *
 * @code
 * dec  oct hex  bin
 * ==================
 *  0    00 0x0  0000
 *  1    01 0x1  0001
 *  2    02 0x2  0010
 *  3    03 0x3  0011
 *  4    04 0x4  0100
 *  5    05 0x5  0101
 *  6    06 0x6  0110
 *  7    07 0x7  0111
 *  8   010 0x8  1000
 *  9   011 0x9  1001
 * 10   012 0xa  1010
 * 11   013 0xb  1011
 * 12   014 0xc  1100
 * 13   015 0xd  1101
 * 14   016 0xe  1110
 * 15   017 0xf  1111
 * @endcode
 */

/**
 * @defgroup basic_types Types
 * @brief Core type definitions for bit manipulation operations
 * @{
 */

/**
 * @brief Boolean type definition for bit operations
 *
 * Defines a consistent boolean type across the library.
 * Uses unsigned char for maximum compatibility.
 */
typedef unsigned char BOOL;

/**
 * @brief Boolean false value
 *
 * Standard false value for boolean operations.
 */
#ifndef FALSE
#define FALSE (0)
#endif /* #ifndef FALSE */

/**
 * @brief Boolean true value
 *
 * Standard true value for boolean operations.
 * Defined as logical NOT of FALSE for consistency.
 */
#ifndef TRUE
#define TRUE (!FALSE)
#endif /* #ifndef TRUE */

/** @} */ /* end of basic_types group */

/** @addtogroup API
 * @{
 */

/**
 * @brief Number of bits in a byte
 *
 * Standard constant defining 8 bits per byte.
 */
#define BITS_PER_BYTE (8)

/**
 * @brief Top bit position in a byte (zero-indexed)
 *
 * The highest bit position in a byte (bit 7).
 */
#define TOPBIT_IN_BYTE (7)

/**
 * @brief Calculate number of bits for a given number of bytes
 * @param bytes Number of bytes
 * @return Number of bits (bytes * 8)
 *
 * @code
 * int bits = N_BITS(4); // returns 32
 * @endcode
 */
#define N_BITS(bytes) ((bytes)*BITS_PER_BYTE)

/**
 * @brief Calculate number of bytes needed to hold a given number of bits
 * @param bits Number of bits
 * @return Number of bytes needed (rounded up)
 *
 * @code
 * int bytes = N_BYTES(17); // returns 3
 * @endcode
 */
#define N_BYTES(bits) (((bits) + TOPBIT_IN_BYTE) / BITS_PER_BYTE)

/**
 * @brief Get the number of bits in a variable's type
 * @param x Variable of any integer type
 * @return Number of bits in the variable's type
 *
 * @code
 * uint32_t val = 0;
 * int bits = BIT_WORDBITS(val); // returns 32
 * @endcode
 */
#define BIT_WORDBITS(x) (N_BITS(sizeof(x)))

/**
 * @brief Get the position of the top bit in a variable's type
 * @param x Variable of any integer type
 * @return Position of the highest bit (zero-indexed)
 *
 * @code
 * uint32_t val = 0;
 * int topbit = BIT_TOPBIT(val); // returns 31
 * @endcode
 */
#define BIT_TOPBIT(x) (BIT_WORDBITS(x) - 1)

/**
 * @brief Bitwise complement of a value
 * @param x Input value
 * @return Bitwise NOT of x
 *
 * @code
 * uint8_t result = BIT_COMPLIMENT(0x5D); // 0x5D -> 0xA2
 * @endcode
 */
#define BIT_COMPLIMENT(x) (~(x))

/**
 * @brief Two's complement negation of a value
 * @param x Input value
 * @return Negative of x using two's complement
 *
 * @code
 * int8_t result = BIT_NEGATE(2); // 2 -> -2 (0xFE)
 * @endcode
 */
#define BIT_NEGATE(x) (-(x))

/**
 * @brief Swap two variables using XOR operations
 * @param a First variable (modified)
 * @param b Second variable (modified)
 *
 * Swaps the values of two variables without using a temporary variable.
 *
 * @warning Variables must be different (not aliases) to avoid undefined behavior.
 *
 * @code
 * int x = 5, y = 10;
 * BIT_SWAP(x, y); // x = 10, y = 5
 * @endcode
 */
#ifndef BIT_SWAP
#define BIT_SWAP(a, b)                                                                             \
    do {                                                                                           \
        (a) ^= (b);                                                                                \
        (b) ^= (a);                                                                                \
        (a) ^= (b);                                                                                \
    } while (0)
#endif

/**
 * @brief Extract the lowest bit that is set
 * @param x Input value
 * @return Value with only the lowest set bit
 *
 * @code
 * uint8_t result = BIT_GET_LOWEST_BIT_SET(0xB4); // 0xB4 -> 0x04
 * @endcode
 */
#define BIT_GET_LOWEST_BIT_SET(x) ((x) & (-(x)))

/**
 * @brief Alias for BIT_GET_LOWEST_BIT_SET
 */
#define BIT_GET_LBS BIT_GET_LOWEST_BIT_SET

/**
 * @brief Remove the lowest bit that is set
 * @param x Input value
 * @return Value with lowest set bit cleared
 *
 * @code
 * uint8_t result = BIT_STRIP_LBS(0xB6); // 0xB6 -> 0xB4
 * @endcode
 */
#define BIT_STRIP_LBS(x) ((x) & ((x)-1))

/**
 * @brief Create mask for bits above and including the lowest bit set
 * @param x Input value
 * @return Bitmask covering lowest set bit and all bits above
 *
 * @code
 * uint8_t mask = BIT_MASK_LBS_AND_ABOVE(0xA8); // 0xA8 -> 0xF8
 * @endcode
 */
#define BIT_MASK_LBS_AND_ABOVE(x) ((x) | (-(x)))

/**
 * @brief Create mask for bits above (not including) the lowest bit set
 * @param x Input value
 * @return Bitmask covering all bits above the lowest set bit
 *
 * @code
 * uint8_t mask = BIT_MASK_ABOVE_LBS(0xA8); // 0xA8 -> 0xF0
 * @endcode
 */
#define BIT_MASK_ABOVE_LBS(x) ((x) ^ (-(x)))

/**
 * @brief Create mask for bits below and including the lowest bit set
 * @param x Input value
 * @return Bitmask covering lowest set bit and all bits below
 *
 * @code
 * uint8_t mask = BIT_MASK_LBS_AND_BELOW(0xAC); // 0xAC -> 0x07
 * @endcode
 */
#define BIT_MASK_LBS_AND_BELOW(x) ((x) ^ ((x)-1))

/**
 * @brief Create mask for bits below (not including) the lowest bit set
 * @param x Input value
 * @return Bitmask covering all bits below the lowest set bit
 *
 * @code
 * uint8_t mask = BIT_MASK_BELOW_LBS(0xAC); // 0xAC -> 0x03
 * @endcode
 */
#define BIT_MASK_BELOW_LBS(x) ((~(x)) & ((x)-1))

/**
 * @brief Fill in all bits below the lowest bit set
 * @param x Input value
 * @return Value with all bits below lowest set bit filled
 *
 * @code
 * uint8_t result = BIT_FILL_BELOW_LBS(0x68); // 0x68 -> 0x6F
 * @endcode
 */
#define BIT_FILL_BELOW_LBS(x) ((x) | ((x)-1))

/**
 * @brief Shift value so lowest set bit ends up at bit position 0
 * @param x Input value
 * @return Right-shifted value with lowest set bit at position 0
 *
 * @code
 * uint8_t result = BIT_SHIFT_LBS_TO_BIT0(0x50); // 0x50 -> 0x05
 * @endcode
 */
#define BIT_SHIFT_LBS_TO_BIT0(x) ((x) / ((x) & (-(x))))

/**
 * @brief Fill in the lowest zero bit
 * @param x Input value
 * @return Value with lowest zero bit set
 *
 * @code
 * uint8_t result = BIT_FILL_LOWEST_ZERO_BIT(0x71); // 0x71 -> 0x73
 * @endcode
 */
#define BIT_FILL_LOWEST_ZERO_BIT(x) ((x) | ((x) + 1))

/**
 * @brief Alias for BIT_FILL_LOWEST_ZERO_BIT
 */
#define BIT_FILL_LZB BIT_FILL_LOWEST_ZERO_BIT

#ifdef ARCH_TWOS_COMPLEMENT

/**
 * @brief Check if value has exactly one bit set (is power of 2)
 * @param x Input value
 * @return Non-zero if exactly one bit is set, zero otherwise
 *
 * Fast two's complement implementation.
 */
#define BIT_ONLY(x) ((!((x) & ((x)-1))) && (x))

/**
 * @brief Return minimum of two values without branching
 * @param a First value
 * @param b Second value
 * @return Minimum of a and b
 *
 * Uses two's complement arithmetic for branchless comparison.
 */
#define BIT_MIN(a, b) ((b) + (((a) - (b)) & -((a) < (b))))

/**
 * @brief Return maximum of two values without branching
 * @param a First value
 * @param b Second value
 * @return Maximum of a and b
 *
 * Uses two's complement arithmetic for branchless comparison.
 */
#define BIT_MAX(a, b) ((a) - (((a) - (b)) & -((a) < (b))))

/**
 * @brief Clamp value to specified range without branching
 * @param a Value to clamp
 * @param min Minimum allowed value
 * @param max Maximum allowed value
 * @return Value clamped to [min, max] range
 */
#define BIT_CLIP(a, min, max) (BIT_MIN(BIT_MAX((a), (min)), (max)))

/**
 * @brief Return value if input is non-zero, otherwise return 0
 * @param x Test value
 * @param return_if_nzero Value to return if x != 0
 * @return return_if_nzero if x != 0, otherwise 0
 */
#define BIT_GET_VAL_IF_NZERO(x, return_if_nzero) (BIT_GET_VAL_IF_ONE((!(x)), return_if_nzero))

/**
 * @brief Return value if input equals 1, otherwise return 0
 * @param zero_or_one Test value (should be 0 or 1)
 * @param return_if_one Value to return if input == 1
 * @return return_if_one if input == 1, otherwise 0
 */
#define BIT_GET_VAL_IF_ONE(zero_or_one, return_if_one) ((return_if_one) & (-(zero_or_one)))

/**
 * @brief Return value if input equals 0, otherwise return 0
 * @param zero_or_one Test value (should be 0 or 1)
 * @param return_if_zero Value to return if input == 0
 * @return return_if_zero if input == 0, otherwise 0
 */
#define BIT_GET_VAL_IF_ZERO(zero_or_one, return_if_zero) ((return_if_zero) & (~(-(zero_or_one))))

/**
 * @brief Conditional value selection without branching
 * @param zero_or_one Condition (should be 0 or 1)
 * @param return_if_one Value to return if condition == 1
 * @param return_if_zero Value to return if condition == 0
 * @return Appropriate value based on condition
 */
#define BIT_GET_COND_VAL(zero_or_one, return_if_one, return_if_zero)                               \
    (BIT_GET_VAL_IF_ZERO(zero_or_one, return_if_zero) |                                            \
     BIT_GET_VAL_IF_ONE(zero_or_one, return_if_one))

/**
 * @brief Return one value if non-zero, another if zero
 * @param x Test value
 * @param return_if_nzero Value to return if x != 0
 * @param return_if_zero Value to return if x == 0
 * @return Appropriate value based on x
 */
#define BIT_GET_INT_VAL(x, return_if_nzero, return_if_zero)                                        \
    (BIT_GET_VAL_IF_ZERO(x, return_if_zero) | BIT_GET_VAL_IF_NZERO(x, return_if_nzero))

/**
 * @brief Conditionally negate a value if condition is non-zero
 * @param comp Condition value
 * @param x Value to potentially negate
 * @return -x if comp != 0, otherwise x
 */
#define BIT_NEGATE_IF_NZERO(comp, x) (((x) ^ (-(!(comp)))) + (!(comp)))

/**
 * @brief Conditionally negate a value if condition equals 1
 * @param zero_or_one Condition (should be 0 or 1)
 * @param x Value to potentially negate
 * @return -x if condition == 1, otherwise x
 */
#define BIT_NEGATE_IF_ONE(zero_or_one, x) (((x) ^ (-(zero_or_one))) + (zero_or_one))

/**
 * @brief Conditionally negate a value if condition equals 0
 * @param zero_or_one Condition (should be 0 or 1)
 * @param x Value to potentially negate
 * @return -x if condition == 0, otherwise x
 */
#define BIT_NEGATE_IF_ZERO(zero_or_one, x) (((zero_or_one) ^ ((zero_or_one)-1)) * (x))

#ifdef SIGNED_SHIFTS
/**
 * @brief Calculate average of two values without overflow (signed shift version)
 * @param a First value
 * @param b Second value
 * @return Average of a and b
 *
 * Uses signed right shifts for efficiency. Requires SIGNED_SHIFTS support.
 */
#define BIT_AVERAGE(a, b) (((a) & (b)) + (((a) ^ (b)) >> 1))
#else
/**
 * @brief Calculate average of two values without overflow (portable version)
 * @param a First value
 * @param b Second value
 * @return Average of a and b
 *
 * Portable implementation that doesn't rely on signed right shift behavior.
 */
#define BIT_AVERAGE(a, b) (((a) & (b)) + (((a) ^ (b)) / 2))
#endif

#else /* !ARCH_TWOS_COMPLEMENT */

/**
 * @brief Portable check if value has exactly one bit set
 * @param x Input value
 * @return Non-zero if exactly one bit is set, zero otherwise
 */
#define BIT_ONLY(x) (1 == BIT32_Count_Bits_Set(x))

/**
 * @brief Portable minimum function
 * @param a First value
 * @param b Second value
 * @return Minimum of a and b
 */
#define BIT_MIN(a, b) ((a < b) ? a : b)

/**
 * @brief Portable maximum function
 * @param a First value
 * @param b Second value
 * @return Maximum of a and b
 */
#define BIT_MAX(a, b) ((a > b) ? a : b)

/**
 * @brief Portable clamp function
 * @param a Value to clamp
 * @param min Minimum value
 * @param max Maximum value
 * @return Value clamped to range
 */
#define BIT_CLIP(a, min, max) ((a) < (min) ? (min) : ((a) > (max) ? (max) : (a)))

/* Portable conditional value selection */
#define BIT_GET_VAL_IF_NZERO(x, return_if_nzero) ((x) ? (return_if_nzero) : 0)
#define BIT_GET_VAL_IF_ONE(x, return_if_one) ((1 == (x)) ? (return_if_one) : 0)
#define BIT_GET_VAL_IF_ZERO(x, return_if_zero) ((0 == (x)) ? (return_if_zero) : 0)
#define BIT_GET_COND_VAL(x, return_if_one, return_if_zero)                                         \
    ((1 == (x)) ? (return_if_one) : ((0 == (x)) ? (return_if_zero) : 0))
#define BIT_GET_INT_VAL(x, return_if_nzero, return_if_zero)                                        \
    ((x) ? (return_if_nzero) : (return_if_zero))

/* Portable conditional negation */
#define BIT_NEGATE_IF_NZERO(comp, x) ((comp) ? (-(x)) : (x))
#define BIT_NEGATE_IF_ONE(zero_or_one, x) (BIT_NEGATE_IF_NZERO((1 == (zero_or_one)), (x)))
#define BIT_NEGATE_IF_ZERO(zero_or_one, x) (BIT_NEGATE_IF_NZERO((0 == (zero_or_one)), (x)))

/**
 * @brief Portable average calculation (may overflow)
 * @param a First value
 * @param b Second value
 * @return Average of a and b
 *
 * @warning This implementation may overflow for large values.
 */
#define BIT_AVERAGE(a, b) (((a) + (b)) / 2)

#endif /* ARCH_TWOS_COMPLEMENT */

#ifdef COUNT_BITS_LOOKUP
/**
 * @brief Lookup table for counting bits set in a byte
 *
 * This table provides O(1) bit counting for 8-bit values.
 * Each index corresponds to a byte value (0-255) and contains
 * the number of bits set in that byte.
 *
 * Used by optimized bit counting functions when COUNT_BITS_LOOKUP is defined.
 *
 * @see BIT8_Count_Bits_Set, BIT16_Count_Bits_Set, BIT32_Count_Bits_Set, BIT64_Count_Bits_Set
 */
extern const uint8_t bits_set_table256[256];
#endif

/**
 * @brief Compile-time literal type selection based on bit position
 * @param bit Bit position
 * @return Appropriate literal (1U for <32 bits, 1ULL for >=32 bits)
 *
 * Automatically selects the correct literal type to avoid overflow
 * when creating bit masks for high bit positions.
 */
#define BIT_LITERAL_1(bit) ((bit) < 32 ? 1U : (bit) < 64 ? 1ULL : 1ULL)

/**
 * @brief Safe bit shifting with automatic type selection and overflow protection
 * @param bit Bit position to shift to
 * @return 1 shifted left by bit positions, or 0 if bit >= 64
 *
 * Provides safe bit shifting that:
 * - Returns 0 for bit positions >= 64 (overflow protection)
 * - Uses appropriate type (32-bit vs 64-bit) based on bit position
 * - Prevents undefined behavior from excessive shifts
 *
 * @code
 * uint64_t mask = BIT_SAFE_SHIFT(35); // Returns 1ULL << 35
 * uint64_t zero = BIT_SAFE_SHIFT(70); // Returns 0 (overflow protection)
 * @endcode
 */
#define BIT_SAFE_SHIFT(bit) ((bit) >= 64 ? 0ULL : (bit) < 32 ? (1U << (bit)) : (1ULL << (bit)))

/**
 * @brief Safe lower bitmask creation with edge case handling
 * @param top_bit Highest bit position to include (inclusive)
 * @return Bitmask with bits 0 through top_bit set
 *
 * Creates a bitmask with all bits from 0 up to and including top_bit set.
 * Handles the edge case where top_bit+1 == 64 which would cause overflow.
 *
 * @code
 * uint32_t mask = BIT_SAFE_LOWER_MASK(4); // Returns 0x1F (bits 0-4 set)
 * uint64_t mask = BIT_SAFE_LOWER_MASK(63); // Returns 0xFFFFFFFFFFFFFFFF
 * @endcode
 */
#define BIT_SAFE_LOWER_MASK(top_bit)                                                               \
    ((top_bit) >= 63                                                                               \
         ? 0xFFFFFFFFFFFFFFFFULL                                                                   \
         : (top_bit) < 31 ? ((1U << ((top_bit) + 1U)) - 1U) : ((1ULL << ((top_bit) + 1U)) - 1ULL))

/**
 * @brief Safe upper bitmask creation with underflow protection
 * @param bottom_bit Lowest bit position to include (inclusive)
 * @return Bitmask with bits from bottom_bit to the top set
 *
 * Creates a bitmask with all bits from bottom_bit to the highest bit set.
 * Handles the edge case where bottom_bit == 0 to prevent underflow.
 *
 * @code
 * uint32_t mask = BIT_SAFE_UPPER_MASK(4); // Returns 0xFFFFFFF0 (bits 4+ set)
 * uint64_t mask = BIT_SAFE_UPPER_MASK(0); // Returns 0xFFFFFFFFFFFFFFFF
 * @endcode
 */
#define BIT_SAFE_UPPER_MASK(bottom_bit)                                                            \
    ((bottom_bit) == 0 ? 0xFFFFFFFFFFFFFFFFULL : ~BIT_SAFE_LOWER_MASK((bottom_bit)-1U))

/**
 * @brief Create a value with the specified bit set
 * @param bit Bit position (0-63)
 * @return Value with only the specified bit set
 *
 * @code
 * uint32_t mask = BIT_X(3); // Returns 8 (0x8)
 * @endcode
 */
#define BIT_X(bit) BIT_SAFE_SHIFT(bit)

/**
 * @brief Extract only the specified bit from a value
 * @param x Input value
 * @param bit Bit position to extract
 * @return Input value with only the specified bit preserved
 *
 * @code
 * uint32_t result = BIT_MASK_BIT(0x0F, 3); // Returns 8 (bit 3 of 0x0F)
 * @endcode
 */
#define BIT_MASK_BIT(x, bit) ((x)&BIT_X(bit))

/**
 * @brief Get the value of a specific bit (0 or 1)
 * @param x Input value
 * @param bit Bit position to test
 * @return 1 if bit is set, 0 if bit is clear
 *
 * @code
 * int bit_val = BIT_GET_BIT(0x0A, 1); // Returns 1 (bit 1 of 0x0A is set)
 * @endcode
 */
#define BIT_GET_BIT(x, bit) (((x) >> (bit)) & 1U)

/**
 * @brief Set a specific bit in a value
 * @param x Input value
 * @param bit Bit position to set
 * @return Value with the specified bit set
 *
 * @code
 * uint32_t result = BIT_SET_BIT(0x04, 1); // Returns 6 (0x06)
 * @endcode
 */
#define BIT_SET_BIT(x, bit) ((x) | BIT_X(bit))

/**
 * @brief Clear a specific bit in a value
 * @param x Input value
 * @param bit Bit position to clear
 * @return Value with the specified bit cleared
 *
 * @code
 * uint32_t result = BIT_CLEAR_BIT(0x06, 1); // Returns 4 (0x04)
 * @endcode
 */
#define BIT_CLEAR_BIT(x, bit) ((x) & (~BIT_X(bit)))

/**
 * @brief Toggle a specific bit in a value
 * @param x Input value
 * @param bit Bit position to toggle
 * @return Value with the specified bit toggled
 *
 * @code
 * uint32_t result = BIT_TOGGLE_BIT(0x05, 2); // Returns 1 (0x01)
 * @endcode
 */
#define BIT_TOGGLE_BIT(x, bit) ((x) ^ BIT_X(bit))

/**
 * @brief Change a specific bit to a new value
 * @param x Input value
 * @param new_bit New bit value (0 or 1)
 * @param bit Bit position to change
 * @return Value with the specified bit changed to new_bit
 *
 * @code
 * uint32_t result = BIT_CHANGE_BIT(0x08, 1, 1); // Returns 10 (0x0A)
 * @endcode
 */
#define BIT_CHANGE_BIT(x, new_bit, bit)                                                            \
    (BIT_CLEAR_BIT((x), (bit)) |                                                                   \
     ((bit) < 32 ? (((new_bit)&1U) << (bit)) : (((new_bit)&1ULL) << (bit))))

/**
 * @brief Create a bitmask with bits 0 through top_bit set
 * @param top_bit Highest bit position (inclusive)
 * @return Bitmask with lower bits set
 *
 * @code
 * uint32_t mask = BIT_LOWER_BITMASK(4); // Returns 0x1F (bits 0-4 set)
 * @endcode
 */
#define BIT_LOWER_BITMASK(top_bit) BIT_SAFE_LOWER_MASK(top_bit)

/**
 * @brief Create a bitmask with bits from bottom_bit to top set
 * @param bottom_bit Lowest bit position (inclusive)
 * @return Bitmask with upper bits set
 *
 * @code
 * uint32_t mask = BIT_UPPER_BITMASK(4); // Returns 0xFFFFFFF0 (bits 4+ set)
 * @endcode
 */
#define BIT_UPPER_BITMASK(bottom_bit) BIT_SAFE_UPPER_MASK(bottom_bit)

/**
 * @brief Create a bitmask covering a range of bits
 * @param start_bit Lowest (rightmost) bit to set indexed from zero. (inclusive)
 * @param end_bit Highest (leftmost) bit to set indexed from zero. (inclusive)
 * @return Bitmask with bits from start_bit to end_bit set
 *
 * Creates a bitmask that covers the specified bit range.
 * If start_bit > end_bit, returns 0.
 *
 * @code
 * uint32_t mask = BIT_BITMASK(4,5); // Returns 0x30 (bits 4-5 set)
 * uint32_t zero = BIT_BITMASK(5,4); // Returns 0x00 (invalid range)
 * uint32_t single = BIT_BITMASK(4,4); // Returns 0x10 (bit 4 set)
 * @endcode
 */
#define BIT_BITMASK(start_bit, end_bit) (BIT_LOWER_BITMASK(end_bit) & BIT_UPPER_BITMASK(start_bit))

/**
 * @brief Extract only the specified bit range from a value
 * @param x Input value
 * @param start_bit Lowest (rightmost) bit to mask indexed from zero. (inclusive)
 * @param end_bit Highest (leftmost) bit to mask indexed from zero. (inclusive)
 * @return Value with only the specified bit range preserved
 *
 * @code
 * uint32_t result = BIT_MASK_BITS(7, 1, 3); // Returns 6 (bits 1-3 of 7)
 * @endcode
 */
#define BIT_MASK_BITS(x, start_bit, end_bit) ((x)&BIT_BITMASK((start_bit), (end_bit)))

/**
 * @brief Extract a bit field and shift it to start at bit 0
 * @param x Input value
 * @param start_bit Lowest (rightmost) bit to get indexed from zero. (inclusive)
 * @param end_bit Highest (leftmost) bit to get indexed from zero. (inclusive)
 * @return Extracted bit field value, right-shifted to start at bit 0
 *
 * @code
 * uint32_t field = BIT_GET_BITS(6, 1, 2); // Returns 3 (bits 1-2 of 6)
 * @endcode
 */
#define BIT_GET_BITS(x, start_bit, end_bit) ((BIT_LOWER_BITMASK(end_bit) & (x)) >> (start_bit))

/**
 * @brief Set all bits in the specified range
 * @param x Input value
 * @param start_bit Lowest (rightmost) bit to set indexed from zero. (inclusive)
 * @param end_bit Highest (leftmost) bit to set indexed from zero. (inclusive)
 * @return Value with specified bit range set
 *
 * @code
 * uint32_t result = BIT_SET_BITS(4, 0, 1); // Returns 7 (sets bits 0-1)
 * @endcode
 */
#define BIT_SET_BITS(x, start_bit, end_bit) ((x) | BIT_BITMASK((start_bit), (end_bit)))

/**
 * @brief Clear all bits in the specified range
 * @param x Input value
 * @param start_bit Lowest (rightmost) bit to clear indexed from zero. (inclusive)
 * @param end_bit Highest (leftmost) bit to clear indexed from zero. (inclusive)
 * @return Value with specified bit range cleared
 *
 * @code
 * uint32_t result = BIT_CLEAR_BITS(7, 0, 1); // Returns 4 (clears bits 0-1)
 * @endcode
 */
#define BIT_CLEAR_BITS(x, start_bit, end_bit) ((x) & (~BIT_BITMASK((start_bit), (end_bit))))

/**
 * @brief Toggle all bits in the specified range
 * @param x Input value
 * @param start_bit Lowest (rightmost) bit to toggle indexed from zero. (inclusive)
 * @param end_bit Highest (leftmost) bit to toggle indexed from zero. (inclusive)
 * @return Value with specified bit range toggled
 *
 * @code
 * uint32_t result = BIT_TOGGLE_BITS(5, 0, 2); // Returns 2 (toggles bits 0-2)
 * @endcode
 */
#define BIT_TOGGLE_BITS(x, start_bit, end_bit) ((x) ^ BIT_BITMASK((start_bit), (end_bit)))

/**
 * @brief Replace a bit field with new values
 * @param x Input value
 * @param new_bits New bit values to insert
 * @param start_bit Lowest (rightmost) bit to change indexed from zero. (inclusive)
 * @param end_bit Highest (leftmost) bit to change indexed from zero. (inclusive)
 * @return Value with bit field replaced
 *
 * Clears the specified bit range and inserts new_bits at that position.
 *
 * @code
 * uint32_t result = BIT_CHANGE_BITS(8, 3, 1, 2); // Returns 14
 * @endcode
 */
#define BIT_CHANGE_BITS(x, new_bits, start_bit, end_bit)                                           \
    (BIT_CLEAR_BITS((x), (start_bit), (end_bit)) |                                                 \
     BIT_MASK_BITS(((new_bits) << (start_bit)), (start_bit), (end_bit)))

/**
 * @brief Check if a signed integer is negative
 * @param x Signed integer value
 * @return 1 if negative, 0 if positive or zero
 *
 * Tests the most significant bit (sign bit) of a signed integer.
 *
 * @code
 * int is_neg1 = BIT_IS_NEG(-5);  // Returns 1
 * int is_neg2 = BIT_IS_NEG(5);   // Returns 0
 * @endcode
 */
#define BIT_IS_NEG(x) (BIT_GET_BIT((x), BIT_TOPBIT(x)))

/**
 * @brief Get the sign of a signed integer
 * @param x Signed integer value
 * @return -1 if negative, 1 if positive or zero
 *
 * Returns the mathematical sign of a value as an integer.
 *
 * @code
 * int sign1 = BIT_GET_SIGN(-5);  // Returns -1
 * int sign2 = BIT_GET_SIGN(5);   // Returns 1
 * int sign3 = BIT_GET_SIGN(0);   // Returns 1
 * @endcode
 */
#define BIT_GET_SIGN(x) ((-(BIT_IS_NEG(x))) | 1)

/**
 * @brief Logical right shift with automatic type selection
 * @param x Value to shift
 * @param nbits Number of bits to shift right
 * @return Logical right shift result with zero-fill
 *
 * Performs logical (unsigned) right shift with proper type handling.
 * Automatically selects 32-bit or 64-bit operations based on type size.
 *
 * @code
 * uint32_t result = BIT_LOGICAL_RSHIFT(0x80000000, 1); // Returns 0x40000000
 * @endcode
 */
#define BIT_LOGICAL_RSHIFT(x, nbits)                                                               \
    (sizeof(x) <= 4 && (nbits) < 32 ? (((unsigned)(x)) >> (nbits))                                 \
                                    : (((unsigned long long)(x)) >> (nbits)))

/**
 * @brief Arithmetic right shift with sign extension
 * @param x Signed value to shift
 * @param nbits Number of bits to shift right
 * @return Arithmetic right shift result with sign extension
 *
 * Performs arithmetic right shift that preserves the sign bit.
 * Negative values are sign-extended with 1s, positive with 0s.
 *
 * @code
 * int32_t result = BIT_ARITHMETIC_RSHIFT(-8, 1); // Returns -4 (sign extended)
 * @endcode
 */
#define BIT_ARITHMETIC_RSHIFT(x, nbits)                                                            \
    (((x) >> (nbits)) | (BIT_IS_NEG(x) ? BIT_UPPER_BITMASK(BIT_WORDBITS(x) - (nbits)) : 0))

/**
 * @brief Right shift division with proper sign handling
 * @param x Signed value to divide
 * @param nbits Number of bit positions (power of 2 divisor)
 * @return Division result equivalent to x / (2^nbits)
 *
 * Performs division by powers of 2 using right shift with correct
 * sign handling for both positive and negative values.
 *
 * @code
 * int32_t result = BIT_RSHIFT_DIV(-7, 2); // Returns -1 (equivalent to -7/4)
 * @endcode
 */
#define BIT_RSHIFT_DIV(x, nbits) (BIT_IS_NEG(x) ? (-((-(x)) >> (nbits))) : ((x) >> (nbits)))

/**
 * @brief Logical left shift with automatic type selection
 * @param x Value to shift
 * @param nbits Number of bits to shift left
 * @return Logical left shift result
 *
 * Performs logical left shift with proper type handling.
 * Automatically selects 32-bit or 64-bit operations based on type size.
 *
 * @code
 * uint32_t result = BIT_LOGICAL_LSHIFT(0x40000000, 1); // Returns 0x80000000
 * @endcode
 */
#define BIT_LOGICAL_LSHIFT(x, nbits)                                                               \
    (sizeof(x) <= 4 && (nbits) < 32 ? (((unsigned)(x)) << (nbits))                                 \
                                    : (((unsigned long long)(x)) << (nbits)))

/**
 * @brief Arithmetic left shift with sign bit preservation
 * @param x Signed value to shift
 * @param nbits Number of bits to shift left
 * @return Arithmetic left shift result with sign preservation
 *
 * Performs arithmetic left shift that preserves the original sign bit.
 *
 * @code
 * int32_t result = BIT_ARITHMETIC_LSHIFT(-4, 1); // Preserves negative sign
 * @endcode
 */
#define BIT_ARITHMETIC_LSHIFT(x, nbits)                                                            \
    (((x) << (nbits)) | (BIT_IS_NEG(x) ? BIT_X(BIT_TOPBIT(x)) : 0))

/**
 * @brief Left shift multiplication with proper sign handling
 * @param x Signed value to multiply
 * @param nbits Number of bit positions (power of 2 multiplier)
 * @return Multiplication result equivalent to x * (2^nbits)
 *
 * Performs multiplication by powers of 2 using left shift with correct
 * sign handling for both positive and negative values.
 *
 * @code
 * int32_t result = BIT_LSHIFT_MULT(-3, 2); // Returns -12 (equivalent to -3*4)
 * @endcode
 */
#define BIT_LSHIFT_MULT(x, nbits) (BIT_IS_NEG(x) ? (-((-(x)) << (nbits))) : ((x) << (nbits)))

/**
 * @brief Get the minimum value for a signed integer type
 * @param s_var Variable of signed integer type
 * @return Minimum value for the type (most negative value)
 *
 * @code
 * int32_t var;
 * int32_t min_val = BIT_STYPE_MIN(var); // Returns -2147483648
 * @endcode
 */
#define BIT_STYPE_MIN(s_var) BIT_SAFE_SHIFT(BIT_TOPBIT(s_var))

/**
 * @brief Get the maximum value for a signed integer type
 * @param s_var Variable of signed integer type
 * @return Maximum value for the type (most positive value)
 *
 * @code
 * int32_t var;
 * int32_t max_val = BIT_STYPE_MAX(var); // Returns 2147483647
 * @endcode
 */
#define BIT_STYPE_MAX(s_var) (BIT_STYPE_MIN(s_var) - 1U)

/**
 * @brief Get the minimum value for an unsigned integer type
 * @param u_var Variable of unsigned integer type
 * @return Minimum value for the type (always 0)
 *
 * @code
 * uint32_t var;
 * uint32_t min_val = BIT_UTYPE_MIN(var); // Returns 0
 * @endcode
 */
#define BIT_UTYPE_MIN(u_var) (0U)

/**
 * @brief Get the maximum value for an unsigned integer type
 * @param u_var Variable of unsigned integer type
 * @return Maximum value for the type
 *
 * @code
 * uint32_t var;
 * uint32_t max_val = BIT_UTYPE_MAX(var); // Returns 4294967295
 * @endcode
 */
#define BIT_UTYPE_MAX(u_var) (BIT_STYPE_MAX(u_var) + BIT_STYPE_MIN(u_var))

/** @} */ /* end of publicAPI_api group */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* of file BIT_OPS_H_ */

/******************************************************************************
 * End of file
 ******************************************************************************/
