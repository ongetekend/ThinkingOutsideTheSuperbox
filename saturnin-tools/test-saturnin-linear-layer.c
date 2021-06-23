#include <stdio.h>

#include "saturnin-linear-layer.h"
#include "saturnin-common.h"

void main() {
    printf("%x\n", generalized_mix_columns(0x2153, 0, 1, 2, 3));
    printf("%x\n", generalized_mix_columns(mix_columns(0x2153), 4, 5, 6, 7));
    printf("%x\n", generalized_mix_columns(0x3713, 0, 2, 6, 7));

    printf("%x\n", mix_columns(0x2153));
    printf("%x\n", mix_columns_inverse(mix_columns(0x2153)));

}
