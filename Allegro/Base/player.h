#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "bugglebuggle.h"
#include "object.h"

typedef enum {
	ePLAYER_STATE_IDLE = 0,
	ePLAYER_STATE_MOVE,
	ePLAYER_STATE_JUMP,
	ePLAYER_STATE_ATTACK,
	ePLAYER_STATE_DEAD,
	ePLAYER_STATE_MAX
}ePLAYER_STATE;

typedef struct {
	/* OBJECT */
	stOBJECT		obj;
	ePLAYER_STATE	state;
	int shot_timer;
	int lives;
	int invincible_timer;
	bool is_jump;
} stPLAYER;

void init_player(stPLAYER* player);
void player_update_input(stPLAYER* player, int allegro_key, unsigned char flag);
void player_update_frame(stPLAYER* player);

#if (DEBUG_PLAYER == 1)
void player_debug(stPLAYER* player);
#endif

#endif
