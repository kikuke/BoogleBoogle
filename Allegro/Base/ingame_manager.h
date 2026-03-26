#ifndef __INGAME_MANAGER_H__
#define __INGAME_MANAGER_H__

#include "object.h"


typedef struct {
	int score;			/* curr point*/
	int high_score;		// max point
	int stage;			// curr stage num
	int lives;			// num of lives
	int stage_timer;	// 
	int is_stage_clear;	// 
	bool is_game_over;	// is game over?

} stINGAME_MANAGER;

// score Manage
void GameManager_AddScore(stINGAME_MANAGER* manager, int points);	// add points to curr score n update high score if exceeded
int GameManager_GetScore(stINGAME_MANAGER* manager);				// get curr score val
int GameManager_GetHighScore(stINGAME_MANAGER* manager);			// get high score val

// game state manage
bool GameManager_CheckGameOver(stINGAME_MANAGER* manager);			// check if game is over condition met
void GameManager_PlayerDeath(stINGAME_MANAGER* manager);			// decrease lives by 1 when player dies
bool GameManager_CheckStageClear(stINGAME_MANAGER* manager);		// check if curr stage is done (all enemies die)
void GameManager_NextStage(stINGAME_MANAGER* manager);				// go to next stage n reset stage timer

// stage timer
void GameManager_UpdateStageTimer(stINGAME_MANAGER* manager);		// increment stage timer per frame

#endif
