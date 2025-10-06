/* Test file to check for 64-bit unsigned integer support */
#include <stdint.h>

int main(void) {
    uint64_t test = 0xFFFFFFFFFFFFFFFFULL;
    return (test == 0xFFFFFFFFFFFFFFFFULL) ? 0 : 1;
}