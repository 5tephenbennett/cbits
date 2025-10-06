/**
 * @file bit16_ops.c
 * @brief 16-bit integer bit manipulation operations implementation
 *
 * Copyright 2009-2010 Stephen M Bennett. All Rights Reserved.
 */

/********************************* INCLUDES **********************************/
#include "bit16_ops.h"
#include "bit_ops.h"

/********************************* CONSTANTS *********************************/

/************************** LOCAL BASIC DATA TYPES ***************************/

/**************************** LOCAL ENUMERATIONS *****************************/

/********************* LOCAL STRUCTURE /UNION DATA TYPES *********************/

/******************************* LOCAL MACROS ********************************/

/************************ LOCAL FUNCTIONS PROTOTYPES *************************/

/**************************** EXPORTED FUNCTIONS *****************************/

uint16_t BIT16_Set(uint8_t bit) { return (BIT16_LESSTHAN_OR_EQUAL_TOPBIT(bit) << bit); }

uint16_t BIT16_RBitmask(uint8_t left_bit) {
    /* Use inverse logic since '<<' moves 0's into place. E.g.
    **
    **        -2 == 1111 1110
    **     <<= 2 == 1111 1000
    **           ~= 0000 0111
    **
    ** The expression below is effectively "(~(-2 << left_bit));"
    ** with the following error checking : -
    **    if left_bit > topbit bitmask will be all ones
    */
    return (-BIT16_MORETHAN_TOPBIT(left_bit) | (~((uint16_t)-2 << left_bit)));
}

uint16_t BIT16_LBitmask(uint8_t right_bit) {
    /* The expression below is effectively "(-1 << right_bit);"
    ** with the following error checking : -
    **    if right_bit > topbit bitmask will be zero
    */
    return ((uint16_t)-BIT16_LESSTHAN_OR_EQUAL_TOPBIT(right_bit) << right_bit);
}

uint16_t BIT16_Bitmask(uint8_t start_bit, uint8_t end_bit) {
    // return (BIT16_RBitmask(end_bit) & BIT16_LBitmask(start_bit));
    return ((-BIT16_MORETHAN_TOPBIT(end_bit) | (~((uint16_t)-2 << end_bit))) &
            ((uint16_t)-BIT16_LESSTHAN_OR_EQUAL_TOPBIT(start_bit) << start_bit));
}

uint16_t BIT16_nBitmask(uint8_t start_bit, uint8_t bit_num) {
    /* The expression below is effectively "((~(-1 << bit_num)) << start_bit);"
    ** with the following error checking : -
    **    if start_bit > topbit bitmask will be zero
    **    else if bit_num > topbit unshifted bitmask will be all one's
    */
    return (-BIT16_LESSTHAN_OR_EQUAL_TOPBIT(start_bit) &
            ((uint16_t)(~((uint16_t)-BIT16_LESSTHAN_OR_EQUAL_TOPBIT(bit_num) << bit_num))
             << start_bit));
}

BOOL BIT16_isNeg(int16_t var) {
#ifdef ARCH_TWOS_COMPLEMENT
    /* SIGNED_SHIFTS is not relavent here: as we're only masking the top-bit */
    return ((var >> BIT16_TOP_BIT) & 1);
#else
    return (var < 0);
#endif
}

int16_t BIT16_Get_Sign(int16_t var) {
#ifdef ARCH_TWOS_COMPLEMENT
    /* SIGNED_SHIFTS is not relavent here: as we're only masking the top-bit  */
    return (-((var >> BIT16_TOP_BIT) & 1) | 1);
#else
    return ((var >= 0) - (var < 0));
#endif
}

uint8_t BIT16_Count_Bits_Set(uint16_t var) {
#ifdef COUNT_BITS_LOOKUP
    uint8_t const *const p = (uint8_t *)&var;
    return (bits_set_table256[p[0]] + bits_set_table256[p[1]]);
#else
    /* SIGNED_SHIFTS is not relavent here: as we're doing unsigned shifts */
    var = var - ((var >> 1) & 0x5555);                    // reuse input as temporary
    var = (var & 0x3333) + ((var >> 2) & 0x3333);         // temp
    return ((var + ((var >> 4) & 0x0F0F)) * 0x0101) >> 8; // count
#endif
}

BOOL BIT16_Has_Zero_Byte(uint16_t var) {
#ifndef ARCH_TWOS_COMPLEMENT

    return ((0 == (var & 0x00ff)) || (0 == (var & 0xff00)));

#else /* #ifndef ARCH_TWOS_COMPLEMENT */

    /* We can optimise finding a zero-byte when we're using twos' complement
    ** math.  Lets consider the problem with 8-bit arithmetic:
    **
    ** We have 2 simple equations that have top-bit-set (0x80) ranges that
    ** only overlap when var == 0x00:
    **
    ** var  | var -0x01 | top_bit? | ~var | top_bit?
    ** -----|-----------|----------|------|---------
    ** 0x80 |    0x7f   |    no    | 0x7f |    no
    ** 0x81 |    0x80   |   yes    | 0x7e |    no
    ** ...  |    ...    |   yes    | ...  |    no
    ** 0xff |    0xfe   |   yes    | 0x00 |    no
    ** 0x00 |    0xff   |   yes    | 0xff |   yes <<<<
    ** 0x01 |    0x00   |    no    | 0xfe |   yes
    ** ...  |    ...    |    no    | ...  |   yes
    ** 0x7f |    0x7e   |    no    | 0x80 |   yes
    **
    ** Our resultant equation is a combination of these three
    ** elements and finally converting to a boolean result
    ** using doing a logical not-equal-to-zero.
    */
    return (0 != ((var - 0x0101) & ~var & 0x8080));

#endif /* #ifndef ARCH_TWOS_COMPLEMENT */
}

uint8_t BIT16_Get_First_Bit_Set(uint16_t var) {
    static const uint8_t debruijn32[32] = {0,  1,  28, 2,  29, 14, 24, 3,  30, 22, 20,
                                           15, 25, 17, 4,  8,  31, 27, 13, 23, 21, 19,
                                           16, 7,  26, 12, 18, 6,  11, 5,  10, 9};
    return (var ? debruijn32[((uint32_t)((var & -var) * 0x077CB531U)) >> 27] : BIT16_BITS);
}

int16_t BIT16_Sign_Extend(uint16_t var, uint8_t bit_num) {
    switch (bit_num) {
    case 1: {
        struct {
            int16_t x : 1;
        } s = {var};
        return s.x;
    }
    case 2: {
        struct {
            int16_t x : 2;
        } s = {var};
        return s.x;
    }
    case 3: {
        struct {
            int16_t x : 3;
        } s = {var};
        return s.x;
    }
    case 4: {
        struct {
            int16_t x : 4;
        } s = {var};
        return s.x;
    }
    case 5: {
        struct {
            int16_t x : 5;
        } s = {var};
        return s.x;
    }
    case 6: {
        struct {
            int16_t x : 6;
        } s = {var};
        return s.x;
    }
    case 7: {
        struct {
            int16_t x : 7;
        } s = {var};
        return s.x;
    }
    case 8: {
        struct {
            int16_t x : 8;
        } s = {var};
        return s.x;
    }
    case 9: {
        struct {
            int16_t x : 9;
        } s = {var};
        return s.x;
    }
    case 10: {
        struct {
            int16_t x : 10;
        } s = {var};
        return s.x;
    }
    case 11: {
        struct {
            int16_t x : 11;
        } s = {var};
        return s.x;
    }
    case 12: {
        struct {
            int16_t x : 12;
        } s = {var};
        return s.x;
    }
    case 13: {
        struct {
            int16_t x : 13;
        } s = {var};
        return s.x;
    }
    case 14: {
        struct {
            int16_t x : 14;
        } s = {var};
        return s.x;
    }
    case 15: {
        struct {
            int16_t x : 15;
        } s = {var};
        return s.x;
    }
    case 16: {
        return var;
    }
    default: { return 0; }
    }
}

uint16_t BIT16_Reverse(uint16_t var) {
    /* SIGNED_SHIFTS is not relavent here: as we're doing unsigned shifts */
    // swap odd and even bits
    var = ((var >> 1) & 0x5555) | ((var & 0x5555) << 1);

    // swap consecutive pairs
    var = ((var >> 2) & 0x3333) | ((var & 0x3333) << 2);

    // swap nibbles ...
    var = ((var >> 4) & 0x0F0F) | ((var & 0x0F0F) << 4);

    // swap bytes
    return ((var >> 8) | (var << 8));
}

uint16_t BIT16_Mask_Bit(uint16_t var, uint8_t bit) { return (var & BIT16_Set(bit)); }

uint16_t BIT16_Mask_Bits(uint16_t var, uint8_t start_bit, uint8_t end_bit) {
    return (var & BIT16_Bitmask(start_bit, end_bit));
}

uint16_t BIT16_Mask_nBits(uint16_t var, uint8_t start_bit, uint8_t bit_num) {
    return (var & BIT16_nBitmask(start_bit, bit_num));
}

BOOL BIT16_Get_Bit(uint16_t var, uint8_t bit) {
    /* SIGNED_SHIFTS is not relavent here: as we're doing unsigned shifts */

    /* maybe we should return BIT16_isNeg(var) if BIT16_MORETHAN_TOPBIT?? */
    return ((var >> bit) & BIT16_LESSTHAN_OR_EQUAL_TOPBIT(bit));
}

uint16_t BIT16_Get_Bits(uint16_t var, uint8_t start_bit, uint8_t end_bit) {
    return (-BIT16_LESSTHAN_OR_EQUAL_TOPBIT(start_bit) &
            ((BIT16_RBitmask(end_bit) & var) >> start_bit));
}

uint16_t BIT16_Get_nBits(uint16_t var, uint8_t start_bit, uint8_t bit_num) {
    /* error check "-(0 != bit_num) &" is used to cover the
    ** condition where "start_bit + bit_num - 1" underflows
    */
    return (-(0 != bit_num) & BIT16_Get_Bits(var, start_bit, start_bit + bit_num - 1));
}

int16_t BIT16_Get_Bits_Signed(uint16_t var, uint8_t start_bit, uint8_t end_bit) {
    /* quick error check */
    if (end_bit < start_bit) {
        return 0;
    }

    return (BIT16_Sign_Extend(BIT16_Get_Bits(var, start_bit, end_bit), end_bit - start_bit + 1));
}

int16_t BIT16_Get_nBits_Signed(uint16_t var, uint8_t start_bit, uint8_t bit_num) {
    return (BIT16_Sign_Extend(BIT16_Get_nBits(var, start_bit, bit_num), bit_num));
}

uint16_t BIT16_Set_Bit(uint16_t var, uint8_t bit) { return (var | BIT16_Set(bit)); }

uint16_t BIT16_Set_Bits(uint16_t var, uint8_t start_bit, uint8_t end_bit) {
    return (var | BIT16_Bitmask(start_bit, end_bit));
}

uint16_t BIT16_Set_nBits(uint16_t var, uint8_t start_bit, uint8_t bit_num) {
    return (var | BIT16_nBitmask(start_bit, bit_num));
}

uint16_t BIT16_Clear_Bit(uint16_t var, uint8_t bit) {
    /* set the bit we're interested in - then flip all the bits over + do AND with var */
    return (var & (~(BIT16_Set(bit))));
}

uint16_t BIT16_Clear_Bits(uint16_t var, uint8_t start_bit, uint8_t end_bit) {
    /* set the bits we're interested in - then flip all the bits over + do AND with var */
    return (var & (~(BIT16_Bitmask(start_bit, end_bit))));
}

uint16_t BIT16_Clear_nBits(uint16_t var, uint8_t start_bit, uint8_t bit_num) {
    /* set the bits we're interested in - then flip all the bits over + do AND with var */
    return (var & (~(BIT16_nBitmask(start_bit, bit_num))));
}

uint16_t BIT16_Toggle_Bit(uint16_t var, uint8_t bit) { return (var ^ (BIT16_Set(bit))); }

uint16_t BIT16_Toggle_Bits(uint16_t var, uint8_t start_bit, uint8_t end_bit) {
    return (var ^ BIT16_Bitmask(start_bit, end_bit));
}

uint16_t BIT16_Toggle_nBits(uint16_t var, uint8_t start_bit, uint8_t bit_num) {
    return (var ^ BIT16_nBitmask(start_bit, bit_num));
}

uint16_t BIT16_Insert_Bit(uint16_t var, BOOL new_bit, uint8_t bit) {
    return (BIT16_Clear_Bit(var, bit) | ((new_bit & 1) << bit));
}

uint16_t BIT16_Insert_Bits(uint16_t var, uint16_t new_bits, uint8_t start_bit, uint8_t end_bit) {
    return (BIT16_Clear_Bits(var, start_bit, end_bit) |
            BIT16_Mask_Bits(new_bits << start_bit, start_bit, end_bit));
}

uint16_t BIT16_Insert_nBits(uint16_t var, uint16_t new_bits, uint8_t start_bit, uint8_t bit_num) {
    return (BIT16_Clear_nBits(var, start_bit, bit_num) |
            BIT16_Mask_nBits(new_bits << start_bit, start_bit, bit_num));
}

char *BIT16_Bitstring(uint16_t var, char bitstring[BIT16_STRING_SIZE]) {
    /* Highly optimized implementation - always includes leading zeros for consistent output */
    static const char digits[2] = {'0', '1'};
    uint16_t mask = (1U << BIT16_TOP_BIT); /* Start with MSB mask */
    char *p = bitstring;

    /* Build string from most significant bit to least significant bit */
    for (; mask != 0; mask >>= 1) {
        *p++ = digits[(var & mask) != 0];
    }

    /* null-terminate the string */
    *p = '\0';
    return bitstring;
}

char *BIT16_BitstringBrief(uint16_t var, char bitstring[BIT16_STRING_SIZE]) {
    /* Highly optimized implementation for no leading zeros */
    static const char digits[2] = {'0', '1'};
    uint16_t mask = 0;
    char *p = bitstring;

    /* Handle zero case explicitly */
    if (var == 0) {
        *p++ = '0';
        *p = '\0';
        return bitstring;
    }

    /* Find MSB position and create initial mask */
    mask = (1U << BIT16_TOP_BIT);
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
