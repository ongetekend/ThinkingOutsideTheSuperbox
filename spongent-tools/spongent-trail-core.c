#include "spongent-common.h"
#include "../global-common.h"
#include "../convolution.h"

#define NUMBER_OF_ELEMENTS (1 << BOX_WIDTH)
#define DT_SIZE (NUMBER_OF_ELEMENTS*NUMBER_OF_ELEMENTS)

int difference_table[DT_SIZE];

/*
 * Count number of input differences equal to v in column corresponding to output difference b
 */
void
count_input(mpz_t c, uint16_t b, int v)
{
	mpz_set_ui(c, 0);
	for (int i = 0; i < NUMBER_OF_ELEMENTS; i++) {
		if (difference_table[NUMBER_OF_ELEMENTS*i+b] == v) {
			mpz_add_ui(c, c, 1);
		}
	}
}

/*
 * Count number of output differences equal to v in row corresponding to input difference a
 */
void
count_output(mpz_t c, uint16_t a, int v)
{
	mpz_set_ui(c, 0);
	for (int i = 0; i < NUMBER_OF_ELEMENTS; i++) {
		if (difference_table[NUMBER_OF_ELEMENTS*a+i] == v) {
			mpz_add_ui(c, c, 1);
		}
	}
}

int
main(void)
{
	Table *weights_superbox = table_create();
	Table *bases[MAX_BOX_WEIGHT];	

	fill_difference_table(difference_table, sbox, BOX_WIDTH);

	mpz_t row, col, val;
	mpz_init_set_ui(row, 0);
	mpz_init_set_ui(col, 0);
	mpz_init(val);
	for (uint32_t b = 0; b <= UINT16_MAX; b++) {
		uint16_t c = permute(b);	

		for (int i = 0; i < MAX_BOX_WEIGHT; i++) {
			bases[i] = table_create();
		}

		for (int i = 0; i < 4; i++) {
			mpz_set_ui(row, 0);
			count_input(val, (b >> 4*i) & 0xF, 16);
			table_insert_and_merge(bases[i], row, col, val, &mpz_add);
			count_output(val, (c >> 4*i) & 0xF, 16);
			table_insert_and_merge(bases[4+i], row, col, val, &mpz_add);

			mpz_set_ui(row, 2);
			count_input(val, (b >> 4*i) & 0xF, 4);
			table_insert_and_merge(bases[i], row, col, val, &mpz_add);
			count_output(val, (c >> 4*i) & 0xF, 4);
			table_insert_and_merge(bases[4+i], row, col, val, &mpz_add);

			mpz_set_ui(row, 3);
			count_input(val, (b >> 4*i) & 0xF, 2);
			table_insert_and_merge(bases[i], row, col, val, &mpz_add);
			count_output(val, (c >> 4*i) & 0xF, 2);
			table_insert_and_merge(bases[4+i], row, col, val, &mpz_add);
		}

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

	table_print("data/spongent-trail-core-superbox.txt", weights_superbox);

	Table *weights_full = convolve_tables(weights_superbox, NUMBER_OF_SUPERBOXES, 0, 0, differential_combiners);

	table_print("data/spongent-trail-core-full.txt", weights_full);

	table_destroy(weights_superbox);
	table_destroy(weights_full);

	return 0;
}
