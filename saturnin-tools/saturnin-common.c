#include "saturnin-common.h"

#define UP(x) (((x) >> 1) | ((((x) ^ ((x) >> 1)) & 1) << 3))
#define UP_INVERSE(x) ((((x) << 1) | (((x) ^ ((x) >> 3)) & 1)) & 0xf) 
#define UP_TRANSPOSED(x) (((((x) << 1) | ((x) >> 3)) ^ (((x) >> 2) & 2)) & 0xf)
#define UP_TRANSPOSED_INVERSE(x) (((x >> 1) ^ (x & 1)) ^ ((x << 3) & 8)) 

static uint64_t swap(const uint64_t, const unsigned long, const unsigned long);

const uint8_t sbox_even[] = {0x0, 0x6, 0xe, 0x1, 0xf, 0x4, 0x7, 0xd, 0x9, 0x8, 0xc, 0x5, 0x2, 0xa, 0x3, 0xb};
const uint8_t sbox_odd[] = {0x0, 0x9, 0xd, 0x2, 0xf, 0x1, 0xb, 0x7, 0x6, 0x4, 0x5, 0x3, 0x8, 0xc, 0xa, 0xe};

uint16_t
mix_columns_transposed(uint16_t x)
{
   	uint16_t a = x & 0xf;
 	uint16_t b = (x >> 4) & 0xf;
 	uint16_t c = (x >> 8) & 0xf;
 	uint16_t d = (x >> 12) & 0xf;

	a ^= d;
	c ^= b;
	d ^= c;
	c = UP_TRANSPOSED(c);
	c = UP_TRANSPOSED(c);
	c ^= b;
	b ^= a;
	c ^= a;
	a = UP_TRANSPOSED(a);
	a = UP_TRANSPOSED(a);
	b = UP_TRANSPOSED(b);
	a ^= d;
	d = UP_TRANSPOSED(d);
	b ^= a;
	d ^= c;

	return (d << 12) | (c << 8) | (b << 4) | (a);
}

uint16_t
mix_columns_transposed_inverse(uint16_t x)
{
   	uint16_t a = x & 0xf;
 	uint16_t b = (x >> 4) & 0xf;
 	uint16_t c = (x >> 8) & 0xf;
 	uint16_t d = (x >> 12) & 0xf;

	d ^= c;
	b ^= a;
	d = UP_TRANSPOSED_INVERSE(d);
	a ^= d;
	b = UP_TRANSPOSED_INVERSE(b);
	a = UP_TRANSPOSED_INVERSE(a);
	a = UP_TRANSPOSED_INVERSE(a);
	c ^= a;
	b ^= a;
	c ^= b;
	c = UP_TRANSPOSED_INVERSE(c);
	c = UP_TRANSPOSED_INVERSE(c);
	d ^= c;
	c ^= b;
	a ^= d;

	return (d << 12) | (c << 8) | (b << 4) | (a);
}

uint16_t
mix_columns_inverse(uint16_t x)
{
   	uint16_t a = x & 0xf;
 	uint16_t b = (x >> 4) & 0xf;
 	uint16_t c = (x >> 8) & 0xf;
 	uint16_t d = (x >> 12) & 0xf;

	d ^= a;
	b ^= c;
	c ^= d;
	a ^= b;
	c  = UP_INVERSE(c);
	c  = UP_INVERSE(c);
	a  = UP_INVERSE(a);
	a  = UP_INVERSE(a);
	d ^= a;
	b ^= c;
	d  = UP_INVERSE(d);
	b  = UP_INVERSE(b);
	c ^= d;
	a ^= b;

	return (d << 12) | (c << 8) | (b << 4) | (a);
}

uint16_t
mix_columns(uint16_t x)
{
   	uint16_t a = x & 0xf;
 	uint16_t b = (x >> 4) & 0xf;
 	uint16_t c = (x >> 8) & 0xf;
 	uint16_t d = (x >> 12) & 0xf;

	a ^= b;
	c ^= d;
	b  = UP(b);
	d  = UP(d);
	b ^= c;
	d ^= a;
	a  = UP(a);
	a  = UP(a);
	c  = UP(c);
	c  = UP(c);
	a ^= b;
	c ^= d;
	b ^= c;
	d ^= a;

	return (d << 12) | (c << 8) | (b << 4) | (a);
}

static uint64_t 
swap(const uint64_t b, const unsigned long i, const unsigned long j)
{
	uint64_t x = ((b >> 4*i) ^ (b >> 4*j)) & 0xf; // XOR temporary
	return b ^ ((x << 4*i) | (x << 4*j));
}

uint64_t 
permute_slice(uint64_t b)
{
	uint64_t r = swap(b, 2, 6);
	r = swap(r, 3, 11);
	r = swap(r, 5, 13);
	r = swap(r, 0, 12);
	r = swap(r, 4, 8);
	return swap(r, 10, 14);
}

uint64_t 
permute_slice_inverse(uint64_t b)
{
	uint64_t r = swap(b, 2, 6);
	r = swap(r, 3, 11);
	r = swap(r, 5, 13);
	r = swap(r, 0, 12);
	r = swap(r, 4, 8);
	return swap(r, 10, 14);
}

uint64_t 
mix_columns_slice(uint64_t x)
{
	uint64_t r = 0;
	for (long i = 0; i < 4; i++) {
		r |= (uint64_t) mix_columns((x >> 16*i) & 0xffff) << 16*i;
	}
	return r;
}

uint64_t 
mix_columns_slice_inverse(uint64_t x)
{
	uint64_t r = 0;
	for (long i = 0; i < 4; i++) {
		r |= (uint64_t) mix_columns_inverse((x >> 16*i) & 0xffff) << 16*i;
	}
	return r;
}

uint64_t 
mix_columns_slice_transposed(uint64_t x)
{
	uint64_t r = 0;
	for (long i = 0; i < 4; i++) {
		r |= (uint64_t) mix_columns_transposed((x >> 16*i) & 0xffff) << 16*i;
	}
	return r;
}

uint64_t 
mix_columns_slice_transposed_inverse(uint64_t x)
{
	uint64_t r = 0;
	for (long i = 0; i < 4; i++) {
		r |= (uint64_t) mix_columns_transposed_inverse((x >> 16*i) & 0xffff) << 16*i;
	}
	return r;
}

uint16_t
sbox_layer(uint16_t x)
{
   	uint16_t a = x & 0xf;
 	uint16_t b = (x >> 4) & 0xf;
 	uint16_t c = (x >> 8) & 0xf;
 	uint16_t d = (x >> 12) & 0xf;

	a = sbox_even[a];
	b = sbox_odd[b];
	c = sbox_even[c];
	d = sbox_odd[d];	

	return (d << 12) | (c << 8) | (b << 4) | (a);
}
