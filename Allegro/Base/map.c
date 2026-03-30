#include "game_manager.h"
#include "bugglebuggle.h"
#include "map.h"

stTILE* stTILE_init(stTILE* tile, stPOSITION* pos)
{
	tile->obj = (stOBJECT){
		.is_active = true,
		.coll = (stCOLLISION){
			.is_static = true,
			.tag = eOBJ_TAG_TILE,
			.box = (stCOLLISION_BOX){
				.height	= CONFIG_OBJECT_COLLISION_TILE_SIZE,
				.width	= CONFIG_OBJECT_COLLISION_TILE_SIZE
			},
		},
		.phy.pos = *pos
	};

	return tile;
}

void map_init_stage(stTILE* map, stENEMY* enemy, stSTAGE_DATA *data) {
	init_player(GAME_MANAGER_GetPlayer(0));
	memset(enemy, 0, sizeof(stENEMY) * CONFIG_OBJECT_ENEMY_MAX);
	for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; ++i) {
		stSTAGE_ENEMY_DATA* e_data = data->enemies + i;
		if (e_data->is_active == true) {
			Enemy_Create(enemy, e_data->type, e_data->pos.x, e_data->pos.y);
		}
	}
		
	
	for (int i = 0; i < CONFIG_MAP_Y_MAX; ++i) {
		for (int j = 0; j < CONFIG_MAP_X_MAX; ++j) {
			stPOSITION pos = {
				.x = j * 10,
				.y = i * 10
			};
			stTILE* tile = &map[(i * CONFIG_MAP_X_MAX) + j];

			tile->obj.is_active = false;

			if (data->stage[i][j] != 0) {
				stTILE_init(tile, &pos);
			}
		}
	}
}


