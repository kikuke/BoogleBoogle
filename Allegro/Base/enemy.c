
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

// input type: BASIC, THROW, BOSS  &  x, y
stENEMY* Enemy_Create(eENEMY_TYPE type, int x, int y) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemy_active[i]) {
            stENEMY* enemy = &enemy_pool[i];
            enemy_active[i] = true;

            enemy->obj.phy.pos.x = x;
            enemy->obj.phy.pos.y = y;
            enemy->obj.phy.speed.x = 0;
            enemy->obj.phy.speed.y = 0;

            enemy->obj.coll.box.width = 1;          // temp val. it should change later
            enemy->obj.coll.box.height = 1;         // temp val. it should change later
            enemy->obj.coll.tag = eOBJ_TAG_ENEMY;
            enemy->obj.coll.is_static = false;

            enemy->obj.rend.is_active = 1;

            enemy->state = eENEMY_STATE_IDLE;
            enemy->type = type;

            enemy->state_timer = 0;
            enemy->trapped_timer = REMAIN_TRAPPED_TIMER;
            enemy->proximity_to_player = 0;
            enemy->is_angry = false;

            return enemy;
        }
    }
    return NULL; // fail to create. no room for pool
}

// useless. if u wanna delete -> Enemy_UpdateDead!
void Enemy_Destroy(stENEMY* enemy) {
    if (enemy == NULL) return;
}

void Enemy_ChangeState(stENEMY* enemy, eENEMY_STATE newState) {
    if (enemy == NULL) return;

    enemy->state = newState;
    enemy->state_timer = 0;
}
eENEMY_STATE Enemy_GetCurrentState(stENEMY* enemy) {
    if (enemy == NULL) return;
    return enemy->state;
}

// mob idle only when begin -> move immidately
void Enemy_UpdateIdle(stENEMY* enemy) {
    if (enemy == NULL) return;
    Enemy_ChangeState(enemy, eENEMY_STATE_MOVE);
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

// it doesn't reset all things. only active arr state
void Enemy_UpdateDead(stENEMY* enemy) {
    if (enemy == NULL) return;
    int index = enemy - enemy_pool;
    enemy_active[index] = false;
    //enemy->state = eENEMY_STATE_DEAD;
}

void Enemy_Update(stENEMY* enemy) {
    if (enemy == NULL) return;

}
void Enemy_UpdateAll(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemy_active[i]) {
            stENEMY* enemy = &enemy_pool[i];

            switch (enemy->state) {
            case eENEMY_STATE_IDLE:
                break;
                    


            }

        }
    }
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