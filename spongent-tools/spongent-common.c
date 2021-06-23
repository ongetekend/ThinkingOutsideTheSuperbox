#include "spongent-common.h"

const uint8_t sbox[] = {0xE, 0xD, 0xB, 0x0, 0x2, 0x1, 0x4, 0xF, 0x7, 0xA, 0x8, 0x5, 0x9, 0xC, 0x3, 0x6};

/* Superboxes 4i, 4i+1, 4i+2, 4i+3 are mapped to superboxes i, i+24, i+48, i+72.
 * We compute the output bit and then subtract the appropriate multiple of 92 
 * to fit it in the 16 bits of output. The 92 is the result of subtracting multiples of 96 
 * to normalize and then adding multiples of 4 to shift the bits to their correct position.
 * The larger permutation restricted to the superbox can then be seen as a permutation on 16 bits.
 */
int index_map[] = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};

uint16_t
permute(uint16_t a)
{
	uint16_t b = 0;

	// We are only considering the first 16 bits, i.e. the first 4 superboxes.
	for (int i = 0; i < 16; i++) {
		b |= ((a >> i) & 1) << index_map[i];
	}

	return b;
}

uint16_t
permute_inverse(uint16_t b)
{
	// Permute as defined above, i.e. taking the normalization into account, is actually an involution.
	return permute(b);
}

uint16_t
sbox_layer(uint16_t x)
{
   	uint16_t a = x & 0xf;
 	uint16_t b = (x >> 4) & 0xf;
 	uint16_t c = (x >> 8) & 0xf;
 	uint16_t d = (x >> 12) & 0xf;

	a = sbox[a];
	b = sbox[b];
	c = sbox[c];
	d = sbox[d];	

	return (d << 12) | (c << 8) | (b << 4) | (a);
}
