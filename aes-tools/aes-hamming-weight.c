#include "aes-common.h"
#include "../convolution.h"

int
main(void)
{
	Table *weight_counters_superbox = table_create(); // Stores triples of the form (hamming weight, not used, number of differences)

	mpz_t row, col, val;
	mpz_inits(row, col, val, NULL);
	mpz_set_ui(col, 0);
	mpz_set_ui(val, 1);
	/*
	 * Consider the superbox. There are 2^32 possible input differences to mix_columns.
	 * For each such difference, say a, compute w := hw(a) + hw(mix_columns(a)),
	 * and increment the entry indexed by w in the histogram by 1.
	 */
	for (uint64_t a = 0; a <= UINT32_MAX; a++) {
		mpz_set_ui(row, hamming_weight32(a) + hamming_weight32(mix_columns(a)));
		table_insert_and_merge(weight_counters_superbox, row, col, val, &mpz_add);
	}
	mpz_clears(row, col, val, NULL);

	table_print("data/aes-hamming-superbox.txt", weight_counters_superbox);

	// Given the histogram of the superbox, compute the histogram of the full state by convoluting all the superboxes. 
	Table *weight_counters_full = convolve_tables(weight_counters_superbox, NUMBER_OF_SUPERBOXES, 0, 0, differential_combiners);
	table_print("data/aes-hamming-full.txt", weight_counters_full);

	table_destroy(weight_counters_superbox);
	table_destroy(weight_counters_full);

	return 0;
}
