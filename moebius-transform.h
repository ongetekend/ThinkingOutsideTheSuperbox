#ifndef __WALSH_TRANSFORM_H__
#define __WALSH_TRANSFORM_H__

#include "global-common.h"

/* Change this such sizeof(TYPE) * 8 == 32 */
#define TYPE unsigned int

void moebius_transform(TYPE *Tab, TYPE size); 

#endif
