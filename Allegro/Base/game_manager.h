#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "bugglebuggle.h"
#include "object.h"
#include "player.h"
#include "map.h"

stPLAYER* GAME_MANAGER_GetPlayer(int player_id);
/* Need to apply map.c */
void GAME_MANAGER_InitStageObject(eGAME_STAGE stage, stOBJECT *obj);
stTILE* GAME_MANAGER_GetMap(void);
void GAME_MANAGER_CheckCollision(void);

void GAME_MANAGER_SetStage(eGAME_STAGE stage);

#endif
