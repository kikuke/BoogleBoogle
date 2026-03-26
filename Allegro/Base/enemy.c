
#if 01

#include "enemy.h"
#include "player.h"
#include "bugglebuggle.h"
#include "game_manager.c"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

void Enemy_InitializePool(void) { 
    for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; i++) {
        (enemy+i)->obj.rend.is_active = 0; // make sure defalt setting to 0
    }
}

int Enemy_GetActiveCount(void) {
    int count = 0;
    for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; i++) {
        if ((enemy + i)->obj.rend.is_active) {
            count++;
        }
    }
    return count;
}

// input type: BASIC, THROW, BOSS  &  x, y
stENEMY* Enemy_Create(eENEMY_TYPE type, int x, int y) {
    for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; i++) {
        if (!(enemy + i)->obj.rend.is_active) {
            stENEMY* enemy = &enemy[i];
            (enemy + i)->obj.rend.is_active = true;

            enemy->obj.phy.pos.x = x;
            enemy->obj.phy.pos.y = y;
            switch (type) {
            case eENEMY_TYPE_BASIC:
                enemy->obj.phy.speed.x = 0;
                enemy->obj.phy.speed.y = 0;
                enemy->obj.coll.box.width = 1;          // temp val. it should change later
                enemy->obj.coll.box.height = 1;         // temp val. it should change later
                enemy->trapped_timer = 30;
            case eENEMY_TYPE_THROW:
                enemy->obj.phy.speed.x = 0;
                enemy->obj.phy.speed.y = 0;
                enemy->obj.coll.box.width = 1;          // temp val. it should change later
                enemy->obj.coll.box.height = 1;         // temp val. it should change later
                enemy->trapped_timer = 30;
            case eENEMY_TYPE_BOSS:
                enemy->obj.phy.speed.x = 0;
                enemy->obj.phy.speed.y = 0;
                enemy->obj.coll.box.width = 1;          // temp val. it should change later
                enemy->obj.coll.box.height = 1;         // temp val. it should change later
                enemy->trapped_timer = 1;
            }
            
            enemy->obj.coll.tag = eOBJ_TAG_ENEMY;
            enemy->obj.coll.is_static = false;

            enemy->obj.rend.is_active = 1;

            enemy->state = eENEMY_STATE_IDLE;
            enemy->type = type;

            enemy->state_timer = 0;
            enemy->is_angry = false;

            return enemy;
        }
    }
    return NULL; // fail to create. no room for pool
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
    enemy->obj.phy.pos.x += enemy->obj.phy.speed.x;
    enemy->obj.phy.pos.y += enemy->obj.phy.speed.y;
}
void Enemy_UpdateJump(stENEMY* enemy) {
    if (enemy == NULL) return;
    enemy->obj.phy.speed.y = -3;                    // temp val, it means go up 
    Enemy_ChangeState(enemy, eENEMY_STATE_JUMP);
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
    int index = enemy - enemy;
    (enemy + index)->obj.rend.is_active = false;
    //enemy->state = eENEMY_STATE_DEAD;
}

void Enemy_Update(stENEMY* enemy) {
    if (enemy == NULL) return;

    // just maintain timer in Update func
    // i thought it make simple
    enemy->state_timer++;

    switch (enemy->state) {
    case eENEMY_STATE_IDLE:
        Enemy_UpdateIdle(enemy);
        break;
    case eENEMY_STATE_MOVE:
        Enemy_UpdateMove(enemy);
        break;
    case eENEMY_STATE_JUMP:
        Enemy_UpdateJump(enemy);
        break;
    case eENEMY_STATE_ATTACK:
        Enemy_UpdateAttack(enemy);
        break;
    case eENEMY_STATE_TRAPPED:
        Enemy_UpdateTrapped(enemy);
        break;
    case eENEMY_STATE_DEAD:
        Enemy_UpdateDead(enemy);
        break;
    default:
        break;
    }
}

void Enemy_UpdateAll(void) {
    for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; i++) {
        if ((enemy + i)->obj.rend.is_active) {
            stENEMY* enemy = &enemy[i];
            Enemy_Update(enemy);
        }
    }
}

void Enemy_DecideNextAction(stENEMY* enemy) {
    if (enemy == NULL) return;

}
void Enemy_MoveTowardPlayer(stENEMY* enemy, int x, int y) {
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

// maintain Throw to arr? or linked list?
stOBJECT* Throw_Create(int x, int y) {
    stOBJECT* throw;
    throw->rend.is_active = true;

    throw->phy.pos.x = x;
    throw->phy.pos.y = y;
    throw->phy.speed.x = 0;
    throw->phy.speed.y = 0;
    throw->coll.box.width = 1;          // temp val. it should change later
    throw->coll.box.height = 1;         // temp val. it should change later
    
    throw->coll.tag = eOBJ_TAG_ENEMY_ATTACK;
    throw->coll.is_static = false;

    throw->rend.is_active = 1;

    return throw;
}

void Throw_Update(stTHROW* throw) {


}


void Throw_MoveTowardPlayer(stOBJECT* throw, stPLAYER* player) {

    float dx = player->obj.phy.pos.x - throw->phy.pos.x;
    float dy = player->obj.phy.pos.y - throw->phy.pos.y;
    float dist = sqrt(dx * dx + dy * dy);
    int speed = 10;

    if (dist > 0) {
        throw->phy.speed.x = (dx / dist) * speed;
        throw->phy.speed.y = (dy / dist) * speed;
    }
    throw->phy.pos.x += throw->phy.speed.x;
    throw->phy.pos.y += throw->phy.speed.y;
}

void Throw_Destroy(stTHROW* throw) {


}

#endif