#include "table_fixed.h"
	
Table_fixed *
table_fixed_create(const size_t size)
{
	Table_fixed *table = (Table_fixed *) malloc(sizeof(Table_fixed));
	if (table == NULL) goto fail;

	table->head = (mpz_t *) malloc(size*sizeof(mpz_t));
	if (table->head == NULL) {
		free(table);
		table = NULL;
	       	goto fail;
	}

	for (size_t i = 0; i < size; i++) {
		mpz_init(table->head[i]);
	}

	table->size = size;

fail:
	return table;
}

Table_fixed *
table_fixed_copy(Table_fixed * const src)
{
	Table_fixed *dst = table_fixed_create(src->size); 
	if (dst == NULL) goto fail;

	for (size_t i = 0; i < src->size; i++) {
		mpz_set(dst->head[i], src->head[i]);	
	}

fail:
	return dst;
}

void
table_fixed_destroy(Table_fixed * const table)
{
	for (size_t i = 0; i < table->size; i++) {
		mpz_clear(table->head[i]);
	}
	free(table->head);
	free(table);
}

void
table_fixed_print(const char *name, Table_fixed * const table)
{
	FILE *fp = fopen(name, "w+");
	if (fp == NULL) {
		exit(0);
	}

	for (size_t i = 0; i < table->size; i++) {
		fprintf(fp, "%lu", i);
		gmp_fprintf(fp, ",%Zd", table->head[i]);
		fprintf(fp, "\n");
	}

	fclose(fp);
}

void
table_fixed_insert_and_merge(Table_fixed * const table, const size_t i, const mpz_t val, void (*f)(mpz_t, const mpz_t, const mpz_t))
{
	(*f)(table->head[i], table->head[i], val);
}
