#include <stdio.h>

#include "convolution.h"
#include "csvparser.h"

int 
main(void) 
{
	Table *superbox = table_create();

	mpz_t row, col, val;
	mpz_inits(row, col, val, NULL);
	mpz_set_ui(col, 0);

	// file consists of tuples
	// Expected parameters are: file, delimiter, first_line_is_header?
	CsvParser *csvparser = CsvParser_new("data/spongent-linear-weight-superbox.txt", ",", 0);
	CsvRow *line;

	puts("Parsing...");
	while ((line = CsvParser_getRow(csvparser)) ) {
       		const char **lineFields = CsvParser_getFields(line);
		// Retrieve first entry of tuple
		mpz_set_str(row, lineFields[0], 10);
		// Retrieve second entry of tuple
		mpz_set_str(val, lineFields[1], 10);
		table_insert_and_merge(superbox, row, col, val, &mpz_add);
        	CsvParser_destroy_row(line);
	}	
	CsvParser_destroy(csvparser);

	mpz_clears(row, col, val, NULL);

	puts("Finished parsing... Time to convolve.");

	// Convolve the histograms of the superboxes to get the histogram of the full 256-bit state.
	// For a 16-bit superbox this means convolving 16 times
	Table *full = convolve_tables(superbox, 16, 0, 0, differential_combiners);
	puts("Finished convolution");
	table_print("data/spongent-linear-weight-full.txt", full);

	table_destroy(superbox);
	table_destroy(full);
	
	return 0;
}
