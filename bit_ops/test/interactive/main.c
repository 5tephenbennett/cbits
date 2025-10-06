#include "bit_interactive.h"
#include "bit_ops.h"
#include "bit_ops_api.h"
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>

#define TIME_NOW() (times(&buf) - orig_time)

static BOOL test_fn(char *line, BOOL quiet);

static BOOL test_fn(char *line, BOOL quiet) {
    switch (line[0]) {
    case 'q':
        return TRUE;

    default:
        BIT_Run_Command(line, quiet);
        break;
    }

    return FALSE;
}

int main() {
    char line[1024] = "";
    BOOL quit_signal = FALSE;
    BOOL is_interactive = isatty(STDIN_FILENO);

    BIT_Initialise();
    BIT8_Initialise();
    BIT16_Initialise();
    BIT32_Initialise();
    BIT64_Initialise();

    while (FALSE == quit_signal) {
        line[0] = '\0';
        if (is_interactive) {
            printf("Enter command to run: ");
        }
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break; /* EOF or error */
        }
        quit_signal = test_fn(line, !is_interactive);
    }

    return 0;
}
