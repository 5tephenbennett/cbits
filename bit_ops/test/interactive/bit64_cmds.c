/**
 * @file bit64_cmds.h
 * @brief 64-bit interactive commands for bit_ops library.
 *
 * Copyright 2010 Stephen M Bennett. All Rights Reserved.
 */

/********************************* INCLUDES **********************************/
#include "bit64_ops.h"
#include "bit_interactive.h"
#include "bit_ops.h"
#include <ctype.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************* CONSTANTS *********************************/

/**************************** LOCAL ENUMERATIONS *****************************/

/********************* LOCAL STRUCTURE /UNION DATA TYPES *********************/

/******************************* LOCAL MACROS ********************************/

/************************ LOCAL FUNCTIONS PROTOTYPES *************************/
static void CMD_BIT64(char *params);
static void CMD_BIT64_RBitmask(char *params);
static void CMD_BIT64_LBitmask(char *params);
static void CMD_BIT64_Bitmask(char *params);
static void CMD_BIT64_nBitmask(char *params);
static void CMD_BIT64_isNeg(char *params);
static void CMD_BIT64_Get_Sign(char *params);
static void CMD_BIT64_Count_Bits_Set(char *params);
static void CMD_BIT64_Has_Zero_Byte(char *params);
static void CMD_BIT64_Get_First_Bit_Set(char *params);
static void CMD_BIT64_Sign_Extend(char *params);
static void CMD_BIT64_Reverse(char *params);
static void CMD_BIT64_Mask_Bit(char *params);
static void CMD_BIT64_Mask_Bits(char *params);
static void CMD_BIT64_Mask_nBits(char *params);
static void CMD_BIT64_Get_Bit(char *params);
static void CMD_BIT64_Get_Bits(char *params);
static void CMD_BIT64_Get_nBits(char *params);
static void CMD_BIT64_Get_Bits_Signed(char *params);
static void CMD_BIT64_Get_nBits_Signed(char *params);
static void CMD_BIT64_Set_Bit(char *params);
static void CMD_BIT64_Set_Bits(char *params);
static void CMD_BIT64_Set_nBits(char *params);
static void CMD_BIT64_Clear_Bit(char *params);
static void CMD_BIT64_Clear_Bits(char *params);
static void CMD_BIT64_Clear_nBits(char *params);
static void CMD_BIT64_Toggle_Bit(char *params);
static void CMD_BIT64_Toggle_Bits(char *params);
static void CMD_BIT64_Toggle_nBits(char *params);
static void CMD_BIT64_Insert_Bit(char *params);
static void CMD_BIT64_Insert_Bits(char *params);
static void CMD_BIT64_Insert_nBits(char *params);

/**************************** EXPORTED FUNCTIONS *****************************/

const COMMAND_LIST bit64_commands[] = {
    /* function,                     name,                       syntax */
    {CMD_BIT64, "BIT64", "uint64_t BIT64 ( uint8_t bit );"},
    {CMD_BIT64_RBitmask, "BIT64_RBitmask", "uint64_t BIT64_RBitmask ( uint8_t left_bit );"},
    {CMD_BIT64_LBitmask, "BIT64_LBitmask", "uint64_t BIT64_LBitmask ( uint8_t right_bit );"},
    {CMD_BIT64_Bitmask,
     "BIT64_Bitmask",
     "uint64_t BIT64_Bitmask ( uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT64_nBitmask,
     "BIT64_nBitmask",
     "uint64_t BIT64_nBitmask ( uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT64_isNeg, "BIT64_isNeg", "bool BIT64_isNeg ( int64_t var );"},
    {CMD_BIT64_Get_Sign, "BIT64_Get_Sign", "int64_t BIT64_Get_Sign ( int64_t var );"},
    {CMD_BIT64_Count_Bits_Set,
     "BIT64_Count_Bits_Set",
     "uint8_t BIT64_Count_Bits_Set (uint64_t var);"},
    {CMD_BIT64_Has_Zero_Byte, "BIT64_Has_Zero_Byte", "bool BIT64_Has_Zero_Byte (uint64_t var);"},
    {CMD_BIT64_Get_First_Bit_Set,
     "BIT64_Get_First_Bit_Set",
     "uint8_t BIT64_Get_First_Bit_Set(uint64_t var);"},
    {CMD_BIT64_Sign_Extend,
     "BIT64_Sign_Extend",
     "int64_t BIT64_Sign_Extend(uint64_t var, uint8_t bit_num);"},
    {CMD_BIT64_Reverse, "BIT64_Reverse", "uint64_t BIT64_Reverse(uint64_t var);"},
    {CMD_BIT64_Mask_Bit,
     "BIT64_Mask_Bit",
     "uint64_t BIT64_Mask_Bit ( uint64_t var, uint8_t bit );"},
    {CMD_BIT64_Mask_Bits,
     "BIT64_Mask_Bits",
     "uint64_t BIT64_Mask_Bits ( uint64_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT64_Mask_nBits,
     "BIT64_Mask_nBits",
     "uint64_t BIT64_Mask_nBits ( uint64_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT64_Get_Bit, "BIT64_Get_Bit", "bool BIT64_Get_Bit ( uint64_t var, uint8_t bit );"},
    {CMD_BIT64_Get_Bits,
     "BIT64_Get_Bits",
     "uint64_t BIT64_Get_Bits ( uint64_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT64_Get_nBits,
     "BIT64_Get_nBits",
     "uint64_t BIT64_Get_nBits ( uint64_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT64_Get_Bits_Signed,
     "BIT64_Get_Bits_Signed",
     "int64_t BIT64_Get_Bits_Signed ( uint64_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT64_Get_nBits_Signed,
     "BIT64_Get_nBits_Signed",
     "int64_t BIT64_Get_nBits_Signed ( uint64_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT64_Set_Bit, "BIT64_Set_Bit", "uint64_t BIT64_Set_Bit ( uint64_t var, uint8_t bit );"},
    {CMD_BIT64_Set_Bits,
     "BIT64_Set_Bits",
     "uint64_t BIT64_Set_Bits ( uint64_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT64_Set_nBits,
     "BIT64_Set_nBits",
     "uint64_t BIT64_Set_nBits ( uint64_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT64_Clear_Bit,
     "BIT64_Clear_Bit",
     "uint64_t BIT64_Clear_Bit ( uint64_t var, uint8_t bit );"},
    {CMD_BIT64_Clear_Bits,
     "BIT64_Clear_Bits",
     "uint64_t BIT64_Clear_Bits ( uint64_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT64_Clear_nBits,
     "BIT64_Clear_nBits",
     "uint64_t BIT64_Clear_nBits ( uint64_t var, uint8_t start_bit, uint8_t bit_num );"},
    {CMD_BIT64_Toggle_Bit,
     "BIT64_Toggle_Bit",
     "uint64_t BIT64_Toggle_Bit ( uint64_t var, uint8_t bit );"},
    {CMD_BIT64_Toggle_Bits,
     "BIT64_Toggle_Bits",
     "uint64_t BIT64_Toggle_Bits ( uint64_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT64_Toggle_nBits,
     "BIT64_Toggle_nBits",
     "uint64_t BIT64_Toggle_nBits( uint64_t var, uint8_t start_bit, uint8_t end_bit );"},
    {CMD_BIT64_Insert_Bit,
     "BIT64_Insert_Bit",
     "uint64_t BIT64_Insert_Bit ( uint64_t var, bool new_bit, uint8_t bit );"},
    {CMD_BIT64_Insert_Bits,
     "BIT64_Insert_Bits",
     "uint64_t BIT64_Insert_Bits ( uint64_t var, uint64_t new_bits, uint8_t start_bit, uint8_t "
     "end_bit );"},
    {CMD_BIT64_Insert_nBits,
     "BIT64_Insert_nBits",
     "uint64_t BIT64_Insert_nBits( uint64_t var, uint64_t new_bits, uint8_t start_bit, uint8_t "
     "bit_num );"},
    {NULL, "", ""}};

/******************************************************************************
 *
 * Name: BIT64_Initialise
 * Description: *//**
* \brief Initialise the BIT64 module.
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
void BIT64_Initialise(void) { BIT_Register_Commands(bit64_commands); }

/******************************************************************************
 *
 * Name: CMD_BIT64
 * Description: *//**
* \brief Command-line extension of BIT64.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64
*
******************************************************************************/
static void CMD_BIT64(char *params) {
    char *params2 = NULL;
    uint8_t bit = (uint8_t)strtoul(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    printf("BIT64(%d) = %s\n\r", bit, BIT64_Bitstring(BIT64(bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_RBitmask
 * Description: *//**
* \brief Command-line extension of BIT64_RBitmask.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_RBitmask
*
******************************************************************************/
static void CMD_BIT64_RBitmask(char *params) {
    char *params2 = NULL;
    uint8_t left_bit = (uint8_t)strtoul(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    printf("BIT64_RBitmask(%d) = %s\n\r",
           left_bit,
           BIT64_Bitstring(BIT64_RBitmask(left_bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_LBitmask
 * Description: *//**
* \brief Command-line extension of BIT64_LBitmask.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_LBitmask
*
******************************************************************************/
static void CMD_BIT64_LBitmask(char *params) {
    char *next_str = NULL;
    uint8_t right_bit = (uint8_t)strtoul(params, &next_str, 0);
    char bitstring[BIT64_STRING_SIZE];
    printf("BIT64_LBitmask(%d) = %s\n\r",
           right_bit,
           BIT64_Bitstring(BIT64_LBitmask(right_bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Bitmask
 * Description: *//**
* \brief Command-line extension of BIT64.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Bitmask
*
******************************************************************************/
static void CMD_BIT64_Bitmask(char *params) {
    char *params2 = NULL;
    uint8_t start_bit = (uint8_t)strtoul(params, &params2, 0);
    uint8_t end_bit = (uint8_t)strtoul(params2, &params, 0);
    char bitstring[BIT64_STRING_SIZE];
    printf("BIT64_Bitmask(%d,%d) = %s\n\r",
           start_bit,
           end_bit,
           BIT64_Bitstring(BIT64_Bitmask(start_bit, end_bit), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_nBitmask
 * Description: *//**
* \brief Command-line extension of BIT64_nBitmask.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_nBitmask
*
******************************************************************************/
static void CMD_BIT64_nBitmask(char *params) {
    char *params2 = NULL;
    uint8_t start_bit = (uint8_t)strtoul(params, &params2, 0);
    uint8_t bit_num = (uint8_t)strtoul(params2, &params, 0);
    char bitstring[BIT64_STRING_SIZE];
    printf("BIT64_nBitmask(%d,%d) = %s\n\r",
           start_bit,
           bit_num,
           BIT64_Bitstring(BIT64_nBitmask(start_bit, bit_num), bitstring));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_isNeg
 * Description: *//**
* \brief Command-line extension of BIT64_isNeg.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_isNeg
*
******************************************************************************/
static void CMD_BIT64_isNeg(char *params) {
    char *next_str = NULL;
    int64_t var = (int64_t)strtoll(params, &next_str, 0);
    printf("BIT64_isNeg(%" PRIu64 ") = %s\n\r", var, BIT64_isNeg(var) ? "TRUE" : "FALSE");
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Get_Sign
 * Description: *//**
* \brief Command-line extension of BIT64_Get_Sign.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Get_Sign
*
******************************************************************************/
static void CMD_BIT64_Get_Sign(char *params) {
    char *next_str = NULL;
    int64_t var = (int64_t)strtoll(params, &next_str, 0);
    printf("BIT64_Get_Sign(%" PRId64 ") = %" PRId64 "\n\r", var, BIT64_Get_Sign(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Count_Bits_Set
 * Description: *//**
* \brief Command-line extension of BIT64_Count_Bits_Set.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Count_Bits_Set
*
******************************************************************************/
static void CMD_BIT64_Count_Bits_Set(char *params) {
    char *next_str = NULL;
    uint64_t var = (uint64_t)strtoull(params, &next_str, 0);
    char bitstring[BIT64_STRING_SIZE];
    printf("BIT64_Count_Bits_Set(%" PRIu64 ":%s) = %u\n\r",
           var,
           BIT64_Bitstring(var, bitstring),
           BIT64_Count_Bits_Set(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Has_Zero_Byte
 * Description: *//**
* \brief Command-line extension of BIT64_Has_Zero_Byte.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Has_Zero_Byte
*
******************************************************************************/
static void CMD_BIT64_Has_Zero_Byte(char *params) {
    char *next_str = NULL;
    uint64_t var = (uint64_t)strtoull(params, &next_str, 0);
    printf("BIT64_Has_Zero_Byte(0x%08lx) = %s(%u)\n\r",
           var,
           BIT64_Has_Zero_Byte(var) ? "TRUE" : "FALSE",
           BIT64_Has_Zero_Byte(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Get_First_Bit_Set
 * Description: *//**
* \brief Command-line extension of BIT64_Get_First_Bit_Set.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Get_First_Bit_Set
*
******************************************************************************/
static void CMD_BIT64_Get_First_Bit_Set(char *params) {
    char *next_str = NULL;
    uint64_t var = (uint64_t)strtoull(params, &next_str, 0);
    char bitstring[BIT64_STRING_SIZE];
    printf("BIT64_Get_First_Bit_Set(%" PRIu64 ":%s) = %u\n\r",
           var,
           BIT64_Bitstring(var, bitstring),
           BIT64_Get_First_Bit_Set(var));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Sign_Extend
 * Description: *//**
* \brief Command-line extension of BIT64_Sign_Extend.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Sign_Extend
*
******************************************************************************/
static void CMD_BIT64_Sign_Extend(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t bitnum = (int64_t)strtoul(params2, &params, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Sign_Extend(%" PRIu64 ":%s,%" PRIu64 ") = %" PRId64 ":%s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           bitnum,
           BIT64_Sign_Extend(var, bitnum),
           BIT64_Bitstring((uint64_t)BIT64_Sign_Extend(var, bitnum), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Reverse
 * Description: *//**
* \brief Command-line extension of BIT64_Reverse.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Reverse
*
******************************************************************************/
static void CMD_BIT64_Reverse(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Reverse(%" PRIu64 ":%s) = %" PRIu64 ":%s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           BIT64_Reverse(var),
           BIT64_Bitstring(BIT64_Reverse(var), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Mask_Bit
 * Description: *//**
* \brief Command-line extension of BIT64_Mask_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Mask_Bit
*
******************************************************************************/
static void CMD_BIT64_Mask_Bit(char *params) {
    char *params2 = NULL;
    uint64_t var = (uint64_t)strtoull(params, &params2, 0);
    uint64_t bitnum = (uint64_t)strtoull(params2, &params, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Mask_Bit(%" PRIu64 ":%s,%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           bitnum,
           BIT64_Bitstring(BIT64_Mask_Bit(var, bitnum), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Mask_Bits
 * Description: *//**
* \brief Command-line extension of BIT64_Mask_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Mask_Bits
*
******************************************************************************/
static void CMD_BIT64_Mask_Bits(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t start_bit = (int64_t)strtoul(params2, &params, 0);
    uint64_t end_bit = (int64_t)strtoul(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Mask_Bits(%" PRIu64 ":%s,%" PRIu64 ",%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT64_Bitstring(BIT64_Mask_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Mask_nBits
 * Description: *//**
* \brief Command-line extension of BIT64_Mask_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Mask_nBits
*
******************************************************************************/
static void CMD_BIT64_Mask_nBits(char *params) {
    char *params2 = NULL;
    uint64_t var = (uint64_t)strtoull(params, &params2, 0);
    uint64_t start_bit = (uint64_t)strtoull(params2, &params, 0);
    uint64_t num_bits = (uint64_t)strtoull(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Mask_nBits(%" PRIu64 ":%s,%" PRIu64 ",%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT64_Bitstring(BIT64_Mask_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Get_Bit
 * Description: *//**
* \brief Command-line extension of BIT64_Get_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Get_Bit
*
******************************************************************************/
static void CMD_BIT64_Get_Bit(char *params) {
    char *params2 = NULL;
    uint64_t var = (uint64_t)strtoull(params, &params2, 0);
    uint64_t bit_num = (uint64_t)strtoull(params2, &params, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Get_Bit(%" PRIu64 ":%s,%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           bit_num,
           BIT64_Bitstring(BIT64_Get_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Get_Bits
 * Description: *//**
* \brief Command-line extension of BIT64_Get_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Get_Bits
*
******************************************************************************/
static void CMD_BIT64_Get_Bits(char *params) {
    char *params2 = NULL;
    uint64_t var = (uint64_t)strtoull(params, &params2, 0);
    uint64_t start_bit = (uint64_t)strtoull(params2, &params, 0);
    uint64_t end_bit = (uint64_t)strtoull(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Get_Bits(%" PRIu64 ":%s,%" PRIu64 ",%" PRIu64 ") = %" PRIu64 ":%s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT64_Get_Bits(var, start_bit, end_bit),
           BIT64_BitstringBrief(BIT64_Get_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Get_nBits
 * Description: *//**
* \brief Command-line extension of BIT64_Get_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Get_nBits
*
******************************************************************************/
static void CMD_BIT64_Get_nBits(char *params) {
    char *params2 = NULL;
    uint64_t var = (uint64_t)strtoull(params, &params2, 0);
    uint64_t start_bit = (uint64_t)strtoull(params2, &params, 0);
    uint64_t num_bits = (uint64_t)strtoull(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Get_nBits(%" PRIu64 ":%s,%" PRIu64 ",%" PRIu64 ") = %" PRIu64 ":%s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT64_Get_nBits(var, start_bit, num_bits),
           BIT64_BitstringBrief(BIT64_Get_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Get_Bits_Signed
 * Description: *//**
* \brief Command-line extension of BIT64_Get_Bits_Signed.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Get_Bits_Signed
*
******************************************************************************/
static void CMD_BIT64_Get_Bits_Signed(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t start_bit = (int64_t)strtoul(params2, &params, 0);
    uint64_t end_bit = (int64_t)strtoul(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Get_Bits_Signed(%" PRIu64 ":%s,%" PRIu64 ",%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT64_Bitstring(BIT64_Get_Bits_Signed(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_nBits_Signed
 * Description: *//**
* \brief Command-line extension of BIT64_nBits_Signed.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_nBits_Signed
*
******************************************************************************/
static void CMD_BIT64_Get_nBits_Signed(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t start_bit = (int64_t)strtoul(params2, &params, 0);
    uint64_t num_bits = (int64_t)strtoul(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Get_nBits_Signed(%" PRIu64 ":%s,%" PRIu64 ",%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT64_Bitstring((uint64_t)BIT64_Get_nBits_Signed(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Set_Bit
 * Description: *//**
* \brief Command-line extension of BIT64_Set_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Set_Bit
*
******************************************************************************/
static void CMD_BIT64_Set_Bit(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t bit_num = (int64_t)strtoul(params2, &params, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Set_Bit(%" PRIu64 ":%s,%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           bit_num,
           BIT64_Bitstring(BIT64_Set_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Set_Bits
 * Description: *//**
* \brief Command-line extension of BIT64_Set_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Set_Bits
*
******************************************************************************/
static void CMD_BIT64_Set_Bits(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t start_bit = (int64_t)strtoul(params2, &params, 0);
    uint64_t end_bit = (int64_t)strtoul(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Set_Bits(%" PRIu64 ":%s,%" PRIu64 ",%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT64_Bitstring(BIT64_Set_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Set_nBits
 * Description: *//**
* \brief Command-line extension of BIT64_Set_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Set_nBits
*
******************************************************************************/
static void CMD_BIT64_Set_nBits(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t start_bit = (int64_t)strtoul(params2, &params, 0);
    uint64_t num_bits = (int64_t)strtoul(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Set_nBits(%" PRIu64 ":%s,%" PRIu64 ",%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT64_Bitstring(BIT64_Set_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Clear_Bit
 * Description: *//**
* \brief Command-line extension of BIT64_Clear_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Clear_Bit
*
******************************************************************************/
static void CMD_BIT64_Clear_Bit(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t bit_num = (int64_t)strtoul(params2, &params, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Clear_Bit(%" PRIu64 ":%s,%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           bit_num,
           BIT64_Bitstring(BIT64_Clear_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Clear_Bits
 * Description: *//**
* \brief Command-line extension of BIT64_Clear_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Clear_Bits
*
******************************************************************************/
static void CMD_BIT64_Clear_Bits(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t start_bit = (int64_t)strtoul(params2, &params, 0);
    uint64_t end_bit = (int64_t)strtoul(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Clear_Bits(%" PRIu64 ":%s,%" PRIu64 ",%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT64_Bitstring(BIT64_Clear_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Clear_nBits
 * Description: *//**
* \brief Command-line extension of BIT64_Clear_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Clear_nBits
*
******************************************************************************/
static void CMD_BIT64_Clear_nBits(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t start_bit = (int64_t)strtoul(params2, &params, 0);
    uint64_t num_bits = (int64_t)strtoul(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Clear_nBits(%" PRIu64 ":%s,%" PRIu64 ",%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT64_Bitstring(BIT64_Clear_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Toggle_Bit
 * Description: *//**
* \brief Command-line extension of BIT64_Toggle_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Toggle_Bit
*
******************************************************************************/
static void CMD_BIT64_Toggle_Bit(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t bit_num = (int64_t)strtoul(params2, &params, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Toggle_Bit(%" PRIu64 ":%s,%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           bit_num,
           BIT64_Bitstring(BIT64_Toggle_Bit(var, bit_num), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Toggle_Bits
 * Description: *//**
* \brief Command-line extension of BIT64_Toggle_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Toggle_Bits
*
******************************************************************************/
static void CMD_BIT64_Toggle_Bits(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t start_bit = (int64_t)strtoul(params2, &params, 0);
    uint64_t end_bit = (int64_t)strtoul(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Toggle_Bits(%" PRIu64 ":%s,%" PRIu64 ",%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           start_bit,
           end_bit,
           BIT64_Bitstring(BIT64_Toggle_Bits(var, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Toggle_nBits
 * Description: *//**
* \brief Command-line extension of BIT64_Toggle_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Toggle_nBits
*
******************************************************************************/
static void CMD_BIT64_Toggle_nBits(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t start_bit = (int64_t)strtoul(params2, &params, 0);
    uint64_t num_bits = (int64_t)strtoul(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Toggle_nBits(%" PRIu64 ":%s,%" PRIu64 ",%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           start_bit,
           num_bits,
           BIT64_Bitstring(BIT64_Toggle_nBits(var, start_bit, num_bits), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Insert_Bit
 * Description: *//**
* \brief Command-line extension of BIT64_Insert_Bit.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Insert_Bit
*
******************************************************************************/
static void CMD_BIT64_Insert_Bit(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t new_bit = strtoul(params2, &params, 0) & 1;
    uint64_t bit = (int64_t)strtoul(params, &params2, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Insert_Bit(%" PRIu64 ":%s,%" PRIu64 ",%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           new_bit,
           bit,
           BIT64_Bitstring(BIT64_Insert_Bit(var, new_bit, bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Insert_Bits
 * Description: *//**
* \brief Command-line extension of BIT64_Insert_Bits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Insert_Bits
*
******************************************************************************/
static void CMD_BIT64_Insert_Bits(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t new_bits = (int64_t)strtoul(params2, &params, 0);
    uint64_t start_bit = (int64_t)strtoul(params, &params2, 0);
    uint64_t end_bit = (int64_t)strtoul(params2, &params, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Insert_Bits(%" PRIu64 ":%s,%" PRIu64 ",%" PRIu64 ",%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           new_bits,
           start_bit,
           end_bit,
           BIT64_Bitstring(BIT64_Insert_Bits(var, new_bits, start_bit, end_bit), bitstring2));
}

/******************************************************************************
 *
 * Name: CMD_BIT64_Insert_nBits
 * Description: *//**
* \brief Command-line extension of BIT64_Insert_nBits.
*
* \author Stephen M Bennett.
*
* \param[in]   see bit64_commands
* \param[out]
*
* \return void
*
* \note
*
* \see Partner function BIT64_Insert_nBits
*
******************************************************************************/
static void CMD_BIT64_Insert_nBits(char *params) {
    char *params2 = NULL;
    uint64_t var = (int64_t)strtoul(params, &params2, 0);
    uint64_t new_bits = (int64_t)strtoul(params2, &params, 0);
    uint64_t start_bit = (int64_t)strtoul(params, &params2, 0);
    uint64_t num_bits = (int64_t)strtoul(params2, &params, 0);
    char bitstring[BIT64_STRING_SIZE];
    char bitstring2[BIT64_STRING_SIZE];
    printf("BIT64_Insert_nBits(%" PRIu64 ":%s,%" PRIu64 ",%" PRIu64 ",%" PRIu64 ") = %s\n\r",
           var,
           BIT64_BitstringBrief(var, bitstring),
           new_bits,
           start_bit,
           num_bits,
           BIT64_Bitstring(BIT64_Insert_nBits(var, new_bits, start_bit, num_bits), bitstring2));
}
