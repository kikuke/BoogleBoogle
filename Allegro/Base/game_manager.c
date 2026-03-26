#include "game_manager.h"
#include "player.h"
#include "enemy.h"

/* Collision */
stPLAYER player[CONFIG_OBJECT_PLAYER_MAX];
stOBJECT bubble[CONFIG_OBJECT_BUBBLE_MAX];
stENEMY enemy[CONFIG_OBJECT_ENEMY_MAX];
stOBJECT enemy_attack[CONFIG_OBJECT_ENEMY_ATTACK_MAX];
stTILE map[CONFIG_OBJECT_MAP_MAX];

stPLAYER *GAME_MANAGER_GetPlayer(int player_id)
{
	return &player[player_id];
}

void GAME_MANAGER_InitStageObject(eGAME_STAGE stage, stOBJECT *obj)
{
	/*
		apply collision table
	*/
}

void GAME_MANAGER_CheckCollision(void)
{

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
			map_init_stage1(GAME_MANAGER_GetMap());
		}
		break;
	default:
		break;
	}
}