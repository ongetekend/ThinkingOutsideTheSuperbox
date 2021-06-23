#ifndef __INT_FUNCTIONS_H__
#define __INT_FUNCTIONS_H__

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <math.h>

extern void (*differential_combiners[3])(mpz_t, const mpz_t, const mpz_t);
extern void (*linear_combiners[3])(mpz_t, const mpz_t, const mpz_t);

unsigned int hamming_weight32(uint32_t);
unsigned int hamming_weight16(uint16_t);
unsigned int hamming_weight8(uint8_t);
uint64_t box_weight(uint64_t, int, int);
size_t power(size_t, size_t);
size_t binomial_coefficient(size_t, size_t);
int dot_prod(uint32_t, uint32_t);
void fill_difference_table(int *, const uint8_t *, const int);
void fill_correlation_table(int *, const uint8_t *, const int);

#endif
