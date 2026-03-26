
#if 01

#include "enemy.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define MAX_ENEMIES (6)

static stENEMY enemy_pool[MAX_ENEMIES];			// for make
static bool enemy_active[MAX_ENEMIES];          // manage active mob to arr

void Enemy_InitializePool(void) { 
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemy_active[i] = false; // make sure defalt setting to 0
    }
}

int Enemy_GetActiveCount(void) {
    int count = 0;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_active[i]) {
            count++;
        }
    }
    return count;
}

stENEMY* Enemy_Create(eENEMY_TYPE type, int x, int y) {


}
void Enemy_Destroy(stENEMY* enemy) {
    if (enemy == NULL) return;


}

void Enemy_ChangeState(stENEMY* enemy, eENEMY_STATE newState) {
    if (enemy == NULL) return;

}
eENEMY_STATE Enemy_GetCurrentState(stENEMY* enemy) {
    if (enemy == NULL) return;

}

void Enemy_UpdateIdle(stENEMY* enemy) {
    if (enemy == NULL) return;

}
void Enemy_UpdateMove(stENEMY* enemy) {
    if (enemy == NULL) return;

}
void Enemy_UpdateJump(stENEMY* enemy) {
    if (enemy == NULL) return;

}
void Enemy_UpdateAttack(stENEMY* enemy) {
    if (enemy == NULL) return;

}
void Enemy_UpdateTrapped(stENEMY* enemy) {
    if (enemy == NULL) return;

}
void Enemy_UpdateDead(stENEMY* enemy) {
    if (enemy == NULL) return;

}

void Enemy_Update(stENEMY* enemy) {
    if (enemy == NULL) return;

}
void Enemy_UpdateAll(void) {

}

bool Enemy_IsPlayerNearby(stENEMY* enemy, int range) {
    if (enemy == NULL) return;

}
void Enemy_DecideNextAction(stENEMY* enemy) {
    if (enemy == NULL) return;

}
void Enemy_MoveTowardPlayer(stENEMY* enemy) {
    if (enemy == NULL) return;

}

void Enemy_Move(stENEMY* enemy) {
    if (enemy == NULL) return;

}
void Enemy_Jump(stENEMY* enemy) {
    if (enemy == NULL) return;

}
void Enemy_Throw(stENEMY* enemy) {
    if (enemy == NULL) return;

}
void Enemy_TakeDamage(stENEMY* enemy, int damage) {
    if (enemy == NULL) return;
}

#endif