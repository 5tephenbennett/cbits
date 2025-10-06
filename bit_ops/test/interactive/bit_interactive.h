#ifndef BIT_INTERACTIVE_H
#define BIT_INTERACTIVE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "bit_ops.h"

// Function pointer type for test commands
typedef void (*BIT_TEST_FN)(char *params);

// Structure for command list entries
typedef struct {
    BIT_TEST_FN fn; // Function pointer to command implementation
    char *fn_name;  // Command name string
    char *usage;    // Usage/help string
} COMMAND_LIST;

/* Maximum number of command lists that can be registered across modules */
#ifndef MAX_REGISTERED_COMMANDS
#define MAX_REGISTERED_COMMANDS 8u
#endif

// Function declarations
void BIT_Register_Commands(COMMAND_LIST const *cmds);
void BIT_Run_Command(char *line, BOOL quiet);

// Module initializations
void BIT_Initialise(void);
void BIT8_Initialise(void);
void BIT16_Initialise(void);
void BIT32_Initialise(void);
void BIT64_Initialise(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // BIT_INTERACTIVE_H
