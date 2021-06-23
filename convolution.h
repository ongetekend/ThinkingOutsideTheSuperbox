#ifndef __CONVOLUTION_H__
#define __CONVOLUTION_H__

#include "global-common.h"
#include "table.h"
#include "table_fixed.h"

Table_fixed *convolve_tables_fixed(Table_fixed *, uint8_t, int, int);
Table_fixed *convolve_fixed(Table_fixed *, Table_fixed *, int, int);
Table *convolve_tables(Table *, uint8_t, int, int, void (*f[3])(mpz_t, const mpz_t, const mpz_t));
Table *convolve(Table *, Table *, int, int, void (*f[3])(mpz_t, const mpz_t, const mpz_t));

#endif
