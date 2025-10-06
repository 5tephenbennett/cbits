/* Test for two's complement arithmetic */
int main() {
    /* In two's complement: -1 has all bits set */
    /* In sign-magnitude: -1 would be 0x80000001 (on 32-bit) */
    /* In ones' complement: -1 would be 0xFFFFFFFE */

    int negative_one = -1;
    unsigned int as_unsigned = (unsigned int)negative_one;

    /* In two's complement, -1 cast to unsigned should have all bits set */
    /* Check if all bits are 1 by comparing with ~0u */
    if (as_unsigned == ~0u) {
        return 0; /* Two's complement detected */
    }

    return 1; /* Not two's complement */
}