#include "convolution.h"

Table_fixed *
convolve_tables_fixed(Table_fixed *t, uint8_t w, int lower_bound, int upper_bound) 
{
	// We search for the most significant bit
	int i = 0;
	for (int j = 7; j >= 0; j--) {
		if ((w & (1 << j)) != 0) {
			i = j;
			break;
		}
	}

	// r = t
	Table_fixed *r = table_fixed_copy(t);
	for (int j = i-1; j >= 0; j--) {
		// r = r*r
		Table_fixed *tmp  = convolve_fixed(r, r, lower_bound, upper_bound);
		table_fixed_destroy(r);
		r = tmp;
		if ((w & (1 << j)) != 0) {
			// r = r*t
			tmp = convolve_fixed(r, t, lower_bound, upper_bound);
			table_fixed_destroy(r);
			r = tmp;
		}	
	}

	return r;
}

Table *
convolve_tables(Table *t, uint8_t w, int lower_bound, int upper_bound, void (*f[3])(mpz_t, const mpz_t, const mpz_t)) 
{
	// We search for the most significant bit
	int i = 0;
	for (int j = 7; j >= 0; j--) {
		if ((w & (1 << j)) != 0) {
			i = j;
			break;
		}
	}

	// r = t
	Table *r = table_copy(t);
	for (int j = i-1; j >= 0; j--) {
		// r = r*r
		Table *tmp  = convolve(r, r, lower_bound, upper_bound, f);
		table_destroy(r);
		r = tmp;
		if ((w & (1 << j)) != 0) {
			// r = r*t
			tmp = convolve(r, t, lower_bound, upper_bound, f);
			table_destroy(r);
			r = tmp;
		}	
	}

	return r;
}

Table_fixed *
convolve_fixed(Table_fixed *t1, Table_fixed *t2, int lower_bound, int upper_bound)
{
	Table_fixed *tmp_table = table_fixed_create(t1->size+t2->size-1);

	mpz_t val_prod;
	mpz_init(val_prod);
	for (size_t i1 = 0; i1 < t1->size; ++i1) {
		if (t1->head[i1] == NULL) continue;
		for (size_t i2 = 0; i2 < t2->size; ++i2) {
			if (t2->head[i2] == NULL) continue;

			if (upper_bound != 0 && (i1+i2 > upper_bound)) continue;
			if (lower_bound != 0 && (i1+i2 < lower_bound)) continue;

			mpz_mul(val_prod, t1->head[i1], t2->head[i2]);
			table_fixed_insert_and_merge(tmp_table, i1+i2, val_prod, &mpz_add);
		}
	}
	mpz_clear(val_prod);

	return tmp_table;
}

Table *
convolve(Table *t1, Table *t2, int lower_bound, int upper_bound, void (*f[3])(mpz_t, const mpz_t, const mpz_t))
{
	Table *tmp_table = table_create();

	mpz_t row_prod, col_prod, val_prod;
	mpz_inits(row_prod, col_prod, val_prod, NULL);
	for (HeadNode *r1 = t1->head; r1 != NULL; r1 = r1->next_row) {
		for (HeadNode *r2 = t2->head; r2 != NULL; r2 = r2->next_row) {
			(*f[0])(row_prod, r1->node.row, r2->node.row);
			if (upper_bound != 0 && mpz_cmp_ui(row_prod, upper_bound) > 0) {
				continue;
			}

			if (lower_bound != 0 && mpz_cmp_ui(row_prod, lower_bound) < 0) {
				continue;
			}

			for (Node *c1 = &r1->node; c1 != NULL; c1 = c1->next_col) {
				for (Node *c2 = &r2->node; c2 != NULL; c2 = c2->next_col) {
					(*f[1])(col_prod, c1->col, c2->col);
					(*f[2])(val_prod, c1->val, c2->val);
					table_insert_and_merge(tmp_table, row_prod, col_prod, val_prod, &mpz_add);
				}
			}
		}
	}
	mpz_clears(row_prod, col_prod, val_prod, NULL);

	return tmp_table;
}
