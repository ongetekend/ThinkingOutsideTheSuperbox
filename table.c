#include "table.h"

static HeadNode * create_node(const mpz_t, const mpz_t, const mpz_t);
	
Table *
table_create(void)
{
	Table *table = (Table *) malloc(sizeof(Table));
	if (table != NULL) {
		table->head = NULL;
		table->number_of_rows = 0;
	}

	return table;
}

Table *
table_copy(Table * const src)
{
	Table *dst = table_create(); 

	for (HeadNode *current_row = src->head; current_row != NULL; current_row = current_row->next_row) {
		for (Node *current_col = &current_row->node; current_col != NULL; current_col = current_col->next_col) {
			table_insert_and_merge(dst, current_col->row, current_col->col, current_col->val, &mpz_add);
		}
	}

	return dst;
}

void
table_destroy(Table *table)
{
	HeadNode *current_row = table->head;
	while(current_row != NULL) {
		HeadNode *node_ptr = current_row;
		current_row = current_row->next_row;
		while (node_ptr != NULL) {
			HeadNode *tmp;
			tmp = (HeadNode *) node_ptr->node.next_col;
			mpz_clears(node_ptr->node.row, node_ptr->node.col, node_ptr->node.val, NULL);
			free(node_ptr);
			node_ptr = tmp;
		}
	}
	free(table);
}

void
table_print(const char *name, Table * const table)
{
	FILE *fp = fopen(name, "w+");
	if (fp == NULL) {
		exit(0);
	}

	for (HeadNode *current_row = table->head; current_row != NULL; current_row = current_row->next_row) {
		for (Node *current_col = &current_row->node; current_col != NULL; current_col = current_col->next_col) {
			gmp_fprintf(fp, "%Zd", current_col->row);
			gmp_fprintf(fp, ",%Zd", current_col->col);
			gmp_fprintf(fp, ",%Zd", current_col->val);
			fprintf(fp, "\n");
		}
	}

	fclose(fp);
}

size_t
table_size(Table * const table)
{
	size_t count = 0;

	for (HeadNode *current_row = table->head; current_row != NULL; current_row = current_row->next_row) {
		for (Node *current_col = &current_row->node; current_col != NULL; current_col = current_col->next_col) {
			++count;
		}
	}

	return count;
}

Node *
table_search(Table * const table, const mpz_t row, const mpz_t col)
{
	Node *result = NULL; 
	for (HeadNode *current_row = table->head; current_row != NULL; current_row = current_row->next_row) {
		if (mpz_cmp(current_row->node.row, row) == 0) {
			for (Node *current_col = &current_row->node; current_col != NULL; current_col = current_col->next_col) {
				if (mpz_cmp(current_col->col, col) == 0) {
					result = current_col;
					break;
				}
			}
		}
	}

	return result;
}

static HeadNode *
create_node(const mpz_t row, const mpz_t col, const mpz_t val)
{
	HeadNode *new_node = (HeadNode *) malloc(sizeof(HeadNode));
	if (new_node != NULL) {
		mpz_init_set(new_node->node.row, row);
		mpz_init_set(new_node->node.col, col);
		mpz_init_set(new_node->node.val, val);
		new_node->node.next_col = NULL;	
		new_node->next_row = NULL;
	}

	return new_node;
}

void
table_insert_and_merge(Table * const table, const mpz_t row, const mpz_t col, const mpz_t val, void (*f)(mpz_t, const mpz_t, const mpz_t))
{
	HeadNode *previous_row = NULL;
	HeadNode *current_row = table->head; 
	HeadNode *new_node = NULL;

	int cmp = 0;
	int cmp_is_valid = 0;
	while (current_row != NULL) {
		cmp = mpz_cmp(current_row->node.row, row);
		cmp_is_valid = 1;
		if (cmp >= 0) {
			break;
		}
		previous_row = current_row;
		current_row = current_row->next_row;
	}

	// current_row == NULL or the row value associated with current is bigger than or equal to row.
	if (current_row == table->head && (cmp > 0 || cmp_is_valid == 0)) {
		new_node = create_node(row, col, val);	
		if (new_node == NULL) goto fail;
		table->head = new_node;
		++table->number_of_rows;
		new_node->next_row = current_row;
	} else if (current_row == NULL || cmp > 0) {
		// current_row is either at the end of the list or current_row is larger than row
		new_node = create_node(row, col, val);	
		if (new_node == NULL) goto fail;
		previous_row->next_row = new_node;
		new_node->next_row = current_row;
		++table->number_of_rows;
	} else {
		// rows are equal, so we scan the columns			
		// current_row != NULL, so we can safely dereference
		Node *previous_col = NULL;
		Node *current_col = &current_row->node; 

		while (current_col != NULL) {
			cmp = mpz_cmp(current_col->col, col);
			if (cmp >= 0) {
				break;
			}
			previous_col = current_col;
			current_col = current_col->next_col;
		}

		if (current_col == &current_row->node && cmp > 0) {
			new_node = create_node(row, col, val);	
			if (new_node == NULL) goto fail;
			if (current_row == table->head) { 
				table->head = new_node;
			} else {
				previous_row->next_row = new_node;
			}
			new_node->next_row = current_row->next_row;
			new_node->node.next_col = current_col;
		} else if (current_col == NULL || cmp > 0) {
			// current_col is either at the end of the list or current_col is larger than col
			new_node = create_node(row, col, val);	
			if (new_node == NULL) goto fail;
			previous_col->next_col = (Node *) new_node;
			new_node->node.next_col = current_col;
		} else {
			// (row, col) pairs are equal, so we merge the two nodes, combining their values with f
			(*f)(current_col->val, current_col->val, val);
		}
	}
fail:
	return;
}
