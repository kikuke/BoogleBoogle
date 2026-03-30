#ifndef __INGAME_MANAGER_H__
#define __INGAME_MANAGER_H__

#include "object.h"
#include "sqlite3.h"


#define STAGE_NUM_TOT (3)

int stage_timer_arr[STAGE_NUM_TOT] = { 0 };	// [ s1, s2, s3 ]
int enemy_num_arr[3] = { 0 };			// [ Basic, Throw, Boss ]

typedef struct {
	int score;				// curr point
	int stage;				// curr stage num
	int lives;				// num of lives
	int* stage_timer;		// 
	int* enemy_num;			// number of killed

} stSCORE;

void Score_Create(char* name, int score);

// score Manage
void Score_AddScore(int stage_num, int points);	// add points to curr score n update high score if exceeded
int Score_GetScore(stSCORE* score_manager);				// get curr score val
int Score_GetHighScore(stSCORE* score_manager);			// get high score val

// stage timer
void Score_UpdateStageTimer(stSCORE* score_manager);		// increment stage timer per frame

#endif
