#include "saturnin-common.h"
#include "../global-common.h"
#include "../convolution.h"
#include <math.h>
#include <stdbool.h>
#include <omp.h>

#define NUMBER_OF_ELEMENTS (1 << BOX_WIDTH)
#define DT_SIZE (NUMBER_OF_ELEMENTS*NUMBER_OF_ELEMENTS)
#define NUM_THREADS 36
#define BOUND 36

static int difference_table_even[DT_SIZE];
static int difference_table_odd[DT_SIZE];

/*
 * Count number of input differences equal to v in column corresponding to output difference b
 */
void
count_input(mpz_t c, const uint16_t b, const int v, int index)
{
	mpz_set_ui(c, 0);
	for (int i = 0; i < 16; i++) {
		if ((index % 2 == 0) && difference_table_even[16*i+b] == v) {
			mpz_add_ui(c, c, 1);
		}

		if ((index % 2 != 0) && difference_table_odd[16*i+b] == v) {
			mpz_add_ui(c, c, 1);
		}
	}
}

/*
 * Count number of output differences equal to v in row corresponding to input difference a
 */
void
count_output(mpz_t c, const uint16_t a, const int v, int index)
{
	mpz_set_ui(c, 0);
	for (int i = 0; i < 16; i++) {
		if ((index % 2 == 0) && difference_table_even[16*a+i] == v) {
			mpz_add_ui(c, c, 1);
		}

		if ((index % 2 != 0) && difference_table_odd[16*a+i] == v) {
			mpz_add_ui(c, c, 1);
		}
	}
}

/*
 * b is an encoding of a 4x4 rectangle of bits as a flat 16-bit array where (i, j) is mapped to (4*i+j).
 * This function swaps entries i and j.
 */
uint16_t 
swap(const uint16_t b, const unsigned long i, const unsigned long j)
{
	uint16_t x = ((b >> i) ^ (b >> j)) & 1U; // XOR temporary
	return b ^ ((x << i) | (x << j));
}

/*
 * An activity pattern is represented by a 4x4 rectangle where the position of an activity bit corresponding to a nibble with a given index
 * is as follows: 
 * 3  2   1  0 
 * 6  5   4  7
 * 9  8  11 10
 * 12 15 14 13
 * For example, the activity bit of a nibble with index 4 is in position (1,1) in the rectangle. 
 * The actual entries consist of 0's and 1's, indicating whether a nibble is active or not.
 * In this representation a row in the rectangle corresponds to the input of mix columns and a column to
 * the input of the inverse of mix columns.
 *
 * The rectangle is stored as 16 bits, where (i, j) is mapped to (4*i+j)
 *
 * This function converts this representation to the sequential one:
 * 0   1  2  3
 * 4   5  6  7
 * 8   9 10 11
 * 12 13 14 15
 */
uint16_t
convert(const uint16_t b)
{
	uint16_t r = swap(b, 0, 3);
	r = swap(r, 1, 2);
	r = swap(r, 4, 6);
	r = swap(r, 8, 9);
	r = swap(r, 10, 11);
	return swap(r, 13, 15);
}

int 
main(void)
{
	// We know that the number of candidate differences is 349, because we used the code below
	// to compute and print them. 
	uint16_t candidate_differences[349];

	// Consider a single slice, consisting of 16 nibbles.
	// Associated with this are 2^16 possible activity patterns. Each activity pattern is represented by a rectangle as 
	// described above where rows correspond to mix columns input and columns to inverse mix columns input.
	// We step through each activity pattern. 
	for (long x = 0, k = 0; x <= UINT16_MAX; x++) {
		long min_box_weight = hamming_weight16(x);
		// Consider each column
		for (long j = 0; j < 4; j++) {
			long active = 0;
			for (long i = 0; i < 4; i++) {
				if ((x & (1U << (4*i+j))) != 0) {
					++active; 
				}
			}
			if (active != 0) {
				// Mix columns ensures at least 5 active nibbles
				min_box_weight += 5-active;
			}
		}

		// Consider each row
		for (long i = 0; i < 4; i++) {
			long active = 0;
			for (long j = 0; j < 4; j++) {
				if ((x & (1U << (4*i+j))) != 0) {
					++active; 
				}
			}
			if (active != 0) {
				// Mix columns inverse  ensures at least 5 active nibbles
				min_box_weight += 5-active;
			}
		}

		// Each active nibble contributes at least a weight of 2, so we check whether this lower bound is below 
		// the upper bound that we set.
		if (min_box_weight <= BOUND/2) {
			// convert to sequential representation.
			candidate_differences[k] = convert(x);
			k++;
		}
	}

	fill_difference_table(difference_table_even, sbox_even, BOX_WIDTH);
	fill_difference_table(difference_table_odd, sbox_odd, BOX_WIDTH);

	Table_fixed *total_weight = table_fixed_create(1+BOUND);

	Table_fixed *weights[349];
       	for (long i = 0; i < 349; i++) {
		weights[i] = table_fixed_create(1+BOUND);
	}

	// The number of iterations of the main loop is bounded from above by 2^9
	omp_set_num_threads(NUM_THREADS);	
	#pragma omp parallel for schedule(dynamic)
		// Consider each candidate difference
		for (long i = 0; i < 349; i++) {
			mpz_t val;
			mpz_init(val);
			uint16_t x = candidate_differences[i];
			int box_weight_a = hamming_weight16(x);
			// The number of iterations of loop is bounded from above by 2^16, since w <= 4
			// Step over all possible differences after the second Sbox-layer,
			// having box-activity pattern equal to x
			for (uint64_t a = 0; a < (uint64_t) pow(2, 4*box_weight_a); a++) {
				if (box_weight(a, box_weight_a, 4) != box_weight_a) {
					continue;
				}

				// Build the difference before the third Sbox-layer
				uint64_t state1 = 0;
				for (long j = 0, m = 0; j < 16 && m < box_weight_a; j++) {
					if ((x & (1U << j)) != 0) {
						state1 |= ((a >> 4*m) & 0xf) << 4*j;
						m++;
					}
				}	

				state1 = permute_slice(state1);
				state1 = mix_columns_slice(state1);
				state1 = permute_slice_inverse(state1);

				// Compute the convolution of the histograms of the third Sbox-layer
				Table_fixed *bases[32];	
				for (long j = 16; j < 32; j++) {
					bases[j] = table_fixed_create(4);
				}

				for (long j = 0; j < 16; j++) {
					count_output(val, (state1 >> 4*j) & 0xf, 16, j);
					table_fixed_insert_and_merge(bases[16+j], 0, val, &mpz_add);

					count_output(val, (state1 >> 4*j) & 0xf, 4, j);
					table_fixed_insert_and_merge(bases[16+j], 2, val, &mpz_add);

					count_output(val, (state1 >> 4*j) & 0xf, 2, j);
					table_fixed_insert_and_merge(bases[16+j], 3, val, &mpz_add);
				}

				Table_fixed *convolved_state1 = table_fixed_copy(bases[16]);
				for (long j = 17; j < 32; j++) {
					Table_fixed *tmp = convolve_fixed(convolved_state1, bases[j], 0, BOUND);	
					table_fixed_destroy(convolved_state1);
					convolved_state1 = tmp;
				}	

				// For a fixed difference AFTER the second Sbox-layer, consider all the differences BEFORE the second Sbox-layer, 
				// the number of which is bounded from above by 2^12 (empirically verified)
				for (uint64_t b = 0; b < (uint64_t) pow(2, 4*box_weight_a); b++) {
					bool valid = true;
					long differential_weight_middle = 0; 
					for (long j = 0, m = 0; j < 16 && m < box_weight_a; j++) {
						if ((x & (1U << j)) != 0) {
							long row_index = (b >> 4*m) & 0xf;
							long column_index = (a >> 4*m) & 0xf; 
							long count;
							if (j % 2 == 0) {
								count = difference_table_even[16*row_index+column_index];
							} else {
								count = difference_table_odd[16*row_index+column_index];
							}
							if (count == 0) {
								valid = false;
								break;
							}
							differential_weight_middle += log2((double) NUMBER_OF_ELEMENTS / count);
							m++;
						}
					}

					if (valid) {
						uint64_t state0 = 0;
						for (long j = 0, m = 0; j < 16 && m < box_weight_a; j++) {
							if ((x & (1U << j)) != 0) {
								state0 |= ((b >> 4*m) & 0xf) << 4*j;
								m++;
							}
						}	

						state0 = mix_columns_slice_inverse(state0);

						// Filter anything that will exceed the bound to save us the work of doing the convolution	
						if (box_weight_a + box_weight(state0, 16, BOX_WIDTH) + box_weight(state1, 16, BOX_WIDTH) > BOUND/2) {
							continue;
						}

						// at this point, we can do the convolution
						// use b1 and b2 to determine active sboxes and
						// convolve
						for (long j = 0; j < 16; j++) {
							/* differential weight 0, 2, and 3 */
							bases[j] = table_fixed_create(4);
						}

						for (long j = 0; j < 16; j++) {
							count_input(val, (state0 >> 4*j) & 0xf, 16, j);
							table_fixed_insert_and_merge(bases[j], 0, val, &mpz_add);

							count_input(val, (state0 >> 4*j) & 0xf, 4, j);
							table_fixed_insert_and_merge(bases[j], 2, val, &mpz_add);

							count_input(val, (state0 >> 4*j) & 0xf, 2, j);
							table_fixed_insert_and_merge(bases[j], 3, val, &mpz_add);
						}

						Table_fixed *convolved_state_total = table_fixed_copy(convolved_state1);
						for (long j = 0; j < 16; j++) {
							Table_fixed *tmp = convolve_fixed(convolved_state_total, bases[j], 0, BOUND-differential_weight_middle);	
							table_fixed_destroy(convolved_state_total);
							convolved_state_total = tmp;
						}	

						for (size_t j = 0; j <= BOUND-differential_weight_middle; j++) {
								table_fixed_insert_and_merge(weights[i], 
										j+differential_weight_middle, 
										convolved_state_total->head[j], 
										&mpz_add);
						}

						for (long j = 0; j < 16; j++) {
							table_fixed_destroy(bases[j]);
						}

						table_fixed_destroy(convolved_state_total);
					}
				}

				for (long j = 16; j < 32; j++) {
					table_fixed_destroy(bases[j]);
				}
						
				table_fixed_destroy(convolved_state1);
			}
			mpz_clear(val);

			char filename[60];
			sprintf(filename, "data/saturnin-differential-trail-three-rounds_%04x.txt", x);
			table_fixed_print(filename, weights[i]);
		}

	for (long i = 0; i <= BOUND; i++) {
		for (long j = 0; j < 349; j++) {
			table_fixed_insert_and_merge(total_weight, i, weights[j]->head[i], &mpz_add);
		}
	}

	table_fixed_print("data/saturnin-differential-trail-three-rounds_total.txt", total_weight);

	for (long i = 0; i < 349; i++) {
		table_fixed_destroy(weights[i]);
	}

	table_fixed_destroy(total_weight);

	return 0;
}
