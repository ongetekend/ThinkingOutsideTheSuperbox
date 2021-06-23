#ifndef __TABLE_H__
#define __TABLE_H__

#include "global-common.h"

typedef struct node {
	mpz_t row;
	mpz_t col;	
	mpz_t val;
	struct node *next_col;
} Node;

typedef struct head_node {
	Node node;
	struct head_node *next_row;
} HeadNode;

typedef struct table {
	HeadNode *head;
	size_t number_of_rows;
} Table;

Table *table_create(void);
void table_destroy(Table *);
Table *table_copy(Table * const);
void table_print(const char *, Table * const);
size_t table_size(Table * const);
Node *table_search(Table * const, const mpz_t, const mpz_t);
void table_insert_and_merge(Table * const, const mpz_t, const mpz_t, const mpz_t, void (*f)(mpz_t, const mpz_t, const mpz_t));

#endif
