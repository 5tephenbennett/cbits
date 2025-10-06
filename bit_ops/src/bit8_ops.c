/**
 * @file bit8_ops.c
 * @brief 8-bit integer bit manipulation operations implementation
 *
 * Copyright 2009-2010 Stephen M Bennett. All Rights Reserved.
 */

/********************************* INCLUDES **********************************/
#include "bit8_ops.h"
#include "bit_ops.h"

/********************************* CONSTANTS *********************************/

/************************** LOCAL BASIC DATA TYPES ***************************/

/**************************** LOCAL ENUMERATIONS *****************************/

/********************* LOCAL STRUCTURE /UNION DATA TYPES *********************/

/******************************* LOCAL MACROS ********************************/

/************************ LOCAL FUNCTIONS PROTOTYPES *************************/

/**************************** EXPORTED FUNCTIONS *****************************/

uint8_t BIT8_Set(uint8_t bit) { return (BIT8_LESSTHAN_OR_EQUAL_TOPBIT(bit) << bit); }

uint8_t BIT8_RBitmask(uint8_t left_bit) {
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
    return (-BIT8_MORETHAN_TOPBIT(left_bit) | (~((uint8_t)-2 << left_bit)));
}

uint8_t BIT8_LBitmask(uint8_t right_bit) {
    /* The expression below is effectively "(-1 << right_bit);"
    ** with the following error checking : -
    **    if right_bit > topbit bitmask will be zero
    */
    return ((uint8_t)-BIT8_LESSTHAN_OR_EQUAL_TOPBIT(right_bit) << right_bit);
}

uint8_t BIT8_Bitmask(uint8_t start_bit, uint8_t end_bit) {
    // return (BIT8_RBitmask(end_bit) & BIT8_LBitmask(start_bit));
    return ((-BIT8_MORETHAN_TOPBIT(end_bit) | (~((uint8_t)-2 << end_bit))) &
            ((uint8_t)-BIT8_LESSTHAN_OR_EQUAL_TOPBIT(start_bit) << start_bit));
}

uint8_t BIT8_nBitmask(uint8_t start_bit, uint8_t bit_num) {
    /* The expression below is effectively "((~(-1 << bit_num)) << start_bit);"
    ** with the following error checking : -
    **    if start_bit > topbit bitmask will be zero
    **    else if bit_num > topbit unshifted bitmask will be all one's
    */
    return (
        -BIT8_LESSTHAN_OR_EQUAL_TOPBIT(start_bit) &
        ((uint8_t)(~((uint8_t)-BIT8_LESSTHAN_OR_EQUAL_TOPBIT(bit_num) << bit_num)) << start_bit));
}

BOOL BIT8_isNeg(int8_t var) {
#ifdef ARCH_TWOS_COMPLEMENT
    /* SIGNED_SHIFTS is not relavent here: as we're only masking the top-bit  */
    return ((var >> BIT8_TOP_BIT) & 1);
#else
    return (var < 0);
#endif
}

int8_t BIT8_Get_Sign(int8_t var) {
#ifdef ARCH_TWOS_COMPLEMENT
    /* SIGNED_SHIFTS is not relavent here: as we're only masking the top-bit  */
    return (-((var >> BIT8_TOP_BIT) & 1) | 1);
#else
    return ((var >= 0) - (var < 0));
#endif
}

uint8_t BIT8_Count_Bits_Set(uint8_t var) {
#ifdef COUNT_BITS_LOOKUP
    return (bits_set_table256[var]);
#else
    /* SIGNED_SHIFTS is not relavent here: as we're doing unsigned shifts */
    var = var - ((var >> 1) & 0x55);          // reuse input as temporary
    var = (var & 0x33) + ((var >> 2) & 0x33); // temp
    return (var + ((var >> 4) & 0x0F));       // count
#endif
}

BOOL BIT8_Has_Zero_Byte(uint8_t var) { return (0 == var); }

uint8_t BIT8_Get_First_Bit_Set(uint8_t var) {
    static const uint8_t debruijn32[32] = {0,  1,  28, 2,  29, 14, 24, 3,  30, 22, 20,
                                           15, 25, 17, 4,  8,  31, 27, 13, 23, 21, 19,
                                           16, 7,  26, 12, 18, 6,  11, 5,  10, 9};
    return (var ? debruijn32[((uint32_t)((var & -var) * 0x077CB531U)) >> 27] : BIT8_BITS);
}

int8_t BIT8_Sign_Extend(uint8_t var, uint8_t bit_num) {
    switch (bit_num) {
    case 1: {
        struct {
            int8_t x : 1;
        } s = {var};
        return s.x;
    }
    case 2: {
        struct {
            int8_t x : 2;
        } s = {var};
        return s.x;
    }
    case 3: {
        struct {
            int8_t x : 3;
        } s = {var};
        return s.x;
    }
    case 4: {
        struct {
            int8_t x : 4;
        } s = {var};
        return s.x;
    }
    case 5: {
        struct {
            int8_t x : 5;
        } s = {var};
        return s.x;
    }
    case 6: {
        struct {
            int8_t x : 6;
        } s = {var};
        return s.x;
    }
    case 7: {
        struct {
            int8_t x : 7;
        } s = {var};
        return s.x;
    }
    case 8: {
        return var;
    }
    default: { return 0; }
    }
}

uint8_t BIT8_Reverse(uint8_t var) {
    /* SIGNED_SHIFTS is not relavent here: as we're doing unsigned shifts */

    // swap odd and even bits
    var = ((var >> 1) & 0x55) | ((var & 0x55) << 1);

    // swap consecutive pairs
    var = ((var >> 2) & 0x33) | ((var & 0x33) << 2);

    // swap nibbles ...
    return ((var >> 4) | (var << 4));
}

uint8_t BIT8_Mask_Bit(uint8_t var, uint8_t bit) { return (var & BIT8_Set(bit)); }

uint8_t BIT8_Mask_Bits(uint8_t var, uint8_t start_bit, uint8_t end_bit) {
    return (var & BIT8_Bitmask(start_bit, end_bit));
}

uint8_t BIT8_Mask_nBits(uint8_t var, uint8_t start_bit, uint8_t bit_num) {
    return (var & BIT8_nBitmask(start_bit, bit_num));
}

BOOL BIT8_Get_Bit(uint8_t var, uint8_t bit) {
    /* SIGNED_SHIFTS is not relavent here: as we're doing unsigned shifts */

    /* maybe we should return BIT8_isNeg(var) if BIT8_MORETHAN_TOPBIT?? */
    return ((var >> bit) & BIT8_LESSTHAN_OR_EQUAL_TOPBIT(bit));
}

uint8_t BIT8_Get_Bits(uint8_t var, uint8_t start_bit, uint8_t end_bit) {
    return (-BIT8_LESSTHAN_OR_EQUAL_TOPBIT(start_bit) &
            ((BIT8_RBitmask(end_bit) & var) >> start_bit));
}

uint8_t BIT8_Get_nBits(uint8_t var, uint8_t start_bit, uint8_t bit_num) {
    /* error check "-(0 != bit_num) &" is used to cover the
    ** condition where "start_bit + bit_num - 1" underflows
    */
    return (-(0 != bit_num) & BIT8_Get_Bits(var, start_bit, start_bit + bit_num - 1));
}

int8_t BIT8_Get_Bits_Signed(uint8_t var, uint8_t start_bit, uint8_t end_bit) {
    /* quick error check */
    if (end_bit < start_bit) {
        return 0;
    }

    return (BIT8_Sign_Extend(BIT8_Get_Bits(var, start_bit, end_bit), end_bit - start_bit + 1));
}

int8_t BIT8_Get_nBits_Signed(uint8_t var, uint8_t start_bit, uint8_t bit_num) {
    return (BIT8_Sign_Extend(BIT8_Get_nBits(var, start_bit, bit_num), bit_num));
}

uint8_t BIT8_Set_Bit(uint8_t var, uint8_t bit) { return (var | BIT8_Set(bit)); }

uint8_t BIT8_Set_Bits(uint8_t var, uint8_t start_bit, uint8_t end_bit) {
    return (var | BIT8_Bitmask(start_bit, end_bit));
}

uint8_t BIT8_Set_nBits(uint8_t var, uint8_t start_bit, uint8_t bit_num) {
    return (var | BIT8_nBitmask(start_bit, bit_num));
}

uint8_t BIT8_Clear_Bit(uint8_t var, uint8_t bit) {
    /* set the bit we're interested in - then flip all the bits over + do AND with var */
    return (var & (~(BIT8_Set(bit))));
}

uint8_t BIT8_Clear_Bits(uint8_t var, uint8_t start_bit, uint8_t end_bit) {
    /* set the bits we're interested in - then flip all the bits over + do AND with var */
    return (var & (~(BIT8_Bitmask(start_bit, end_bit))));
}

uint8_t BIT8_Clear_nBits(uint8_t var, uint8_t start_bit, uint8_t bit_num) {
    return (var & (~(BIT8_nBitmask(start_bit, bit_num))));
}

uint8_t BIT8_Toggle_Bit(uint8_t var, uint8_t bit) { return (var ^ (BIT8_Set(bit))); }

uint8_t BIT8_Toggle_Bits(uint8_t var, uint8_t start_bit, uint8_t end_bit) {
    return (var ^ BIT8_Bitmask(start_bit, end_bit));
}

uint8_t BIT8_Toggle_nBits(uint8_t var, uint8_t start_bit, uint8_t bit_num) {
    return (var ^ BIT8_nBitmask(start_bit, bit_num));
}

uint8_t BIT8_Insert_Bit(uint8_t var, BOOL new_bit, uint8_t bit) {
    return (BIT8_Clear_Bit(var, bit) | ((new_bit & 1) << bit));
}

uint8_t BIT8_Insert_Bits(uint8_t var, uint8_t new_bits, uint8_t start_bit, uint8_t end_bit) {
    return (BIT8_Clear_Bits(var, start_bit, end_bit) |
            BIT8_Mask_Bits(new_bits << start_bit, start_bit, end_bit));
}

uint8_t BIT8_Insert_nBits(uint8_t var, uint8_t new_bits, uint8_t start_bit, uint8_t bit_num) {
    return (BIT8_Clear_nBits(var, start_bit, bit_num) |
            BIT8_Mask_nBits(new_bits << start_bit, start_bit, bit_num));
}

char *BIT8_Bitstring(uint8_t var, char bitstring[BIT8_STRING_SIZE]) {
    /* Highly optimized implementation - always includes leading zeros for consistent output */
    static const char digits[2] = {'0', '1'};
    uint8_t mask = (1U << BIT8_TOP_BIT); /* Start with MSB mask */
    char *p = bitstring;

    /* Build string from most significant bit to least significant bit */
    for (; mask != 0; mask >>= 1) {
        *p++ = digits[(var & mask) != 0];
    }

    /* null-terminate the string */
    *p = '\0';
    return bitstring;
}

char *BIT8_BitstringBrief(uint8_t var, char bitstring[BIT8_STRING_SIZE]) {
    /* Highly optimized implementation for no leading zeros */
    static const char digits[2] = {'0', '1'};
    uint8_t mask = 0;
    uint8_t str_pos = 0;

    /* Handle zero case explicitly */
    if (var == 0) {
        bitstring[0] = '0';
        bitstring[1] = '\0';
        return bitstring;
    }

    /* Find MSB position and create initial mask */
    mask = (1U << BIT8_TOP_BIT);
    while ((var & mask) == 0) {
        mask >>= 1;
    }

    /* Build string from highest set bit down to bit 0 */
    while (mask != 0) {
        bitstring[str_pos++] = digits[(var & mask) != 0];
        mask >>= 1;
    }

    /* null-terminate the string */
    bitstring[str_pos] = '\0';
    return bitstring;
}

/***************************** LOCAL FUNCTIONS *******************************/

/******************************************************************************
 * End of file
 ******************************************************************************/
