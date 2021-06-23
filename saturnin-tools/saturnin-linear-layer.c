#include <stdint.h>
#include <stdio.h>

#include "saturnin-linear-layer.h"

const uint64_t M_all[70][4] = {

    /* [0, 1, 2, 3] */
    {
        0xa882d449e22c5114,
        0xe8c6f46b72b5d19c,
        0x82a849d42ce21451,
        0xc6e86bf4b5729cd1,
    },

    /* [0, 1, 2, 4] */
    {
        0x1df28e794f3c3ae4,
        0x36461b2b85957c8c,
        0x4448222499928881,
        0xc6e86bf4b5729cd1,
    },

    /* [0, 1, 2, 5] */
    {
        0x76463b2b1595fc8c,
        0xd9bdec5ef6afa26a,
        0xabb3d551eaa85667,
        0xde66efbbf755adcc,
    },

    /* [0, 1, 2, 6] */
    {
        0xbece5f6fa7b76d9d,
        0xfa8e7d4f3e27e51d,
        0xcff76773b3319eef,
        0x16ee8bff45773cdd,
    },

    /* [0, 1, 2, 7] */
    {
        0x59ecacf6d67bb2d9,
        0x78db34e512faf1a6,
        0x617fb8375413c3fe,
        0x4bff25779a3386ee,
    },

    /* [0, 1, 3, 4] */
    {
        0x846242b9295c18c4,
        0x8df64e7b2f351aec,
        0x4448222499928881,
        0x82a849d42ce21451,
    },

    /* [0, 1, 3, 5] */
    {
        0xf5757a3a3d1debfb,
        0x1b9f85c74a63362e,
        0xeaa2fdd97eecd554,
        0x5d55aeaadfddbabb,
    },

    /* [0, 1, 3, 6] */
    {
        0x9313c18168482737,
        0xd753e3a1f1d8afb7,
        0x5119a88cd446b332,
        0x3b3315118a887677,
    },

    /* [0, 1, 3, 7] */
    {
        0x6c8ab64d5b2ec915,
        0xf58e7a4f3d27eb1d,
        0xd94cec26f69ba289,
        0x2a889d44ce224511,
    },

    /* [0, 1, 4, 5] */
    {
        0xe2cef96f7cb7d49d,
        0x72e839f41c72f4d1,
        0x285d94aec2df41ba,
        0x8811448822441133,
    },

    /* [0, 1, 4, 6] */
    {
        0xc28a694dbc2e9415,
        0x52aca9d6dcebb459,
        0xa84cd426e29b5189,
        0x8811448822441133,
    },

    /* [0, 1, 4, 7] */
    {
        0xce136f81b7489d37,
        0x5717a383d141bf3f,
        0xa219d98cec465432,
        0x8811448822441133,
    },

    /* [0, 1, 5, 6] */
    {
        0x22119988cc444433,
        0xb2375913ac81647f,
        0xa84cd426e29b5189,
        0x285d94aec2df41ba,
    },

    /* [0, 1, 5, 7] */
    {
        0x8a4c4d262e9b1589,
        0xcec46f62b7b99d98,
        0xb3745132a81967f8,
        0x336511ba885d77cb,
    },

    /* [0, 1, 6, 7] */
    {
        0xdff2e779f33caee4,
        0x3125189a84cd734b,
        0xd5beea5ffda7ab6d,
        0x55afaad7dde3bb5e,
    },

    /* [0, 2, 3, 4] */
    {
        0xffc7776333b1ee9f,
        0xecbef65f7ba7d96d,
        0xa8fed47fe23751ed,
        0x6e1ebf8f5747cd3d,
    },

    /* [0, 2, 3, 5] */
    {
        0x176f83b741533fce,
        0x9e5ccfa667db2db9,
        0x8d7b4e352f1a1af6,
        0xbf4f5727a3936e8e,
    },

    /* [0, 2, 3, 6] */
    {
        0x4448222499928881,
        0xf1d278e934fce3a4,
        0x436621bb985587cc,
        0xec68f6b47b52d9c1,
    },

    /* [0, 2, 3, 7] */
    {
        0xbab35d51aea86567,
        0x476623bb91558fcc,
        0xbd9d5eceaf6f6a2a,
        0x6de6befb5f75cadc,
    },

    /* [0, 2, 4, 5] */
    {
        0xb5ae5adfade76b5d,
        0x72e839f41c72f4d1,
        0x979dc3ce616f2f2a,
        0x7cc136681bb4f993,
    },

    /* [0, 2, 4, 6] */
    {
        0x39fc1c76863b72e9,
        0x93cfc16768b3279e,
        0xfae87df43e72e5d1,
        0xaf8ed74fe3275e1d,
    },

    /* [0, 2, 4, 7] */
    {
        0x6d94bec25f69ca28,
        0xe829f49c72c6d142,
        0x9b7bc5356a1a26f6,
        0x5952aca9d6dcb2b4,
    },

    /* [0, 2, 5, 6] */
    {
        0x8e924fc9276c1d24,
        0xd649eb2cf596ac82,
        0x9525ca9a6dcd2b4b,
        0xb9b75c53a6a1626f,
    },

    /* [0, 2, 5, 7] */
    {
        0xd919ec8cf646a232,
        0x9d91cec86f642a23,
        0x7db83e541fa2fa61,
        0xd78be345f12aaf16,
    },

    /* [0, 2, 6, 7] */
    {
        0x728e394f1c27f41d,
        0xb5ea5afdad7e6bd5,
        0x97d9c3ec61f62fa2,
        0x7c1c36861b4bf939,
    },

    /* [0, 3, 4, 5] */
    {
        0x5d3dae1edf8fba7a,
        0x25669abbcd554bcc,
        0xf9a77cd336e1e25f,
        0xc3dc61e6b8fb97a9,
    },

    /* [0, 3, 4, 6] */
    {
        0xdf55e7aaf3ddaebb,
        0x1df58e7a4f3d3aeb,
        0x26ba9b5dc5ae4c65,
        0xb2fb5975ac3a64e6,
    },

    /* [0, 3, 4, 7] */
    {
        0x9948cc2466922281,
        0xa334d112e8895778,
        0xf5e87af43d72ebd1,
        0xdf68e7b4f352aec1,
    },

    /* [0, 3, 5, 6] */
    {
        0x7b2435921ac9f648,
        0xac12d689eb4c5934,
        0x7e493f2c1796fd82,
        0xa76dd3bee15f5fca,
    },

    /* [0, 3, 5, 7] */
    {
        0xcff76773b3319eef,
        0x8b7f45372a1316fe,
        0xca9a6dcdbe6e9525,
        0x5c74a632db19b9f8,
    },

    /* [0, 3, 6, 7] */
    {
        0x856d4abe2d5f1bca,
        0xf76373b13158efc7,
        0xfc1c76863b4be939,
        0x93cbc16568ba2796,
    },

    /* [0, 4, 5, 6] */
    {
        0x9268c9b46c5224c1,
        0xcfe367f1b3789ed7,
        0x6b23b5915ac8c647,
        0xea33fd117e88d577,
    },

    /* [0, 4, 5, 7] */
    {
        0xa6d3dbe1e5f85ca7,
        0xddb6ee5bffa5aa6c,
        0xe289f94c7c26d412,
        0x6399b1cc5866c722,
    },

    /* [0, 4, 6, 7] */
    {
        0x3ee71ff387717ddf,
        0x19a48cd246e93258,
        0x656ababd5d5ecbc5,
        0xe47af23d791ed8f5,
    },

    /* [0, 5, 6, 7] */
    {
        0x1cbf86574ba3396e,
        0x4cd226e99bfc89a4,
        0xb87f5437a21361fe,
        0xf5bd7a5e3dafeb6a,
    },

    /* [1, 2, 3, 4] */
    {
        0x115988ac44d633b2,
        0x319318c184687327,
        0x75d33ae11df8fba7,
        0xb3335111a8886777,
    },

    /* [1, 2, 3, 5] */
    {
        0x94dcc2e669fb28a9,
        0xc86a64bdb25e91c5,
        0x58fea47fd237b1ed,
        0xa828d494e2c25141,
    },

    /* [1, 2, 3, 6] */
    {
        0x4448222499928881,
        0x6842b429529cc184,
        0xf8d674eb32f5e1ac,
        0xa828d494e2c25141,
    },

    /* [1, 2, 3, 7] */
    {
        0xaea2dfd9e7ec5d54,
        0x7f5537aa13ddfebb,
        0x91bfc85764a3236e,
        0x55d5aaeaddfdbbab,
    },

    /* [1, 2, 4, 5] */
    {
        0xf736731b3185ef7c,
        0x85d64aeb2df51bac,
        0xfcc176683bb4e993,
        0x93bcc15668ab2769,
    },

    /* [1, 2, 4, 6] */
    {
        0xd15fe8a7f4d3a3be,
        0xfd557eaa3fddeabb,
        0x2bbf9557caa3466e,
        0x62abb9d55ceac456,
    },

    /* [1, 2, 4, 7] */
    {
        0xca216d98bec49543,
        0xb7425329a19c6f84,
        0x7ad63deb1ef5f5ac,
        0xe794f3c27169df28,
    },

    /* [1, 2, 5, 6] */
    {
        0x3a431d218e987587,
        0x9984cc4266292218,
        0xfd867e4b3f25ea1c,
        0x5f8ea74fd327be1d,
    },

    /* [1, 2, 5, 7] */
    {
        0xb8f75473a23161ef,
        0xfc7f76373b13e9fe,
        0xc5476a23bd919b8f,
        0xaca9d6dcebe65952,
    },

    /* [1, 2, 6, 7] */
    {
        0x25669abbcd554bcc,
        0x5dd3aee1dff8baa7,
        0xf97a7c3d361ee2f5,
        0xc3cd616eb8bf979a,
    },

    /* [1, 3, 4, 5] */
    {
        0x38cb146582ba7196,
        0xb3bf5157a8a3676e,
        0x1ee98ffc47763dd2,
        0xef7ef73f7317defd,
    },

    /* [1, 3, 4, 6] */
    {
        0x2c8a964dcb2e4915,
        0xc2a869d4bce29451,
        0xc1146882b4499338,
        0x1c4186284b943983,
    },

    /* [1, 3, 4, 7] */
    {
        0x9948cc2466922281,
        0x6f14b7825349ce38,
        0x6ca8b6d45be2c951,
        0x46282b9495c28c41,
    },

    /* [1, 3, 5, 6] */
    {
        0xf6417b283594ec83,
        0x9984cc4266292218,
        0x6482b249592cc814,
        0xc68a6b4db52e9c15,
    },

    /* [1, 3, 5, 7] */
    {
        0x5119a88cd446b332,
        0x15918ac84d643b23,
        0x4fcf276793b38e9e,
        0xf4fc7276393be8e9,
    },

    /* [1, 3, 6, 7] */
    {
        0xb3fb5175a83a67e6,
        0x38bc145682ab7169,
        0x1e9e8fcf47673d2d,
        0xefe7f7f37371dedf,
    },

    /* [1, 4, 5, 6] */
    {
        0x9268c9b46c5224c1,
        0x8773433121181ff7,
        0x23b39151c8a84767,
        0xa2a3d9d1ece85457,
    },

    /* [1, 4, 5, 7] */
    {
        0xe5f2fa797d3cdbe4,
        0x5af5ad7ade3db5eb,
        0xdc96e6cbfb65a92c,
        0x5d86ae4bdf25ba1c,
    },

    /* [1, 4, 6, 7] */
    {
        0x4339211c98868772,
        0x38bc145682ab7169,
        0xbedf5fe7a7f36dae,
        0x3fcf176783b37e9e,
    },

    /* [1, 5, 6, 7] */
    {
        0x9aeccdf66e7b25d9,
        0x22ab99d5ccea4456,
        0xa4c8d264e9b25891,
        0x8773433121181ff7,
    },

    /* [2, 3, 4, 5] */
    {
        0xdff2e779f33caee4,
        0x3125189a84cd734b,
        0xd5beea5ffda7ab6d,
        0x55afaad7dde3bb5e,
    },

    /* [2, 3, 4, 6] */
    {
        0x2c8a964dcb2e4915,
        0x25ac9ad6cdeb4b59,
        0x8811448822441133,
        0x8a4c4d262e9b1589,
    },

    /* [2, 3, 4, 7] */
    {
        0x22119988cc444433,
        0x2b379513ca81467f,
        0x825d49ae2cdf14ba,
        0x8a4c4d262e9b1589,
    },

    /* [2, 3, 5, 6] */
    {
        0xec13f6817b48d937,
        0x75173a831d41fb3f,
        0x8811448822441133,
        0x2a199d8cce464532,
    },

    /* [2, 3, 5, 7] */
    {
        0xa84cd426e29b5189,
        0xecc4f6627bb9d998,
        0x336511ba885d77cb,
        0x3b7415328a1976f8,
    },

    /* [2, 3, 6, 7] */
    {
        0xe2cef96f7cb7d49d,
        0x72e839f41c72f4d1,
        0x285d94aec2df41ba,
        0x8811448822441133,
    },

    /* [2, 4, 5, 6] */
    {
        0x37ee13ff81777fdd,
        0x6a56bdab5ed5c5bc,
        0xea47fd237e91d58f,
        0x149a82cd496e3825,
    },

    /* [2, 4, 5, 7] */
    {
        0x1fcb876543ba3e96,
        0xbf875743a3216e1f,
        0xfd5b7ea53fdaeab6,
        0x42cd296e9cbf849a,
    },

    /* [2, 4, 6, 7] */
    {
        0x9826c49b62c5214c,
        0xe3a3f1d178e8d757,
        0xc3fe617fb83797ed,
        0x63b2b15958acc764,
    },

    /* [2, 5, 6, 7] */
    {
        0xa36dd1bee85f57ca,
        0x6939bc1c5686c272,
        0xd6dbebe5f5faaca6,
        0xe928fc9476c2d241,
    },

    /* [3, 4, 5, 6] */
    {
        0x49332c1196888277,
        0xbfed57fea37f6eda,
        0x3ffc1776833b7ee9,
        0x3c8b16458b2a7916,
    },

    /* [3, 4, 5, 7] */
    {
        0x9caec6df6be7295d,
        0xa84cd426e29b5189,
        0x83774133281117ff,
        0x2b2a959dcace4645,
    },

    /* [3, 4, 6, 7] */
    {
        0x9826c49b62c5214c,
        0xa32ad19de8ce5745,
        0x83774133281117ff,
        0x233b9115c88a4776,
    },

    /* [3, 5, 6, 7] */
    {
        0xe25ff9a77cd3d4be,
        0x56d8abe4d5f2bca1,
        0x55afaad7dde3bb5e,
        0xd6c9eb6cf5b6ac92,
    },

    /* [4, 5, 6, 7] */
    {
        0x6574ba325d19cbf8,
        0x77123389114cff34,
        0x746532ba195df8cb,
        0x127789334c1134ff,
    },
};


const uint64_t masks[16] = {
    0x0,
    0xffff,
    0xffff0000,
    0xffffffff,
    0xffff00000000,
    0xffff0000ffff,
    0xffffffff0000,
    0xffffffffffff,
    0xffff000000000000,
    0xffff00000000ffff,
    0xffff0000ffff0000,
    0xffff0000ffffffff,
    0xffffffff00000000,
    0xffffffff0000ffff,
    0xffffffffffff0000,
    0xffffffffffffffff
};

const uint8_t nCr[9][5] = {
    { 1, 0, 0, 0, 0 },
    { 1, 1, 0, 0, 0 },
    { 1, 2, 1, 0, 0 },
    { 1, 3, 3, 1, 0 },
    { 1, 4, 6, 4, 1 },
    { 1, 5, 10, 10, 5 },
    { 1, 6, 15, 20, 15 },
    { 1, 7, 21, 35, 35 },
    { 1, 8, 28, 56, 70 },
};


const uint64_t *fetchM(uint8_t p[4])
{
    if (p[0] >= p[1] || p[1] >= p[2] || p[2] >= p[3])
        return 0;


    uint64_t t = 0;
    uint8_t i, r;

    for (i = 0; i < 4; i++) {
        t += nCr[8 - 1 - p[i]][4 - i];
    }

    //printf("%d\n", 70 - t - 1);
    return M_all[nCr[8][4]-t-1];
}


uint16_t generalized_mix_columns(uint16_t x, uint8_t p0, uint8_t p1, uint8_t
        p2, uint8_t p3)
{
    uint8_t i, nib;
    uint64_t mask;
    uint64_t tmp_res = 0;
    uint8_t p[4] = { p0, p1, p2, p3 };
    const uint64_t *M = fetchM(p);
    //printf("%x\n", M[0]);

    /* Optimized binary vector-matrix multiplication using masks */
    for (i = 0; i < 4; i++) {
        nib = (x >> 4*i) & 0xf;

        mask = masks[nib];
        tmp_res ^= (mask & M[i]);
    }

    /* Fold the intermediate result to get the result of the multiplication */
    tmp_res = (tmp_res & 0xffffffff) ^ (tmp_res >> 32);
    return (tmp_res & 0xffff) ^ (tmp_res >> 16);

}
