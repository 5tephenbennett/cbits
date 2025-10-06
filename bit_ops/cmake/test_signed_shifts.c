/* Test for signed right shift behavior (arithmetic vs logical) */
int main() {
    /* Test if signed right shifts preserve sign (arithmetic shift) */
    /* vs. zero-fill (logical shift) */

    int negative = -8;  /* In binary: ...11111000 */
    int shifted = negative >> 1;  /* Should be -4 if arithmetic, large positive if logical */

    /* In arithmetic right shift: -8 >> 1 == -4 */
    /* In logical right shift: would be a large positive number */
    if (shifted == -4) {
        return 0; /* Arithmetic (signed) right shifts detected */
    }

    return 1; /* Logical right shifts or undefined behavior */
}