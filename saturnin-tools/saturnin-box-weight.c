#include "saturnin-common.h"
#include "../convolution.h"

int
main(void)
{
	Table *box_weights_superbox = table_create();
	Table *box_partition_superbox = table_create();

	mpz_t number_of_subsets, cardinality, row, col, val;
	mpz_inits(number_of_subsets, cardinality, row, col, NULL);
	mpz_init_set_ui(val, 1);
	// Insert (0, 0, 1) into the histogram of box weights for the superbox.
	table_insert_and_merge(box_weights_superbox, row, col, val, &mpz_add);
	mpz_set_ui(col, 1);
	// Insert (0, 1, 1) into the partition histogram for the superbox.
	table_insert_and_merge(box_partition_superbox, row, col, val, &mpz_add);
	// Due to the linear layer, every box pattern has a box weight of at least 5.
	for (int pattern_box_weight = 5; pattern_box_weight <= MAX_BOX_WEIGHT; pattern_box_weight++) {
		cardinality_box_pattern(cardinality, BOX_WIDTH, NUMBER_OF_BOXES_IN_SUPERBOX, pattern_box_weight);
		mpz_bin_uiui(number_of_subsets, MAX_BOX_WEIGHT, pattern_box_weight);
		mpz_set_ui(row, pattern_box_weight);
		mpz_set_ui(col, 0);
		mpz_mul(val, number_of_subsets, cardinality);
		table_insert_and_merge(box_weights_superbox, row, col, val, &mpz_add);
		table_insert_and_merge(box_partition_superbox, row, cardinality, number_of_subsets, &mpz_add);
	}
	mpz_clears(number_of_subsets, cardinality, row, col, val, NULL);

	// To get the histograms of the full state, we convolve the histograms of the super boxes.
	Table *box_weights_full = convolve_tables(box_weights_superbox, NUMBER_OF_SUPERBOXES, 0, 0, differential_combiners);
	Table *box_partition_full = convolve_tables(box_partition_superbox, NUMBER_OF_SUPERBOXES, 0, 0, differential_combiners);

	table_print("data/saturnin-box-superbox.txt", box_weights_superbox);
	table_print("data/saturnin-box-full.txt", box_weights_full);
	table_print("data/saturnin-partition-superbox.txt", box_partition_superbox);	
	table_print("data/saturnin-partition-full.txt", box_partition_full);	

	table_destroy(box_weights_superbox);
	table_destroy(box_weights_full);
	table_destroy(box_partition_superbox);
	table_destroy(box_partition_full);

	return 0;
}
