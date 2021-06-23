#include "spongent-common.h"
#include "../convolution.h"

uint8_t box_pattern(uint16_t);

uint64_t box_pattern_cardinalities[256];

uint8_t
box_pattern(uint16_t a)
{
	uint8_t pattern = 0;
	uint16_t b = permute(a);

	for (int i = 0; i < 4; i++) {
		if ((a & 0xf) != 0) {
			pattern |= 1 << i;
		}
		if ((b & 0xf) != 0) {
			pattern |= 1 << (4 + i);
		}
		a >>= 4;
		b >>= 4;
	}

	return pattern;
}

int
main(void)
{
	Table *box_weights_subgroup = table_create();
	Table *box_partition_subgroup = table_create();

	mpz_t row, col, val;
	mpz_init(row);
	mpz_init_set_ui(col, 0);
	mpz_init_set_ui(val, 1);

	for (uint32_t a = 0; a <= UINT16_MAX; a++) {
		box_pattern_cardinalities[box_pattern(a)]++;
		mpz_set_ui(row, box_weight(a, NUMBER_OF_BOXES, BOX_WIDTH) + box_weight(permute(a), NUMBER_OF_BOXES, BOX_WIDTH));
		table_insert_and_merge(box_weights_subgroup, row, col, val, &mpz_add);
	}	

	mpz_set_ui(val, 1);
	for (int i = 0; i < 256; i++) {
		uint64_t b = box_pattern_cardinalities[i];
		if (b != 0) {
			mpz_set_ui(col, b);
			mpz_set_ui(row, hamming_weight8(i));
			table_insert_and_merge(box_partition_subgroup, row, col, val, &mpz_add);
		}
	}
	mpz_clears(row, col, val, NULL);

	Table *box_weights_full = convolve_tables(box_weights_subgroup, NUMBER_OF_SUPERBOXES, 0, 0, differential_combiners);	
	Table *box_partition_full = convolve_tables(box_partition_subgroup, NUMBER_OF_SUPERBOXES, 0, 50, differential_combiners);	

	table_print("data/spongent-box-superbox.txt", box_weights_subgroup);
	table_print("data/spongent-box-full.txt", box_weights_full);
	table_print("data/spongent-partition-superbox.txt", box_partition_subgroup);
	table_print("data/spongent-partition-full.txt", box_partition_full);

	table_destroy(box_weights_subgroup);
	table_destroy(box_partition_subgroup);
	table_destroy(box_weights_full);
	table_destroy(box_partition_full);

	return 0;
}
