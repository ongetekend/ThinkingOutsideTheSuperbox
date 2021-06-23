#include "spongent-common.h"
#include "../global-common.h"
#include "../convolution.h"

#define NUMBER_OF_ELEMENTS (1 << BOX_WIDTH)
#define DT_SIZE (NUMBER_OF_ELEMENTS*NUMBER_OF_ELEMENTS)

int correlation_table[DT_SIZE];

/*
 * Count number of input correlations equal to v in column corresponding to output correlation b
 */
void
count_input(mpz_t c, uint16_t b, int v)
{
	mpz_set_ui(c, 0);
	for (int i = 0; i < NUMBER_OF_ELEMENTS; i++) {
		if (abs(correlation_table[NUMBER_OF_ELEMENTS*i+b]) == v) {
			mpz_add_ui(c, c, 1);
		}
	}
}

/*
 * Count number of output correlations equal to v in row corresponding to input correlation a
 */
void
count_output(mpz_t c, uint16_t a, int v)
{
	mpz_set_ui(c, 0);
	for (int i = 0; i < NUMBER_OF_ELEMENTS; i++) {
		if (abs(correlation_table[NUMBER_OF_ELEMENTS*a+i]) == v) {
			mpz_add_ui(c, c, 1);
		}
	}
}

int
main(void)
{
	Table *weights_superbox = table_create();
	Table *bases[MAX_BOX_WEIGHT];	

	fill_correlation_table(correlation_table, sbox, BOX_WIDTH);

	mpz_t row, col, val;
	mpz_init_set_ui(row, 0);
	mpz_init_set_ui(col, 0);
	mpz_init(val);
	for (uint32_t b = 0; b <= UINT16_MAX; b++) {
		// The transpose of a permutation matrix is the inverse.
		// Computing a in this way ensures that the correlation over the linear layer equals 1.
		uint16_t a = permute_inverse(b);	

		for (int i = 0; i < MAX_BOX_WEIGHT; i++) {
			bases[i] = table_create();
		}

		for (int i = 0; i < 4; i++) {
			// 2*(4 - log_2(16))
			mpz_set_ui(row, 0);
			count_output(val, (b >> 4*i) & 0xF, 16);
			table_insert_and_merge(bases[4+i], row, col, val, &mpz_add);
			count_input(val, (a >> 4*i) & 0xF, 16);
			table_insert_and_merge(bases[i], row, col, val, &mpz_add);

			// 2*(4 - log_2(4))
			mpz_set_ui(row, 4);
			count_output(val, (b >> 4*i) & 0xF, 4);
			table_insert_and_merge(bases[4+i], row, col, val, &mpz_add);
			count_input(val, (a >> 4*i) & 0xF, 4);
			table_insert_and_merge(bases[i], row, col, val, &mpz_add);

			// 2*(4 - log_2(8))
			mpz_set_ui(row, 2);
			count_output(val, (b >> 4*i) & 0xF, 8);
			table_insert_and_merge(bases[4+i], row, col, val, &mpz_add);
			count_input(val, (a >> 4*i) & 0xF, 8);
			table_insert_and_merge(bases[i], row, col, val, &mpz_add);
		}

		Table *r = table_copy(bases[0]);
		for (int i = 1; i < MAX_BOX_WEIGHT; i++) {
			Table *tmp = convolve(r, bases[i], 0, 0, differential_combiners);	
			table_destroy(r);
			r = tmp;
		}	

		for (HeadNode *node = r->head; node != NULL; node = node->next_row) {
			table_insert_and_merge(weights_superbox, node->node.row, node->node.col, node->node.val, &mpz_add);
		}

		for (int i = 0; i < MAX_BOX_WEIGHT; i++) {
			table_destroy(bases[i]);
		}

		table_destroy(r);
	}
	mpz_clears(row, col, val, NULL);

	table_print("data/spongent-linear-trail-superbox.txt", weights_superbox);
	Table *weights_full = convolve_tables(weights_superbox, NUMBER_OF_SUPERBOXES, 0, 0, differential_combiners);
	table_print("data/spongent-linear-trail-full.txt", weights_full);

	table_destroy(weights_superbox);
	table_destroy(weights_full);

	return 0;
}
