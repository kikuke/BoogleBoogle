#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "object.h"
#include "player.h"
#include <stdbool.h>

typedef enum {
	eENEMY_STATE_IDLE = 0,
	eENEMY_STATE_MOVE,
	eENEMY_STATE_ATTACK,
	eENEMY_STATE_TRAPPED,
	eENEMY_STATE_DEAD,
	eENEMY_STATE_MAX		// no mean. number of STATE -> for (..i<eENEMY_MAX..)
} eENEMY_STATE;

typedef enum {
	eENEMY_TYPE_BASIC = 0,	// normal mob
	eENEMY_TYPE_THROW,		// throw mob
	eENEMY_TYPE_BOSS		// boss mob
}eENEMY_TYPE;

typedef struct {
	/* OBJECT */
	stOBJECT		obj;	// collsion, p
	eENEMY_TYPE		type;	// type of mob
	eENEMY_STATE	state;
	
	int state_timer;		// 
	int trapped_timer;		// count down of bubble escape
	int throw_timer;		// 
	bool is_angry;			// angry state
	
} stENEMY;

//stENEMY
//├── stOBJECT obj
//│   ├── bool is_active
//│   ├── stCOLLISION coll
//│   │   ├── bool is_static
//│   │   ├── eOBJ_TAG tag
//│   │   │   ├── eOBJ_TAG_TILE(0)
//│   │   │   ├── eOBJ_TAG_PLAYER(1)
//│   │   │   ├── eOBJ_TAG_ENEMY(2)
//│   │   │   ├── eOBJ_TAG_BUBBLE(3)
//│   │   │   ├── eOBJ_TAG_ENEMY_ATTACK(4)
//│   │   │   └── eOBJ_TAG_MAX(5)
//│   │   └── stCOLLISION_BOX box
//│   │       ├── double height
//│   │       └── double width
//│   ├── stPHYSICS phy
//│   │   ├── bool is_move
//│   │   ├── bool is_gravity
//│   │   ├── eDIR_LOOK look
//│   │   │   ├── eDIR_LOOK_RIGHT(0)
//│   │   │   ├── eDIR_LOOK_LEFT(1)
//│   │   │   ├── eDIR_LOOK_UP(2)
//│   │   │   ├── eDIR_LOOK_DOWN(3)
//│   │   │   └── eDIR_LOOK_MAX(4)
//│   │   ├── stPOSITION pos
//│   │   │   ├── double x
//│   │   │   └── double y
//│   │   └── stPOSITION speed
//│   │       ├── double x
//│   │       └── double y
//│   └── stRENDER rend
//│       ├── int is_active
//│       └── bool is_move
//├── eENEMY_STATE state
//│   ├── eENEMY_STATE_IDLE(0)
//│   ├── eENEMY_STATE_MOVE(1)
//│   ├── eENEMY_STATE_ATTACK(3)
//│   ├── eENEMY_STATE_TRAPPED(4)
//│   ├── eENEMY_STATE_DEAD(5)
//│   └── eENEMY_STATE_MAX(6)
//├── eENEMY_TYPE type
//│   ├── eENEMY_TYPE_BASIC(0)
//│   ├── eENEMY_TYPE_THROW(1)
//│   └── eENEMY_TYPE_BOSS(2)
//├── int state_timer
//├── int trapped_timer
//├── int throw_timer
//└── bool is_angry

// make random number between 1 to 9
int Get_RandNum_1_to_9(void);

// single enemy manage
stENEMY* Enemy_Create(stENEMY* enemy, eENEMY_TYPE type, int x, int y);			// create n initialize an enemy

// for main update
void Enemy_Update(stENEMY* enemy, stOBJECT* target_player, stOBJECT* throw);	// update single enemy
void Throw_Update(stOBJECT* throw, stOBJECT* target_player);					// update throw

// Update state all
void Enemy_Update_ALL(stENEMY* enemy, stPLAYER* player, stOBJECT* throw);
void Throw_Update_ALL(stENEMY* enemy, stPLAYER* player, stOBJECT* throw);

// state manage
void Enemy_ChangeState(stENEMY* enemy, eENEMY_STATE newState);					// change the enemy's current state
eENEMY_STATE Enemy_GetCurrentState(stENEMY* enemy);								// get the enemy's current state

// state logic handler
// They define what the enemy continuously does while it is in a specific state, 
// and when it should transition out of that state
// They are called every frame by Enemy_Update loop
void Enemy_UpdateIdle(stENEMY* enemy);											// update IDLE state
void Enemy_UpdateMove(stENEMY* enemy, stOBJECT* target_player);					// update MOVE state
void Enemy_UpdateAttack(stENEMY* enemy, stOBJECT* target_player, stOBJECT* throw);// update ATTACK state
void Enemy_UpdateTrapped(stENEMY* enemy);										// update TRAPPED (bubble) state

// AI n behavior
void Enemy_ToPlayer_Ground(stENEMY* enemy, stOBJECT* target_player);			// move enemy toward the player
void Enemy_ToPlayer_Fly(stENEMY* enemy, stOBJECT* target_player);				// move enemy toward the player

// atomic actions
void Enemy_Throw(stENEMY* enemy, stOBJECT* target_player, stOBJECT* throw);		// make enemy Throw

// throw maintain
stOBJECT* Throw_Create(stOBJECT* throw, int x, int y);							// create throw obj
void Throw_MoveTowardPlayer(stOBJECT* throw, stOBJECT* target_player);			// keep forward to player

// make random number between 1 to 9
int Get_RandNum_1_to_9(void);

#endif
