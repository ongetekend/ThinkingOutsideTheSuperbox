#include "../convolution.h"
#include "../global-common.h"

#define WIDTH 384
#define BOX_WIDTH 4
#define NUMBER_OF_BOXES (WIDTH / BOX_WIDTH)
#define NUMBER_OF_BOXES_IN_SUPERBOX 4
#define NUMBER_OF_SUPERBOXES (NUMBER_OF_BOXES / NUMBER_OF_BOXES_IN_SUPERBOX)

int
main(void)
{
	Table *box_weights_superbox = table_create();

	mpz_t row, col, val;
	mpz_init(row);
	mpz_init_set_ui(col, 0);
	mpz_init_set_ui(val, 1);

	for (uint32_t a = 0; a <= (1U << (NUMBER_OF_BOXES_IN_SUPERBOX*BOX_WIDTH))-1; a++) {
		mpz_set_ui(row, 2*box_weight(a, NUMBER_OF_BOXES, BOX_WIDTH));
		table_insert_and_merge(box_weights_superbox, row, col, val, &mpz_add);
	}	

	mpz_clears(row, col, val, NULL);

	Table *box_weights_full = convolve_tables(box_weights_superbox, NUMBER_OF_SUPERBOXES, 0, 0, differential_combiners);	

	table_print("data/identity-box-superbox.txt", box_weights_superbox);
	table_print("data/identity-box-full.txt", box_weights_full);

	table_destroy(box_weights_superbox);
	table_destroy(box_weights_full);

	return 0;
}
