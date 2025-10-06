/* Test file to check for 64-bit signed integer support */
#include <stdint.h>

int main(void) {
    int64_t test = 0x7FFFFFFFFFFFFFFFLL;
    return (test == 0x7FFFFFFFFFFFFFFFLL) ? 0 : 1;
}