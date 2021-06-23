#ifndef __SATURNIN_COMMON_H__
#define __SATURNIN_COMMON_H__

#include "../mds-common.h"

#define NUMBER_OF_SUPERBOXES 16
#define NUMBER_OF_BOXES_IN_SUPERBOX 4	
#define MAX_BOX_WEIGHT (2*NUMBER_OF_BOXES_IN_SUPERBOX)
#define BOX_WIDTH 4

extern const uint8_t sbox_even[];
extern const uint8_t sbox_odd[];

uint16_t mix_columns(uint16_t);
uint16_t mix_columns_inverse(uint16_t);
uint16_t mix_columns_transposed(uint16_t);
uint16_t mix_columns_transposed_inverse(uint16_t);
uint16_t sbox_layer(uint16_t);

uint64_t permute_slice(uint64_t);
uint64_t permute_slice_inverse(uint64_t);
uint64_t mix_columns_slice(uint64_t);
uint64_t mix_columns_slice_inverse(uint64_t);
uint64_t mix_columns_slice_transposed(uint64_t);
uint64_t mix_columns_slice_transposed_inverse(uint64_t);

#endif
