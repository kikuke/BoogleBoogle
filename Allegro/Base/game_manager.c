#include "game_manager.h"
#include "collision.h"
#include "player.h"
#include "physics.h"
#include "enemy.h"
#include "bubble.h"
#include "map_1.h"
#include "map_2.h" 

/************************************************/
/*         Local Variable Declaration           */
/************************************************/
/* Object Resourece */
static stPLAYER player[CONFIG_OBJECT_PLAYER_MAX]; 
static stBUBBLE bubble[CONFIG_OBJECT_BUBBLE_MAX];
static stENEMY enemy[CONFIG_OBJECT_ENEMY_MAX];
static stOBJECT enemy_attack[CONFIG_OBJECT_ENEMY_ATTACK_MAX];
static stTILE map[CONFIG_OBJECT_MAP_MAX];

/************************************************/
/*          Global Function Definition          */
/************************************************/
eGAME_STATE GAME_MANAGER_UpdateState(void)
{
	//TODO: Need to modify
	GAME_MANAGER_SetStage(eGAME_STAGE_1);// Need To Condition
	return eGAME_STATE_INGAME;
}

stPLAYER *GAME_MANAGER_GetPlayer(int player_id)
{
	return &player[player_id];
}

stOBJECT* GAME_MANAGER_GetEnemyAttacks(void)
{
	return enemy_attack;
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

			Collide_Object_Tile(pPlayer, tile);
		}
	}
}

void GAME_MANAGER_UpdatePhysics(void)
{
	// TODO: Need to apply enemy
	stOBJECT_UpdatePhysics(&player[0].obj);
}

void GAME_MANAGER_UpdateObject(void)
{
	player_update_frame(&player[0]);
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