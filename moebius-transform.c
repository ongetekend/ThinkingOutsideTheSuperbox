/*
 * Adapted from Joux' algorithmic cryptanalysis, page 287
 */

#include "moebius-transform.h"

/* In place Moebius transform
 * Assumes that value vector of Boolean function is stored into Tab
 */
void 
moebius_transform(TYPE *Tab, TYPE size)
{
        int Wsize;
        TYPE i, i0, i1;  
        TYPE step;
        Wsize = size / (8 * sizeof(TYPE));

        /* Moebius transform for high order bits, using word ops */
        for (step = 1; step < Wsize; step <<= 1) {
                for (i1 = 0; i1 < Wsize; i1 += 2 * step) {
                        for (i0 = 0; i0 < step; i0++) {
                                i = i1 + i0;
                                Tab[i + step] ^= Tab[i];
                        }
                } 
        }

        /* Moebius transform for low order bits, within words */
        /* Assumes 8 * sizeof(TYPE) == 32 */
        for(i = 0; i < Wsize; i++) {
                TYPE tmp;
                tmp = Tab[i];
                tmp ^= (tmp & 0x0000ffff) << 16;
                tmp ^= (tmp & 0x00ff00ff) << 8;
                tmp ^= (tmp & 0x0f0f0f0f) << 4;
                tmp ^= (tmp & 0x33333333) << 2;
                tmp ^= (tmp & 0x55555555) << 1;
                Tab[i] = tmp;
        } 
}

/* In place Moebius transform
 * Assumes that value vector of Boolean function is stored into Tab
 */
void 
moebius_transform_4bit(uint16_t *Tab)
{
        uint16_t tmp;
        tmp = *Tab;
        tmp ^= (tmp & 0x00ff) << 8;
        tmp ^= (tmp & 0x0f0f) << 4;
        tmp ^= (tmp & 0x3333) << 2;
        tmp ^= (tmp & 0x5555) << 1;
        *Tab = tmp;
}


/* In place Moebius transform
 * Assumes that value vector of Boolean function is stored into Tab
 */
void 
moebius_transform_3bit(uint8_t *Tab)
{
        uint8_t tmp;
        tmp = *Tab;
        tmp ^= (tmp & 0x0f) << 4;
        tmp ^= (tmp & 0x33) << 2;
        tmp ^= (tmp & 0x55) << 1;
        *Tab = tmp;
}

/* 5 = 0101
 * 3 = 0011
 * f = 1111
 */

int
main(void)
{
    /* Input     0 1 2 3 4 5 6 7 8 9 a b c d e f
     * Output    c 6 9 0 1 a 2 b 3 8 5 d 4 e 7 f   Hex (as presented)  Hex (reversed)
     *           - - - - - - - - - - - - - - - - - ----------------------------------
     * Sbox[1] | 1 0 1 0 0 1 0 1 0 1 0 1 0 1 0 1 | a555               | aaa5
     * Sbox[2] | 1 1 0 0 0 0 0 0 0 0 1 1 1 1 1 1 | c03f               | fc03
     * Sbox[3] | 0 1 0 0 0 1 1 1 1 0 0 0 0 1 1 1 | 4787               | e1e2
     * Sbox[4] | 0 0 1 0 1 0 0 1 1 0 1 1 0 0 1 1 | 29b3               | cd94
     */

    /* Input a[0], ..., a[2^n-1], that is, when the bits are viewed as numbers, they form an increasing sequence
     * Here, a[2^n-1] corresponds to the MSB (the left-most bit) and a[0] corresponds to the LSB (right-most bit).
     * Output needs to be interpreted in the same way.
     * (a[2^n-1], ..., a[0])
     */

    uint16_t skinny_sbox_coordinate1 = 0xaaa5;
    uint16_t skinny_sbox_coordinate2 = 0xfc03;
    uint16_t skinny_sbox_coordinate3 = 0xe1e2;
    uint16_t skinny_sbox_coordinate4 = 0xcd94;

    moebius_transform_4bit(&skinny_sbox_coordinate1);
    moebius_transform_4bit(&skinny_sbox_coordinate2);
    moebius_transform_4bit(&skinny_sbox_coordinate3);
    moebius_transform_4bit(&skinny_sbox_coordinate4);

    printf("ANF coefficients bit vector of coordinate 1 is %04x\n", skinny_sbox_coordinate1);
    printf("ANF coefficients bit vector of coordinate 2 is %04x\n", skinny_sbox_coordinate2);
    printf("ANF coefficients bit vector of coordinate 3 is %04x\n", skinny_sbox_coordinate3);
    printf("ANF coefficients bit vector of coordinate 4 is %04x\n", skinny_sbox_coordinate4);

    uint8_t example_boolean_function = 0xe2;
    moebius_transform_3bit(&example_boolean_function);
    printf("ANF coefficients bit vector of example Boolean function is %02x\n", example_boolean_function);

    return 0;
}
