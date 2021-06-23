#ifndef __AES_COMMON_H__
#define __AES_COMMON_H__

#include "../mds-common.h"

/* Taking 256 bits for the AES state corresponds to 8 superboxes */
#define NUMBER_OF_SUPERBOXES 8
#define NUMBER_OF_BOXES_IN_SUPERBOX 4	
#define MAX_BOX_WEIGHT (2*NUMBER_OF_BOXES_IN_SUPERBOX)
#define BOX_WIDTH 8

#ifdef __cplusplus
extern "C"
{
#endif

extern const uint8_t sbox[];
extern const uint8_t inv_sbox[];

uint32_t mix_columns(uint32_t);
uint32_t sub_bytes(uint32_t);
uint32_t inv_sub_bytes(uint32_t);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
