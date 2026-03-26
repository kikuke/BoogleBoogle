#ifndef __PLAYER_H__
#define __PLAYER_H__
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
	ePLAYER_STATE	state;
	stOBJECT		obj;
	int shot_timer;
	int lives;
	int invincible_timer;
} stPLAYER;

#endif
