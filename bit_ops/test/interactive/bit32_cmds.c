/**
 * @file bit32_cmds.h
 * @brief 32-bit interactive commands for bit_ops library.
 *
 * Copyright 2010 Stephen M Bennett. All Rights Reserved.
 */

/********************************* INCLUDES **********************************/
#include "bit32_ops.h"
#include "bit_interactive.h"
#include "bit_ops.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************* CONSTANTS *********************************/

/**************************** LOCAL ENUMERATIONS *****************************/

/********************* LOCAL STRUCTURE /UNION DATA TYPES *********************/

/******************************* LOCAL MACROS ********************************/

/************************ LOCAL FUNCTIONS PROTOTYPES *************************/
static void CMD_BIT32(char *params);
static void CMD_BIT32_RBitmask(char *params);
static void CMD_BIT32_LBitmask(char *params);
static void CMD_BIT32_Bitmask(char *params);
static void CMD_BIT32_nBitmask(char *params);
static void CMD_BIT32_isNeg(char *params);
static void CMD_BIT32_Get_Sign(char *params);
static void CMD_BIT32_Count_Bits_Set(char *params);
static void CMD_BIT32_Has_Zero_Byte(char *params);
static void CMD_BIT32_Get_First_Bit_Set(char *params);
static void CMD_BIT32_Sign_Extend(char *params);
static void CMD_BIT32_Reverse(char *params);
static void CMD_BIT32_Mask_Bit(char *params);
static void CMD_BIT32_Mask_Bits(char *params);
static void CMD_BIT32_Mask_nBits(char *params);
static void CMD_BIT32_Get_Bit(char *params);
static void CMD_BIT32_Get_Bits(char *params);
static void CMD_BIT32_Get_nBits(char *params);
static void CMD_BIT32_Get_Bits_Signed(char *params);
static void CMD_BIT32_Get_nBits_Signed(char *params);
static void CMD_BIT32_Set_Bit(char *params);
static void CMD_BIT32_Set_Bits(char *params);
static void CMD_BIT32_Set_nBits(char *params);
static void CMD_BIT32_Clear_Bit(char *params);
static void CMD_BIT32_Clear_Bits(char *params);
static void CMD_BIT32_Clear_nBits(char *params);
static void CMD_BIT32_Toggle_Bit(char *params);
static void CMD_BIT32_Toggle_Bits(char *params);
static void CMD_BIT32_Toggle_nBits(char *params);
static void CMD_BIT32_Insert_Bit(char *params);
static void CMD_BIT32_Insert_Bits(char *params);
static void CMD_BIT32_Insert_nBits(char *params);
static void CMD_BIT32_SWAP_ENDIAN(char *params);

/**************************** EXPORTED FUNCTIONS *****************************/

const COMMAND_LIST bit32_commands[] = {
    /* function,                     name,                       syntax */
    {CMD_BIT32, "BIT32", "uint32_t BIT32 ( uint8_t bit );"},
    {CMD_BIT32_RBitmask, "BIT32_RBitmask", "uint32_t BIT32_RBitmask ( uint8_t left_bit );"},
    {CMD_BIT32_LBitmask, "BIT32_LBitmask", "uint32_t BIT32_LBitmask ( uint8_t right_bit );"},
    {CMD_BIT32_Bitmask,
     "BIT32_Bitmask",
     "uint32_t BIT32_Bitmask ( uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT32_nBitmask,
     "BIT32_nBitmask",
     "uint32_t BIT32_nBitmask ( uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT32_isNeg, "BIT32_isNeg", "bool BIT32_isNeg ( int32_t var );"},
    {CMD_BIT32_Get_Sign, "BIT32_Get_Sign", "int32_t BIT32_Get_Sign ( int32_t var );"},
    {CMD_BIT32_Count_Bits_Set,
     "BIT32_Count_Bits_Set",
     "uint8_t BIT32_Count_Bits_Set (uint32_t var);"},
    {CMD_BIT32_Has_Zero_Byte, "BIT32_Has_Zero_Byte", "bool BIT32_Has_Zero_Byte (uint32_t var);"},
    {CMD_BIT32_Get_First_Bit_Set,
     "BIT32_Get_First_Bit_Set",
     "uint8_t BIT32_Get_First_Bit_Set(uint32_t var);"},
    {CMD_BIT32_Sign_Extend,
     "BIT32_Sign_Extend",
     "int32_t BIT32_Sign_Extend(uint32_t var, uint8_t bit_num);"},
    {CMD_BIT32_Reverse, "BIT32_Reverse", "uint32_t BIT32_Reverse(uint32_t var);"},
    {CMD_BIT32_Mask_Bit,
     "BIT32_Mask_Bit",
     "uint32_t BIT32_Mask_Bit ( uint32_t var, uint8_t bit );"},
    {CMD_BIT32_Mask_Bits,
     "BIT32_Mask_Bits",
     "uint32_t BIT32_Mask_Bits ( uint32_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT32_Mask_nBits,
     "BIT32_Mask_nBits",
     "uint32_t BIT32_Mask_nBits ( uint32_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT32_Get_Bit, "BIT32_Get_Bit", "bool BIT32_Get_Bit ( uint32_t var, uint8_t bit );"},
    {CMD_BIT32_Get_Bits,
     "BIT32_Get_Bits",
     "uint32_t BIT32_Get_Bits ( uint32_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT32_Get_nBits,
     "BIT32_Get_nBits",
     "uint32_t BIT32_Get_nBits ( uint32_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT32_Get_Bits_Signed,
     "BIT32_Get_Bits_Signed",
     "int32_t BIT32_Get_Bits_Signed ( uint32_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT32_Get_nBits_Signed,
     "BIT32_Get_nBits_Signed",
     "int32_t BIT32_Get_nBits_Signed ( uint32_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT32_Set_Bit, "BIT32_Set_Bit", "uint32_t BIT32_Set_Bit ( uint32_t var, uint8_t bit );"},
    {CMD_BIT32_Set_Bits,
     "BIT32_Set_Bits",
     "uint32_t BIT32_Set_Bits ( uint32_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT32_Set_nBits,
     "BIT32_Set_nBits",
     "uint32_t BIT32_Set_nBits ( uint32_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT32_Clear_Bit,
     "BIT32_Clear_Bit",
     "uint32_t BIT32_Clear_Bit ( uint32_t var, uint8_t bit );"},
    {CMD_BIT32_Clear_Bits,
     "BIT32_Clear_Bits",
     "uint32_t BIT32_Clear_Bits ( uint32_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT32_Clear_nBits,
     "BIT32_Clear_nBits",
     "uint32_t BIT32_Clear_nBits ( uint32_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT32_Toggle_Bit,
     "BIT32_Toggle_Bit",
     "uint32_t BIT32_Toggle_Bit ( uint32_t var, uint8_t bit );"},
    {CMD_BIT32_Toggle_Bits,
     "BIT32_Toggle_Bits",
     "uint32_t BIT32_Toggle_Bits ( uint32_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT32_Toggle_nBits,
     "BIT32_Toggle_nBits",
     "uint32_t BIT32_Toggle_nBits( uint32_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT32_Insert_Bit,
     "BIT32_Insert_Bit",
     "uint32_t BIT32_Insert_Bit ( uint32_t var, bool new_bit, uint8_t bit );"},
    {CMD_BIT32_Insert_Bits,
     "BIT32_Insert_Bits",
     "uint32_t BIT32_Insert_Bits ( uint32_t var, uint32_t new_bits, uint8_t start_bit, uint8_t "
     "end_bit );"},
    {CMD_BIT32_Insert_nBits,
     "BIT32_Insert_nBits",
     "uint32_t BIT32_Insert_nBits( uint32_t var, uint32_t new_bits, uint8_t start_bit, uint8_t "
     "bit_num );"},
    {CMD_BIT32_SWAP_ENDIAN, "BIT32_SWAP_ENDIAN", "CMD_BIT32_SWAP_ENDIAN(var)"},
    {NULL, "", ""}};

/******************************************************************************
 *
 * Name: BIT32_Initialise
 * Description: *//**
* \brief Initialise the BIT32 module.
*
* \author Stephen M Bennett.
*
* \param[in]
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function
*
******************************************************************************/
void BIT32_Initialise(void) { BIT_Register_Commands(bit32_commands); }

/******************************************************************************
 *
 * Name: CMD_BIT32
 * Description: *//**
* \brief Command-line extension of BIT32.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32
*
******************************************************************************/
static void CMD_BIT32(char *params) {
    char *params2 = NULL;
    uint8_t bit = (uint8_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    printf("BIT32(%d) = %s\n\r", bit, BIT32_Bitstring(BIT32(bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_RBitmask
 * Description: *//**
* \brief Command-line extension of BIT32_RBitmask.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_RBitmask
*
******************************************************************************/
static void CMD_BIT32_RBitmask(char *params) {
    char *params2 = NULL;
    uint8_t left_bit = (uint8_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    printf("BIT32_RBitmask(%d) = %s\n\r",
           left_bit,
           BIT32_Bitstring(BIT32_RBitmask(left_bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_LBitmask
 * Description: *//**
* \brief Command-line extension of BIT32_LBitmask.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_LBitmask
*
******************************************************************************/
static void CMD_BIT32_LBitmask(char *params) {
    char *next_str = NULL;
    uint8_t right_bit = (uint8_t)strtoul(params, &next_str, 0);
    char bitstring[BIT32_STRING_SIZE];
    printf("BIT32_LBitmask(%d) = %s\n\r",
           right_bit,
           BIT32_Bitstring(BIT32_LBitmask(right_bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Bitmask
 * Description: *//**
* \brief Command-line extension of BIT32.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Bitmask
*
******************************************************************************/
static void CMD_BIT32_Bitmask(char *params) {
    char *params2 = NULL;
    uint8_t start_bit = (uint8_t)strtoul(params, &params2, 0);
    uint8_t end_bit = (uint8_t)strtoul(params2, &params, 0);
    char bitstring[BIT32_STRING_SIZE];
    printf("BIT32_Bitmask(%d,%d) = %s\n\r",
           start_bit,
           end_bit,
           BIT32_Bitstring(BIT32_Bitmask(start_bit, end_bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_nBitmask
 * Description: *//**
* \brief Command-line extension of BIT32_nBitmask.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_nBitmask
*
******************************************************************************/
static void CMD_BIT32_nBitmask(char *params) {
    char *params2 = NULL;
    uint8_t start_bit = (uint8_t)strtoul(params, &params2, 0);
    uint8_t bit_num = (uint8_t)strtoul(params2, &params, 0);
    char bitstring[BIT32_STRING_SIZE];
    printf("BIT32_nBitmask(%d,%d) = %s\n\r",
           start_bit,
           bit_num,
           BIT32_Bitstring(BIT32_nBitmask(start_bit, bit_num), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_isNeg
 * Description: *//**
* \brief Command-line extension of BIT32_isNeg.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_isNeg
*
******************************************************************************/
static void CMD_BIT32_isNeg(char *params) {
    char *next_str = NULL;
    int32_t var = (int32_t)strtol(params, &next_str, 0);
    printf("BIT32_isNeg(%d) = %s\n\r", var, BIT32_isNeg(var) ? "TRUE" : "FALSE");
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Get_Sign
 * Description: *//**
* \brief Command-line extension of BIT32_Get_Sign.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Get_Sign
*
******************************************************************************/
static void CMD_BIT32_Get_Sign(char *params) {
    char *next_str = NULL;
    int32_t var = (int32_t)strtol(params, &next_str, 0);
    printf("BIT32_Get_Sign(%d) = %d\n\r", var, BIT32_Get_Sign(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Count_Bits_Set
 * Description: *//**
* \brief Command-line extension of BIT32_Count_Bits_Set.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Count_Bits_Set
*
******************************************************************************/
static void CMD_BIT32_Count_Bits_Set(char *params) {
    char *next_str = NULL;
    uint32_t var = (uint32_t)strtoul(params, &next_str, 0);
    char bitstring[BIT32_STRING_SIZE];
    printf("BIT32_Count_Bits_Set(%d:%s) = %d\n\r",
           var,
           BIT32_Bitstring(var, bitstring),
           BIT32_Count_Bits_Set(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Has_Zero_Byte
 * Description: *//**
* \brief Command-line extension of BIT32_Has_Zero_Byte.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Has_Zero_Byte
*
******************************************************************************/
static void CMD_BIT32_Has_Zero_Byte(char *params) {
    char *next_str = NULL;
    uint32_t var = (uint32_t)strtoul(params, &next_str, 0);
    printf("BIT32_Has_Zero_Byte(0x%08x) = %s(%d)\n\r",
           var,
           BIT32_Has_Zero_Byte(var) ? "TRUE" : "FALSE",
           BIT32_Has_Zero_Byte(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Get_First_Bit_Set
 * Description: *//**
* \brief Command-line extension of BIT32_Get_First_Bit_Set.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Get_First_Bit_Set
*
******************************************************************************/
static void CMD_BIT32_Get_First_Bit_Set(char *params) {
    char *next_str = NULL;
    uint32_t var = (uint32_t)strtoul(params, &next_str, 0);
    char bitstring[BIT32_STRING_SIZE];
    printf("BIT32_Get_First_Bit_Set(%d:%s) = %d\n\r",
           var,
           BIT32_Bitstring(var, bitstring),
           BIT32_Get_First_Bit_Set(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Sign_Extend
 * Description: *//**
* \brief Command-line extension of BIT32_Sign_Extend.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Sign_Extend
*
******************************************************************************/
static void CMD_BIT32_Sign_Extend(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t bitnum = (int32_t)strtoul(params2, &params, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Sign_Extend(%d:%s,%d) = %d:%s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           bitnum,
           BIT32_Sign_Extend(var, bitnum),
           BIT32_Bitstring(BIT32_Sign_Extend(var, bitnum), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Reverse
 * Description: *//**
* \brief Command-line extension of BIT32_Reverse.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Reverse
*
******************************************************************************/
static void CMD_BIT32_Reverse(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Reverse(%d:%s) = %d:%s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           BIT32_Reverse(var),
           BIT32_Bitstring(BIT32_Reverse(var), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Mask_Bit
 * Description: *//**
* \brief Command-line extension of BIT32_Mask_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Mask_Bit
*
******************************************************************************/
static void CMD_BIT32_Mask_Bit(char *params) {
    char *params2 = NULL;
    uint32_t var = (uint32_t)strtoul(params, &params2, 0);
    uint32_t bit_num = (uint32_t)strtoul(params2, &params, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Mask_Bit(%d:%s,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           bit_num,
           BIT32_Bitstring(BIT32_Mask_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Mask_Bits
 * Description: *//**
* \brief Command-line extension of BIT32_Mask_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Mask_Bits
*
******************************************************************************/
static void CMD_BIT32_Mask_Bits(char *params) {
    char *params2 = NULL;
    uint32_t var = (uint32_t)strtoul(params, &params2, 0);
    uint32_t start_bit = (uint32_t)strtoul(params2, &params, 0);
    uint32_t end_bit = (uint32_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Mask_Bits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT32_Bitstring(BIT32_Mask_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Mask_nBits
 * Description: *//**
* \brief Command-line extension of BIT32_Mask_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Mask_nBits
*
******************************************************************************/
static void CMD_BIT32_Mask_nBits(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t start_bit = (int32_t)strtoul(params2, &params, 0);
    uint32_t num_bits = (int32_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Mask_nBits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT32_Bitstring(BIT32_Mask_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Get_Bit
 * Description: *//**
* \brief Command-line extension of BIT32_Get_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Get_Bit
*
******************************************************************************/
static void CMD_BIT32_Get_Bit(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t bit_num = (int32_t)strtoul(params2, &params, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Get_Bit(%d:%s,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           bit_num,
           BIT32_Bitstring(BIT32_Get_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Get_Bits
 * Description: *//**
* \brief Command-line extension of BIT32_Get_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Get_Bits
*
******************************************************************************/
static void CMD_BIT32_Get_Bits(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t start_bit = (int32_t)strtoul(params2, &params, 0);
    uint32_t end_bit = (int32_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Get_Bits(%d:%s,%d,%d) = %d:%s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT32_Get_Bits(var, start_bit, end_bit),
           BIT32_BitstringBrief(BIT32_Get_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Get_nBits
 * Description: *//**
* \brief Command-line extension of BIT32_Get_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Get_nBits
*
******************************************************************************/
static void CMD_BIT32_Get_nBits(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t start_bit = (int32_t)strtoul(params2, &params, 0);
    uint32_t num_bits = (int32_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Get_nBits(%d:%s,%d,%d) = %d:%s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT32_Get_nBits(var, start_bit, num_bits),
           BIT32_BitstringBrief(BIT32_Get_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Get_Bits_Signed
 * Description: *//**
* \brief Command-line extension of BIT32_Get_Bits_Signed.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Get_Bits_Signed
*
******************************************************************************/
static void CMD_BIT32_Get_Bits_Signed(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t start_bit = (int32_t)strtoul(params2, &params, 0);
    uint32_t end_bit = (int32_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Get_Bits_Signed(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT32_Bitstring(BIT32_Get_Bits_Signed(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_nBits_Signed
 * Description: *//**
* \brief Command-line extension of BIT32_nBits_Signed.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_nBits_Signed
*
******************************************************************************/
static void CMD_BIT32_Get_nBits_Signed(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t start_bit = (int32_t)strtoul(params2, &params, 0);
    uint32_t num_bits = (int32_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Get_nBits_Signed(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT32_Bitstring((uint32_t)BIT32_Get_nBits_Signed(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Set_Bit
 * Description: *//**
* \brief Command-line extension of BIT32_Set_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Set_Bit
*
******************************************************************************/
static void CMD_BIT32_Set_Bit(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t bit_num = (int32_t)strtoul(params2, &params, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Set_Bit(%d:%s,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           bit_num,
           BIT32_Bitstring(BIT32_Set_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Set_Bits
 * Description: *//**
* \brief Command-line extension of BIT32_Set_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Set_Bits
*
******************************************************************************/
static void CMD_BIT32_Set_Bits(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t start_bit = (int32_t)strtoul(params2, &params, 0);
    uint32_t end_bit = (int32_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Set_Bits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT32_Bitstring(BIT32_Set_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Set_nBits
 * Description: *//**
* \brief Command-line extension of BIT32_Set_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Set_nBits
*
******************************************************************************/
static void CMD_BIT32_Set_nBits(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t start_bit = (int32_t)strtoul(params2, &params, 0);
    uint32_t num_bits = (int32_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Set_nBits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT32_Bitstring(BIT32_Set_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Clear_Bit
 * Description: *//**
* \brief Command-line extension of BIT32_Clear_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Clear_Bit
*
******************************************************************************/
static void CMD_BIT32_Clear_Bit(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t bit_num = (int32_t)strtoul(params2, &params, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Clear_Bit(%d:%s,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           bit_num,
           BIT32_Bitstring(BIT32_Clear_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Clear_Bits
 * Description: *//**
* \brief Command-line extension of BIT32_Clear_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Clear_Bits
*
******************************************************************************/
static void CMD_BIT32_Clear_Bits(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t start_bit = (int32_t)strtoul(params2, &params, 0);
    uint32_t end_bit = (int32_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Clear_Bits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT32_Bitstring(BIT32_Clear_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Clear_nBits
 * Description: *//**
* \brief Command-line extension of BIT32_Clear_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Clear_nBits
*
******************************************************************************/
static void CMD_BIT32_Clear_nBits(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t start_bit = (int32_t)strtoul(params2, &params, 0);
    uint32_t num_bits = (int32_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Clear_nBits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT32_Bitstring(BIT32_Clear_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Toggle_Bit
 * Description: *//**
* \brief Command-line extension of BIT32_Toggle_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Toggle_Bit
*
******************************************************************************/
static void CMD_BIT32_Toggle_Bit(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t bit_num = (int32_t)strtoul(params2, &params, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Toggle_Bit(%d:%s,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           bit_num,
           BIT32_Bitstring(BIT32_Toggle_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Toggle_Bits
 * Description: *//**
* \brief Command-line extension of BIT32_Toggle_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Toggle_Bits
*
******************************************************************************/
static void CMD_BIT32_Toggle_Bits(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t start_bit = (int32_t)strtoul(params2, &params, 0);
    uint32_t end_bit = (int32_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Toggle_Bits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT32_Bitstring(BIT32_Toggle_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Toggle_nBits
 * Description: *//**
* \brief Command-line extension of BIT32_Toggle_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Toggle_nBits
*
******************************************************************************/
static void CMD_BIT32_Toggle_nBits(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t start_bit = (int32_t)strtoul(params2, &params, 0);
    uint32_t num_bits = (int32_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Toggle_nBits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT32_Bitstring(BIT32_Toggle_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Insert_Bit
 * Description: *//**
* \brief Command-line extension of BIT32_Insert_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Insert_Bit
*
******************************************************************************/
static void CMD_BIT32_Insert_Bit(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t new_bit = strtoul(params2, &params, 0) & 1;
    uint32_t bit = (int32_t)strtoul(params, &params2, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Insert_Bit(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           new_bit,
           bit,
           BIT32_Bitstring(BIT32_Insert_Bit(var, new_bit, bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Insert_Bits
 * Description: *//**
* \brief Command-line extension of BIT32_Insert_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Insert_Bits
*
******************************************************************************/
static void CMD_BIT32_Insert_Bits(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t new_bits = (int32_t)strtoul(params2, &params, 0);
    uint32_t start_bit = (int32_t)strtoul(params, &params2, 0);
    uint32_t end_bit = (int32_t)strtoul(params2, &params, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Insert_Bits(%d:%s,%d,%d,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           new_bits,
           start_bit,
           end_bit,
           BIT32_Bitstring(BIT32_Insert_Bits(var, new_bits, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT32_Insert_nBits
 * Description: *//**
* \brief Command-line extension of BIT32_Insert_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit32_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT32_Insert_nBits
*
******************************************************************************/
static void CMD_BIT32_Insert_nBits(char *params) {
    char *params2 = NULL;
    uint32_t var = (int32_t)strtoul(params, &params2, 0);
    uint32_t new_bits = (int32_t)strtoul(params2, &params, 0);
    uint32_t start_bit = (int32_t)strtoul(params, &params2, 0);
    uint32_t num_bits = (int32_t)strtoul(params2, &params, 0);
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    printf("BIT32_Insert_nBits(%d:%s,%d,%d,%d) = %s\n\r",
           var,
           BIT32_BitstringBrief(var, bitstring),
           new_bits,
           start_bit,
           num_bits,
           BIT32_Bitstring(BIT32_Insert_nBits(var, new_bits, start_bit, num_bits), bitstring2));
}

static void CMD_BIT32_SWAP_ENDIAN(char *params) {
    char *params2 = NULL;
    uint32_t var = (uint32_t)strtoul(params, &params2, 0);
    uint32_t var_swap = var;

    BIT32_SWAP_ENDIAN(var_swap);

    printf("BIT32_SWAP_ENDIAN(0x%08x) = 0x%08x\n\r", var, var_swap);
}
