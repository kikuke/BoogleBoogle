#include "game_manager.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "map_1.h"
#include "map_2.h"

/* Collision */
static stPLAYER player[CONFIG_OBJECT_PLAYER_MAX];
static stOBJECT bubble[CONFIG_OBJECT_BUBBLE_MAX];
static stENEMY enemy[CONFIG_OBJECT_ENEMY_MAX];
static stOBJECT enemy_attack[CONFIG_OBJECT_ENEMY_ATTACK_MAX];
static stTILE map[CONFIG_OBJECT_MAP_MAX];

stPLAYER *GAME_MANAGER_GetPlayer(int player_id)
{
	return &player[player_id];
}

void GAME_MANAGER_InitStageObject(eGAME_STAGE stage, stOBJECT *obj)
{
}

void GAME_MANAGER_CheckCollision(void)
{
	/* Step 1. Check Map */
	for (int iTile = 0; iTile < CONFIG_OBJECT_MAP_MAX; ++iTile) {
		stOBJECT* tile = &map[iTile].obj;

		if (tile->is_active == false)
			continue;

		for (int iPlayer = 0; iPlayer < CONFIG_OBJECT_PLAYER_MAX; ++iPlayer) {
			stPLAYER* pPlayer = &player[iPlayer];
			if (pPlayer->obj.is_active == false)
				continue;

			Collide_Player_Wall(pPlayer, tile);
		}
	}
}

stTILE* GAME_MANAGER_GetMap(void)
{
	return map;
}

void GAME_MANAGER_SetStage(eGAME_STAGE stage)
{
	switch (stage) {
	case eGAME_STAGE_1:
		{
			map_init_stage(GAME_MANAGER_GetMap(), MAP_1_GetData());
		}
		break;
	case eGAME_STAGE_2:
		{
		map_init_stage(GAME_MANAGER_GetMap(), MAP_2_GetData());
		}
	default:
		break;
	}
}