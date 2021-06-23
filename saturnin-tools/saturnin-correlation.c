#include "saturnin-common.h"
#include "../convolution.h"
#include "../walsh-transform.h"

#define SIZE (UINT16_MAX+1)

int
main(void)
{	
	mpz_t val;
	mpz_init(val);
	Table_fixed *superbox_counts = table_fixed_create(SIZE+1);
	Table_fixed *correlation_table_column;
	// Fix an output mask.
	for (uint32_t v = 0; v < SIZE; v++) {
		correlation_table_column = table_fixed_create(SIZE);
		// Compute a histogram storing pairs of the form (x, (-1)^(S(M(S(x))) dot v))
		for (uint32_t x = 0; x < SIZE; x++) {
			uint16_t y = sbox_layer(x);
			y = mix_columns(y);	
			y = sbox_layer(y);
			mpz_set_si(val, dot_prod(y, v) ? -1 : 1);
			table_fixed_insert_and_merge(correlation_table_column, x, val, &mpz_add);
		}

		// Compute the linear characteristics of S(M(S(x))) dot v using the Walsh transform on the table storing the above defined pairs.
		// The transform takes around 16*2^16 = 2^20 steps and has a data complexity of 2^20 bits
		walsh_transform(correlation_table_column->head, correlation_table_column->size);

		// Given the linear characteristics column, extract the counts
		mpz_set_ui(val, 1);
		for (int i = 0; i < SIZE; i++) {
			mpz_abs(correlation_table_column->head[i], correlation_table_column->head[i]);
			table_fixed_insert_and_merge(superbox_counts, mpz_get_ui(correlation_table_column->head[i]), val, &mpz_add); 
		}

		table_fixed_destroy(correlation_table_column);
	}
	mpz_clear(val);
	// To get the correlation squared from these entries, they need to be divided by 2^16 and squared.
	table_fixed_print("data/saturnin-correlation-superbox.txt", superbox_counts);
	table_fixed_destroy(superbox_counts);
}
