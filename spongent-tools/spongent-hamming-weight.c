#include "spongent-common.h"
#include "../convolution.h"

int
main(void)
{
	Table *hamming_weights = table_create();

	mpz_t row, col, val;
	mpz_inits(row, col, val, NULL);
	/*
	 * Every wire counts two active bits. Therefore, we fix a set of wires with a given cardinality, C say, and count the number of such sets.
	 * This then equals the number of states with hamming weight equal to 2*C.
	 */
	for (size_t i = 0; i <= WIDTH; i++) {
		mpz_bin_uiui(val, WIDTH, i);	
		mpz_set_ui(row, 2*i);
		table_insert_and_merge(hamming_weights, row, col, val, &mpz_add);
	}
	mpz_clears(row, col, val, NULL);

	table_print("data/spongent-hamming-full.txt", hamming_weights);
	table_destroy(hamming_weights);

	return 0;
}
