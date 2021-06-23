#include "global-common.h"

void (*differential_combiners[3])(mpz_t, const mpz_t, const mpz_t) = {&mpz_add, &mpz_mul, &mpz_mul};
void (*linear_combiners[3])(mpz_t, const mpz_t, const mpz_t) = {&mpz_mul, &mpz_add, &mpz_mul};

unsigned int
hamming_weight32(uint32_t w)
{
	unsigned int res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0f0f0f0f;
	res = res + (res >> 8);
	return (res + (res >> 16)) & 0x000000ff;
}

unsigned int
hamming_weight16(uint16_t w)
{
	unsigned int res = w - ((w >> 1) & 0x5555);
	res = (res & 0x3333) + ((res >> 2) & 0x3333);
	res = (res + (res >> 4)) & 0x0f0f;
	return (res + (res >> 8)) & 0x00ff;
}

unsigned int 
hamming_weight8(uint8_t w)
{
	unsigned int res = w - ((w >> 1) & 0x55);
	res = (res & 0x33) + ((res >> 2) & 0x33);
	return (res + (res >> 4)) & 0x0F;
}

uint64_t
box_weight(uint64_t a, int number_of_boxes, int box_width) {
	uint64_t weight = 0;
	for (int i = 0; i < number_of_boxes; i++) {
		if ((a & ((1 << box_width)-1)) != 0) {
			weight++;
		}	
		a >>= box_width;
	}

	return weight;
}

/* Computes sum_{i=0}^31 x[i]*y[i] over GF(2) */
int
dot_prod(uint32_t x, uint32_t y)
{
	uint64_t sum = 0;
	uint32_t z = x & y;
	for (int i = 0; i < 32; i++) {
		sum ^= (z & 1);
		z >>= 1;
	}

	return sum;
}

void
fill_correlation_table(int *correlation_table, const uint8_t *sbox, const int width)
{
	int number_of_elements = (1 << width);
	for (int a = 0; a < number_of_elements; a++) {
		for (int b = 0; b < number_of_elements; b++) {
			for (int x = 0; x < number_of_elements; x++) {
				if (dot_prod(a, x) == dot_prod(b, sbox[x])) {
					correlation_table[number_of_elements*a+b]++;
				} else {
					correlation_table[number_of_elements*a+b]--;
				}
			}
		}
	}
}

void
fill_difference_table(int *difference_table, const uint8_t *sbox, const int width)
{
	int number_of_elements = (1 << width);
	for (int a = 0; a < number_of_elements; a++) {
		for (int b = 0; b < number_of_elements; b++) {
			for (int x = 0; x < number_of_elements; x++) {
				if ((sbox[x] ^ sbox[x^a]) == b) {
					difference_table[number_of_elements*a+b]++;
				}
			}
		}
	}
}
