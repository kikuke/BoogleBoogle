#ifndef __MAP_H__
#define __MAP_H__

#include "bugglebuggle.h"
#include "object.h"

#define MAP_TILE_SIZE		(2.0f)

typedef struct {
	stOBJECT obj;
} stTILE;

stTILE* stTILE_init(stTILE *tile, stPOSITION *pos);
void map_init_stage(stTILE* map, int (*stage)[CONFIG_MAP_X_MAX]);

#endif
