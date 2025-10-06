/**
 * @file bit16_cmds.h
 * @brief 16-bit interactive commands for bit_ops library.
 *
 * Copyright 2010 Stephen M Bennett. All Rights Reserved.
 */

/********************************* INCLUDES **********************************/
#include "bit16_ops.h"
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
static void CMD_BIT16(char *params);
static void CMD_BIT16_RBitmask(char *params);
static void CMD_BIT16_LBitmask(char *params);
static void CMD_BIT16_Bitmask(char *params);
static void CMD_BIT16_nBitmask(char *params);
static void CMD_BIT16_isNeg(char *params);
static void CMD_BIT16_Get_Sign(char *params);
static void CMD_BIT16_Count_Bits_Set(char *params);
static void CMD_BIT16_Has_Zero_Byte(char *params);
static void CMD_BIT16_Get_First_Bit_Set(char *params);
static void CMD_BIT16_Sign_Extend(char *params);
static void CMD_BIT16_Reverse(char *params);
static void CMD_BIT16_Mask_Bit(char *params);
static void CMD_BIT16_Mask_Bits(char *params);
static void CMD_BIT16_Mask_nBits(char *params);
static void CMD_BIT16_Get_Bit(char *params);
static void CMD_BIT16_Get_Bits(char *params);
static void CMD_BIT16_Get_nBits(char *params);
static void CMD_BIT16_Get_Bits_Signed(char *params);
static void CMD_BIT16_Get_nBits_Signed(char *params);
static void CMD_BIT16_Set_Bit(char *params);
static void CMD_BIT16_Set_Bits(char *params);
static void CMD_BIT16_Set_nBits(char *params);
static void CMD_BIT16_Clear_Bit(char *params);
static void CMD_BIT16_Clear_Bits(char *params);
static void CMD_BIT16_Clear_nBits(char *params);
static void CMD_BIT16_Toggle_Bit(char *params);
static void CMD_BIT16_Toggle_Bits(char *params);
static void CMD_BIT16_Toggle_nBits(char *params);
static void CMD_BIT16_Insert_Bit(char *params);
static void CMD_BIT16_Insert_Bits(char *params);
static void CMD_BIT16_Insert_nBits(char *params);
static void CMD_BIT16_SWAP_ENDIAN(char *params);

/**************************** EXPORTED FUNCTIONS *****************************/

const COMMAND_LIST bit16_commands[] = {
    /* function,                     name,                       syntax */
    {CMD_BIT16, "BIT16", "uint16_t BIT16 ( uint8_t bit );"},
    {CMD_BIT16_RBitmask, "BIT16_RBitmask", "uint16_t BIT16_RBitmask ( uint8_t left_bit );"},
    {CMD_BIT16_LBitmask, "BIT16_LBitmask", "uint16_t BIT16_LBitmask ( uint8_t right_bit );"},
    {CMD_BIT16_Bitmask,
     "BIT16_Bitmask",
     "uint16_t BIT16_Bitmask ( uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT16_nBitmask,
     "BIT16_nBitmask",
     "uint16_t BIT16_nBitmask ( uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT16_isNeg, "BIT16_isNeg", "bool BIT16_isNeg ( int16_t var );"},
    {CMD_BIT16_Get_Sign, "BIT16_Get_Sign", "int16_t BIT16_Get_Sign ( int16_t var );"},
    {CMD_BIT16_Count_Bits_Set,
     "BIT16_Count_Bits_Set",
     "uint8_t BIT16_Count_Bits_Set (uint16_t var);"},
    {CMD_BIT16_Has_Zero_Byte, "BIT16_Has_Zero_Byte", "bool BIT16_Has_Zero_Byte (uint16_t var);"},
    {CMD_BIT16_Get_First_Bit_Set,
     "BIT16_Get_First_Bit_Set",
     "uint8_t BIT16_Get_First_Bit_Set(uint16_t var);"},
    {CMD_BIT16_Sign_Extend,
     "BIT16_Sign_Extend",
     "int16_t BIT16_Sign_Extend(uint16_t var, uint8_t bit_num);"},
    {CMD_BIT16_Reverse, "BIT16_Reverse", "uint16_t BIT16_Reverse(uint16_t var);"},
    {CMD_BIT16_Mask_Bit,
     "BIT16_Mask_Bit",
     "uint16_t BIT16_Mask_Bit ( uint16_t var, uint8_t bit );"},
    {CMD_BIT16_Mask_Bits,
     "BIT16_Mask_Bits",
     "uint16_t BIT16_Mask_Bits ( uint16_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT16_Mask_nBits,
     "BIT16_Mask_nBits",
     "uint16_t BIT16_Mask_nBits ( uint16_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT16_Get_Bit, "BIT16_Get_Bit", "bool BIT16_Get_Bit ( uint16_t var, uint8_t bit );"},
    {CMD_BIT16_Get_Bits,
     "BIT16_Get_Bits",
     "uint16_t BIT16_Get_Bits ( uint16_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT16_Get_nBits,
     "BIT16_Get_nBits",
     "uint16_t BIT16_Get_nBits ( uint16_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT16_Get_Bits_Signed,
     "BIT16_Get_Bits_Signed",
     "int16_t BIT16_Get_Bits_Signed ( uint16_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT16_Get_nBits_Signed,
     "BIT16_Get_nBits_Signed",
     "int16_t BIT16_Get_nBits_Signed ( uint16_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT16_Set_Bit, "BIT16_Set_Bit", "uint16_t BIT16_Set_Bit ( uint16_t var, uint8_t bit );"},
    {CMD_BIT16_Set_Bits,
     "BIT16_Set_Bits",
     "uint16_t BIT16_Set_Bits ( uint16_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT16_Set_nBits,
     "BIT16_Set_nBits",
     "uint16_t BIT16_Set_nBits ( uint16_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT16_Clear_Bit,
     "BIT16_Clear_Bit",
     "uint16_t BIT16_Clear_Bit ( uint16_t var, uint8_t bit );"},
    {CMD_BIT16_Clear_Bits,
     "BIT16_Clear_Bits",
     "uint16_t BIT16_Clear_Bits ( uint16_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT16_Clear_nBits,
     "BIT16_Clear_nBits",
     "uint16_t BIT16_Clear_nBits ( uint16_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT16_Toggle_Bit,
     "BIT16_Toggle_Bit",
     "uint16_t BIT16_Toggle_Bit ( uint16_t var, uint8_t bit );"},
    {CMD_BIT16_Toggle_Bits,
     "BIT16_Toggle_Bits",
     "uint16_t BIT16_Toggle_Bits ( uint16_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT16_Toggle_nBits,
     "BIT16_Toggle_nBits",
     "uint16_t BIT16_Toggle_nBits( uint16_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT16_Insert_Bit,
     "BIT16_Insert_Bit",
     "uint16_t BIT16_Insert_Bit ( uint16_t var, bool new_bit, uint8_t bit );"},
    {CMD_BIT16_Insert_Bits,
     "BIT16_Insert_Bits",
     "uint16_t BIT16_Insert_Bits ( uint16_t var, uint16_t new_bits, uint8_t start_bit, uint8_t "
     "end_bit );"},
    {CMD_BIT16_Insert_nBits,
     "BIT16_Insert_nBits",
     "uint16_t BIT16_Insert_nBits( uint16_t var, uint16_t new_bits, uint8_t start_bit, uint8_t "
     "bit_num );"},
    {CMD_BIT16_SWAP_ENDIAN, "BIT16_SWAP_ENDIAN", "CMD_BIT16_SWAP_ENDIAN(var)"},
    {NULL, "", ""}};

/******************************************************************************
 *
 * Name: BIT16_Initialise
 * Description: *//**
* \brief Initialise the BIT16 module.
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
void BIT16_Initialise(void) { BIT_Register_Commands(bit16_commands); }

/******************************************************************************
 *
 * Name: CMD_BIT16
 * Description: *//**
* \brief Command-line extension of BIT16.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16
*
******************************************************************************/
static void CMD_BIT16(char *params) {
    char *params2 = NULL;
    uint8_t bit = (uint8_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    printf("BIT16(%d) = %s\n\r", bit, BIT16_Bitstring(BIT16(bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_RBitmask
 * Description: *//**
* \brief Command-line extension of BIT16_RBitmask.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_RBitmask
*
******************************************************************************/
static void CMD_BIT16_RBitmask(char *params) {
    char *params2 = NULL;
    uint8_t left_bit = (uint8_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    printf("BIT16_RBitmask(%d) = %s\n\r",
           left_bit,
           BIT16_Bitstring(BIT16_RBitmask(left_bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_LBitmask
 * Description: *//**
* \brief Command-line extension of BIT16_LBitmask.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_LBitmask
*
******************************************************************************/
static void CMD_BIT16_LBitmask(char *params) {
    char *params2 = NULL;
    uint8_t right_bit = (uint8_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    printf("BIT16_LBitmask(%d) = %s\n\r",
           right_bit,
           BIT16_Bitstring(BIT16_LBitmask(right_bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Bitmask
 * Description: *//**
* \brief Command-line extension of BIT16.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Bitmask
*
******************************************************************************/
static void CMD_BIT16_Bitmask(char *params) {
    char *params2 = NULL;
    uint8_t start_bit = (uint8_t)strtoul(params, &params2, 0);
    uint8_t end_bit = (uint8_t)strtoul(params2, &params, 0);
    char bitstring[BIT16_STRING_SIZE];
    printf("BIT16_Bitmask(%d,%d) = %s\n\r",
           start_bit,
           end_bit,
           BIT16_Bitstring(BIT16_Bitmask(start_bit, end_bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_nBitmask
 * Description: *//**
* \brief Command-line extension of BIT16_nBitmask.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_nBitmask
*
******************************************************************************/
static void CMD_BIT16_nBitmask(char *params) {
    char *params2 = NULL;
    uint8_t start_bit = (uint8_t)strtoul(params, &params2, 0);
    uint8_t bit_num = (uint8_t)strtoul(params2, &params, 0);
    char bitstring[BIT16_STRING_SIZE];
    printf("BIT16_nBitmask(%d,%d) = %s\n\r",
           start_bit,
           bit_num,
           BIT16_Bitstring(BIT16_nBitmask(start_bit, bit_num), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_isNeg
 * Description: *//**
* \brief Command-line extension of BIT16_isNeg.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_isNeg
*
******************************************************************************/
static void CMD_BIT16_isNeg(char *params) {
    char *params2 = NULL;
    int16_t var = (int16_t)strtol(params, &params2, 0);
    printf("BIT16_isNeg(%d) = %s\n\r", var, BIT16_isNeg(var) ? "TRUE" : "FALSE");
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Get_Sign
 * Description: *//**
* \brief Command-line extension of BIT16_Get_Sign.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Get_Sign
*
******************************************************************************/
static void CMD_BIT16_Get_Sign(char *params) {
    char *params2 = NULL;
    int16_t var = (int16_t)strtol(params, &params2, 0);
    printf("BIT16_Get_Sign(%d) = %d\n\r", var, BIT16_Get_Sign(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Count_Bits_Set
 * Description: *//**
* \brief Command-line extension of BIT16_Count_Bits_Set.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Count_Bits_Set
*
******************************************************************************/
static void CMD_BIT16_Count_Bits_Set(char *params) {
    char *params2 = NULL;
    uint16_t var = (uint16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    printf("BIT16_Count_Bits_Set(%d:%s) = %d\n\r",
           var,
           BIT16_Bitstring(var, bitstring),
           BIT16_Count_Bits_Set(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Has_Zero_Byte
 * Description: *//**
* \brief Command-line extension of BIT16_Has_Zero_Byte.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Has_Zero_Byte
*
******************************************************************************/
static void CMD_BIT16_Has_Zero_Byte(char *params) {
    char *params2 = NULL;
    uint16_t var = (uint16_t)strtoul(params, &params2, 0);
    printf("BIT16_Has_Zero_Byte(0x%08x) = %s(%d)\n\r",
           var,
           BIT16_Has_Zero_Byte(var) ? "TRUE" : "FALSE",
           BIT16_Has_Zero_Byte(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Get_First_Bit_Set
 * Description: *//**
* \brief Command-line extension of BIT16_Get_First_Bit_Set.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Get_First_Bit_Set
*
******************************************************************************/
static void CMD_BIT16_Get_First_Bit_Set(char *params) {
    char *params2 = NULL;
    uint16_t var = (uint16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    printf("BIT16_Get_First_Bit_Set(%d:%s) = %d\n\r",
           var,
           BIT16_Bitstring(var, bitstring),
           BIT16_Get_First_Bit_Set(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Sign_Extend
 * Description: *//**
* \brief Command-line extension of BIT16_Sign_Extend.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Sign_Extend
*
******************************************************************************/
static void CMD_BIT16_Sign_Extend(char *params) {
    char *params2 = NULL;
    uint16_t var = (uint16_t)strtoul(params, &params2, 0);
    uint16_t bitnum = (uint16_t)strtoul(params2, &params, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Sign_Extend(%d:%s,%d) = %d:%s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           bitnum,
           BIT16_Sign_Extend(var, bitnum),
           BIT16_Bitstring(BIT16_Sign_Extend(var, bitnum), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Reverse
 * Description: *//**
* \brief Command-line extension of BIT16_Reverse.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Reverse
*
******************************************************************************/
static void CMD_BIT16_Reverse(char *params) {
    char *params2 = NULL;
    uint16_t var = (uint16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Reverse(%d:%s) = %d:%s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           BIT16_Reverse(var),
           BIT16_Bitstring(BIT16_Reverse(var), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Mask_Bit
 * Description: *//**
* \brief Command-line extension of BIT16_Mask_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Mask_Bit
*
******************************************************************************/
static void CMD_BIT16_Mask_Bit(char *params) {
    char *params2 = NULL;
    uint16_t var = (uint16_t)strtoul(params, &params2, 0);
    uint16_t bit_num = (uint16_t)strtoul(params2, &params, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Mask_Bit(%d:%s,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           bit_num,
           BIT16_Bitstring(BIT16_Mask_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Mask_Bits
 * Description: *//**
* \brief Command-line extension of BIT16_Mask_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Mask_Bits
*
******************************************************************************/
static void CMD_BIT16_Mask_Bits(char *params) {
    char *params2 = NULL;
    uint16_t var = (uint16_t)strtoul(params, &params2, 0);
    uint16_t start_bit = (uint16_t)strtoul(params2, &params, 0);
    uint16_t end_bit = (uint16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Mask_Bits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT16_Bitstring(BIT16_Mask_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Mask_nBits
 * Description: *//**
* \brief Command-line extension of BIT16_Mask_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Mask_nBits
*
******************************************************************************/
static void CMD_BIT16_Mask_nBits(char *params) {
    char *params2 = NULL;
    uint16_t var = (uint16_t)strtoul(params, &params2, 0);
    uint16_t start_bit = (uint16_t)strtoul(params2, &params, 0);
    uint16_t num_bits = (uint16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Mask_nBits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT16_Bitstring(BIT16_Mask_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Get_Bit
 * Description: *//**
* \brief Command-line extension of BIT16_Get_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Get_Bit
*
******************************************************************************/
static void CMD_BIT16_Get_Bit(char *params) {
    char *params2 = NULL;
    uint16_t var = (uint16_t)strtoul(params, &params2, 0);
    uint16_t bit_num = (uint16_t)strtoul(params2, &params, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Get_Bit(%d:%s,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           bit_num,
           BIT16_Bitstring(BIT16_Get_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Get_Bits
 * Description: *//**
* \brief Command-line extension of BIT16_Get_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Get_Bits
*
******************************************************************************/
static void CMD_BIT16_Get_Bits(char *params) {
    char *params2 = NULL;
    uint16_t var = (uint16_t)strtoul(params, &params2, 0);
    uint16_t start_bit = (uint16_t)strtoul(params2, &params, 0);
    uint16_t end_bit = (uint16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Get_Bits(%d:%s,%d,%d) = %d:%s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT16_Get_Bits(var, start_bit, end_bit),
           BIT16_BitstringBrief(BIT16_Get_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Get_nBits
 * Description: *//**
* \brief Command-line extension of BIT16_Get_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Get_nBits
*
******************************************************************************/
static void CMD_BIT16_Get_nBits(char *params) {
    char *params2 = NULL;
    uint16_t var = (uint16_t)strtoul(params, &params2, 0);
    uint16_t start_bit = (uint16_t)strtoul(params2, &params, 0);
    uint16_t num_bits = (uint16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Get_nBits(%d:%s,%d,%d) = %d:%s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT16_Get_nBits(var, start_bit, num_bits),
           BIT16_BitstringBrief(BIT16_Get_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Get_Bits_Signed
 * Description: *//**
* \brief Command-line extension of BIT16_Get_Bits_Signed.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Get_Bits_Signed
*
******************************************************************************/
static void CMD_BIT16_Get_Bits_Signed(char *params) {
    char *params2 = NULL;
    uint16_t var = (int16_t)strtoul(params, &params2, 0);
    uint16_t start_bit = (int16_t)strtoul(params2, &params, 0);
    uint16_t end_bit = (int16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Get_Bits_Signed(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT16_Bitstring(BIT16_Get_Bits_Signed(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_nBits_Signed
 * Description: *//**
* \brief Command-line extension of BIT16_nBits_Signed.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_nBits_Signed
*
******************************************************************************/
static void CMD_BIT16_Get_nBits_Signed(char *params) {
    char *params2 = NULL;
    uint16_t var = (int16_t)strtoul(params, &params2, 0);
    uint16_t start_bit = (int16_t)strtoul(params2, &params, 0);
    uint16_t num_bits = (int16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Get_nBits_Signed(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT16_Bitstring((uint16_t)BIT16_Get_nBits_Signed(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Set_Bit
 * Description: *//**
* \brief Command-line extension of BIT16_Set_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Set_Bit
*
******************************************************************************/
static void CMD_BIT16_Set_Bit(char *params) {
    char *params2 = NULL;
    uint16_t var = (int16_t)strtoul(params, &params2, 0);
    uint16_t bit_num = (int16_t)strtoul(params2, &params, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Set_Bit(%d:%s,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           bit_num,
           BIT16_Bitstring(BIT16_Set_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Set_Bits
 * Description: *//**
* \brief Command-line extension of BIT16_Set_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Set_Bits
*
******************************************************************************/
static void CMD_BIT16_Set_Bits(char *params) {
    char *params2 = NULL;
    uint16_t var = (int16_t)strtoul(params, &params2, 0);
    uint16_t start_bit = (int16_t)strtoul(params2, &params, 0);
    uint16_t end_bit = (int16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Set_Bits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT16_Bitstring(BIT16_Set_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Set_nBits
 * Description: *//**
* \brief Command-line extension of BIT16_Set_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Set_nBits
*
******************************************************************************/
static void CMD_BIT16_Set_nBits(char *params) {
    char *params2 = NULL;
    uint16_t var = (int16_t)strtoul(params, &params2, 0);
    uint16_t start_bit = (int16_t)strtoul(params2, &params, 0);
    uint16_t num_bits = (int16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Set_nBits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT16_Bitstring(BIT16_Set_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Clear_Bit
 * Description: *//**
* \brief Command-line extension of BIT16_Clear_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Clear_Bit
*
******************************************************************************/
static void CMD_BIT16_Clear_Bit(char *params) {
    char *params2 = NULL;
    uint16_t var = (int16_t)strtoul(params, &params2, 0);
    uint16_t bit_num = (int16_t)strtoul(params2, &params, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Clear_Bit(%d:%s,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           bit_num,
           BIT16_Bitstring(BIT16_Clear_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Clear_Bits
 * Description: *//**
* \brief Command-line extension of BIT16_Clear_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Clear_Bits
*
******************************************************************************/
static void CMD_BIT16_Clear_Bits(char *params) {
    char *params2 = NULL;
    uint16_t var = (int16_t)strtoul(params, &params2, 0);
    uint16_t start_bit = (int16_t)strtoul(params2, &params, 0);
    uint16_t end_bit = (int16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Clear_Bits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT16_Bitstring(BIT16_Clear_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Clear_nBits
 * Description: *//**
* \brief Command-line extension of BIT16_Clear_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Clear_nBits
*
******************************************************************************/
static void CMD_BIT16_Clear_nBits(char *params) {
    char *params2 = NULL;
    uint16_t var = (int16_t)strtoul(params, &params2, 0);
    uint16_t start_bit = (int16_t)strtoul(params2, &params, 0);
    uint16_t num_bits = (int16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Clear_nBits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT16_Bitstring(BIT16_Clear_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Toggle_Bit
 * Description: *//**
* \brief Command-line extension of BIT16_Toggle_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Toggle_Bit
*
******************************************************************************/
static void CMD_BIT16_Toggle_Bit(char *params) {
    char *params2 = NULL;
    uint16_t var = (int16_t)strtoul(params, &params2, 0);
    uint16_t bit_num = (int16_t)strtoul(params2, &params, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Toggle_Bit(%d:%s,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           bit_num,
           BIT16_Bitstring(BIT16_Toggle_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Toggle_Bits
 * Description: *//**
* \brief Command-line extension of BIT16_Toggle_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Toggle_Bits
*
******************************************************************************/
static void CMD_BIT16_Toggle_Bits(char *params) {
    char *params2 = NULL;
    uint16_t var = (int16_t)strtoul(params, &params2, 0);
    uint16_t start_bit = (int16_t)strtoul(params2, &params, 0);
    uint16_t end_bit = (int16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Toggle_Bits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT16_Bitstring(BIT16_Toggle_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Toggle_nBits
 * Description: *//**
* \brief Command-line extension of BIT16_Toggle_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Toggle_nBits
*
******************************************************************************/
static void CMD_BIT16_Toggle_nBits(char *params) {
    char *params2 = NULL;
    uint16_t var = (int16_t)strtoul(params, &params2, 0);
    uint16_t start_bit = (int16_t)strtoul(params2, &params, 0);
    uint16_t num_bits = (int16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Toggle_nBits(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT16_Bitstring(BIT16_Toggle_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Insert_Bit
 * Description: *//**
* \brief Command-line extension of BIT16_Insert_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Insert_Bit
*
******************************************************************************/
static void CMD_BIT16_Insert_Bit(char *params) {
    char *params2 = NULL;
    uint16_t var = (int16_t)strtoul(params, &params2, 0);
    uint16_t new_bit = strtoul(params2, &params, 0) & 1;
    uint16_t bit = (int16_t)strtoul(params, &params2, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Insert_Bit(%d:%s,%d,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           new_bit,
           bit,
           BIT16_Bitstring(BIT16_Insert_Bit(var, new_bit, bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Insert_Bits
 * Description: *//**
* \brief Command-line extension of BIT16_Insert_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Insert_Bits
*
******************************************************************************/
static void CMD_BIT16_Insert_Bits(char *params) {
    char *params2 = NULL;
    uint16_t var = (int16_t)strtoul(params, &params2, 0);
    uint16_t new_bits = (int16_t)strtoul(params2, &params, 0);
    uint16_t start_bit = (int16_t)strtoul(params, &params2, 0);
    uint16_t end_bit = (int16_t)strtoul(params2, &params, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Insert_Bits(%d:%s,%d,%d,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           new_bits,
           start_bit,
           end_bit,
           BIT16_Bitstring(BIT16_Insert_Bits(var, new_bits, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT16_Insert_nBits
 * Description: *//**
* \brief Command-line extension of BIT16_Insert_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit16_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT16_Insert_nBits
*
******************************************************************************/
static void CMD_BIT16_Insert_nBits(char *params) {
    char *params2 = NULL;
    uint16_t var = (int16_t)strtoul(params, &params2, 0);
    uint16_t new_bits = (int16_t)strtoul(params2, &params, 0);
    uint16_t start_bit = (int16_t)strtoul(params, &params2, 0);
    uint16_t num_bits = (int16_t)strtoul(params2, &params, 0);
    char bitstring[BIT16_STRING_SIZE];
    char bitstring2[BIT16_STRING_SIZE];
    printf("BIT16_Insert_nBits(%d:%s,%d,%d,%d) = %s\n\r",
           var,
           BIT16_BitstringBrief(var, bitstring),
           new_bits,
           start_bit,
           num_bits,
           BIT16_Bitstring(BIT16_Insert_nBits(var, new_bits, start_bit, num_bits), bitstring2));
}

static void CMD_BIT16_SWAP_ENDIAN(char *params) {
    char *params2 = NULL;
    uint32_t var = (uint32_t)strtoul(params, &params2, 0);
    uint32_t var_swap = var;

    BIT16_SWAP_ENDIAN(var_swap);

    printf("BIT16_SWAP_ENDIAN(0x%04x) = 0x%04x\n\r", var, var_swap);
}
