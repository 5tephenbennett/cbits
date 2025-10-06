/**
 * @file bit_ops.c
 * @brief General bit manipulation operations implementation
 *
 * Copyright 2009-2010 Stephen M Bennett. All Rights Reserved.
 */

/********************************* INCLUDES **********************************/
#include "bit_ops.h"
#include "bit_ops_api.h"

/********************************* CONSTANTS *********************************/
#define DEFINE_TO_STRING(x) #x
#define TO_STRING(x) DEFINE_TO_STRING(x)
#define LIB_VERSION 0.2
#define VERSION_STRING "BIT_OPS Library v" TO_STRING(LIB_VERSION)

static char const cssc_what_compatible_version_string[] = "@(#)" VERSION_STRING;

#ifdef COUNT_BITS_LOOKUP
/* Lookup table for counting bits set in a byte (0-255) */
const uint8_t bits_set_table256[256] = {
    /* 0x00 */ 0, 1, 1, 2, 1, 2, 2, 3,
    /* 0x08 */ 1, 2, 2, 3, 2, 3, 3, 4,
    /* 0x10 */ 1, 2, 2, 3, 2, 3, 3, 4,
    /* 0x18 */ 2, 3, 3, 4, 3, 4, 4, 5,
    /* 0x20 */ 1, 2, 2, 3, 2, 3, 3, 4,
    /* 0x28 */ 2, 3, 3, 4, 3, 4, 4, 5,
    /* 0x30 */ 2, 3, 3, 4, 3, 4, 4, 5,
    /* 0x38 */ 3, 4, 4, 5, 4, 5, 5, 6,
    /* 0x40 */ 1, 2, 2, 3, 2, 3, 3, 4,
    /* 0x48 */ 2, 3, 3, 4, 3, 4, 4, 5,
    /* 0x50 */ 2, 3, 3, 4, 3, 4, 4, 5,
    /* 0x58 */ 3, 4, 4, 5, 4, 5, 5, 6,
    /* 0x60 */ 2, 3, 3, 4, 3, 4, 4, 5,
    /* 0x68 */ 3, 4, 4, 5, 4, 5, 5, 6,
    /* 0x70 */ 3, 4, 4, 5, 4, 5, 5, 6,
    /* 0x78 */ 4, 5, 5, 6, 5, 6, 6, 7,
    /* 0x80 */ 1, 2, 2, 3, 2, 3, 3, 4,
    /* 0x88 */ 2, 3, 3, 4, 3, 4, 4, 5,
    /* 0x90 */ 2, 3, 3, 4, 3, 4, 4, 5,
    /* 0x98 */ 3, 4, 4, 5, 4, 5, 5, 6,
    /* 0xa0 */ 2, 3, 3, 4, 3, 4, 4, 5,
    /* 0xa8 */ 3, 4, 4, 5, 4, 5, 5, 6,
    /* 0xb0 */ 3, 4, 4, 5, 4, 5, 5, 6,
    /* 0xb8 */ 4, 5, 5, 6, 5, 6, 6, 7,
    /* 0xc0 */ 2, 3, 3, 4, 3, 4, 4, 5,
    /* 0xc8 */ 3, 4, 4, 5, 4, 5, 5, 6,
    /* 0xd0 */ 3, 4, 4, 5, 4, 5, 5, 6,
    /* 0xd8 */ 4, 5, 5, 6, 5, 6, 6, 7,
    /* 0xe0 */ 3, 4, 4, 5, 4, 5, 5, 6,
    /* 0xe8 */ 4, 5, 5, 6, 5, 6, 6, 7,
    /* 0xf0 */ 4, 5, 5, 6, 5, 6, 6, 7,
    /* 0xf8 */ 5, 6, 6, 7, 6, 7, 7, 8};
#endif

/************************** LOCAL BASIC DATA TYPES ***************************/

/**************************** LOCAL ENUMERATIONS *****************************/

/********************* LOCAL STRUCTURE /UNION DATA TYPES *********************/

/******************************* LOCAL MACROS ********************************/

/************************ LOCAL FUNCTIONS PROTOTYPES *************************/

/**************************** EXPORTED FUNCTIONS *****************************/

const char *BIT_Get_Version_String(void) { return cssc_what_compatible_version_string; }

/***************************** LOCAL FUNCTIONS *******************************/

/******************************************************************************
 * End of file
 ******************************************************************************/
