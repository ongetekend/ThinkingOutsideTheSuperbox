#ifndef __SPONGENT_COMMON_H__
#define __SPONGENT_COMMON_H__

#include "../global-common.h"

#define WIDTH 384
#define BOX_WIDTH 4
#define NUMBER_OF_BOXES (WIDTH / BOX_WIDTH)
#define NUMBER_OF_BOXES_IN_SUPERBOX 4
#define NUMBER_OF_SUPERBOXES (NUMBER_OF_BOXES / NUMBER_OF_BOXES_IN_SUPERBOX)
#define MAX_BOX_WEIGHT 8
	
extern const uint8_t sbox[];

uint16_t permute(uint16_t);
uint16_t permute_inverse(uint16_t);
uint16_t sbox_layer(uint16_t);

#endif
