#include "spongent-common.h"

#define NUMBER_OF_ELEMENTS (1 << BOX_WIDTH)
#define SIZE (NUMBER_OF_ELEMENTS*NUMBER_OF_ELEMENTS)

int 
main(void)
{
	int difference_table[SIZE];
	int correlation_table[SIZE];

	memset(difference_table, 0, SIZE*sizeof(int));
	memset(correlation_table, 0, SIZE*sizeof(int));
	
	fill_difference_table(difference_table, sbox, BOX_WIDTH);
	fill_correlation_table(correlation_table, sbox, BOX_WIDTH);

	printf("Difference table:\n"); 
	for (int a = 0; a < NUMBER_OF_ELEMENTS; a++) {
		for (int b = 0; b < NUMBER_OF_ELEMENTS; b++) {
			printf("%d ", difference_table[NUMBER_OF_ELEMENTS*a+b]);
		}
		printf("\n");
	}
	
	printf("Correlation table:\n"); 
	for (int a = 0; a < NUMBER_OF_ELEMENTS; a++) {
		for (int b = 0; b < NUMBER_OF_ELEMENTS; b++) {
			printf("%d ", correlation_table[NUMBER_OF_ELEMENTS*a+b]);
		}
		printf("\n");
	}
}
