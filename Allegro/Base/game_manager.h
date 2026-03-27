#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "bugglebuggle.h"
#include "object.h"
#include "player.h"
#include "bubble.h"
#include "map.h"
#include "enemy.h"

/************************************************/
/*          Global Function Declaration         */
/************************************************/
eGAME_STATE GAME_MANAGER_UpdateState(void);

stPLAYER* GAME_MANAGER_GetPlayer(int player_id);
stBUBBLE* GAME_MANAGER_GetBubble(void);
stENEMY* GAME_MANAGER_GetEnemy(void);
stOBJECT* GAME_MANAGER_GetEnemyAttacks(void);
/* Need to apply map.c */
void GAME_MANAGER_InitStageObject(eGAME_STAGE stage, stOBJECT *obj);
stTILE* GAME_MANAGER_GetMap(void);
void GAME_MANAGER_CheckCollision(void);
void GAME_MANAGER_UpdatePhysics(void);
void GAME_MANAGER_UpdateObject(void);

void GAME_MANAGER_SetStage(eGAME_STAGE stage);

#endif
