#include "spongent-common.h"
#include "../convolution.h"
#include "../walsh-transform.h"

#define SIZE (UINT16_MAX+1)

int
main(void)
{	
	mpz_t val;
	mpz_init(val);
	Table_fixed *superbox_counts = table_fixed_create(SIZE+1);
	Table_fixed *correlation_table_superbox;
	for (uint32_t v = 0; v < SIZE; v++) {
		correlation_table_superbox = table_fixed_create(SIZE);
		for (uint32_t x = 0; x < SIZE; x++) {
			uint16_t y = sbox_layer(x);
			y = permute(y);	
			y = sbox_layer(y);
			mpz_set_si(val, dot_prod(y, v) ? -1 : 1);
			table_fixed_insert_and_merge(correlation_table_superbox, x, val, &mpz_add);
		}

		// The transform takes around 16*2^16 = 2^20 steps.
		// and uses around 2^20 bits
		walsh_transform(correlation_table_superbox->head, correlation_table_superbox->size);

		mpz_set_ui(val, 1);
		for (int i = 0; i < SIZE; i++) {
			mpz_abs(correlation_table_superbox->head[i], correlation_table_superbox->head[i]);
			table_fixed_insert_and_merge(superbox_counts, mpz_get_ui(correlation_table_superbox->head[i]), val, &mpz_add); 
		}

		table_fixed_destroy(correlation_table_superbox);
	}
	mpz_clear(val);
	table_fixed_print("data/spongent-correlation-superbox.txt", superbox_counts);
	table_fixed_destroy(superbox_counts);
}
