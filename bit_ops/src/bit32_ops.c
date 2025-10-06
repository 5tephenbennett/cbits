/**
 * @file bit32_ops.c
 * @brief 32-bit integer bit manipulation operations implementation
 *
 * Copyright 2009-2010 Stephen M Bennett. All Rights Reserved.
 */

/********************************* INCLUDES **********************************/
#include "bit32_ops.h"
#include "bit_ops.h"

/********************************* CONSTANTS *********************************/

/************************** LOCAL BASIC DATA TYPES ***************************/

/**************************** LOCAL ENUMERATIONS *****************************/

/********************* LOCAL STRUCTURE /UNION DATA TYPES *********************/

/******************************* LOCAL MACROS ********************************/

/************************ LOCAL FUNCTIONS PROTOTYPES *************************/

/**************************** EXPORTED FUNCTIONS *****************************/

uint32_t BIT32_Set(uint8_t bit) { return (BIT32_LESSTHAN_OR_EQUAL_TOPBIT(bit) ? (1U << bit) : 0U); }

uint32_t BIT32_RBitmask(uint8_t left_bit) {
    /* Use inverse logic since '<<' moves 0's into place. E.g.
    **
    **       -2U == 1111 1110
    **     <<= 2 == 1111 1000
    **           ~= 0000 0111
    **
    ** The expression below is effectively "(~(-2U << left_bit));"
    ** with the following error checking : -
    **    if left_bit > topbit bitmask will be all ones
    */
    return (-BIT32_MORETHAN_TOPBIT(left_bit) | (~(-2U << left_bit)));
}

uint32_t BIT32_LBitmask(uint8_t right_bit) {
    /* The expression below is effectively "(-1 << right_bit);"
    ** with the following error checking : -
    **    if right_bit > topbit bitmask will be zero
    */
    return ((uint32_t)-BIT32_LESSTHAN_OR_EQUAL_TOPBIT(right_bit) << right_bit);
}

uint32_t BIT32_Bitmask(uint8_t start_bit, uint8_t end_bit) {
    // return (BIT32_RBitmask(end_bit) & BIT32_LBitmask(start_bit));
    return ((-BIT32_MORETHAN_TOPBIT(end_bit) | (~(-2UL << end_bit))) &
            ((uint32_t)-BIT32_LESSTHAN_OR_EQUAL_TOPBIT(start_bit) << start_bit));
}

uint32_t BIT32_nBitmask(uint8_t start_bit, uint8_t bit_num) {
    /* The expression below is effectively "((~(-1 << bit_num)) << start_bit);"
    ** with the following error checking : -
    **    if start_bit > topbit bitmask will be zero
    **    if bit_num > available bits from start_bit, bitmask will be zero
    */
    if (start_bit > BIT32_TOP_BIT) {
        return 0;
    }
    if (bit_num == 0) {
        return 0;
    }
    if (start_bit + bit_num > BIT32_BITS) {
        return 0; // Invalid: would exceed 32 bits
    }
    return ((~(~0UL << bit_num)) << start_bit);
}

BOOL BIT32_isNeg(int32_t var) {
#ifdef ARCH_TWOS_COMPLEMENT
    /* SIGNED_SHIFTS is not relavent here: as we're only masking the top-bit  */
    return ((var >> BIT32_TOP_BIT) & 1);
#else
    return (var < 0);
#endif
}

int32_t BIT32_Get_Sign(int32_t var) {
#ifdef ARCH_TWOS_COMPLEMENT
    /* SIGNED_SHIFTS is not relavent here: as we're only masking the top-bit  */
    return (-((var >> BIT32_TOP_BIT) & 1) | 1);
#else
    return ((var >= 0) - (var < 0));
#endif
}

uint8_t BIT32_Count_Bits_Set(uint32_t var) {
#ifdef COUNT_BITS_LOOKUP
    uint8_t const *const p = (uint8_t *)&var;
    return (bits_set_table256[p[0]] + bits_set_table256[p[1]] + bits_set_table256[p[2]] +
            bits_set_table256[p[3]]);
#else
    /* SIGNED_SHIFTS is not relavent here: as we're doing unsigned shifts */
    var -= ((var >> 1) & 0x55555555);                            // reuse input as temporary
    var = (var & 0x33333333) + ((var >> 2) & 0x33333333);        // temp
    return ((var + ((var >> 4) & 0xF0F0F0F)) * 0x1010101) >> 24; // count
#endif
}

BOOL BIT32_Has_Zero_Byte(uint32_t var) {
#ifndef ARCH_TWOS_COMPLEMENT

    return ((0 == (var & 0x000000ff)) || (0 == (var & 0x0000ff00)) || (0 == (var & 0x00ff0000)) ||
            (0 == (var & 0xff000000)));

#else /* #ifndef ARCH_TWOS_COMPLEMENT */

    /* We can optimise finding a zero-byte when we're using twos' complement
    ** math.  Lets consider the problem with 8-bit arithmetic:
    **
    ** We have 2 simple equations that have top-bit-set (0x80) ranges that
    ** only overlap when var == 0x00:
    **
    **     var  | var -0x01 | top_bit? | ~var | top_bit?
    **     -----|-----------|----------|------|---------
    **     0x80 |    0x7f   |    no    | 0x7f |    no
    **     0x81 |    0x80   |   yes    | 0x7e |    no
    **     ...  |    ...    |   yes    | ...  |    no
    **     0xff |    0xfe   |   yes    | 0x00 |    no
    **     0x00 |    0xff   |   yes    | 0xff |   yes <<<<
    **     0x01 |    0x00   |    no    | 0xfe |   yes
    **     ...  |    ...    |    no    | ...  |   yes
    **     0x7f |    0x7e   |    no    | 0x80 |   yes
    **
    ** Our resultant equation is a combination of these three
    ** elements and finally converting to a boolean result
    ** using doing a logical not-equal-to-zero.
    **
    ** Mathmatical proof (credit: Jonny Woodrow) of zero byte function:
    ** ----------------------------------------------------------------
    **
    **     (((var - 0x01010101UL) & ~var & 0x80808080UL) != 0) == zero_bytes?
    **
    ** Take the bottom byte of var:
    **
    **     X = var & 0xFF
    **     X = a7, a6, a5, a4, a3, a2, a1, a0 (LSB)
    **     X = a7, a[0..6]
    **
    **     Y = X - 0x1
    **     Y = b7, b[0..6]
    **
    **     Z = ~X & 0x80
    **     Z = c7, c[0..6]
    **
    **     if (b7 != 0)        => (a7 == 0 && a[0..6] == 0) || (a7 != 0 && a[0..6] != 0)
    **
    **     if (c7 != 0)        => (a7 == 0)
    **
    **     if ((b7 & c7) != 0) =>  (a7 == 0 && a[0..6] == 0) || (a7 != 0 && a[0..6] != 0)
    **                           && (a7 == 0)
    **
    **           simplifies to =>  (a7 == 0 && a[0..6] == 0)
    **                     i.e.    TRUE if X = 0
    **
    ** Extending to several bytes
    ** --------------------------
    **  If 'var' is 2 bytes, 0xYYXX, then:
    **     1) subtracting 1 from Y will never effect X
    **     2) subtracting 1 from X will only effect Y if X is zero
    **
    **  Since we only need to establish if X OR Y is zero, the second rule
    **  has no infuence on the success of the function.
    */
    return (0 != ((var - 0x01010101UL) & ~var & 0x80808080UL));

#endif /* #ifndef ARCH_TWOS_COMPLEMENT */
}

uint8_t BIT32_Get_First_Bit_Set(uint32_t var) {
    static const uint8_t debruijn32[32] = {0,  1,  28, 2,  29, 14, 24, 3,  30, 22, 20,
                                           15, 25, 17, 4,  8,  31, 27, 13, 23, 21, 19,
                                           16, 7,  26, 12, 18, 6,  11, 5,  10, 9};
    return (var ? debruijn32[((uint32_t)((var & -var) * 0x077CB531U)) >> 27] : BIT32_BITS);
}

int32_t BIT32_Sign_Extend(uint32_t var, uint8_t bit_num) {
#ifdef SIGNED_SHIFTS
    uint32_t shift_amount = 32U - bit_num; /* bits to shift left then right for sign extension */
    return ((int32_t)(var << shift_amount) >> shift_amount);
#else
    switch (bit_num) {
    case 1: {
        struct {
            int32_t x : 1;
        } s = {var};
        return s.x;
    }
    case 2: {
        struct {
            int32_t x : 2;
        } s = {var};
        return (s.x);
    }
    case 3: {
        struct {
            int32_t x : 3;
        } s = {var};
        return (s.x);
    }
    case 4: {
        struct {
            int32_t x : 4;
        } s = {var};
        return (s.x);
    }
    case 5: {
        struct {
            int32_t x : 5;
        } s = {var};
        return (s.x);
    }
    case 6: {
        struct {
            int32_t x : 6;
        } s = {var};
        return (s.x);
    }
    case 7: {
        struct {
            int32_t x : 7;
        } s = {var};
        return (s.x);
    }
    case 8: {
        struct {
            int32_t x : 8;
        } s = {var};
        return (s.x);
    }
    case 9: {
        struct {
            int32_t x : 9;
        } s = {var};
        return (s.x);
    }
    case 10: {
        struct {
            int32_t x : 10;
        } s = {var};
        return (s.x);
    }
    case 11: {
        struct {
            int32_t x : 11;
        } s = {var};
        return (s.x);
    }
    case 12: {
        struct {
            int32_t x : 12;
        } s = {var};
        return (s.x);
    }
    case 13: {
        struct {
            int32_t x : 13;
        } s = {var};
        return (s.x);
    }
    case 14: {
        struct {
            int32_t x : 14;
        } s = {var};
        return (s.x);
    }
    case 15: {
        struct {
            int32_t x : 15;
        } s = {var};
        return (s.x);
    }
    case 16: {
        struct {
            int32_t x : 16;
        } s = {var};
        return (s.x);
    }
    case 17: {
        struct {
            int32_t x : 17;
        } s = {var};
        return (s.x);
    }
    case 18: {
        struct {
            int32_t x : 18;
        } s = {var};
        return (s.x);
    }
    case 19: {
        struct {
            int32_t x : 19;
        } s = {var};
        return (s.x);
    }
    case 20: {
        struct {
            int32_t x : 20;
        } s = {var};
        return (s.x);
    }
    case 21: {
        struct {
            int32_t x : 21;
        } s = {var};
        return (s.x);
    }
    case 22: {
        struct {
            int32_t x : 22;
        } s = {var};
        return (s.x);
    }
    case 23: {
        struct {
            int32_t x : 23;
        } s = {var};
        return (s.x);
    }
    case 24: {
        struct {
            int32_t x : 24;
        } s = {var};
        return (s.x);
    }
    case 25: {
        struct {
            int32_t x : 25;
        } s = {var};
        return (s.x);
    }
    case 26: {
        struct {
            int32_t x : 26;
        } s = {var};
        return (s.x);
    }
    case 27: {
        struct {
            int32_t x : 27;
        } s = {var};
        return (s.x);
    }
    case 28: {
        struct {
            int32_t x : 28;
        } s = {var};
        return (s.x);
    }
    case 29: {
        struct {
            int32_t x : 29;
        } s = {var};
        return (s.x);
    }
    case 30: {
        struct {
            int32_t x : 30;
        } s = {var};
        return (s.x);
    }
    case 31: {
        struct {
            int32_t x : 31;
        } s = {var};
        return (s.x);
    }
    case 32: {
        return var;
    }
    default: { return 0; }
    }
#endif
}

uint32_t BIT32_Convert_Grey_Code(uint32_t gray_code) {
    gray_code ^= (gray_code >> 16);
    gray_code ^= (gray_code >> 8);
    gray_code ^= (gray_code >> 4);
    gray_code ^= (gray_code >> 2);
    gray_code ^= (gray_code >> 1);
    return (gray_code);
}

uint32_t BIT32_Reverse(uint32_t var) {
    /* SIGNED_SHIFTS is not relavent here: as we're doing unsigned shifts */

    // swap odd and even bits
    var = ((var >> 1) & 0x55555555) | ((var & 0x55555555) << 1);

    // swap consecutive pairs
    var = ((var >> 2) & 0x33333333) | ((var & 0x33333333) << 2);

    // swap nibbles ...
    var = ((var >> 4) & 0x0F0F0F0F) | ((var & 0x0F0F0F0F) << 4);

    // swap bytes
    var = ((var >> 8) & 0x00FF00FF) | ((var & 0x00FF00FF) << 8);

    // swap 2-byte long pairs
    return ((var >> 16) | (var << 16));
}

uint32_t BIT32_Mask_Bit(uint32_t var, uint8_t bit) { return (var & BIT32_Set(bit)); }

uint32_t BIT32_Mask_Bits(uint32_t var, uint8_t start_bit, uint8_t end_bit) {
    return (var & BIT32_Bitmask(start_bit, end_bit));
}

uint32_t BIT32_Mask_nBits(uint32_t var, uint8_t start_bit, uint8_t bit_num) {
    return (var & BIT32_nBitmask(start_bit, bit_num));
}

BOOL BIT32_Get_Bit(uint32_t var, uint8_t bit) {
    /* SIGNED_SHIFTS is not relavent here: as we're doing unsigned shifts */

    /* maybe we should return BIT32_isNeg(var) if BIT32_MORETHAN_TOPBIT?? */
    return ((var >> bit) & BIT32_LESSTHAN_OR_EQUAL_TOPBIT(bit));
}

uint32_t BIT32_Get_Bits(uint32_t var, uint8_t start_bit, uint8_t end_bit) {
    /* maybe we should return BIT32_isNeg(var) if BIT32_MORETHAN_TOPBIT?? */

    /* Using all our function goodness we could have: -
    **   return (-BIT32_LESSTHAN_OR_EQUAL_TOPBIT(start_bit)
    **        & ((BIT32_RBitmask(end_bit) & var) >> start_bit));
    **
    ** However since function calls may be costly we expand BIT32_RBitmask() here: -
    */
    return (((-BIT32_MORETHAN_TOPBIT(end_bit) | (~(-2UL << end_bit))) & var) >> start_bit);
}

uint32_t BIT32_Get_nBits(uint32_t var, uint8_t start_bit, uint8_t bit_num) {
    return (((BIT32_LESSTHAN_OR_EQUAL_TOPBIT(bit_num) << bit_num) - 1) & (var >> start_bit));
}

int32_t BIT32_Get_Bits_Signed(uint32_t var, uint8_t start_bit, uint8_t end_bit) {
    /* quick error check */
    if (end_bit < start_bit) {
        return 0;
    }

    return (BIT32_Sign_Extend(BIT32_Get_Bits(var, start_bit, end_bit), end_bit - start_bit + 1));
}

int32_t BIT32_Get_nBits_Signed(uint32_t var, uint8_t start_bit, uint8_t bit_num) {
    return (BIT32_Sign_Extend(BIT32_Get_nBits(var, start_bit, bit_num), bit_num));
}

uint32_t BIT32_Set_Bit(uint32_t var, uint8_t bit) { return (var | BIT32_Set(bit)); }

uint32_t BIT32_Set_Bits(uint32_t var, uint8_t start_bit, uint8_t end_bit) {
    return (var | BIT32_Bitmask(start_bit, end_bit));
}

uint32_t BIT32_Set_nBits(uint32_t var, uint8_t start_bit, uint8_t bit_num) {
    return (var | BIT32_nBitmask(start_bit, bit_num));
}

uint32_t BIT32_Clear_Bit(uint32_t var, uint8_t bit) {
    /* set the bit we're interested in - then flip all the bits over + do AND with var */
    return (var & (~(BIT32_Set(bit))));
}

uint32_t BIT32_Clear_Bits(uint32_t var, uint8_t start_bit, uint8_t end_bit) {
    /* set the bits we're interested in - then flip all the bits over + do AND with var */
    return (var & (~(BIT32_Bitmask(start_bit, end_bit))));
}

uint32_t BIT32_Clear_nBits(uint32_t var, uint8_t start_bit, uint8_t bit_num) {
    return (var & (~(BIT32_nBitmask(start_bit, bit_num))));
}

uint32_t BIT32_Toggle_Bit(uint32_t var, uint8_t bit) { return (var ^ (BIT32_Set(bit))); }

uint32_t BIT32_Toggle_Bits(uint32_t var, uint8_t start_bit, uint8_t end_bit) {
    return (var ^ BIT32_Bitmask(start_bit, end_bit));
}

uint32_t BIT32_Toggle_nBits(uint32_t var, uint8_t start_bit, uint8_t bit_num) {
    return (var ^ BIT32_nBitmask(start_bit, bit_num));
}

uint32_t BIT32_Insert_Bit(uint32_t var, BOOL new_bit, uint8_t bit) {
    return (BIT32_Clear_Bit(var, bit) | ((new_bit & 1) << bit));
}

uint32_t BIT32_Insert_Bits(uint32_t var, uint32_t new_bits, uint8_t start_bit, uint8_t end_bit) {
    return (BIT32_Clear_Bits(var, start_bit, end_bit) |
            BIT32_Mask_Bits(new_bits << start_bit, start_bit, end_bit));
}

uint32_t BIT32_Insert_nBits(uint32_t var, uint32_t new_bits, uint8_t start_bit, uint8_t bit_num) {
    return (BIT32_Clear_nBits(var, start_bit, bit_num) |
            BIT32_Mask_nBits(new_bits << (start_bit & BIT32_TOP_BIT), start_bit, bit_num));
}

char *BIT32_Bitstring(uint32_t var, char bitstring[BIT32_STRING_SIZE]) {
    /* Highly optimized implementation - always includes leading zeros for consistent output */
    static const char digits[2] = {'0', '1'};
    uint32_t mask = (1U << BIT32_TOP_BIT); /* Start with MSB mask */
    char *p = bitstring;

    /* Build string from most significant bit to least significant bit */
    for (; mask != 0; mask >>= 1) {
        *p++ = digits[(var & mask) != 0];
    }

    /* null-terminate the string */
    *p = '\0';
    return bitstring;
}

char *BIT32_BitstringBrief(uint32_t var, char bitstring[BIT32_STRING_SIZE]) {
    /* Highly optimized implementation for no leading zeros */
    static const char digits[2] = {'0', '1'};
    uint32_t mask = 0;
    char *p = bitstring;

    /* Handle zero case explicitly */
    if (var == 0) {
        *p++ = '0';
        *p = '\0';
        return bitstring;
    }

    /* Find MSB position and create initial mask */
    mask = (1U << BIT32_TOP_BIT);
    while ((var & mask) == 0) {
        mask >>= 1;
    }

    /* Build string from highest set bit down to bit 0 */
    for (; mask != 0; mask >>= 1) {
        *p++ = digits[(var & mask) != 0];
    }

    /* null-terminate the string */
    *p = '\0';
    return bitstring;
}

/***************************** LOCAL FUNCTIONS *******************************/

/******************************************************************************
 * End of file
 ******************************************************************************/
