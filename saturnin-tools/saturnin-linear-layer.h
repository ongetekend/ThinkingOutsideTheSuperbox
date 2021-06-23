#ifndef __SATURNIN_LINEAR_LAYER_H__
#define __SATURNIN_LINEAR_LAYER_H__

#include <stdint.h>


/* All 70 (4 among 8) matrices used to compute the missing nibbles */
extern const uint64_t M_all[70][4];


/* Masks for optimized multiplication */
extern const uint64_t masks[16];

extern const uint8_t nCr[9][5];;

/*
 * Return the right matrix that corresponds to the combination of positions
 * given inside `p`. The p[i] must be sorted in increasing order.
 * 
 */
const uint64_t *fetchM(uint8_t p[4]);

/*
 * Given `x` a collection of four nibble at pos `p0`, `p1`, `p2`, `p3`, return
 * the collection of four nibble at the missing position.
 *
 */
uint16_t generalized_mix_columns(uint16_t x, uint8_t p0, uint8_t p1, uint8_t
        p2, uint8_t p3);




#endif /* __SATURNIN_LINEAR_LAYER_H__ */
