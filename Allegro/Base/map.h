#ifndef __MAP_H__
#define __MAP_H__

#include "bugglebuggle.h"
#include "object.h"

typedef struct {
	stOBJECT obj;
} stTILE;

stTILE* stTILE_init(stTILE *tile, stPOSITION *pos);
void map_init_stage(stTILE* map, int (*stage)[CONFIG_MAP_X_MAX]);

#endif
