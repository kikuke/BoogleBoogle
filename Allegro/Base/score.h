#ifndef __SCORE_H__
#define __SCORE_H__

#include <allegro5/allegro_font.h>
#include "object.h"
#include "enemy.h"

#define PLAYER_NAME_MAX (12)
#define STAGE_NUM_TOT (3)

#if 0
typedef struct {
	char* player_name[PLAYER_NAME_MAX];
	int score;				// curr point
	int stage;				// curr stage num
	int lives;				// num of lives
	int* stage_timer;		// 
	int* enemy_num;			// number of killed

} stSCORE;
#endif

typedef struct {
	bool is_player_dead;
	int player_lives;

	/* eliminated eneymy */
	int enemy_remain;
	int enemy_el[eENEMY_TYPE_MAX];
	int enemy_max[eENEMY_TYPE_MAX];

	int stage_frame;
} stSTAGE_INFO;

typedef struct {
	int rank;
	char player_name[PLAYER_NAME_MAX];
	int score;
} stBOARD;


void Score_Test(ALLEGRO_FONT* font);

//void Score_Create(char* name, int score);

void Score_Add(ALLEGRO_FONT* font, const char* name, int score);

stBOARD* Score_Get(void);

void Score_Print(ALLEGRO_FONT* font, stSTAGE_INFO* score_manager);

#endif
