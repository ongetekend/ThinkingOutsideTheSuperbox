#include "saturnin-common.h"
#include "../global-common.h"
#include "../convolution.h"

#define NUMBER_OF_ELEMENTS (1 << BOX_WIDTH)
#define DT_SIZE (NUMBER_OF_ELEMENTS*NUMBER_OF_ELEMENTS)

static int difference_table_even[DT_SIZE];
static int difference_table_odd[DT_SIZE];

/*
 * Count number of input differences equal to v in column corresponding to output difference b
 */
void
count_input(mpz_t c, uint16_t b, int v, int index)
{
	mpz_set_ui(c, 0);
	for (int i = 0; i < NUMBER_OF_ELEMENTS; i++) {
		if ((index % 2 == 0) && difference_table_even[NUMBER_OF_ELEMENTS*i+b] == v) {
			mpz_add_ui(c, c, 1);
		} 

		if ((index % 2 != 0) && difference_table_odd[NUMBER_OF_ELEMENTS*i+b] == v) {
			mpz_add_ui(c, c, 1);
		} 
	}
}

/*
 * Count number of output differences equal to v in row corresponding to input difference a
 */
void
count_output(mpz_t c, uint16_t a, int v, int index)
{
	mpz_set_ui(c, 0);
	for (int i = 0; i < NUMBER_OF_ELEMENTS; i++) {
		if ((index % 2 == 0) && difference_table_even[NUMBER_OF_ELEMENTS*a+i] == v) {
			mpz_add_ui(c, c, 1);
		}

		if ((index % 2 != 0) && difference_table_odd[NUMBER_OF_ELEMENTS*a+i] == v) {
			mpz_add_ui(c, c, 1);
		}
	}
}

int
main(void)
{
	Table *weights_superbox = table_create();
	Table *bases[MAX_BOX_WEIGHT];	

	fill_difference_table(difference_table_even, sbox_even, BOX_WIDTH);
	fill_difference_table(difference_table_odd, sbox_odd, BOX_WIDTH);

	mpz_t row, col, val;
	mpz_inits(row, col, val, NULL);
	// We fix the input difference to mix_columns
	for (uint32_t b = 0; b <= UINT16_MAX; b++) {
		// This determines the output difference to mix_columns
		uint16_t c = mix_columns(b);	

		// We create a trail weight histogram for the sboxes before and after mix_columns
		for (int i = 0; i < MAX_BOX_WEIGHT; i++) {
			/* weight 0, 2, and 3 */
			bases[i] = table_create();
		}

		// For fixed b and c, count the number of input differences having a given differential weight. 
		for (int i = 0; i < 4; i++) {
			mpz_set_ui(row, 0);
			count_input(val, (b >> 4*i) & 0xF, 16, i);
			table_insert_and_merge(bases[i], row, col, val, &mpz_add);
			count_output(val, (c >> 4*i) & 0xF, 16, i);
			table_insert_and_merge(bases[4+i], row, col, val, &mpz_add);

			mpz_set_ui(row, 2);
			count_input(val, (b >> 4*i) & 0xF, 4, i);
			table_insert_and_merge(bases[i], row, col, val, &mpz_add);
			count_output(val, (c >> 4*i) & 0xF, 4, i);
			table_insert_and_merge(bases[4+i], row, col, val, &mpz_add);

			mpz_set_ui(row, 3);
			count_input(val, (b >> 4*i) & 0xF, 2, i);
			table_insert_and_merge(bases[i], row, col, val, &mpz_add);
			count_output(val, (c >> 4*i) & 0xF, 2, i);
			table_insert_and_merge(bases[4+i], row, col, val, &mpz_add);
		}

		// Convolute the histograms of these sboxes.
		Table *r = table_copy(bases[0]);
		for (int i = 1; i < MAX_BOX_WEIGHT; i++) {
			Table *tmp = convolve(r, bases[i], 0, 0, differential_combiners);	
			table_destroy(r);
			r = tmp;
		}	

		// Step over all of the weights and increment the relevant entries in the histogram of the superbox.
		mpz_t val;
	        mpz_init_set_ui(val, 1);	
		for (HeadNode *row = r->head; row != NULL; row = row->next_row) {
			if (mpz_cmp_ui(row->node.val, 0) != 0) {
				table_insert_and_merge(weights_superbox, row->node.row, row->node.col, val, &mpz_add);
				break;
			}
		}

		for (int i = 0; i < MAX_BOX_WEIGHT; i++) {
			table_destroy(bases[i]);
		}

		table_destroy(r);
	}
	mpz_clears(row, col, val, NULL);

	// We convolve the histograms of the superboxes to get the histogram of the full state.
	Table *weights_full = convolve_tables(weights_superbox, NUMBER_OF_SUPERBOXES, 0, 0, differential_combiners);

	table_print("data/saturnin-trail-core-superbox.txt", weights_superbox);
	table_print("data/saturnin-trail-core-full.txt", weights_full);

	table_destroy(weights_superbox);
	table_destroy(weights_full);

	return 0;
}
