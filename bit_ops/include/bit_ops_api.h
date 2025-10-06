/**
 * @file bit_ops_api.h
 * @brief PRIMARY PUBLIC API - Main entry point for the CBits library
 *
 * CBits - Bit Manipulation Library
 *
 * @section api_features Key Features
 *
 * - Optimized bit manipulation functions
 * - Support for 8, 16, 32, and 64-bit operations
 * - High-performance implementations with lookup tables
 * - Documentation with examples
 * - Single header include for API access
 *
 * Simply include this single header to access all functionality of the CBits bit manipulation
 * library.
 *
 * @section API Public API - Main Library Interface
 * @brief Main public API for the CBits library
 * @{
 *
 * The CBits library provides bit manipulation operations for:
 * - 8-bit integers (bit8_ops.h)
 * - 16-bit integers (bit16_ops.h)
 * - 32-bit integers (bit32_ops.h)
 * - 64-bit integers (bit64_ops.h) - when available
 * - Generic macros (bit_ops.h)
 *
 * All operations are optimized for performance and include:
 * - Bit counting and manipulation
 * - Bit reversing and swapping
 * - Bit extraction and insertion
 * - Conditional operations
 *
 * @section quick_start Quick Start Guide
 *
 * The CBits library provides a unified Public API through this single header:
 *
 * @code
 * #include "bit_ops_api.h"
 *
 * // Access all bit manipulation functions across all bit widths
 * uint32_t count = BIT32_Count_Bits_Set(0xABCD1234);  // Count set bits
 * uint8_t reversed = BIT8_Reverse(0x5D);              // Reverse bit order
 * uint16_t swapped = BIT16_Endian_Swap(0x1234);       // Byte swap
 * uint64_t result = BIT64_Set_Bit(value, 42);         // Set specific bit
 * @endcode
 *
 * Copyright 2009-2010 Stephen M Bennett. All Rights Reserved.
 */

#ifndef BIT_OPS_API_H_
#define BIT_OPS_API_H_

#include "bit16_ops.h"
#include "bit32_ops.h"
#include "bit8_ops.h"
#include "bit_ops.h"

#ifdef HAVE_64_BIT_SUPPORT
#include "bit64_ops.h"
#endif /* #ifdef HAVE_64_BIT_SUPPORT */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************** CONSTANTS **********************************/

/***************************** BASIC DATA TYPES ******************************/

/******************************* ENUMERATIONS ********************************/

/************************ STRUCTURE/UNION DATA TYPES *************************/

/********************************** MACROS ***********************************/

/*************************** FUNCTION PROTOTYPES ****************************/

/** @} */ /* end of API section */

/** @addtogroup API
 * @{
 */

/**
 * @brief Get the library version string
 * @return Pointer to a null-terminated version string
 *
 * Returns a string containing the version information for the CBits library.
 * The string format is typically "Major.Minor.Patch" (e.g., "1.0.0").
 *
 * @code
 * const char *version = BIT_Get_Version_String();
 * printf("CBits version: %s\n", version);
 * @endcode
 *
 * @note The returned pointer points to static storage and should not be freed.
 */
const char *BIT_Get_Version_String(void);

/** @} */ /* end of API group */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* of file BIT_OPS_API_H_ */

/******************************************************************************
 * End of file
 ******************************************************************************/
