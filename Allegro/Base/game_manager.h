#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "bugglebuggle.h"
#include "object.h"
#include "player.h"
#include "bubble.h"
#include "map.h"
#include "enemy.h"

typedef enum {
	eGAME_STATE_MAIN = 0,
	eGAME_STATE_INGAME,
	eGAME_STATE_END
} eGAME_STATE;

typedef enum {
	eGAME_STAGE_1 = 0,
	eGAME_STAGE_2,
	eGAME_STAGE_3,
	eGAME_STAGE_MAX
} eGAME_STAGE;

/************************************************/
/*          Global Function Declaration         */
/************************************************/
eGAME_STATE GAME_MANAGER_UpdateState(void);

stPLAYER* GAME_MANAGER_GetPlayer(int player_id);
stBUBBLE* GAME_MANAGER_GetBubble(void);
stOBJECT* GAME_MANAGER_GetEnemyAttacks(void);
/* Need to apply map.c */
void GAME_MANAGER_InitStageObject(eGAME_STAGE stage, stOBJECT *obj);
stTILE* GAME_MANAGER_GetMap(void);
void GAME_MANAGER_CheckCollision(void);
void GAME_MANAGER_UpdatePhysics(void);

void GAME_MANAGER_SetStage(eGAME_STAGE stage);

#endif
