#include "aes-common.h"
#include "../global-common.h"
#include "../convolution.h"

int 
main(void)
{
	Table *weights_sbox = table_create();
	Table *weights_superbox = table_create();

	mpz_t row, col, val, number_of_differences;
	mpz_init(number_of_differences);
	mpz_init_set_ui(row, 0);
	mpz_init_set_ui(col, 0);
	mpz_init_set_ui(val, 1);
	// Insert (0, 0, 1) into the superbox weights histogram. This triple corresponds to the trivial differential.
	table_insert_and_merge(weights_superbox, row, col, val, &mpz_add); 

	mpz_set_ui(row, 6);
	// Insert (6, 0, 1) into the sbox weights histogram.
	table_insert_and_merge(weights_sbox, row, col, val, &mpz_add);
	mpz_set_ui(row, 7);
	mpz_set_ui(val, 126);
	// Insert (7, 0, 126) into the sbox weights histogram.
	table_insert_and_merge(weights_sbox, row, col, val, &mpz_add);

	// Due to the linear layer, there are always at least 5 active sboxes.
	for (int w = 5; w <= MAX_BOX_WEIGHT; w++) {
		// Count number of box patterns of box weight w. 
		mds_weight_enumerator(number_of_differences, 256, MAX_BOX_WEIGHT, 5, w);
		// Convolute the histograms of the sboxes w times.
		Table *r = convolve_tables(weights_sbox, w, 0, 0, differential_combiners);
		for (HeadNode *node = r->head; node != NULL; node = node->next_row) {
			// Multiply the number of box patterns with the number of differences in a box pattern and insert.
			mpz_mul(number_of_differences, number_of_differences, node->node.val);
			table_insert_and_merge(weights_superbox, node->node.row, col, number_of_differences, &mpz_add);
		}
		table_destroy(r);
	}

	mpz_clears(row, col, val, number_of_differences, NULL);

	table_print("data/aes-differential-trail-superbox.txt", weights_superbox);
	// Convolute the differential weight histogram of all superboxes to get the histogram of the full state
	Table *weights_full = convolve_tables(weights_superbox, NUMBER_OF_SUPERBOXES, 0, 0, differential_combiners);
	table_print("data/aes-differential-trail-full.txt", weights_full);

	table_destroy(weights_sbox);
	table_destroy(weights_superbox);
	table_destroy(weights_full);

	return 0;
}
