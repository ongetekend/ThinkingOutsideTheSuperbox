#ifndef __TABLE_FIXED_H__
#define __TABLE_FIXED_H__

#include "global-common.h"

typedef struct table_fixed {
	mpz_t *head;
	size_t size;
} Table_fixed;

Table_fixed *table_fixed_create(const size_t size);
void table_fixed_destroy(Table_fixed * const);
Table_fixed *table_fixed_copy(Table_fixed * const);
void table_fixed_print(const char *, Table_fixed * const);
void table_fixed_insert_and_merge(Table_fixed * const, const size_t, const mpz_t, void (*f)(mpz_t, const mpz_t, const mpz_t));

#endif
