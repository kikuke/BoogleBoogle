#ifndef __INGAME_MANAGER_H__
#define __INGAME_MANAGER_H__

#include "object.h"
#include "sqlite3.h"

#define PLAYER_NAME_MAX (12)
#define STAGE_NUM_TOT (3)

int stage_timer_arr[STAGE_NUM_TOT] = { 0 };	// [ s1, s2, s3 ]
int enemy_num_arr[3] = { 0 };			// [ Basic, Throw, Boss ]

typedef struct {
	char* player_name[PLAYER_NAME_MAX];
	int score;				// curr point
	int stage;				// curr stage num
	int lives;				// num of lives
	int* stage_timer;		// 
	int* enemy_num;			// number of killed

} stSCORE;

typedef struct {
	int rank;
	char* player_name[PLAYER_NAME_MAX];
	int score;
} stBOARD;

stBOARD* leaderboard[11];


void Score_Test();

//void Score_Create(char* name, int score);

void Score_Add(stSCORE* score_manager);

stBOARD* Score_Get(stSCORE* score_manager, stBOARD* leaderboard);

void Score_Print(stSCORE* score_manager, stBOARD* leaderboard);

#endif
