/**
 * @file bit64_ops.c
 * @brief 64-bit integer bit manipulation operations implementation
 *
 * Copyright 2009-2010 Stephen M Bennett. All Rights Reserved.
 */

/********************************* INCLUDES **********************************/
#include "bit64_ops.h"
#include "bit_ops.h"

/********************************* CONSTANTS *********************************/

/************************** LOCAL BASIC DATA TYPES ***************************/

/**************************** LOCAL ENUMERATIONS *****************************/

/********************* LOCAL STRUCTURE /UNION DATA TYPES *********************/

/******************************* LOCAL MACROS ********************************/

/************************ LOCAL FUNCTIONS PROTOTYPES *************************/

/**************************** EXPORTED FUNCTIONS *****************************/

uint64_t BIT64_Set(uint8_t bit) { return (bit <= BIT64_TOP_BIT) ? (1ULL << bit) : 0; }

uint64_t BIT64_RBitmask(uint8_t left_bit) {
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
    if (left_bit > BIT64_TOP_BIT) {
        return 0xFFFFFFFFFFFFFFFFULL;
    }
    return (~(-2ULL << left_bit));
}

uint64_t BIT64_LBitmask(uint8_t right_bit) {
    /* The expression below is effectively "(-1 << right_bit);"
    ** with the following error checking : -
    **    if right_bit > topbit bitmask will be zero
    */
    if (right_bit > BIT64_TOP_BIT) {
        return 0;
    }
    return (~0ULL << right_bit);
}

uint64_t BIT64_Bitmask(uint8_t start_bit, uint8_t end_bit) {
    return (BIT64_RBitmask(end_bit) & BIT64_LBitmask(start_bit));
}

uint64_t BIT64_nBitmask(uint8_t start_bit, uint8_t bit_num) {
    /* The expression below is effectively "((~(-1 << bit_num)) << start_bit);"
    ** with the following error checking : -
    **    if start_bit > topbit bitmask will be zero
    **    else if bit_num > topbit unshifted bitmask will be all one's
    */
    if (start_bit > BIT64_TOP_BIT) {
        return 0;
    }
    if (bit_num > BIT64_BITS) {
        return (~0ULL << start_bit);
    }
    return ((~(~0ULL << bit_num)) << start_bit);
}

BOOL BIT64_isNeg(int64_t var) {
#ifdef ARCH_TWOS_COMPLEMENT
    /* SIGNED_SHIFTS is not relavent here: as we're only masking the top-bit  */
    return ((var >> BIT64_TOP_BIT) & 1);
#else
    return (var < 0);
#endif
}

int64_t BIT64_Get_Sign(int64_t var) {
#ifdef ARCH_TWOS_COMPLEMENT
    /* SIGNED_SHIFTS is not relavent here: as we're only masking the top-bit  */
    return (-((var >> BIT64_TOP_BIT) & 1) | 1);
#else
    return ((var >= 0) - (var < 0));
#endif
}

uint8_t BIT64_Count_Bits_Set(uint64_t var) {
#ifdef COUNT_BITS_LOOKUP
    uint8_t const *const p = (uint8_t *)&var;
    return (bits_set_table256[p[0]] + bits_set_table256[p[1]] + bits_set_table256[p[2]] +
            bits_set_table256[p[3]] + bits_set_table256[p[4]] + bits_set_table256[p[5]] +
            bits_set_table256[p[6]] + bits_set_table256[p[7]]);
#else
    /* SIGNED_SHIFTS is not relavent here: as we're doing unsigned shifts */
    var = var - ((var >> 1) & 0x5555555555555555); // reuse input as temporary
    var = (var & 0x3333333333333333) + ((var >> 2) & 0x3333333333333333);        // temp
    return ((var + (var >> 4) & 0x0F0F0F0F0F0F0F0F) * 0x0101010101010101) >> 48; // count
#endif
}

BOOL BIT64_Has_Zero_Byte(uint64_t var) {
    return (0 != ((var - 0x0101010101010101ULL) & ~var & 0x8080808080808080ULL));
}

uint8_t BIT64_Get_First_Bit_Set(uint64_t var) {
    static const uint8_t debruijn64[64] = {
        63, 0,  58, 1,  59, 47, 53, 2,  60, 39, 48, 27, 54, 33, 42, 3,  61, 51, 37, 40, 49, 18,
        28, 20, 55, 30, 34, 11, 43, 14, 22, 4,  62, 57, 46, 52, 38, 26, 32, 41, 50, 36, 17, 19,
        29, 10, 13, 21, 56, 45, 25, 31, 35, 16, 9,  12, 44, 24, 15, 8,  23, 7,  6,  5};
    return (var ? debruijn64[((uint64_t)((var & -var) * 0x07EDD5E59A4E28C2ULL)) >> 58]
                : BIT64_BITS);
}

int64_t BIT64_Sign_Extend(uint64_t var, uint8_t bit_num) {
    switch (bit_num) {
    case 1: {
        struct {
            int64_t x : 1;
        } s = {var};
        return s.x;
    }
    case 2: {
        struct {
            int64_t x : 2;
        } s = {var};
        return s.x;
    }
    case 3: {
        struct {
            int64_t x : 3;
        } s = {var};
        return s.x;
    }
    case 4: {
        struct {
            int64_t x : 4;
        } s = {var};
        return s.x;
    }
    case 5: {
        struct {
            int64_t x : 5;
        } s = {var};
        return s.x;
    }
    case 6: {
        struct {
            int64_t x : 6;
        } s = {var};
        return s.x;
    }
    case 7: {
        struct {
            int64_t x : 7;
        } s = {var};
        return s.x;
    }
    case 8: {
        struct {
            int64_t x : 8;
        } s = {var};
        return s.x;
    }
    case 9: {
        struct {
            int64_t x : 9;
        } s = {var};
        return s.x;
    }
    case 10: {
        struct {
            int64_t x : 10;
        } s = {var};
        return s.x;
    }
    case 11: {
        struct {
            int64_t x : 11;
        } s = {var};
        return s.x;
    }
    case 12: {
        struct {
            int64_t x : 12;
        } s = {var};
        return s.x;
    }
    case 13: {
        struct {
            int64_t x : 13;
        } s = {var};
        return s.x;
    }
    case 14: {
        struct {
            int64_t x : 14;
        } s = {var};
        return s.x;
    }
    case 15: {
        struct {
            int64_t x : 15;
        } s = {var};
        return s.x;
    }
    case 16: {
        struct {
            int64_t x : 16;
        } s = {var};
        return s.x;
    }
    case 17: {
        struct {
            int64_t x : 17;
        } s = {var};
        return s.x;
    }
    case 18: {
        struct {
            int64_t x : 18;
        } s = {var};
        return s.x;
    }
    case 19: {
        struct {
            int64_t x : 19;
        } s = {var};
        return s.x;
    }
    case 20: {
        struct {
            int64_t x : 20;
        } s = {var};
        return s.x;
    }
    case 21: {
        struct {
            int64_t x : 21;
        } s = {var};
        return s.x;
    }
    case 22: {
        struct {
            int64_t x : 22;
        } s = {var};
        return s.x;
    }
    case 23: {
        struct {
            int64_t x : 23;
        } s = {var};
        return s.x;
    }
    case 24: {
        struct {
            int64_t x : 24;
        } s = {var};
        return s.x;
    }
    case 25: {
        struct {
            int64_t x : 25;
        } s = {var};
        return s.x;
    }
    case 26: {
        struct {
            int64_t x : 26;
        } s = {var};
        return s.x;
    }
    case 27: {
        struct {
            int64_t x : 27;
        } s = {var};
        return s.x;
    }
    case 28: {
        struct {
            int64_t x : 28;
        } s = {var};
        return s.x;
    }
    case 29: {
        struct {
            int64_t x : 29;
        } s = {var};
        return s.x;
    }
    case 30: {
        struct {
            int64_t x : 30;
        } s = {var};
        return s.x;
    }
    case 31: {
        struct {
            int64_t x : 31;
        } s = {var};
        return s.x;
    }
    case 32: {
        struct {
            int64_t x : 32;
        } s = {var};
        return s.x;
    }
    case 33: {
        struct {
            int64_t x : 33;
        } s = {var};
        return s.x;
    }
    case 34: {
        struct {
            int64_t x : 34;
        } s = {var};
        return s.x;
    }
    case 35: {
        struct {
            int64_t x : 35;
        } s = {var};
        return s.x;
    }
    case 36: {
        struct {
            int64_t x : 36;
        } s = {var};
        return s.x;
    }
    case 37: {
        struct {
            int64_t x : 37;
        } s = {var};
        return s.x;
    }
    case 38: {
        struct {
            int64_t x : 38;
        } s = {var};
        return s.x;
    }
    case 39: {
        struct {
            int64_t x : 39;
        } s = {var};
        return s.x;
    }
    case 40: {
        struct {
            int64_t x : 40;
        } s = {var};
        return s.x;
    }
    case 41: {
        struct {
            int64_t x : 41;
        } s = {var};
        return s.x;
    }
    case 42: {
        struct {
            int64_t x : 42;
        } s = {var};
        return s.x;
    }
    case 43: {
        struct {
            int64_t x : 43;
        } s = {var};
        return s.x;
    }
    case 44: {
        struct {
            int64_t x : 44;
        } s = {var};
        return s.x;
    }
    case 45: {
        struct {
            int64_t x : 45;
        } s = {var};
        return s.x;
    }
    case 46: {
        struct {
            int64_t x : 46;
        } s = {var};
        return s.x;
    }
    case 47: {
        struct {
            int64_t x : 47;
        } s = {var};
        return s.x;
    }
    case 48: {
        struct {
            int64_t x : 48;
        } s = {var};
        return s.x;
    }
    case 49: {
        struct {
            int64_t x : 49;
        } s = {var};
        return s.x;
    }
    case 50: {
        struct {
            int64_t x : 50;
        } s = {var};
        return s.x;
    }
    case 51: {
        struct {
            int64_t x : 51;
        } s = {var};
        return s.x;
    }
    case 52: {
        struct {
            int64_t x : 52;
        } s = {var};
        return s.x;
    }
    case 53: {
        struct {
            int64_t x : 53;
        } s = {var};
        return s.x;
    }
    case 54: {
        struct {
            int64_t x : 54;
        } s = {var};
        return s.x;
    }
    case 55: {
        struct {
            int64_t x : 55;
        } s = {var};
        return s.x;
    }
    case 56: {
        struct {
            int64_t x : 56;
        } s = {var};
        return s.x;
    }
    case 57: {
        struct {
            int64_t x : 57;
        } s = {var};
        return s.x;
    }
    case 58: {
        struct {
            int64_t x : 58;
        } s = {var};
        return s.x;
    }
    case 59: {
        struct {
            int64_t x : 59;
        } s = {var};
        return s.x;
    }
    case 60: {
        struct {
            int64_t x : 60;
        } s = {var};
        return s.x;
    }
    case 61: {
        struct {
            int64_t x : 61;
        } s = {var};
        return s.x;
    }
    case 62: {
        struct {
            int64_t x : 62;
        } s = {var};
        return s.x;
    }
    case 63: {
        struct {
            int64_t x : 63;
        } s = {var};
        return s.x;
    }
    case 64: {
        return var;
    }
    default: { return 0; }
    }
}

uint64_t BIT64_Reverse(uint64_t var) {
    /* SIGNED_SHIFTS is not relavent here: as we're doing unsigned shifts */

    // swap odd and even bits
    var = ((var >> 1) & 0x5555555555555555) | ((var & 0x5555555555555555) << 1);

    // swap consecutive pairs
    var = ((var >> 2) & 0x3333333333333333) | ((var & 0x3333333333333333) << 2);

    // swap nibbles ...
    var = ((var >> 4) & 0x0F0F0F0F0F0F0F0F) | ((var & 0x0F0F0F0F0F0F0F0F) << 4);

    // swap bytes
    var = ((var >> 8) & 0x00FF00FF00FF00FF) | ((var & 0x00FF00FF00FF00FF) << 8);

    // swap 2-byte long pairs
    var = ((var >> 16) & 0x0000FFFF0000FFFF) | ((var & 0x0000FFFF0000FFFF) << 16);

    // swap 4-byte long pairs
    return ((var >> 32) | (var << 32));
}

uint64_t BIT64_Mask_Bit(uint64_t var, uint8_t bit) { return (var & BIT64_Set(bit)); }

uint64_t BIT64_Mask_Bits(uint64_t var, uint8_t start_bit, uint8_t end_bit) {
    return (var & BIT64_Bitmask(start_bit, end_bit));
}

uint64_t BIT64_Mask_nBits(uint64_t var, uint8_t start_bit, uint8_t bit_num) {
    return (var & BIT64_nBitmask(start_bit, bit_num));
}

BOOL BIT64_Get_Bit(uint64_t var, uint8_t bit) {
    /* SIGNED_SHIFTS is not relavent here: as we're doing unsigned shifts */

    /* maybe we should return BIT64_isNeg(var) if BIT64_MORETHAN_TOPBIT?? */
    return ((var >> bit) & BIT64_LESSTHAN_OR_EQUAL_TOPBIT(bit));
}

uint64_t BIT64_Get_Bits(uint64_t var, uint8_t start_bit, uint8_t end_bit) {
    /* maybe we should return BIT64_isNeg(var) ?? */
    return (-BIT64_LESSTHAN_OR_EQUAL_TOPBIT(start_bit) &
            ((BIT64_RBitmask(end_bit) & var) >> (start_bit & BIT64_TOP_BIT)));
}

uint64_t BIT64_Get_nBits(uint64_t var, uint8_t start_bit, uint8_t bit_num) {
    /* error check "-(0 != bit_num) &" is used to cover the
    ** condition where "start_bit + bit_num - 1" underflows
    */
    return (-(0 != bit_num) & BIT64_Get_Bits(var, start_bit, start_bit + bit_num - 1));
}

int64_t BIT64_Get_Bits_Signed(uint64_t var, uint8_t start_bit, uint8_t end_bit) {
    /* quick error check */
    if (end_bit < start_bit) {
        return 0;
    }

    return (BIT64_Sign_Extend(BIT64_Get_Bits(var, start_bit, end_bit), end_bit - start_bit + 1));
}

int64_t BIT64_Get_nBits_Signed(uint64_t var, uint8_t start_bit, uint8_t bit_num) {
    return (BIT64_Sign_Extend(BIT64_Get_nBits(var, start_bit, bit_num), bit_num));
}

uint64_t BIT64_Set_Bit(uint64_t var, uint8_t bit) { return (var | BIT64_Set(bit)); }

uint64_t BIT64_Set_Bits(uint64_t var, uint8_t start_bit, uint8_t end_bit) {
    return (var | BIT64_Bitmask(start_bit, end_bit));
}

uint64_t BIT64_Set_nBits(uint64_t var, uint8_t start_bit, uint8_t bit_num) {
    return (var | BIT64_nBitmask(start_bit, bit_num));
}

uint64_t BIT64_Clear_Bit(uint64_t var, uint8_t bit) {
    /* set the bit we're interested in - then flip all the bits over + do AND with var */
    return (var & (~(BIT64_Set(bit))));
}

uint64_t BIT64_Clear_Bits(uint64_t var, uint8_t start_bit, uint8_t end_bit) {
    /* set the bits we're interested in - then flip all the bits over + do AND with var */
    return (var & (~(BIT64_Bitmask(start_bit, end_bit))));
}

uint64_t BIT64_Clear_nBits(uint64_t var, uint8_t start_bit, uint8_t bit_num) {
    return (var & (~(BIT64_nBitmask(start_bit, bit_num))));
}

uint64_t BIT64_Toggle_Bit(uint64_t var, uint8_t bit) { return (var ^ (BIT64_Set(bit))); }

uint64_t BIT64_Toggle_Bits(uint64_t var, uint8_t start_bit, uint8_t end_bit) {
    return (var ^ BIT64_Bitmask(start_bit, end_bit));
}

uint64_t BIT64_Toggle_nBits(uint64_t var, uint8_t start_bit, uint8_t bit_num) {
    return (var ^ BIT64_nBitmask(start_bit, bit_num));
}

uint64_t BIT64_Insert_Bit(uint64_t var, BOOL new_bit, uint8_t bit) {
    return (BIT64_Clear_Bit(var, bit) | ((new_bit & 1ULL) << (bit & BIT64_TOP_BIT)));
}

uint64_t BIT64_Insert_Bits(uint64_t var, uint64_t new_bits, uint8_t start_bit, uint8_t end_bit) {
    return (BIT64_Clear_Bits(var, start_bit, end_bit) |
            BIT64_Mask_Bits(new_bits << (start_bit & BIT64_TOP_BIT), start_bit, end_bit));
}

uint64_t BIT64_Insert_nBits(uint64_t var, uint64_t new_bits, uint8_t start_bit, uint8_t bit_num) {
    return (BIT64_Clear_nBits(var, start_bit, bit_num) |
            BIT64_Mask_nBits(new_bits << (start_bit & BIT64_TOP_BIT), start_bit, bit_num));
}

char *BIT64_Bitstring(uint64_t var, char bitstring[BIT64_STRING_SIZE]) {
    /* Highly optimized implementation - always includes leading zeros for consistent output */
    static const char digits[2] = {'0', '1'};
    uint64_t mask = (1ULL << BIT64_TOP_BIT); /* Start with MSB mask */
    char *p = bitstring;

    /* Build string from most significant bit to least significant bit */
    for (; mask != 0; mask >>= 1) {
        *p++ = digits[(var & mask) != 0];
    }

    /* null-terminate the string */
    *p = '\0';
    return bitstring;
}

char *BIT64_BitstringBrief(uint64_t var, char bitstring[BIT64_STRING_SIZE]) {
    /* Highly optimized implementation for no leading zeros */
    static const char digits[2] = {'0', '1'};
    uint64_t mask = 0;
    char *p = bitstring;

    /* Handle zero case explicitly */
    if (var == 0) {
        *p++ = '0';
        *p = '\0';
        return bitstring;
    }

    /* Find MSB position and create initial mask */
    mask = (1ULL << BIT64_TOP_BIT);
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
