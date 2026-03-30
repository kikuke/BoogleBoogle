#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "bugglebuggle.h"
#include "object.h"
#include "player.h"
#include "bubble.h"
#include "map.h"
#include "score.h"
#include "enemy.h"
/************************************************/
/*          Global Function Declaration         */
/************************************************/
/* Game State */
eGAME_STATE GAME_MANAGER_UpdateState(void);
void GAME_MANAGER_SetGameState(eGAME_STATE state);
eGAME_STAGE GAME_MANAGER_GetGameStage(void);
void GAME_MANAGER_SetGameStage_Next(void);
void GAME_MANAGER_UpdateStage(void);
const stSTAGE_INFO* GAME_MANAGER_GetStageInfo(void);
int GAME_MANAGER_GetScore(void);
bool GAME_MANAGER_IsLoading(void);

/* Request Resource */
stPLAYER* GAME_MANAGER_GetPlayer(int player_id);
stBUBBLE* GAME_MANAGER_GetBubble(void);
stENEMY* GAME_MANAGER_GetEnemy(void);
stOBJECT* GAME_MANAGER_GetEnemyAttacks(void);
stTILE* GAME_MANAGER_GetMap(void);

/* Processing Collision, Physics, Object State */
void GAME_MANAGER_CheckCollision(void);
void GAME_MANAGER_UpdatePhysics(void);
void GAME_MANAGER_UpdateObject(void);

#endif
