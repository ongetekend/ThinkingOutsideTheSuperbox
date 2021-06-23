/*
 * Adapted from Joux' algorithmic cryptanalysis, page 278
 */

#include "walsh-transform.h"

void 
walsh_transform(mpz_t *tab, const int size)
{
	mpz_t temp;
	mpz_init(temp);
	for (int step = 1; step < size; step <<= 1) {
		for (int i1 = 0; i1 < size; i1 += 2*step) {
			for (int i0 = 0; i0 < step; i0++) {
				int i = i1 + i0;
				mpz_set(temp, tab[i]);
				mpz_add(tab[i], tab[i], tab[i+step]);
				mpz_sub(tab[i+step], temp, tab[i+step]);
			}
		}
	}
	mpz_clear(temp);
}
