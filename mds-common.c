#include "mds-common.h"

/*
 * Let C be an [n, k, d] MDS code over F_q where k = n-d+1. This function sets r equal to the
 * number of code words in C of weight w.
 */
void
mds_weight_enumerator(mpz_t r, int q, int n, int d, int w)
{
	mpz_t sum, term, tmp;
	mpz_init_set_ui(sum, 0);
	mpz_init(tmp);
	for (int j = 0; j <= w-d; j++) {
		if ((j & 1) == 0) {
			mpz_init_set_ui(term, 1);
		} else {
			mpz_init_set_ui(term, 0);
		}
		mpz_bin_uiui(tmp, w-1, j);
		mpz_mul(term, term, tmp);
		mpz_ui_pow_ui(tmp, q, w-d-j);
		mpz_mul(term, term, tmp);
		mpz_add(sum, sum, term);
	}

	mpz_bin_uiui(tmp, n, w);
	mpz_set(r, tmp);
	mpz_mul_ui(r, r, q-1);
	mpz_mul(r, r, sum);
	mpz_clears(sum, term, tmp, NULL);
}

/*
 * The number of states in a box pattern having
 * box weight equal to w, number of boxes equal to n,
 * and box width equal to m.
 */
void
cardinality_box_pattern(mpz_t r, int m, int n, int w)
{
	mpz_t t;
	mpz_init(t);
	mpz_ui_pow_ui(t, 2, m);

	if (w == n+1) {
		mpz_sub_ui(r, t, 1);
		mpz_clear(t);
		return;

	} else if (w == n+2) {
		mpz_t tmp;
		mpz_init(tmp);
		mpz_sub_ui(r, t, 1);
		mpz_sub_ui(tmp, t, 1);
		mpz_sub_ui(tmp, tmp, n);
		mpz_mul(r, r, tmp);
		mpz_clears(t, tmp, NULL);
		return;
	}	

	mpz_t sum, tmp, tmp2;
	mpz_inits(sum, tmp, tmp2, NULL);
	for (int i = 1; i <= w-n-1; i++) {
		cardinality_box_pattern(tmp, m, n, w-i);
		mpz_bin_uiui(tmp2, n, i);
		mpz_mul(tmp, tmp, tmp2);
		mpz_add(sum, sum, tmp);
	}
	mpz_clears(tmp, tmp2, NULL);

	mpz_sub_ui(r, t, 1);
	mpz_pow_ui(r, r, w-n);
	mpz_sub(r, r, sum);
	mpz_clear(sum);
}
