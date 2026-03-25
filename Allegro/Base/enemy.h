#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "object.h"

typedef enum {
	eENEMY_STATE_IDLE = 0,
	eENEMY_STATE_MOVE,
	eENEMY_STATE_JUMP,
	eENEMY_STATE_ATTACK,
	eENEMY_STATE_TRAPPED,
	eENEMY_STATE_DEAD,
	eENEMY_STATE_MAX
} eENEMY_STATE;

typedef struct {
	/* OBJECT */
	eENEMY_STATE	state;
	stOBJECT		obj;

	/*
	start_timer
	proximity to player
	is_angry
	*/
} stENEMY;

#endif
