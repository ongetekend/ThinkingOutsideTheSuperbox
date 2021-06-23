#include "saturnin-common.h"
#include "../convolution.h"

int
main(void)
{
	Table_fixed *number_of_differentials_superbox = table_fixed_create(UINT16_MAX+2);
	mpz_t val;
	mpz_init(val);
	mpz_set_ui(val, 1);
	unsigned int *table = (unsigned int *) malloc((UINT16_MAX+1)*sizeof(int));
	for (uint32_t a = 0; a <= UINT16_MAX; a++) {
		memset(table, 0, sizeof(int)*(UINT16_MAX+1));
		for (uint32_t x = 0; x <= UINT16_MAX; x++) {
			uint16_t left = sbox_layer(x);
			uint16_t right = sbox_layer(x^a);	
			left = mix_columns(left);
			right = mix_columns(right);
			left = sbox_layer(left);
			right = sbox_layer(right);
			table[left^right]++;
		}

		// iterate over a row of the table and increment relevant entries of number_of_differentials table
		for (int i = 0; i <= UINT16_MAX; i++) {
			if (table[i] == 0) continue;

			double differential_weight = (double) (1U << 16);
			differential_weight /= table[i];
			differential_weight = log2(differential_weight);
			differential_weight = round(differential_weight);
			table_fixed_insert_and_merge(number_of_differentials_superbox, (unsigned long) differential_weight, val, &mpz_add);
		}
	}	
	free(table);
	mpz_clear(val);

	table_fixed_print("data/saturnin-differential-superbox.txt", number_of_differentials_superbox);

	Table_fixed *number_of_differentials_full = convolve_tables_fixed(number_of_differentials_superbox, NUMBER_OF_SUPERBOXES, 0, 0);

	table_fixed_print("data/saturnin-differential-full.txt", number_of_differentials_full);

	table_fixed_destroy(number_of_differentials_superbox);
	table_fixed_destroy(number_of_differentials_full);
}
