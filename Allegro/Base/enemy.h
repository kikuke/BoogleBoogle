#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "object.h"
#include <stdbool.h>

typedef enum {
	eENEMY_STATE_IDLE = 0,
	eENEMY_STATE_MOVE,
	eENEMY_STATE_JUMP,
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
	eENEMY_STATE	state;
	eENEMY_TYPE		type;	// type of mob
	
	int state_timer;		// 
	//int proximity_to_player;// just in case
	int trapped_timer;		// count down of bubble escape
	bool is_angry;			// angry state
	
} stENEMY;

//stENEMY
//├── stOBJECT obj
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
//│       └── int is_active
//├── eENEMY_STATE state
//│   ├── eENEMY_STATE_IDLE(0)
//│   ├── eENEMY_STATE_MOVE(1)
//│   ├── eENEMY_STATE_JUMP(2)
//│   ├── eENEMY_STATE_ATTACK(3)
//│   ├── eENEMY_STATE_TRAPPED(4)
//│   ├── eENEMY_STATE_DEAD(5)
//│   └── eENEMY_STATE_MAX(6)
//├── eENEMY_TYPE type
//│   ├── eENEMY_TYPE_BASIC(0)
//│   ├── eENEMY_TYPE_THROW(1)
//│   └── eENEMY_TYPE_BOSS(2)
//├── int state_timer
//├── int proximity_to_player
//├── int trapped_timer
//└── bool is_angry



// pool management
void Enemy_InitializePool(stENEMY* enemy);								// initialize enemy pool when begin
int Enemy_GetActiveCount(stENEMY* enemy);									// get number of active enemies

// single enemy manage
stENEMY* Enemy_Create(stENEMY* enemy, eENEMY_TYPE type, int x, int y);			// create n initialize an enemy
//void Enemy_Destroy(stENEMY* enemy);								// destroy an enemy

// state manage
void Enemy_ChangeState(stENEMY* enemy, eENEMY_STATE newState);	// change the enemy's current state
eENEMY_STATE Enemy_GetCurrentState(stENEMY* enemy);				// get the enemy's current state

// state logic handler
// They define what the enemy continuously does while it is in a specific state, 
// and when it should transition out of that state
// They are called every frame by Enemy_Update loop
void Enemy_UpdateIdle(stENEMY* enemy);							// 0 update IDLE state
void Enemy_UpdateMove(stENEMY* enemy);							// 0 update MOVE state
void Enemy_UpdateAttack(stENEMY* enemy);						// 0 update ATTACK state
void Enemy_UpdateTrapped(stENEMY* enemy);						// 0 update TRAPPED (bubble) state
void Enemy_UpdateDead(stENEMY* enemy, stENEMY* e);							// update DEAD state

// for main update
void Enemy_Update(stENEMY* enemy, stENEMY* e);								// update single enemy
void Enemy_UpdateAll(stENEMY* enemy);										// update all active enemies

// AI n behavior
void Enemy_DecideNextAction(stENEMY* enemy);					// 0 decide the next action based on AI
void Enemy_MoveTowardPlayer(stENEMY* enemy, int x, int y);		// 0 move enemy toward the player

// atomic actions
// define how a single, specific action or behavior is physically performed
// they are called when a specific action is needed
// No State Decision: These functions typically do not decide state transitions themselves. 
// They just perform their job.
void Enemy_Move(stENEMY* enemy);								// 0 move enemy (generic movement)
void Enemy_Throw(stENEMY* enemy);								// 0 make enemy Throw

// throw maintain
stOBJECT* Throw_Create(stOBJECT* obj, int x, int y);							// 0 create throw obj
void Throw_Update(stOBJECT* throw, stPLAYER* player);								// 0 update throw
void Throw_MoveTowardPlayer(stOBJECT* throw, stPLAYER* player);					// 0 keep forward to player
void Throw_Destroy(stOBJECT* throw);								// 0 destroy touch player or out of map


// make random number between 1 to 9
int Get_RandNum_1_to_9(void);

#endif
