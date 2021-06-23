#include "saturnin-common.h"

#define NUMBER_OF_ELEMENTS (1 << BOX_WIDTH)
#define SIZE (NUMBER_OF_ELEMENTS*NUMBER_OF_ELEMENTS)

int 
main(void)
{
	int difference_table_even[SIZE];
	int correlation_table_even[SIZE];
	int difference_table_odd[SIZE];
	int correlation_table_odd[SIZE];

	memset(difference_table_even, 0, SIZE*sizeof(int));
	memset(correlation_table_even, 0, SIZE*sizeof(int));
	memset(difference_table_odd, 0, SIZE*sizeof(int));
	memset(correlation_table_odd, 0, SIZE*sizeof(int));
	
	fill_difference_table(difference_table_even, sbox_even, BOX_WIDTH);
	fill_correlation_table(correlation_table_even, sbox_even, BOX_WIDTH);
	fill_difference_table(difference_table_odd, sbox_odd, BOX_WIDTH);
	fill_correlation_table(correlation_table_odd, sbox_odd, BOX_WIDTH);

	printf("Difference table even:\n");
	for (int a = 0; a < NUMBER_OF_ELEMENTS; a++) {
		for (int b = 0; b < NUMBER_OF_ELEMENTS; b++) {
			printf("%d ", difference_table_even[NUMBER_OF_ELEMENTS*a+b]);
		}
		printf("\n");
	}

	printf("Correlation table even:\n");
	for (int a = 0; a < NUMBER_OF_ELEMENTS; a++) {
		for (int b = 0; b < NUMBER_OF_ELEMENTS; b++) {
			printf("%d ", correlation_table_even[NUMBER_OF_ELEMENTS*a+b]);
		}
		printf("\n");
	}

	printf("Difference table odd:\n");
	for (int a = 0; a < NUMBER_OF_ELEMENTS; a++) {
		for (int b = 0; b < NUMBER_OF_ELEMENTS; b++) {
			printf("%d ", difference_table_odd[NUMBER_OF_ELEMENTS*a+b]);
		}
		printf("\n");
	}

	printf("Correlation table odd:\n");
	for (int a = 0; a < NUMBER_OF_ELEMENTS; a++) {
		for (int b = 0; b < NUMBER_OF_ELEMENTS; b++) {
			printf("%d ", correlation_table_odd[NUMBER_OF_ELEMENTS*a+b]);
		}
		printf("\n");
	}
}
