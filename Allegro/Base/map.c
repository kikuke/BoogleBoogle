#include "bugglebuggle.h"
#include "map.h"

stTILE* stTILE_init(stTILE* tile, stPOSITION* pos)
{
	tile->obj = (stOBJECT){
		.coll = (stCOLLISION){
			.is_static = true,
			.tag = eOBJ_TAG_TILE,
			.box = (stCOLLISION_BOX){
				.height	= MAP_TILE_SIZE,
				.width	= MAP_TILE_SIZE
			},
		},
		.phy.pos = *pos
	};

	return tile;
}

void map_init_stage(stTILE *map, int (*stage)[CONFIG_MAP_X_MAX]) {
	for (int i = 0; i < CONFIG_MAP_Y_MAX; ++i) {
		for (int j = 0; j < CONFIG_MAP_X_MAX; ++j) {
			stPOSITION pos;

			if (stage[i][j] != 0) {
				pos.x = j * 10;
				pos.y = i * 10;
				stTILE_init(&map[(i * CONFIG_MAP_X_MAX) + j], &pos);
			}
		}
	}
	
}


