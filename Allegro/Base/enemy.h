//#ifndef __ENEMY_H__
//#define __ENEMY_H__
//#include "object.h"
//#include <stdbool.h>
//
//typedef enum {
//	eENEMY_STATE_IDLE = 0,
//	eENEMY_STATE_MOVE,
//	eENEMY_STATE_JUMP,
//	eENEMY_STATE_ATTACK,
//	eENEMY_STATE_TRAPPED,
//	eENEMY_STATE_DEAD,
//	eENEMY_STATE_MAX		// no mean. number of STATE
//} eENEMY_STATE;
//
//typedef enum {
//	eENEMY_TYPE_BASIC = 0,	// normal mob
//	eENEMY_TYPE_THROW,		// throw mob
//	eENEMY_TYPE_BOSS		// boss mob
//}eENEMY_TYPE;
//
//typedef struct {
//	/* OBJECT */
//	stOBJECT		obj;
//	eENEMY_STATE	state;
//	eENEMY_TYPE		type;	// type of mob
//	
//	int state_timer;		//
//	int proximity_to_player;// just in case
//	int trapped_timer;		// count down of bubble escape
//	bool is_angry;			// angry state
//	
//} stENEMY;
//
//typedef struct {
//	stOBJECT obj;
//	int damage;			// damage
//	int lifetime_timer;	// obj disappear timer
//	// i added cause it's simple then calc map boundary
//	//int owner_tag;		// it make use eENEMY_THROW both player n mob
//
//}eENEMY_THROW;
//
//// pool management
//void Enemy_InitializePool(void);								// initialize enemy pool when begin
//int Enemy_GetActiveCount(void);									// get number of active enemies
//
//// single enemy manage
//stENEMY* Enemy_Create(eENEMY_TYPE type, int x, int y);			// create n initialize an enemy
//void Enemy_Destroy(stENEMY* enemy);								// destroy n free memory for an enemy
//
//// state manage
//void Enemy_ChangeState(stENEMY* enemy, eENEMY_STATE newState);	// change the enemy's current state
//eENEMY_STATE Enemy_GetCurrentState(stENEMY* enemy);				// get the enemy's current state
//
//// update state
//void Enemy_UpdateIdle(stENEMY* enemy);							// update IDLE state
//void Enemy_UpdateMove(stENEMY* enemy);							// update MOVE state
//void Enemy_UpdateJump(stENEMY* enemy);							// update JUMP state
//void Enemy_UpdateAttack(stENEMY* enemy);						// update ATTACK state
//void Enemy_UpdateTrapped(stENEMY* enemy);						// update TRAPPED (bubble) state
//void Enemy_UpdateDead(stENEMY* enemy);							// update DEAD state
//
//// for main update
//void Enemy_Update(stENEMY* enemy);								// update single enemy
//void Enemy_UpdateAll(void);										// update all active enemies
//
//// AI n behavior
//bool Enemy_IsPlayerNearby(stENEMY* enemy, int range);			// check if player is within a certain range
//void Enemy_DecideNextAction(stENEMY* enemy);					// decide the next action based on AI
//void Enemy_MoveTowardPlayer(stENEMY* enemy);					// move enemy towards the player
//
//// actions
//void Enemy_Move(stENEMY* enemy);								// move enemy (generic movement)
//void Enemy_Jump(stENEMY* enemy);								// make enemy jump
//void Enemy_Attack(stENEMY* enemy);								// make enemy attack
//void Enemy_TakeDamage(stENEMY* enemy, int damage);				// apply damage to enemy
//
//#endif
