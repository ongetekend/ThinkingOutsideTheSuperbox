#include "aes-common.h"
#include "../convolution.h"

#define MIN_DIFFERENTIAL_WEIGHT 6

int
main(void)
{
	Table *box_weights_superbox = table_create(); // Stores triples of the form (box weight, not used, number of differences)

	mpz_t number_of_subsets, cardinality, row, col, val;
	mpz_inits(number_of_subsets, cardinality, row, col, NULL);
	mpz_init_set_ui(val, 1);
	// Insert (0, 0, 1) into the histogram of box weights for the superbox.
	table_insert_and_merge(box_weights_superbox, row, col, val, &mpz_add);
	// There are no box patterns having a box weight between 1 and 4 (inclusive).
	// So we start at 5.
	for (int pattern_box_weight = 5; pattern_box_weight <= MAX_BOX_WEIGHT; pattern_box_weight++) {
		// Count the number of differences in a fixed box pattern of weight pattern_box_weight.
		cardinality_box_pattern(cardinality, BOX_WIDTH, NUMBER_OF_BOXES_IN_SUPERBOX, pattern_box_weight);
		// There are \binom{MAX_BOX_WEIGHT}{pattern_box_weight} box patterns having box_weight equal to pattern_box_weight.
		mpz_bin_uiui(number_of_subsets, MAX_BOX_WEIGHT, pattern_box_weight);
		mpz_set_ui(row, pattern_box_weight*MIN_DIFFERENTIAL_WEIGHT);
		mpz_set_ui(col, 0);
		mpz_mul(val, number_of_subsets, cardinality);
		table_insert_and_merge(box_weights_superbox, row, col, val, &mpz_add);
	}
	mpz_clears(number_of_subsets, cardinality, row, col, val, NULL);

	// To get the histograms of the full state, we convolve the histograms of the super boxes.
	Table *box_weights_full = convolve_tables(box_weights_superbox, NUMBER_OF_SUPERBOXES, 0, 0, differential_combiners);

	table_print("data/aes-trail-core-superbox.txt", box_weights_superbox);
	table_print("data/aes-trail-core-full.txt", box_weights_full);

	table_destroy(box_weights_superbox);
	table_destroy(box_weights_full);

	return 0;
}
