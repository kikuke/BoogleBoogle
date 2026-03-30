#include <string.h>
#include "game_manager.h"
#include "collision.h"
#include "player.h"
#include "physics.h"
#include "enemy.h"
#include "bubble.h"
#include "map_1.h"
#include "map_2.h" 
#include "map_3.h"

/************************************************/
/*         Local Variable Declaration           */
/************************************************/

typedef struct {
	eGAME_STATE state;
	eGAME_STAGE stage;
	stSTAGE_INFO stage_info[eGAME_STAGE_MAX];
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
			if (++game_manager.stage >= eGAME_STAGE_MAX) {
				// TODO: Jump to Score
				break;
			}

			GAME_MANAGER_SetStage(game_manager.stage);// Need To Condition
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

void GAME_MANAGER_UpdateStage(void)
{
	stSTAGE_INFO* info = &game_manager.stage_info[game_manager.stage];
	stPLAYER* pPlayer = &player[0];

	info->is_player_dead = pPlayer->state == ePLAYER_STATE_DEAD;
	info->player_lives = pPlayer->lives;

	info->enemy_remain = 0;
	for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; ++i) {
		stENEMY* pEnemy = &enemy[i];
		if (pEnemy->obj.is_active == false)
			continue;

		info->enemy_el[pEnemy->type]++;
		info->enemy_remain++;
	}
	for (int iType = 0; iType < eENEMY_TYPE_MAX; ++iType) {
		info->enemy_el[iType] = info->enemy_max[iType] - info->enemy_el[iType];
	}

	info->stage_frame++;

	if (info->is_player_dead) {
		/* TODO: Go to Score */
		return;
	}
	if (info->enemy_remain <= 0) {
		/* TODO: Go to Next Stage */
		return;
	}
}

int GAME_MANAGER_GetScore(void)
{
	//TODO: 
}

const stSTAGE_INFO* GAME_MANAGER_GetStageInfo(eGAME_STAGE stage)
{
	return &game_manager.stage_info[stage];
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
		for (int iEnemy = 0; iEnemy < CONFIG_OBJECT_ENEMY_MAX; ++iEnemy) {
			stOBJECT* obj = &enemy[iEnemy].obj;
			if (obj->is_active == true) {
				Collide_Object_Tile(obj, tile);
			}
		}
		for (int iBubble = 0; iBubble < CONFIG_OBJECT_BUBBLE_MAX; ++iBubble) {
			stOBJECT* obj = &bubble[iBubble].obj;
			if (obj->is_active == true) {
				Collide_Object_Tile(obj, tile);
			}
		}
	}

	/* Collide_Enemy_Player */
	for (int iEnemy = 0; iEnemy < CONFIG_OBJECT_ENEMY_MAX; ++iEnemy) {
		stOBJECT* obj = &enemy[iEnemy].obj;
		if (obj->is_active == true) {
			Collide_Enemy_Player(obj, &player[0]);
		}
	}

	/* enemy throw */
	for (int iThrow = 0; iThrow < CONFIG_OBJECT_ENEMY_ATTACK_MAX; ++iThrow) {
		if (enemy_attack[iThrow].is_active == true) {
			Collide_Enemy_Player(&enemy_attack[iThrow], &player[0]);
		}
	}

	/* Collide_Object_Bubble */ 

	for (int i = 0; i < CONFIG_OBJECT_BUBBLE_MAX; ++i) {
		if (bubble[i].obj.is_active) {
			Collide_Object_Bubble(&(player[0].obj), &bubble[i]);
		}
	}

	for (int iEnemy = 0; iEnemy < CONFIG_OBJECT_ENEMY_MAX; ++iEnemy) {
		if (!enemy[iEnemy].obj.is_active) continue;

		for (int iBubble = 0; iBubble < CONFIG_OBJECT_BUBBLE_MAX; ++iBubble) {
			if (!bubble[iBubble].obj.is_active) continue;

			Collide_Object_Bubble(&(enemy[iEnemy].obj), &bubble[iBubble]);
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
	stSTAGE_INFO* info = &game_manager.stage_info[stage];
	memset(info, 0x00, sizeof(stSTAGE_INFO));
	
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
	case eGAME_STAGE_3:
		{
			map_init_stage(map, enemy, MAP_3_GetData());
		}
		break;
	default:
		break;
	}

	/* Initialize Stage Info */
	info->player_lives = player[0].lives;
	for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; ++i) {
		stENEMY* pEnemy = &enemy[i];
		if (pEnemy->obj.is_active == false)
			continue;

		info->enemy_max[pEnemy->type]++;
	}
}