/**
 * @file bit8_cmds.h
 * @brief 8-bit interactive commands for bit_ops library.
 *
 * Copyright 2010 Stephen M Bennett. All Rights Reserved.
 */

/********************************* INCLUDES **********************************/
#include "bit8_ops.h"
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
static void CMD_BIT8(char *params);
static void CMD_BIT8_RBitmask(char *params);
static void CMD_BIT8_LBitmask(char *params);
static void CMD_BIT8_Bitmask(char *params);
static void CMD_BIT8_nBitmask(char *params);
static void CMD_BIT8_isNeg(char *params);
static void CMD_BIT8_Get_Sign(char *params);
static void CMD_BIT8_Count_Bits_Set(char *params);
static void CMD_BIT8_Has_Zero_Byte(char *params);
static void CMD_BIT8_Get_First_Bit_Set(char *params);
static void CMD_BIT8_Sign_Extend(char *params);
static void CMD_BIT8_Reverse(char *params);
static void CMD_BIT8_Mask_Bit(char *params);
static void CMD_BIT8_Mask_Bits(char *params);
static void CMD_BIT8_Mask_nBits(char *params);
static void CMD_BIT8_Get_Bit(char *params);
static void CMD_BIT8_Get_Bits(char *params);
static void CMD_BIT8_Get_nBits(char *params);
static void CMD_BIT8_Get_Bits_Signed(char *params);
static void CMD_BIT8_Get_nBits_Signed(char *params);
static void CMD_BIT8_Set_Bit(char *params);
static void CMD_BIT8_Set_Bits(char *params);
static void CMD_BIT8_Set_nBits(char *params);
static void CMD_BIT8_Clear_Bit(char *params);
static void CMD_BIT8_Clear_Bits(char *params);
static void CMD_BIT8_Clear_nBits(char *params);
static void CMD_BIT8_Toggle_Bit(char *params);
static void CMD_BIT8_Toggle_Bits(char *params);
static void CMD_BIT8_Toggle_nBits(char *params);
static void CMD_BIT8_Insert_Bit(char *params);
static void CMD_BIT8_Insert_Bits(char *params);
static void CMD_BIT8_Insert_nBits(char *params);

/**************************** EXPORTED FUNCTIONS *****************************/

const COMMAND_LIST bit8_commands[] = {
    /* function,                     name,                       syntax */
    {CMD_BIT8, "BIT8", "uint8_t BIT8 ( uint8_t bit );"},
    {CMD_BIT8_RBitmask, "BIT8_RBitmask", "uint8_t BIT8_RBitmask ( uint8_t left_bit );"},
    {CMD_BIT8_LBitmask, "BIT8_LBitmask", "uint8_t BIT8_LBitmask ( uint8_t right_bit );"},
    {CMD_BIT8_Bitmask,
     "BIT8_Bitmask",
     "uint8_t BIT8_Bitmask ( uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT8_nBitmask,
     "BIT8_nBitmask",
     "uint8_t BIT8_nBitmask ( uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT8_isNeg, "BIT8_isNeg", "bool BIT8_isNeg ( int8_t var );"},
    {CMD_BIT8_Get_Sign, "BIT8_Get_Sign", "int8_t BIT8_Get_Sign ( int8_t var );"},
    {CMD_BIT8_Count_Bits_Set, "BIT8_Count_Bits_Set", "uint8_t BIT8_Count_Bits_Set (uint8_t var);"},
    {CMD_BIT8_Has_Zero_Byte, "BIT8_Has_Zero_Byte", "bool BIT8_Has_Zero_Byte (uint8_t var);"},
    {CMD_BIT8_Get_First_Bit_Set,
     "BIT8_Get_First_Bit_Set",
     "uint8_t BIT8_Get_First_Bit_Set(uint8_t var);"},
    {CMD_BIT8_Sign_Extend,
     "BIT8_Sign_Extend",
     "int8_t BIT8_Sign_Extend(uint8_t var, uint8_t bit_num);"},
    {CMD_BIT8_Reverse, "BIT8_Reverse", "uint8_t BIT8_Reverse(uint8_t var);"},
    {CMD_BIT8_Mask_Bit, "BIT8_Mask_Bit", "uint8_t BIT8_Mask_Bit ( uint8_t var, uint8_t bit );"},
    {CMD_BIT8_Mask_Bits,
     "BIT8_Mask_Bits",
     "uint8_t BIT8_Mask_Bits ( uint8_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT8_Mask_nBits,
     "BIT8_Mask_nBits",
     "uint8_t BIT8_Mask_nBits ( uint8_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT8_Get_Bit, "BIT8_Get_Bit", "bool BIT8_Get_Bit ( uint8_t var, uint8_t bit );"},
    {CMD_BIT8_Get_Bits,
     "BIT8_Get_Bits",
     "uint8_t BIT8_Get_Bits ( uint8_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT8_Get_nBits,
     "BIT8_Get_nBits",
     "uint8_t BIT8_Get_nBits ( uint8_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT8_Get_Bits_Signed,
     "BIT8_Get_Bits_Signed",
     "int8_t BIT8_Get_Bits_Signed ( uint8_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT8_Get_nBits_Signed,
     "BIT8_Get_nBits_Signed",
     "int8_t BIT8_Get_nBits_Signed ( uint8_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT8_Set_Bit, "BIT8_Set_Bit", "uint8_t BIT8_Set_Bit ( uint8_t var, uint8_t bit );"},
    {CMD_BIT8_Set_Bits,
     "BIT8_Set_Bits",
     "uint8_t BIT8_Set_Bits ( uint8_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT8_Set_nBits,
     "BIT8_Set_nBits",
     "uint8_t BIT8_Set_nBits ( uint8_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT8_Clear_Bit, "BIT8_Clear_Bit", "uint8_t BIT8_Clear_Bit ( uint8_t var, uint8_t bit );"},
    {CMD_BIT8_Clear_Bits,
     "BIT8_Clear_Bits",
     "uint8_t BIT8_Clear_Bits ( uint8_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT8_Clear_nBits,
     "BIT8_Clear_nBits",
     "uint8_t BIT8_Clear_nBits ( uint8_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT8_Toggle_Bit,
     "BIT8_Toggle_Bit",
     "uint8_t BIT8_Toggle_Bit ( uint8_t var, uint8_t bit );"},
    {CMD_BIT8_Toggle_Bits,
     "BIT8_Toggle_Bits",
     "uint8_t BIT8_Toggle_Bits ( uint8_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT8_Toggle_nBits,
     "BIT8_Toggle_nBits",
     "uint8_t BIT8_Toggle_nBits( uint8_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT8_Insert_Bit,
     "BIT8_Insert_Bit",
     "uint8_t BIT8_Insert_Bit ( uint8_t var, bool new_bit, uint8_t bit );"},
    {CMD_BIT8_Insert_Bits,
     "BIT8_Insert_Bits",
     "uint8_t BIT8_Insert_Bits ( uint8_t var, uint8_t new_bits, uint8_t start_bit, uint8_t end_bit "
     ");"},
    {CMD_BIT8_Insert_nBits,
     "BIT8_Insert_nBits",
     "uint8_t BIT8_Insert_nBits( uint8_t var, uint8_t new_bits, uint8_t start_bit, uint8_t bit_num "
     ");"},
    {NULL, "", ""}};

/******************************************************************************
 *
 * Name: BIT8_Initialise
 * Description: *//**
* \brief Initialise the BIT8 module.
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
void BIT8_Initialise(void) { BIT_Register_Commands(bit8_commands); }

/******************************************************************************
 *
 * Name: CMD_BIT8
 * Description: *//**
* \brief Command-line extension of BIT8.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8
*
******************************************************************************/
static void CMD_BIT8(char *params) {
    char *params2 = NULL;
    uint8_t bit = (uint8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    printf("BIT8(%d) = %s\n\r", bit, BIT8_Bitstring(BIT8(bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_RBitmask
 * Description: *//**
* \brief Command-line extension of BIT8_RBitmask.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_RBitmask
*
******************************************************************************/
static void CMD_BIT8_RBitmask(char *params) {
    char *params2 = NULL;
    uint8_t left_bit = (uint8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    printf(
        "BIT8_RBitmask(%d) = %s\n\r", left_bit, BIT8_Bitstring(BIT8_RBitmask(left_bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_LBitmask
 * Description: *//**
* \brief Command-line extension of BIT8_LBitmask.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_LBitmask
*
******************************************************************************/
static void CMD_BIT8_LBitmask(char *params) {
    char *next_str = NULL;
    uint8_t right_bit = (uint8_t)strtoul(params, &next_str, 0);
    char bitstring[BIT8_STRING_SIZE];
    printf("BIT8_LBitmask(%d) = %s\n\r",
           right_bit,
           BIT8_Bitstring(BIT8_LBitmask(right_bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Bitmask
 * Description: *//**
* \brief Command-line extension of BIT8.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Bitmask
*
******************************************************************************/
static void CMD_BIT8_Bitmask(char *params) {
    char *params2 = NULL;
    uint8_t start_bit = (uint8_t)strtoul(params, &params2, 0);
    uint8_t end_bit = (uint8_t)strtoul(params2, &params, 0);
    char bitstring[BIT8_STRING_SIZE];
    printf("BIT8_Bitmask(%d,%d) = %s\n\r",
           start_bit,
           end_bit,
           BIT8_Bitstring(BIT8_Bitmask(start_bit, end_bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_nBitmask
 * Description: *//**
* \brief Command-line extension of BIT8_nBitmask.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_nBitmask
*
******************************************************************************/
static void CMD_BIT8_nBitmask(char *params) {
    char *params2 = NULL;
    uint8_t start_bit = (uint8_t)strtoul(params, &params2, 0);
    uint8_t bit_num = (uint8_t)strtoul(params2, &params, 0);
    char bitstring[BIT8_STRING_SIZE];
    printf("BIT8_nBitmask(%d,%d) = %s\n\r",
           start_bit,
           bit_num,
           BIT8_Bitstring(BIT8_nBitmask(start_bit, bit_num), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_isNeg
 * Description: *//**
* \brief Command-line extension of BIT8_isNeg.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_isNeg
*
******************************************************************************/
static void CMD_BIT8_isNeg(char *params) {
    char *next_str = NULL;
    int8_t var = (int8_t)strtol(params, &next_str, 0);
    printf("BIT8_isNeg(%d) = %s\n\r", var, BIT8_isNeg(var) ? "TRUE" : "FALSE");
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Get_Sign
 * Description: *//**
* \brief Command-line extension of BIT8_Get_Sign.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Get_Sign
*
******************************************************************************/
static void CMD_BIT8_Get_Sign(char *params) {
    char *next_str = NULL;
    int8_t var = (int8_t)strtol(params, &next_str, 0);
    printf("BIT8_Get_Sign(%d) = %d\n\r", var, BIT8_Get_Sign(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Count_Bits_Set
 * Description: *//**
* \brief Command-line extension of BIT8_Count_Bits_Set.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Count_Bits_Set
*
******************************************************************************/
static void CMD_BIT8_Count_Bits_Set(char *params) {
    char *next_str = NULL;
    uint8_t var = (uint8_t)strtoul(params, &next_str, 0);
    char bitstring[BIT8_STRING_SIZE];
    printf("BIT8_Count_Bits_Set(%d:%s) = %d\n\r",
           var,
           BIT8_Bitstring(var, bitstring),
           BIT8_Count_Bits_Set(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Has_Zero_Byte
 * Description: *//**
* \brief Command-line extension of BIT8_Has_Zero_Byte.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Has_Zero_Byte
*
******************************************************************************/
static void CMD_BIT8_Has_Zero_Byte(char *params) {
    char *next_str = NULL;
    uint8_t var = (uint8_t)strtoul(params, &next_str, 0);
    printf("BIT8_Has_Zero_Byte(0x%08x) = %s(%d)\n\r",
           var,
           BIT8_Has_Zero_Byte(var) ? "TRUE" : "FALSE",
           BIT8_Has_Zero_Byte(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Get_First_Bit_Set
 * Description: *//**
* \brief Command-line extension of BIT8_Get_First_Bit_Set.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Get_First_Bit_Set
*
******************************************************************************/
static void CMD_BIT8_Get_First_Bit_Set(char *params) {
    char *next_str = NULL;
    uint8_t var = (uint8_t)strtoul(params, &next_str, 0);
    char bitstring[BIT8_STRING_SIZE];
    printf("BIT8_Get_First_Bit_Set(%d:%s) = %d\n\r",
           var,
           BIT8_Bitstring(var, bitstring),
           BIT8_Get_First_Bit_Set(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Sign_Extend
 * Description: *//**
* \brief Command-line extension of BIT8_Sign_Extend.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Sign_Extend
*
******************************************************************************/
static void CMD_BIT8_Sign_Extend(char *params) {
    char *params2 = NULL;
    uint8_t var = (int8_t)strtoul(params, &params2, 0);
    uint8_t bitnum = (int8_t)strtoul(params2, &params, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Sign_Extend(%d:%s,%d) = %d:%s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           bitnum,
           BIT8_Sign_Extend(var, bitnum),
           BIT8_Bitstring(BIT8_Sign_Extend(var, bitnum), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Reverse
 * Description: *//**
* \brief Command-line extension of BIT8_Reverse.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Reverse
*
******************************************************************************/
static void CMD_BIT8_Reverse(char *params) {
    char *params2 = NULL;
    uint8_t var = (int8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Reverse(%d:%s) = %d:%s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           BIT8_Reverse(var),
           BIT8_Bitstring(BIT8_Reverse(var), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Mask_Bit
 * Description: *//**
* \brief Command-line extension of BIT8_Mask_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Mask_Bit
*
******************************************************************************/
static void CMD_BIT8_Mask_Bit(char *params) {
    char *params2 = NULL;
    uint8_t var = (int8_t)strtoul(params, &params2, 0);
    uint8_t bit_num = (int8_t)strtoul(params2, &params, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Mask_Bit(%d:%s,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           bit_num,
           BIT8_Bitstring(BIT8_Mask_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Mask_Bits
 * Description: *//**
* \brief Command-line extension of BIT8_Mask_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Mask_Bits
*
******************************************************************************/
static void CMD_BIT8_Mask_Bits(char *params) {
    char *params2 = NULL;
    uint8_t var = (uint8_t)strtoul(params, &params2, 0);
    uint8_t start_bit = (uint8_t)strtoul(params2, &params, 0);
    uint8_t end_bit = (uint8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Mask_Bits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT8_Bitstring(BIT8_Mask_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Mask_nBits
 * Description: *//**
* \brief Command-line extension of BIT8_Mask_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Mask_nBits
*
******************************************************************************/
static void CMD_BIT8_Mask_nBits(char *params) {
    char *params2 = NULL;
    uint8_t var = (int8_t)strtoul(params, &params2, 0);
    uint8_t start_bit = (int8_t)strtoul(params2, &params, 0);
    uint8_t num_bits = (int8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Mask_nBits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT8_Bitstring(BIT8_Mask_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Get_Bit
 * Description: *//**
* \brief Command-line extension of BIT8_Get_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Get_Bit
*
******************************************************************************/
static void CMD_BIT8_Get_Bit(char *params) {
    char *params2 = NULL;
    uint8_t var = (uint8_t)strtoul(params, &params2, 0);
    uint8_t bit_num = (int8_t)strtoul(params2, &params, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Get_Bit(%d:%s,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           bit_num,
           BIT8_Bitstring(BIT8_Get_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Get_Bits
 * Description: *//**
* \brief Command-line extension of BIT8_Get_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Get_Bits
*
******************************************************************************/
static void CMD_BIT8_Get_Bits(char *params) {
    char *params2 = NULL;
    uint8_t var = (uint8_t)strtoul(params, &params2, 0);
    uint8_t start_bit = (uint8_t)strtoul(params2, &params, 0);
    uint8_t end_bit = (uint8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Get_Bits(%d:%s,%d,%d) = %d:%s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT8_Get_Bits(var, start_bit, end_bit),
           BIT8_BitstringBrief(BIT8_Get_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Get_nBits
 * Description: *//**
* \brief Command-line extension of BIT8_Get_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Get_nBits
*
******************************************************************************/
static void CMD_BIT8_Get_nBits(char *params) {
    char *params2 = NULL;
    uint8_t var = (int8_t)strtoul(params, &params2, 0);
    uint8_t start_bit = (int8_t)strtoul(params2, &params, 0);
    uint8_t num_bits = (int8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Get_nBits(%d:%s,%d,%d) = %d:%s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT8_Get_nBits(var, start_bit, num_bits),
           BIT8_BitstringBrief(BIT8_Get_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Get_Bits_Signed
 * Description: *//**
* \brief Command-line extension of BIT8_Get_Bits_Signed.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Get_Bits_Signed
*
******************************************************************************/
static void CMD_BIT8_Get_Bits_Signed(char *params) {
    char *params2 = NULL;
    uint8_t var = (int8_t)strtoul(params, &params2, 0);
    uint8_t start_bit = (int8_t)strtoul(params2, &params, 0);
    uint8_t end_bit = (int8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Get_Bits_Signed(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT8_Bitstring(BIT8_Get_Bits_Signed(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_nBits_Signed
 * Description: *//**
* \brief Command-line extension of BIT8_nBits_Signed.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_nBits_Signed
*
******************************************************************************/
static void CMD_BIT8_Get_nBits_Signed(char *params) {
    char *params2 = NULL;
    uint8_t var = (int8_t)strtoul(params, &params2, 0);
    uint8_t start_bit = (int8_t)strtoul(params2, &params, 0);
    uint8_t num_bits = (int8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Get_nBits_Signed(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT8_Bitstring((uint8_t)BIT8_Get_nBits_Signed(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Set_Bit
 * Description: *//**
* \brief Command-line extension of BIT8_Set_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Set_Bit
*
******************************************************************************/
static void CMD_BIT8_Set_Bit(char *params) {
    char *params2 = NULL;
    uint8_t var = (uint8_t)strtoul(params, &params2, 0);
    uint8_t bit_num = (int8_t)strtoul(params2, &params, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Set_Bit(%d:%s,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           bit_num,
           BIT8_Bitstring(BIT8_Set_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Set_Bits
 * Description: *//**
* \brief Command-line extension of BIT8_Set_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Set_Bits
*
******************************************************************************/
static void CMD_BIT8_Set_Bits(char *params) {
    char *params2 = NULL;
    uint8_t var = (uint8_t)strtoul(params, &params2, 0);
    uint8_t start_bit = (uint8_t)strtoul(params2, &params, 0);
    uint8_t end_bit = (uint8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Set_Bits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT8_Bitstring(BIT8_Set_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Set_nBits
 * Description: *//**
* \brief Command-line extension of BIT8_Set_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Set_nBits
*
******************************************************************************/
static void CMD_BIT8_Set_nBits(char *params) {
    char *params2 = NULL;
    uint8_t var = (int8_t)strtoul(params, &params2, 0);
    uint8_t start_bit = (int8_t)strtoul(params2, &params, 0);
    uint8_t num_bits = (int8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Set_nBits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT8_Bitstring(BIT8_Set_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Clear_Bit
 * Description: *//**
* \brief Command-line extension of BIT8_Clear_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Clear_Bit
*
******************************************************************************/
static void CMD_BIT8_Clear_Bit(char *params) {
    char *params2 = NULL;
    uint8_t var = (uint8_t)strtoul(params, &params2, 0);
    uint8_t bit_num = (int8_t)strtoul(params2, &params, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Clear_Bit(%d:%s,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           bit_num,
           BIT8_Bitstring(BIT8_Clear_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Clear_Bits
 * Description: *//**
* \brief Command-line extension of BIT8_Clear_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Clear_Bits
*
******************************************************************************/
static void CMD_BIT8_Clear_Bits(char *params) {
    char *params2 = NULL;
    uint8_t var = (uint8_t)strtoul(params, &params2, 0);
    uint8_t start_bit = (uint8_t)strtoul(params2, &params, 0);
    uint8_t end_bit = (uint8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Clear_Bits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT8_Bitstring(BIT8_Clear_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Clear_nBits
 * Description: *//**
* \brief Command-line extension of BIT8_Clear_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Clear_nBits
*
******************************************************************************/
static void CMD_BIT8_Clear_nBits(char *params) {
    char *params2 = NULL;
    uint8_t var = (int8_t)strtoul(params, &params2, 0);
    uint8_t start_bit = (int8_t)strtoul(params2, &params, 0);
    uint8_t num_bits = (int8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Clear_nBits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT8_Bitstring(BIT8_Clear_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Toggle_Bit
 * Description: *//**
* \brief Command-line extension of BIT8_Toggle_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Toggle_Bit
*
******************************************************************************/
static void CMD_BIT8_Toggle_Bit(char *params) {
    char *params2 = NULL;
    uint8_t var = (uint8_t)strtoul(params, &params2, 0);
    uint8_t bit_num = (int8_t)strtoul(params2, &params, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Toggle_Bit(%d:%s,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           bit_num,
           BIT8_Bitstring(BIT8_Toggle_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Toggle_Bits
 * Description: *//**
* \brief Command-line extension of BIT8_Toggle_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Toggle_Bits
*
******************************************************************************/
static void CMD_BIT8_Toggle_Bits(char *params) {
    char *params2 = NULL;
    uint8_t var = (uint8_t)strtoul(params, &params2, 0);
    uint8_t start_bit = (uint8_t)strtoul(params2, &params, 0);
    uint8_t end_bit = (uint8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Toggle_Bits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT8_Bitstring(BIT8_Toggle_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Toggle_nBits
 * Description: *//**
* \brief Command-line extension of BIT8_Toggle_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Toggle_nBits
*
******************************************************************************/
static void CMD_BIT8_Toggle_nBits(char *params) {
    char *params2 = NULL;
    uint8_t var = (int8_t)strtoul(params, &params2, 0);
    uint8_t start_bit = (int8_t)strtoul(params2, &params, 0);
    uint8_t num_bits = (int8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Toggle_nBits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT8_Bitstring(BIT8_Toggle_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Insert_Bit
 * Description: *//**
* \brief Command-line extension of BIT8_Insert_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Insert_Bit
*
******************************************************************************/
static void CMD_BIT8_Insert_Bit(char *params) {
    char *params2 = NULL;
    uint8_t var = (uint8_t)strtoul(params, &params2, 0);
    uint8_t new_bit = strtoul(params2, &params, 0) & 1;
    uint8_t bit = (int8_t)strtoul(params, &params2, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Insert_Bit(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           new_bit,
           bit,
           BIT8_Bitstring(BIT8_Insert_Bit(var, new_bit, bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Insert_Bits
 * Description: *//**
* \brief Command-line extension of BIT8_Insert_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Insert_Bits
*
******************************************************************************/
static void CMD_BIT8_Insert_Bits(char *params) {
    char *params2 = NULL;
    uint8_t var = (uint8_t)strtoul(params, &params2, 0);
    uint8_t new_bits = (int8_t)strtoul(params2, &params, 0);
    uint8_t start_bit = (int8_t)strtoul(params, &params2, 0);
    uint8_t end_bit = (int8_t)strtoul(params2, &params, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Insert_Bits(%d:%s,%d,%d,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           new_bits,
           start_bit,
           end_bit,
           BIT8_Bitstring(BIT8_Insert_Bits(var, new_bits, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT8_Insert_nBits
 * Description: *//**
* \brief Command-line extension of BIT8_Insert_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit8_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT8_Insert_nBits
*
******************************************************************************/
static void CMD_BIT8_Insert_nBits(char *params) {
    char *params2 = NULL;
    uint8_t var = (int8_t)strtoul(params, &params2, 0);
    uint8_t new_bits = (int8_t)strtoul(params2, &params, 0);
    uint8_t start_bit = (int8_t)strtoul(params, &params2, 0);
    uint8_t num_bits = (int8_t)strtoul(params2, &params, 0);
    char bitstring[BIT8_STRING_SIZE];
    char bitstring2[BIT8_STRING_SIZE];
    printf("BIT8_Insert_nBits(%d:%s,%d,%d,%d) = %s\n\r",
           var,
           BIT8_BitstringBrief(var, bitstring),
           new_bits,
           start_bit,
           num_bits,
           BIT8_Bitstring(BIT8_Insert_nBits(var, new_bits, start_bit, num_bits), bitstring2));
}
