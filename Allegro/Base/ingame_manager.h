#ifndef __INGAME_MANAGER_H__
#define __INGAME_MANAGER_H__

#include "object.h"

// temp vals
#define MAX_OBJECTS 300
#define MAX_ENEMIES 6
#define MAX_BUBBLES 50
#define MAX_PROJECTILES 30

typedef struct {
	int score;			/* curr point*/
	int high_score;		// max point
	int stage;			// curr stage num
	int lives;			// num of lives
	int stage_timer;	// 
	int is_stage_clear;	// 
	bool is_game_over;	// is game over?

} stINGAME_MANAGER;

// begine
void GameManager_Initialize(stINGAME_MANAGER* manager);				// begin with default val
void GameManager_ResetStage(stINGAME_MANAGER* manager);				// reset curr stage for replay

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
int GameManager_GetTimeBonus(stINGAME_MANAGER* manager);			// get remaining time bonus points (time-dependent reward)

// object manage
void GameManager_RegisterObject(stOBJECT* object, eOBJ_TAG tag);	// register an object into the object pool for collision/logic updates
void GameManager_UnregisterObject(stOBJECT* object);				// unregister an object from the object pool
int GameManager_GetObjectCountByTag(eOBJ_TAG tag);					// get object count by tag type (for stage clear check...)

// collision n rule
bool GameManager_CheckCollision(stOBJECT* obj1, stOBJECT* obj2);	// check collision between two objects and return true if they overlap
void GameManager_HandleCollision(stOBJECT* obj1, stOBJECT* obj2);	// handle collision logic (bubble trapping, enemy death, score.....)
bool GameManager_IsPlayerHit(stOBJECT* player, eOBJ_TAG threat_tag);// check if player caught by enemy or hit by throw
bool GameManager_IsEnemyTrapped(stOBJECT* enemy);					// check if enemy trapped in bubble (for score, state change)

// update
void GameManager_Update(stINGAME_MANAGER* manager);					// main game logic update per frame (collisions, timers, rules)

#endif
