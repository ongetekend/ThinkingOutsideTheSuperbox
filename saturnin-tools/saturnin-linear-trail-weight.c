#include "saturnin-common.h"
#include "../global-common.h"
#include "../convolution.h"

#define NUMBER_OF_ELEMENTS (1 << BOX_WIDTH)
#define DT_SIZE (NUMBER_OF_ELEMENTS*NUMBER_OF_ELEMENTS)

static int correlation_table_even[DT_SIZE];
static int correlation_table_odd[DT_SIZE];

/*
 * Count number of input correlations equal to v in column corresponding to output correlation b
 */
void
count_input(mpz_t c, uint16_t b, int v, int index)
{
	mpz_set_ui(c, 0);
	for (int i = 0; i < NUMBER_OF_ELEMENTS; i++) {
		if ((index % 2 == 0) && abs(correlation_table_even[NUMBER_OF_ELEMENTS*i+b]) == v) {
			mpz_add_ui(c, c, 1);
		}

		if ((index % 2 != 0) && abs(correlation_table_odd[NUMBER_OF_ELEMENTS*i+b]) == v) {
			mpz_add_ui(c, c, 1);
		}
	}
}

/*
 * Count number of output correlations equal to v in row corresponding to input correlation a
 */
void
count_output(mpz_t c, uint16_t a, int v, int index)
{
	mpz_set_ui(c, 0);
	for (int i = 0; i < NUMBER_OF_ELEMENTS; i++) {
		if ((index % 2 == 0)  && abs(correlation_table_even[NUMBER_OF_ELEMENTS*a+i]) == v) {
			mpz_add_ui(c, c, 1);
		}

		if ((index % 2 != 0)  && abs(correlation_table_odd[NUMBER_OF_ELEMENTS*a+i]) == v) {
			mpz_add_ui(c, c, 1);
		}
	}
}

int
main(void)
{
	Table *superbox_correlations = table_create();
	Table *sbox_correlations[MAX_BOX_WEIGHT];	

	fill_correlation_table(correlation_table_even, sbox_even, BOX_WIDTH);
	fill_correlation_table(correlation_table_odd, sbox_odd, BOX_WIDTH);

	mpz_t row, col, val;
	mpz_init_set_ui(row, 0);
	mpz_init_set_ui(col, 0);
	mpz_init(val);
	// Fix the mask b at the output of mix_columns. 
	for (uint32_t b = 0; b <= UINT16_MAX; b++) {
		// This mask is connected through the transpose of mix_columns to a mask c at the input.
		// This ensures that the correlation over the linear layer equals 1.
		uint16_t a = mix_columns_transposed(b);	

		// Create a histogram for every sbox before and after mix_columns.
		for (int i = 0; i < MAX_BOX_WEIGHT; i++) {
			sbox_correlations[i] = table_create();
		}

		// Fill the histograms of the sboxes
		for (int i = 0; i < 4; i++) {
			// 2*(4 - log2(16))
			mpz_set_ui(row, 0);
			count_output(val, (a >> 4*i) & 0xF, 16, i);
			table_insert_and_merge(sbox_correlations[4+i], row, col, val, &mpz_add);
			count_input(val, (b >> 4*i) & 0xF, 16, i);
			table_insert_and_merge(sbox_correlations[i], row, col, val, &mpz_add);

			// 2*(4 - log2(4))
			mpz_set_ui(row, 4);
			count_output(val, (a >> 4*i) & 0xF, 4, i);
			table_insert_and_merge(sbox_correlations[4+i], row, col, val, &mpz_add);
			count_input(val, (b >> 4*i) & 0xF, 4, i);
			table_insert_and_merge(sbox_correlations[i], row, col, val, &mpz_add);

			// 2*(4 - log2(8))
			mpz_set_ui(row, 2);
			count_output(val, (a >> 4*i) & 0xF, 8, i);
			table_insert_and_merge(sbox_correlations[4+i], row, col, val, &mpz_add);
			count_input(val, (b >> 4*i) & 0xF, 8, i);
			table_insert_and_merge(sbox_correlations[i], row, col, val, &mpz_add);
		}

		// Convolute the histograms of the sboxes to get the histogram of the superbox.
		Table *r = table_copy(sbox_correlations[0]);
		for (int i = 1; i < MAX_BOX_WEIGHT; i++) {
			Table *tmp = convolve(r, sbox_correlations[i], 0, 0, differential_combiners);	
			table_destroy(r);
			r = tmp;
		}	

		for (HeadNode *node = r->head; node != NULL; node = node->next_row) {
			table_insert_and_merge(superbox_correlations, node->node.row, node->node.col, node->node.val, &mpz_add);
		}

		for (int i = 0; i < MAX_BOX_WEIGHT; i++) {
			table_destroy(sbox_correlations[i]);
		}

		table_destroy(r);
	}
	mpz_clears(row, col, val, NULL);

	table_print("data/saturnin-linear-trail-superbox.txt", superbox_correlations);
	// To get the histogram of the full state, we convolve the histograms of the superboxes.
	Table *full_correlations = convolve_tables(superbox_correlations, NUMBER_OF_SUPERBOXES, 0, 0, differential_combiners);
	table_print("data/saturnin-linear-trail-full.txt", full_correlations);

	table_destroy(superbox_correlations);
	table_destroy(full_correlations);

	return 0;
}
