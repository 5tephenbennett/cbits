/**
 * @file bit_ops_cmds.h
 * @brief Generic interactive commands for bit_ops library.
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

/*** CONSTANTS are provided via bit_interactive.h (MAX_REGISTERED_COMMANDS) ***/

/**************************** LOCAL ENUMERATIONS *****************************/

/********************* LOCAL STRUCTURE /UNION DATA TYPES *********************/

/******************************* LOCAL MACROS ********************************/

/************************ LOCAL FUNCTIONS PROTOTYPES *************************/
void CMD_N_BITS(char *params);
void CMD_N_BYTES(char *params);
void CMD_BIT_WORDBITS(char *params);
void CMD_BIT_TOPBIT(char *params);
void CMD_BIT_SWAP(char *params);
void CMD_BIT_COMPLIMENT(char *params);
void CMD_BIT_NEGATE(char *params);
void CMD_BIT_GET_LOWEST_BIT_SET(char *params);
void CMD_BIT_GET_LBS(char *params);
void CMD_BIT_STRIP_LBS(char *params);
void CMD_BIT_MASK_LBS_AND_ABOVE(char *params);
void CMD_BIT_MASK_ABOVE_LBS(char *params);
void CMD_BIT_MASK_LBS_AND_BELOW(char *params);
void CMD_BIT_MASK_BELOW_LBS(char *params);
void CMD_BIT_FILL_BELOW_LBS(char *params);
void CMD_BIT_SHIFT_LBS_TO_BIT0(char *params);
void CMD_BIT_FILL_LOWEST_ZERO_BIT(char *params);
void CMD_BIT_FILL_LZB(char *params);
void CMD_BIT_ONLY(char *params);
void CMD_BIT_MIN(char *params);
void CMD_BIT_MAX(char *params);
void CMD_BIT_CLIP(char *params);
void CMD_BIT_GET_VAL_IF_NZERO(char *params);
void CMD_BIT_GET_VAL_IF_ONE(char *params);
void CMD_BIT_GET_VAL_IF_ZERO(char *params);
void CMD_BIT_GET_COND_VAL(char *params);
void CMD_BIT_GET_INT_VAL(char *params);
void CMD_BIT_NEGATE_IF_NZERO(char *params);
void CMD_BIT_NEGATE_IF_ONE(char *params);
void CMD_BIT_NEGATE_IF_ZERO(char *params);
void CMD_BIT_AVERAGE(char *params);
void CMD_BIT_IS_BIG_ENDIAN(char *params);
void CMD_BIT_SWAP_ENDIAN(char *params);

static BOOL BIT_Is_Big_Endian(void);
static void BIT_Swap_Endian(void *data, uint32_t bytes);

static BOOL BIT_Is_Big_Endian(void) {
    uint32_t i = 0x01000000;
    return ((BOOL) * ((uint8_t *)&i));
}

/* note: */
static void BIT_Swap_Endian(void *const data, uint32_t bytes) {
    uint8_t *start_ptr = (uint8_t *)data;
    uint8_t *end_ptr = (uint8_t *)data + bytes - 1;

    while (start_ptr < end_ptr) {
        BIT_SWAP(*start_ptr, *end_ptr);
        start_ptr++;
        end_ptr--;
    }
}

/**************************** EXPORTED FUNCTIONS *****************************/

COMMAND_LIST const *registered_commands[MAX_REGISTERED_COMMANDS];

const COMMAND_LIST bit_commands[] = {
    /* function,                     name,                        syntax */
    {CMD_N_BITS, "N_BITS", "N_BITS(bytes);"},
    {CMD_N_BYTES, "N_BYTES", "N_BYTES(bits);"},
    {CMD_BIT_WORDBITS, "BIT_WORDBITS", "BIT_WORDBITS(x)"},
    {CMD_BIT_TOPBIT, "BIT_TOPBIT", "BIT_TOPBIT(x)"},
    {CMD_BIT_SWAP, "BIT_SWAP", "BIT_SWAP(a, b)"},
    {CMD_BIT_COMPLIMENT, "BIT_COMPLIMENT", "BIT_COMPLIMENT(x)"},
    {CMD_BIT_NEGATE, "BIT_NEGATE", "BIT_NEGATE(x)"},
    {CMD_BIT_GET_LOWEST_BIT_SET, "BIT_GET_LOWEST_BIT_SET", "BIT_GET_LOWEST_BIT_SET(x)"},
    {CMD_BIT_GET_LBS, "BIT_GET_LBS", "BIT_GET_LBS(x)"},
    {CMD_BIT_STRIP_LBS, "BIT_STRIP_LBS", "BIT_STRIP_LBS(x)"},
    {CMD_BIT_MASK_LBS_AND_ABOVE, "BIT_MASK_LBS_AND_ABOVE", "BIT_MASK_LBS_AND_ABOVE(x)"},
    {CMD_BIT_MASK_ABOVE_LBS, "BIT_MASK_ABOVE_LBS", "BIT_MASK_ABOVE_LBS(x)"},
    {CMD_BIT_MASK_LBS_AND_BELOW, "BIT_MASK_LBS_AND_BELOW", "BIT_MASK_LBS_AND_BELOW(x)"},
    {CMD_BIT_MASK_BELOW_LBS, "BIT_MASK_BELOW_LBS", "BIT_MASK_BELOW_LBS(x)"},
    {CMD_BIT_FILL_BELOW_LBS, "BIT_FILL_BELOW_LBS", "BIT_FILL_BELOW_LBS(x)"},
    {CMD_BIT_SHIFT_LBS_TO_BIT0, "BIT_SHIFT_LBS_TO_BIT0", "BIT_SHIFT_LBS_TO_BIT0(x)"},
    {CMD_BIT_FILL_LOWEST_ZERO_BIT, "BIT_FILL_LOWEST_ZERO_BIT", "BIT_FILL_LOWEST_ZERO_BIT(x)"},
    {CMD_BIT_FILL_LZB, "BIT_FILL_LZB", "BIT_FILL_LZB(x)"},
    {CMD_BIT_ONLY, "BIT_ONLY", "BIT_ONLY(x)"},
    {CMD_BIT_MIN, "BIT_MIN", "BIT_MIN(a, b);"},
    {CMD_BIT_MAX, "BIT_MAX", "BIT_MAX(a, b);"},
    {CMD_BIT_CLIP, "BIT_CLIP", "BIT_CLIP(a, min, max)"},
    {CMD_BIT_GET_VAL_IF_NZERO, "BIT_GET_VAL_IF_NZERO", "BIT_GET_VAL_IF_NZERO(x, return_if_nzero)"},
    {CMD_BIT_GET_VAL_IF_ONE,
     "BIT_GET_VAL_IF_ONE",
     "BIT_GET_VAL_IF_ONE(zero_or_one, return_if_one)"},
    {CMD_BIT_GET_VAL_IF_ZERO,
     "BIT_GET_VAL_IF_ZERO",
     "BIT_GET_VAL_IF_ZERO(zero_or_one, return_if_zero)"},
    {CMD_BIT_GET_COND_VAL,
     "BIT_GET_COND_VAL",
     "BIT_GET_COND_VAL(zero_or_one, return_if_one, return_if_zero)"},
    {CMD_BIT_GET_INT_VAL, "BIT_GET_INT_VAL", "BIT_GET_INT_VAL(x, return_if_nzero, return_if_zero)"},
    {CMD_BIT_NEGATE_IF_NZERO, "BIT_NEGATE_IF_NZERO", "BIT_NEGATE_IF_NZERO(comp, x)"},
    {CMD_BIT_NEGATE_IF_ONE, "BIT_NEGATE_IF_ONE", "BIT_NEGATE_IF_ONE(zero_or_one, x)"},
    {CMD_BIT_NEGATE_IF_ZERO, "BIT_NEGATE_IF_ZERO", "BIT_NEGATE_IF_ZERO(zero_or_one, x)"},
    {CMD_BIT_AVERAGE, "BIT_AVERAGE", "BIT_AVERAGE(a, b)"},
    {CMD_BIT_IS_BIG_ENDIAN, "BIT_Is_Big_Endian", "BIT_Is_Big_Endian()"},
    {CMD_BIT_SWAP_ENDIAN,
     "BIT_Swap_Endian",
     "BIT_Swap_Endian(var, bytes) [NOTE: bytes < 4 or stack overflow]"},
    {NULL, "", ""}};

void BIT_Initialise(void) { BIT_Register_Commands(bit_commands); }

void BIT_Register_Commands(COMMAND_LIST const *const cmds) {
    uint32_t i = 0;

    for (i = 0; i < MAX_REGISTERED_COMMANDS; i++) {
        if (NULL == registered_commands[i]) {
            registered_commands[i] = cmds;
            break;
        }
    }
}

void BIT_Run_Command(char *line, BOOL quiet) {
    uint32_t cmd_len = 0;
    uint32_t i = 0;
    uint32_t j = 0;
    BIT_TEST_FN cmd = NULL;
    char *params = NULL;

    /* null-terminate the string after the command and
    ** store the command length so we know where parameters begin
    */
    for (cmd_len = 0; line[cmd_len] != '\0' && !isspace((unsigned char)line[cmd_len]); cmd_len++) {
        ;
    }

    /* Capture the character where the command ends before we overwrite it */
    char saved_split = line[cmd_len];
    /* Ensure command token is null-terminated even if it already was */
    line[cmd_len] = '\0';
    /* Compute the start of parameters: skip the split char (if any) and any additional whitespace
     */
    uint32_t p = cmd_len;
    if (saved_split != '\0') {
        p++;
    }
    while (line[p] != '\0' && isspace((unsigned char)line[p])) {
        p++;
    }
    params = &line[p];

    for (i = 0; i < MAX_REGISTERED_COMMANDS; i++) {
        if (NULL != registered_commands[i]) {
            for (j = 0; NULL != registered_commands[i][j].fn; j++) {
                if (0 == strcmp(line, registered_commands[i][j].fn_name)) {
                    cmd = registered_commands[i][j].fn;
                    break;
                }
            }
        }
    }

    if (NULL != cmd) {
        /* run the function */
        cmd(params);
    } else {
        /* print help */
        if (!quiet) {
            printf("\n\r%-30s - Usage [note: numbers should be written for strtoul(,,0)] \n\r",
                   "Command List");
            printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\r");

            for (i = 0; i < MAX_REGISTERED_COMMANDS; i++) {
                if (NULL != registered_commands[i]) {
                    for (j = 0; NULL != registered_commands[i][j].fn; j++) {
                        printf("%-30s - %s\n\r",
                               registered_commands[i][j].fn_name,
                               registered_commands[i][j].usage);
                    }
                }
            }
            printf("\n\n\r");
        }
    }
}

void CMD_N_BITS(char *params) {
    char *params2 = NULL;
    uint32_t bytes = (uint32_t)strtoul(params, &params2, 0);
    printf("N_BITS(%d) = %d\n\r", bytes, N_BITS(bytes));
}

void CMD_N_BYTES(char *params) {
    char *params2 = NULL;
    uint32_t bits = (uint32_t)strtoul(params, &params2, 0);
    printf("N_BYTES(%d) = %d\n\r", bits, N_BYTES(bits));
}

void CMD_BIT_WORDBITS(char *params) {
    char *params2 = NULL;
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_WORDBITS(%d)=%zu\n\r", x, BIT_WORDBITS(x));
}

void CMD_BIT_TOPBIT(char *params) {
    char *params2 = NULL;
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_TOPBIT(%d)=%zu\n\r", x, BIT_TOPBIT(x));
}

void CMD_BIT_SWAP(char *params) {
    char *params2 = NULL;
    int32_t a = (int32_t)strtoul(params, &params2, 0);
    int32_t b = (int32_t)strtoul(params2, &params, 0);
    int32_t a_start = a;
    int32_t b_start = b;
    BIT_SWAP(a, b);

    printf("BIT_SWAP(%d,%d) = %d, %d\n\r", a_start, b_start, a, b);
}

void CMD_BIT_COMPLIMENT(char *params) {
    char *params2 = NULL;
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_COMPLIMENT(%d)=%d\n\r", x, BIT_COMPLIMENT(x));
}

void CMD_BIT_NEGATE(char *params) {
    char *params2 = NULL;
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_NEGATE(%d)=%d\n\r", x, BIT_NEGATE(x));
}

void CMD_BIT_GET_LOWEST_BIT_SET(char *params) {
    char *params2 = NULL;
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_GET_LOWEST_BIT_SET(%sb)=%sb\n\r",
           BIT32_Bitstring(x, bitstring),
           BIT32_Bitstring(BIT_GET_LOWEST_BIT_SET(x), bitstring2));
}

void CMD_BIT_GET_LBS(char *params) {
    char *params2 = NULL;
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_GET_LBS(%sb)=%sb\n\r",
           BIT32_Bitstring(x, bitstring),
           BIT32_Bitstring(BIT_GET_LBS(x), bitstring2));
}

void CMD_BIT_STRIP_LBS(char *params) {
    char *params2 = NULL;
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_STRIP_LBS(%sb)=%sb\n\r",
           BIT32_Bitstring(x, bitstring),
           BIT32_Bitstring(BIT_STRIP_LBS(x), bitstring2));
}

void CMD_BIT_MASK_LBS_AND_ABOVE(char *params) {
    char *params2 = NULL;
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_MASK_LBS_AND_ABOVE(%sb)=%sb\n\r",
           BIT32_Bitstring(x, bitstring),
           BIT32_Bitstring(BIT_MASK_LBS_AND_ABOVE(x), bitstring2));
}

void CMD_BIT_MASK_ABOVE_LBS(char *params) {
    char *params2 = NULL;
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_MASK_ABOVE_LBS(%sb)=%sb\n\r",
           BIT32_Bitstring(x, bitstring),
           BIT32_Bitstring(BIT_MASK_ABOVE_LBS(x), bitstring2));
}

void CMD_BIT_MASK_LBS_AND_BELOW(char *params) {
    char *params2 = NULL;
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_MASK_LBS_AND_BELOW(%sb)=%sb\n\r",
           BIT32_Bitstring(x, bitstring),
           BIT32_Bitstring(BIT_MASK_LBS_AND_BELOW(x), bitstring2));
}

void CMD_BIT_MASK_BELOW_LBS(char *params) {
    char *params2 = NULL;
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_MASK_BELOW_LBS(%sb)=%sb\n\r",
           BIT32_Bitstring(x, bitstring),
           BIT32_Bitstring(BIT_MASK_BELOW_LBS(x), bitstring2));
}

void CMD_BIT_FILL_BELOW_LBS(char *params) {
    char *params2 = NULL;
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_FILL_BELOW_LBS(%sb)=%sb\n\r",
           BIT32_Bitstring(x, bitstring),
           BIT32_Bitstring(BIT_FILL_BELOW_LBS(x), bitstring2));
}

void CMD_BIT_SHIFT_LBS_TO_BIT0(char *params) {
    char *params2 = NULL;
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_SHIFT_LBS_TO_BIT0(%sb)=%sb\n\r",
           BIT32_Bitstring(x, bitstring),
           BIT32_Bitstring(BIT_SHIFT_LBS_TO_BIT0(x), bitstring2));
}

void CMD_BIT_FILL_LOWEST_ZERO_BIT(char *params) {
    char *params2 = NULL;
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_FILL_LOWEST_ZERO_BIT(%sb)=%sb\n\r",
           BIT32_Bitstring(x, bitstring),
           BIT32_Bitstring(BIT_FILL_LOWEST_ZERO_BIT(x), bitstring2));
}

void CMD_BIT_FILL_LZB(char *params) {
    char *params2 = NULL;
    char bitstring[BIT32_STRING_SIZE];
    char bitstring2[BIT32_STRING_SIZE];
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_FILL_LZB(%sb)=%sb\n\r",
           BIT32_Bitstring(x, bitstring),
           BIT32_Bitstring(BIT_FILL_LZB(x), bitstring2));
}

void CMD_BIT_ONLY(char *params) {
    char *params2 = NULL;
    uint32_t var = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_ONLY(%d) = %s\n\r", var, BIT_ONLY(var) ? "TRUE" : "FALSE");
}

void CMD_BIT_MIN(char *params) {
    char *params2 = NULL;
    int32_t a = (int32_t)strtoul(params, &params2, 0);
    int32_t b = (int32_t)strtoul(params2, &params, 0);
    printf("BIT_MIN(%d,%d) = %d\n\r", a, b, BIT_MIN(a, b));
}

void CMD_BIT_MAX(char *params) {
    char *params2 = NULL;
    int32_t a = (int32_t)strtoul(params, &params2, 0);
    int32_t b = (int32_t)strtoul(params2, &params, 0);
    printf("BIT_MAX(%d,%d) = %d\n\r", a, b, BIT_MAX(a, b));
}

void CMD_BIT_CLIP(char *params) {
    char *params2 = NULL;
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    uint32_t xmin = (uint32_t)strtoul(params2, &params, 0);
    uint32_t xmax = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_CLIP(%d,min=%d,max=%d) = %d\n\r", x, xmin, xmax, BIT_CLIP(x, xmin, xmax));
}

void CMD_BIT_GET_VAL_IF_NZERO(char *params) {
    char *params2 = NULL;
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    uint32_t nzero_val = (uint32_t)strtoul(params2, &params, 0);
    printf("BIT_GET_VAL_IF_NZERO(cond=%d, nzero_val=%d) = %d\n\r",
           x,
           nzero_val,
           BIT_GET_VAL_IF_NZERO(x, nzero_val));
}

void CMD_BIT_GET_VAL_IF_ONE(char *params) {
    char *params2 = NULL;
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    uint32_t one_val = (uint32_t)strtoul(params2, &params, 0);
    printf("BIT_GET_VAL_IF_ONE(cond=%d, one_val=%d) = %d\n\r",
           x,
           one_val,
           BIT_GET_VAL_IF_ONE(x, one_val));
}

void CMD_BIT_GET_VAL_IF_ZERO(char *params) {
    char *params2 = NULL;
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    uint32_t zero_val = (uint32_t)strtoul(params2, &params, 0);
    printf("BIT_GET_VAL_IF_ZERO(cond=%d, zero_val=%d) = %d\n\r",
           x,
           zero_val,
           BIT_GET_VAL_IF_ZERO(x, zero_val));
}

void CMD_BIT_GET_COND_VAL(char *params) {
    char *params2 = NULL;
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    uint32_t one_val = (uint32_t)strtoul(params2, &params, 0);
    uint32_t zero_val = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_GET_COND_VAL(cond=%d, one_val=%d, zero_val=%d) = %d\n\r",
           x,
           one_val,
           zero_val,
           BIT_GET_COND_VAL(x, one_val, zero_val));
}

void CMD_BIT_GET_INT_VAL(char *params) {
    char *params2 = NULL;
    uint32_t x = (uint32_t)strtoul(params, &params2, 0);
    uint32_t nzero_val = (uint32_t)strtoul(params2, &params, 0);
    uint32_t zero_val = (uint32_t)strtoul(params, &params2, 0);
    printf("BIT_GET_INT_VAL(cond=%d, nzero_val=%d, zero_val=%d) = %d\n\r",
           x,
           nzero_val,
           zero_val,
           BIT_GET_INT_VAL(x, nzero_val, zero_val));
}

void CMD_BIT_NEGATE_IF_NZERO(char *params) {
    char *params2 = NULL;
    uint32_t zero_one_val = (uint32_t)strtoul(params, &params2, 0);
    uint32_t x = (uint32_t)strtoul(params2, &params, 0);
    printf("BIT_NEGATE_IF_NZERO(%d, %d)=%d\n\r",
           zero_one_val,
           x,
           BIT_NEGATE_IF_NZERO(zero_one_val, x));
}

void CMD_BIT_NEGATE_IF_ONE(char *params) {
    char *params2 = NULL;
    uint32_t zero_one_val = (uint32_t)strtoul(params, &params2, 0);
    uint32_t x = (uint32_t)strtoul(params2, &params, 0);
    printf("BIT_NEGATE_IF_ONE(%d, %d)=%d\n\r", zero_one_val, x, BIT_NEGATE_IF_ONE(zero_one_val, x));
}

void CMD_BIT_NEGATE_IF_ZERO(char *params) {
    char *params2 = NULL;
    uint32_t zero_one_val = (uint32_t)strtoul(params, &params2, 0);
    uint32_t x = (uint32_t)strtoul(params2, &params, 0);
    printf(
        "BIT_NEGATE_IF_ZERO(%d, %d)=%d\n\r", zero_one_val, x, BIT_NEGATE_IF_ZERO(zero_one_val, x));
}

void CMD_BIT_AVERAGE(char *params) {
    char *params2 = NULL;
    uint32_t a = (uint32_t)strtoul(params, &params2, 0);
    uint32_t b = (uint32_t)strtoul(params2, &params, 0);

    printf("[unsigned]BIT_AVERAGE(%u,%u) = %u\n\r", a, b, BIT_AVERAGE(a, b));
    printf("[signed]  BIT_AVERAGE(%d,%d) = %d\n\r",
           (int32_t)a,
           (int32_t)b,
           BIT_AVERAGE((int32_t)a, (int32_t)b));
}

void CMD_BIT_IS_BIG_ENDIAN(char *params __attribute__((unused))) {
    printf("BIT_Is_Big_Endian() = %s\n\r", BIT_Is_Big_Endian() ? "TRUE" : "FALSE");
}

void CMD_BIT_SWAP_ENDIAN(char *params) {
    uint32_t var_swap = 0;
    char *params2 = NULL;
    uint32_t var = (uint32_t)strtoul(params, &params2, 0);
    uint32_t bytes = (uint32_t)strtoul(params2, &params, 0);
    var_swap = var;

    BIT_Swap_Endian(&var_swap, bytes);

    printf("BIT_Swap_Endian(0x%08x, %d) = 0x%08x\n\r", var, bytes, var_swap);
}
