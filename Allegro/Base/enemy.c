
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
        stENEMY* new_enemy = &enemy[i];

        if (!new_enemy->obj.rend.is_active) {
            stOBJECT* obj = &new_enemy->obj;
            stPHYSICS* phy = &obj->phy;
            stCOLLISION* coll = &obj->coll;

            obj->rend.is_active = true;
            phy->pos.x = x;
            phy->pos.y = y;

            switch (type) {
            case eENEMY_TYPE_BASIC:
                phy->speed.x = 0;
                phy->speed.y = 0;
                coll->box.width = 1;          // temp val. it should change later
                coll->box.height = 1;         // temp val. it should change later
                new_enemy->trapped_timer = 30;
                break;
            case eENEMY_TYPE_THROW:
                phy->speed.x = 0;
                phy->speed.y = 0;
                coll->box.width = 1;          // temp val. it should change later
                coll->box.height = 1;         // temp val. it should change later
                new_enemy->trapped_timer = 30;
                break;
            case eENEMY_TYPE_BOSS:
                phy->speed.x = 0;
                phy->speed.y = 0;
                coll->box.width = 1;          // temp val. it should change later
                coll->box.height = 1;         // temp val. it should change later
                new_enemy->trapped_timer = 1;
                break;
            }

            coll->tag = eOBJ_TAG_ENEMY;
            coll->is_static = false;

            obj->rend.is_active = 1;

            new_enemy->state = eENEMY_STATE_IDLE;
            new_enemy->type = type;

            new_enemy->state_timer = 0;
            new_enemy->is_angry = false;

            return new_enemy;
        }
    }
    return NULL; // fail to create. no room for pool
}

void Enemy_ChangeState(stENEMY* e, eENEMY_STATE newState) {
    if (e == NULL) return;

    e->state = newState;
    e->state_timer = 0;
}
eENEMY_STATE Enemy_GetCurrentState(stENEMY* e) {
    if (e == NULL) return;
    return e->state;
}

// mob idle only when begin -> move immidately
void Enemy_UpdateIdle(stENEMY* e) {
    if (e == NULL) return;
    Enemy_ChangeState(e, eENEMY_STATE_MOVE);
}

void Enemy_UpdateMove(stENEMY* e) {
    if (e == NULL) return;

    stPHYSICS* phy = &e->obj.phy;

    phy->pos.x += phy->speed.x;
    phy->pos.y += phy->speed.y;
}

void Enemy_UpdateAttack(stENEMY* e) {
    if (e == NULL) return;

}
void Enemy_UpdateTrapped(stENEMY* e) {
    if (e == NULL) return;

}

// it doesn't reset all things. only active arr state
void Enemy_UpdateDead(stENEMY* e) {
    if (e == NULL) return;
    int index = enemy - e;
    (e + index)->obj.rend.is_active = false;
    //enemy->state = eENEMY_STATE_DEAD;
}

void Enemy_Update(stENEMY* e) {
    if (e == NULL) return;

    // just maintain timer in Update func
    // i thought it make simple
    e->state_timer++;

    switch (e->state) {
    case eENEMY_STATE_IDLE:
        Enemy_UpdateIdle(e);
        break;
    case eENEMY_STATE_MOVE:
        Enemy_UpdateMove(e);
        break;
    case eENEMY_STATE_JUMP:
        Enemy_UpdateJump(e);
        break;
    case eENEMY_STATE_ATTACK:
        Enemy_UpdateAttack(e);
        break;
    case eENEMY_STATE_TRAPPED:
        Enemy_UpdateTrapped(e);
        break;
    case eENEMY_STATE_DEAD:
        Enemy_UpdateDead(e);
        break;
    default:
        break;
    }
}

void Enemy_UpdateAll(void) {
    for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; i++) {
        stENEMY* e = &enemy[i];
        if (e->obj.rend.is_active) {
            Enemy_Update(e);
        }
    }
}

void Enemy_DecideNextAction(stENEMY* e) {
    if (e == NULL) return;

}
void Enemy_MoveTowardPlayer(stENEMY* e, int x, int y) {
    if (e == NULL) return;

}

void Enemy_Move(stENEMY* e) {
    if (e == NULL) return;

}
void Enemy_Jump(stENEMY* e) {
    if (e == NULL) return;

}
void Enemy_Throw(stENEMY* e) {
    if (e == NULL) return;

}

stOBJECT* Throw_Create(int x, int y) {
    stOBJECT* obj;

    stPHYSICS* phy = &obj->phy;
    stCOLLISION* coll = &obj->coll;
    stRENDER* rend = &obj->rend;

    rend->is_active = true;

    phy->pos.x = x;
    phy->pos.y = y;
    phy->speed.x = 0;
    phy->speed.y = 0;

    coll->box.width = 1;                    // temp val. it should change later
    coll->box.height = 1;                   // temp val. it should change later
    coll->tag = eOBJ_TAG_ENEMY_ATTACK;
    coll->is_static = false;

    rend->is_active = 1;

    return obj;
}

void Throw_Update(stTHROW* throw) {


}


void Throw_MoveTowardPlayer(stOBJECT* throw, stPLAYER* player) {
    stPHYSICS* t_phy = &throw->phy;
    stPHYSICS* p_phy = &player->obj.phy;

    float dx = p_phy->pos.x - t_phy->pos.x;
    float dy = p_phy->pos.y - t_phy->pos.y;

    float dist = sqrt(dx * dx + dy * dy);
    int   speed = 10;

    if (dist > 0) {
        t_phy->speed.x = (dx / dist) * speed;
        t_phy->speed.y = (dy / dist) * speed;
    }

    t_phy->pos.x += t_phy->speed.x;
    t_phy->pos.y += t_phy->speed.y;
}

void Throw_Destroy(stTHROW* throw) {


}

#endif