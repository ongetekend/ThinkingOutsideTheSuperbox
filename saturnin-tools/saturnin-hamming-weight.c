#include "saturnin-common.h"
#include "../convolution.h"

int
main(void)
{
	Table *weight_counters_superbox = table_create();

	mpz_t row, col, val;
	mpz_inits(row, col, val, NULL);
	mpz_set_ui(col, 0);
	mpz_set_ui(val, 1);
	/*
	 * For each input difference, say a, compute w := hw(a) + hw(mix_columns)
	 * and increment the value associated with w.
	 */
	for (uint32_t a = 0; a <= UINT16_MAX; a++) {
		mpz_set_ui(row, hamming_weight16(a) + hamming_weight16(mix_columns(a)));
		table_insert_and_merge(weight_counters_superbox, row, col, val, &mpz_add);
	}
	mpz_clears(row, col, val, NULL);

	table_print("data/saturnin-hamming-superbox.txt", weight_counters_superbox);

	// Convolute the histograms of the superboxes to get the histogram of the full state.
	Table *weight_counters_full = convolve_tables(weight_counters_superbox, NUMBER_OF_SUPERBOXES, 0, 0, differential_combiners);
	table_print("data/saturnin-hamming-full.txt", weight_counters_full);

	table_destroy(weight_counters_superbox);
	table_destroy(weight_counters_full);

	return 0;
}
