
#if 01

#include "enemy.h"
#include "bugglebuggle.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <stddef.h> 

int Get_RandNum_1_to_9(void) {
    static uint32_t x = 123456789;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return (x % 9) + 1;
}

void Enemy_InitializePool(stENEMY *enemy) {
    for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; i++) {
        (enemy+i)->obj.rend.is_active = 0; // make sure defalt setting to 0
    }
}

int Enemy_GetActiveCount(stENEMY* enemy) {
    int count = 0;
    for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; i++) {
        if ((enemy + i)->obj.rend.is_active) {
            count++;
        }
    }
    return count;
}

// input type: BASIC, THROW, BOSS  &  x, y
stENEMY* Enemy_Create(stENEMY* enemy, eENEMY_TYPE type, int x, int y) {
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
    if (e == NULL) return eENEMY_STATE_MAX;
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
void Enemy_UpdateDead(stENEMY* enemy, stENEMY* e) {
    if (e == NULL) return;
    ptrdiff_t index = e - enemy;

    if (index >= 0 && index < CONFIG_OBJECT_ENEMY_MAX) {
        e->obj.rend.is_active = false;
    }
}

void Enemy_Update(stENEMY* enemy, stENEMY* e) {
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
    case eENEMY_STATE_ATTACK:
        Enemy_UpdateAttack(e);
        break;
    case eENEMY_STATE_TRAPPED:
        Enemy_UpdateTrapped(e);
        break;
    case eENEMY_STATE_DEAD:
        Enemy_UpdateDead(enemy, e);
        break;
    default:
        break;
    }
}

void Enemy_UpdateAll(stENEMY* enemy) {
    for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; i++) {
        stENEMY* e = &enemy[i];
        if (e->obj.rend.is_active) {
            Enemy_Update(enemy, e);
        }
    }
}
// reference
//extern bool IsHittingWall(double x, double y);
//extern bool HasGroundAhead(double x, double y, int dir);
//void update_patrol_movement(Monster* m, Map* map, Player* p) {
//    // if angry -> faster
//    float current_speed = m->base_speed * (m->is_angry ? 1.8f : 1.0f);
//    m->vx = m->dir * current_speed;
//
//    // wall collidion check n dir change
//    if (is_hitting_wall(map, m->x + m->vx, m->y)) {
//        m->dir *= -1; 
//        m->vx = m->dir * current_speed;
//    }
//
//    // jump logic
//    // - if infront of fall || go up wall
//    // - if player high floor -> 5%chance jump
//    if (m->state == ST_WALK && can_jump(m)) {
//        if (!has_ground_ahead(map, m->x, m->y, m->dir) |
//
//            |
//            (p->y < m->y && rand() % 100 < 5)) { 
//            m->vy = -JUMP_FORCE;
//            m->state = ST_JUMP;
//        }
//    }
//
//    m->x += m->vx;
//    m->y += m->vy;
//}
/*
void Enemy_ToPlayer_Ground(stENEMY* enemy, stPLAYER* player) {
    if (enemy == NULL || player == NULL) return;

    stPOSITION* e_pos = &enemy->obj.phy.pos;
    stPOSITION* e_speed = &enemy->obj.phy.speed;
    eDIR_LOOK* e_look = &enemy->obj.phy.look;

    float begin_speed = 1.0f;
    float cur_speed = begin_speed * (enemy->is_angry ? 1.8f : 1.0f);

    if (e_speed->x == 0) e_speed->x = cur_speed;

    int dir = (e_speed->x > 0) ? 1 : -1;
    e_speed->x = dir * cur_speed;

    // wall collision check and reflect dir
    if (IsHittingWall(e_pos->x + e_speed->x, e_pos->y)) {
        e_speed->x *= -1; // reflect dir
        if (*e_look == 0) *e_look = 1;
        if (*e_look == 1) *e_look = 0;
    }

    // jump logic
    if (enemy->state == eENEMY_STATE_MOVE) {

        if (!HasGroundAhead(e_pos->x, e_pos->y, dir) 
            || (player->obj.phy.pos.y < e_pos->y && (rand() % 100 < 5))) {
            e_speed->y = -5.0f;
            Enemy_ChangeState(enemy, eENEMY_STATE_MOVE);
            // chage to MOVE!
        }
    }

    // this part going to replace MOVE func that share with player
    e_pos->x += e_speed->x;
    e_pos->y += e_speed->y;

}

// reference
//float speed = m->base_speed * (m->is_angry ? 2.0f : 1.0f);
//
//if (is_hitting_wall(map, m->x + m->vx, m->y)) m->vx *= -1; 
//if (is_hitting_wall(map, m->x, m->y + m->vy)) m->vy *= -1;
//
//m->x += m->vx;
//m->y += m->vy;

// bool IsHittingWall(double x, double y);

void Enemy_ToPlayer_Fly(stENEMY* enemy, stPLAYER* player) {
    if (enemy == NULL) return;

    stPOSITION* e_pos = &enemy->obj.phy.pos;
    stPOSITION* e_speed = &enemy->obj.phy.speed;
    eDIR_LOOK* e_look = &enemy->obj.phy.look;


    float begin_speed = 1.0f;
    float speed_mul = (enemy->is_angry ? 2.0f : 1.0f);

    // if speed == 0 -> setting
    if (e_speed->x == 0) e_speed->x = begin_speed * speed_mul;
    if (e_speed->y == 0) e_speed->y = begin_speed * speed_mul;

    // reflect on collision -> keep 45 degree
    if (IsHittingWall(e_pos->x + e_pos->x, e_pos->y)) {
        e_speed->x *= -1; // reflect left/right
        if (*e_look == 0) *e_look = 1;
        if (*e_look == 1) *e_look = 0;
    }
    if (IsHittingWall(e_pos->x, e_pos->y + e_pos->y)) {
        e_speed->y *= -1; // reflect up/down
        if (*e_look == 2) *e_look = 3;
        if (*e_look == 3) *e_look = 2;
    }

    // this part going to replace MOVE func that share with player
    e_pos->x += e_speed->x;
    e_pos->y += e_speed->y;

}*/


void Enemy_Throw(stENEMY* e) {
    if (e == NULL) return;

}

stOBJECT* Throw_Create(stOBJECT* obj, int x, int y) {

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

void Throw_Update(stOBJECT* throw, stPLAYER* player) {
    int* t_active = &throw->rend.is_active;
    stPOSITION* t_pos = &throw->phy.pos;

    if (throw == NULL || !t_active) return;
    Throw_MoveTowardPlayer(throw, player);
    if (t_pos->x < 0 || t_pos->x > CONFIG_MAP_X_MAX ||
        t_pos->y < 0 || t_pos->y > CONFIG_MAP_Y_MAX) {
        t_active = false;
    }
}


void Throw_MoveTowardPlayer(stOBJECT* throw, stPLAYER* player) {
    stPOSITION* p_pos = &player->obj.phy.pos;
    stPOSITION* t_pos = &throw->phy.pos;
    stPOSITION* t_speed = &throw->phy.speed;

    float dx = (float)p_pos->x - (float)t_pos->x;
    float dy = (float)p_pos->y - (float)t_pos->y;

    float dist = (float)sqrt(dx * dx + dy * dy);
    int speed = 10;

    if (dist > 0) {
        t_speed->x = (dx / dist) * speed;
        t_speed->y = (dy / dist) * speed;
    }

    t_pos->x += t_speed->x;
    t_pos->y += t_speed->y;
}



#endif