#ifndef __MAP_H__
#define __MAP_H__

#include "enemy.h"

typedef struct {
	stOBJECT obj;
} stTILE;

typedef struct {
	bool is_active;
	eENEMY_TYPE type;
	stPOSITION	pos;
} stSTAGE_ENEMY_DATA;

typedef struct {
	int (*stage)[CONFIG_MAP_X_MAX];
	stSTAGE_ENEMY_DATA *enemies;
} stSTAGE_DATA;

stTILE* stTILE_init(stTILE *tile, stPOSITION *pos);
void map_init_stage(stTILE* map, stENEMY* enemy, stSTAGE_DATA *data);

#endif
