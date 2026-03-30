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

typedef struct {
	eGAME_STATE state;
	eGAME_STAGE stage;
	bool flag_next_stage;
} stGAME_MANAGER;

/************************************************/
/*         Local Function Declaration           */
/************************************************/
void GAME_MANAGER_SetStage(eGAME_STAGE stage);

/************************************************/
/*         Local Variable Definition            */
/************************************************/
/* Object Resourece */
static stPLAYER player[CONFIG_OBJECT_PLAYER_MAX]; 
static stBUBBLE bubble[CONFIG_OBJECT_BUBBLE_MAX];
static stENEMY enemy[CONFIG_OBJECT_ENEMY_MAX];
static stOBJECT enemy_attack[CONFIG_OBJECT_ENEMY_ATTACK_MAX];
static stTILE map[CONFIG_OBJECT_MAP_MAX];

static stGAME_MANAGER game_manager;

/************************************************/
/*          Global Function Definition          */
/************************************************/
eGAME_STATE GAME_MANAGER_UpdateState(void)
{
	switch (game_manager.state) {
	case eGAME_STATE_MAIN:
		break;
	case eGAME_STATE_INGAME:
	{
		if (game_manager.flag_next_stage == true) {
			GAME_MANAGER_SetStage(++game_manager.stage);// Need To Condition
			game_manager.flag_next_stage = false;
		}
	}
	break;
	case eGAME_STATE_SCORE:
		break;
	case eGAME_STATE_END:
		break;
	default:
		break;
	}

	return game_manager.state;
}

void GAME_MANAGER_SetGameState(eGAME_STATE state)
{
	game_manager.state = state;
}

void GAME_MANAGER_SetGameStage_Next(void)
{
	game_manager.state = eGAME_STATE_INGAME;
	game_manager.flag_next_stage = true;
}

stPLAYER *GAME_MANAGER_GetPlayer(int player_id)
{
	return &player[player_id];
}

stBUBBLE* GAME_MANAGER_GetBubble(void)
{
	return bubble;
}

stENEMY* GAME_MANAGER_GetEnemy(void)
{
	return enemy;
}

stOBJECT* GAME_MANAGER_GetEnemyAttacks(void)
{
	return enemy_attack;
}

stTILE* GAME_MANAGER_GetMap(void)
{
	return map;
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
		for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; ++i) {
			stOBJECT* obj = &enemy[i].obj;
			if (obj->is_active == true) {
				Collide_Object_Tile(obj, tile);
			}
		}
		for (int i = 0; i < CONFIG_OBJECT_BUBBLE_MAX; ++i) {
			stOBJECT* obj = &bubble[i].obj;
			if (obj->is_active == true) {
				Collide_Object_Tile(obj, tile);
			}
		}
	}

	/* Collide_Enemy_Player */
	for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; ++i) {
		stOBJECT* obj = &enemy[i].obj;
		if (obj->is_active == true) {
			Collide_Enemy_Player(obj, &player[0]);
		}
	}

	/* Collide_Object_Bubble */ 
	for (int iPlayer = 0; iPlayer < CONFIG_OBJECT_PLAYER_MAX; ++iPlayer) {
		stPLAYER* pPlayer = &player[iPlayer];
		if (pPlayer->obj.is_active == false)
			continue;

		Collide_Object_Bubble(&player[0], bubble);
	}
	for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; ++i) {
		stOBJECT* obj = &enemy[i].obj;
		if (obj->is_active == true) {
			Collide_Object_Bubble(obj, bubble);
		}
	}

}

void GAME_MANAGER_UpdatePhysics(void)
{
	// TODO: Need to apply enemy
	stOBJECT_UpdatePhysics(&player[0].obj);

	for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; ++i) {
		stOBJECT* obj = &enemy[i].obj;
		if (obj->is_active == true) {
			stOBJECT_UpdatePhysics(obj);
		}
	}

	bubble_update(bubble);
}

void GAME_MANAGER_UpdateObject(void)
{
	player_update_frame(&player[0]);
	bubble_update_frame(bubble);

	// i wrote it here cause it's obj.... is correct...?
	Enemy_Update_ALL(enemy, player, enemy_attack);
	Throw_Update_ALL(enemy, player, enemy_attack);
}

/************************************************/
/*          Local Function Definition           */
/************************************************/
static void GAME_MANAGER_SetStage(eGAME_STAGE stage)
{
	switch (stage) {
	case eGAME_STAGE_1:
		{
			map_init_stage(map, enemy, MAP_1_GetData());
		}
		break;
	case eGAME_STAGE_2:
		{
			map_init_stage(map, enemy, MAP_2_GetData());
		}
		break;
	default:
		break;
	}
}