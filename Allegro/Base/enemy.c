
#if 01

#include "enemy.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define MAX_ENEMIES (6)
#define REMAIN_TRAPPED_TIMER (30)

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
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemy_active[i]) {
            stENEMY* enemy = &enemy_pool[i];
            enemy_active[i] = true;

            enemy->obj.phy.pos.x = x;
            enemy->obj.phy.pos.y = y;
            enemy->obj.phy.speed.x = 0;
            enemy->obj.phy.speed.y = 0;

            enemy->obj.coll.box.width = 32;
            enemy->obj.coll.box.height = 32;
            enemy->obj.coll.tag = eOBJ_TAG_ENEMY;
            enemy->obj.coll.is_static = false;

            enemy->obj.rend.is_active = 1;

            enemy->state = eENEMY_STATE_IDLE;
            enemy->type = type;

            enemy->state_timer = 0;
            enemy->trapped_timer = 0;
            enemy->proximity_to_player = 0;
            enemy->is_angry = false;

            return enemy;
        }
    }
    return NULL; // 생성 실패 (풀에 여유 공간 없음)
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