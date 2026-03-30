
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

#include "enemy.h"
#include "bugglebuggle.h"
#include "collision.h"
#include "game_manager.h"

/************************************************/
/*         Local Function Declaration           */
/************************************************/
static int Get_RandNum_1_to_9(void);

/************************************************/
/*          Global Function Definition          */
/************************************************/

// input type: BASIC, THROW, BOSS  &  x, y
stENEMY* Enemy_Create(stENEMY* enemy_pool, eENEMY_TYPE type, int x, int y) {
    for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; i++) {
        stENEMY* new_enemy = &enemy_pool[i];

        if (!new_enemy->obj.is_active) {
            stOBJECT* obj = &new_enemy->obj;
            stPHYSICS* phy = &obj->phy;
            stCOLLISION* coll = &obj->coll;
            stRENDER* rend = &obj->rend;

            obj->is_active = true;
            rend->is_active = true;
            rend->is_move = true;
            phy->is_gravity = true;

            coll->tag = eOBJ_TAG_ENEMY;
            coll->is_static = false;

            new_enemy->state = eENEMY_STATE_IDLE;
            new_enemy->type = type;

            new_enemy->state_timer = 0;
            new_enemy->throw_timer = 0;
            new_enemy->is_angry = false;

            phy->speed.x = 0;
            phy->speed.y = 0;
            coll->box.width = PLAYER_W;
            coll->box.height = PLAYER_W;

            phy->pos.x = x;
            phy->pos.y = y;

            switch (type) {
            case eENEMY_TYPE_BASIC:
                new_enemy->trapped_timer = 300;
                break;
            case eENEMY_TYPE_THROW:
                new_enemy->trapped_timer = 300;
                break;
            case eENEMY_TYPE_BOSS:
                new_enemy->trapped_timer = 600;
                break;
            }

            return new_enemy;
        }
    }
    return NULL;
}

void Enemy_Update(stENEMY* e, stOBJECT* p, stOBJECT* t) {
    if (e == NULL || !e->obj.is_active) return;

    e->state_timer++;
    if (e->state_timer > 2000000000) e->state_timer = 0;

    if (e->type == eENEMY_TYPE_THROW && e->state != eENEMY_STATE_TRAPPED) {
        Enemy_UpdateAttack(e, p, t);
    }

    switch (e->state) {
    case eENEMY_STATE_IDLE:
        Enemy_UpdateIdle(e);
        break;
    case eENEMY_STATE_MOVE:
        Enemy_UpdateMove(e, p);
        break;
    case eENEMY_STATE_ATTACK:
        Enemy_UpdateAttack(e, p, t);
        break;
    case eENEMY_STATE_TRAPPED:
        Enemy_UpdateTrapped(e);
        break;
    default:
        break;
    }
}

void Throw_Update(stOBJECT* throw, stOBJECT* target_player) {
    int* t_active = &throw->rend.is_active;
    stPOSITION* t_pos = &throw->phy.pos;
    stPOSITION* t_speed = &throw->phy.speed;

    if (throw == NULL || !t_active) return;

    //Throw_MoveTowardPlayer(throw, target_player);

    t_pos->x += t_speed->x;
    t_pos->y += t_speed->y;

    if (t_pos->x < 0 || t_pos->x > CONFIG_MAP_X_MAX * CONFIG_OBJECT_COLLISION_TILE_SIZE ||
        t_pos->y < 0 || t_pos->y > CONFIG_MAP_Y_MAX * CONFIG_OBJECT_COLLISION_TILE_SIZE) {
        t_active = false;
        throw->is_active = false;
    }
}

void Enemy_ChangeState(stENEMY* e, eENEMY_STATE newState) {
    if (e == NULL || e->state == newState) return;

    e->state = newState;
    e->state_timer = 0;
}

void Enemy_Update_ALL(stENEMY* enemy, stPLAYER* player, stOBJECT* throw) {
    for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; i++) {
        if (enemy[i].obj.is_active) {
            Enemy_Update(&enemy[i], &player[0].obj, throw);
        }
    }
}
void Throw_Update_ALL(stENEMY* enemy, stPLAYER* player, stOBJECT* throw) {
    for (int i = 0; i < CONFIG_OBJECT_ENEMY_ATTACK_MAX; ++i) {
        if (throw[i].is_active) {
            Throw_Update(&throw[i], &player[0].obj);
        }
    }
}

/************************************************/
/*         Local Function Definition            */
/************************************************/

eENEMY_STATE Enemy_GetCurrentState(stENEMY* e) {
    if (e == NULL) return eENEMY_STATE_MAX;
    return e->state;
}

void Enemy_UpdateIdle(stENEMY* e) {
    if (e == NULL) return;
    Enemy_ChangeState(e, eENEMY_STATE_MOVE);
}

void Enemy_UpdateMove(stENEMY* e, stOBJECT* p) {
    if (e == NULL) return;
    switch (e->type) {
    case eENEMY_TYPE_BASIC:
        Enemy_ToPlayer_Ground(e, p);
        break;
    case eENEMY_TYPE_THROW:
    case eENEMY_TYPE_BOSS:
        Enemy_ToPlayer_Fly(e, p);
        break;
    default:
        break;
    }
}

// only fly mob can attack
void Enemy_UpdateAttack(stENEMY* e, stOBJECT* p, stOBJECT* t) {
    int* e_t_timer = &e->throw_timer;

    if (e == NULL || e->type != eENEMY_TYPE_THROW) return;

    if (*e_t_timer == 0) {
        *e_t_timer = 180 + (Get_RandNum_1_to_9() * 15);
    }

    if (e->state_timer >= *e_t_timer) {
        Enemy_Throw(e, p, t);
        e->state_timer = 0;
        *e_t_timer = 180 + (Get_RandNum_1_to_9() * 15);
    }
}

void Enemy_UpdateTrapped(stENEMY* e) {
    if (e == NULL) return;
    e->trapped_timer++;
}

void Enemy_ToPlayer_Ground(stENEMY* enemy, stOBJECT* target) {
    if (enemy == NULL || target == NULL) return;

    stPOSITION* e_pos = &enemy->obj.phy.pos;
    stPOSITION* e_speed = &enemy->obj.phy.speed;
    eDIR_LOOK* e_look = &enemy->obj.phy.look;

    float begin_speed = 1.0f;
    float cur_speed = begin_speed * (enemy->is_angry ? 1.8f : 1.0f);

    // is_move == false -> enemy hit the wall -> change dir
    if (enemy->state_timer > 1 && enemy->obj.rend.is_move == false) {
        if (*e_look == eDIR_LOOK_RIGHT) {
            *e_look = eDIR_LOOK_LEFT;
        }
        else {
            *e_look = eDIR_LOOK_RIGHT;
        }
    }

    if (*e_look == eDIR_LOOK_RIGHT || *e_look == eDIR_LOOK_UP || *e_look == eDIR_LOOK_DOWN) {
        *e_look = eDIR_LOOK_RIGHT;
        e_speed->x = cur_speed;
    }
    else {
        *e_look = eDIR_LOOK_LEFT;
        e_speed->x = -cur_speed;
    }

    if (enemy->state == eENEMY_STATE_MOVE && !enemy->obj.phy.is_jump) {
        // if char upper then enemy && 1 per second && 50% chance
        if ((target->phy.pos.y < e_pos->y - 10.0f) && (enemy->state_timer % 60 == 0) && (Get_RandNum_1_to_9() <= 5)) {
            e_speed->y = -10.0f;
            enemy->obj.phy.is_jump = true;
        }
    }
}

void Enemy_ToPlayer_Fly(stENEMY* enemy, stOBJECT* player) {
    if (enemy == NULL) return;

    stPOSITION* e_pos = &enemy->obj.phy.pos;
    stPOSITION* e_speed = &enemy->obj.phy.speed;
    eDIR_LOOK* e_look = &enemy->obj.phy.look;

    float begin_speed = 1.0f;
    float speed_mul = (enemy->is_angry ? 2.0f : 1.0f);
    float cur_speed = begin_speed * speed_mul;

    // no gravity
    enemy->obj.phy.is_gravity = false;

    if (enemy->state_timer > 1 && enemy->obj.rend.is_move == false) {
        if (*e_look == eDIR_LOOK_RIGHT) {
            *e_look = eDIR_LOOK_LEFT;
        }
        else {
            *e_look = eDIR_LOOK_RIGHT;
        }
    }

    if (*e_look == eDIR_LOOK_RIGHT || *e_look == eDIR_LOOK_UP || *e_look == eDIR_LOOK_DOWN) {
        *e_look = eDIR_LOOK_RIGHT;
        e_speed->x = cur_speed;
    }
    else {
        *e_look = eDIR_LOOK_LEFT;
        e_speed->x = -cur_speed;
    }

    if (enemy->state_timer > 1 && e_speed->y == 0.0f) {
        e_speed->y = -cur_speed;
    }

    if (e_pos->y <= 0.0f) {
        e_pos->y = 1.0f;
        e_speed->y = cur_speed;
    }

    if (e_speed->y == 0.0f) {
        e_speed->y = cur_speed;
    }
}

void Enemy_Throw(stENEMY* e, stOBJECT* p, stOBJECT* throw_pool) {
    if (e == NULL || p == NULL) return;
    stOBJECT* throw_obj = Throw_Create(throw_pool, (int)e->obj.phy.pos.x, (int)e->obj.phy.pos.y);
    
    if (throw_obj) {
        Throw_MoveTowardPlayer(throw_obj, p);
    }
}

stOBJECT* Throw_Create(stOBJECT* throw_pool, int x, int y) {
    for (int i = 0; i < CONFIG_OBJECT_ENEMY_ATTACK_MAX; i++) {
        stOBJECT* obj = &throw_pool[i];
        if (!obj->is_active) {
            stPHYSICS* phy = &obj->phy;
            stCOLLISION* coll = &obj->coll;
            stRENDER* rend = &obj->rend;

            obj->is_active = true;
            rend->is_active = true;

            phy->pos.x = x;
            phy->pos.y = y;
            phy->speed.x = 0;
            phy->speed.y = 0;

            coll->box.width = CONFIG_OBJECT_COLLISION_TILE_SIZE;
            coll->box.height = CONFIG_OBJECT_COLLISION_TILE_SIZE;
            coll->tag = eOBJ_TAG_ENEMY_ATTACK;
            coll->is_static = false;

            rend->is_active = 1;

            return obj;
        }
    }
    return NULL;
}

void Throw_MoveTowardPlayer(stOBJECT* throw, stOBJECT* target_player) {
    if (!throw || !target_player) return;

    stPOSITION* p_pos = &target_player->phy.pos;
    stPOSITION* t_pos = &throw->phy.pos;
    stPOSITION* t_speed = &throw->phy.speed;

    float dx = (float)p_pos->x - (float)t_pos->x;
    float dy = (float)p_pos->y - (float)t_pos->y;

    float dist = (float)sqrt(dx * dx + dy * dy);
    int speed = 2;

    if (dist > 0) {
        t_speed->x = (dx / dist) * speed;
        t_speed->y = (dy / dist) * speed;
    }

    t_pos->x += t_speed->x;
    t_pos->y += t_speed->y;
}


static int Get_RandNum_1_to_9(void) {
    static uint32_t x = 123456789;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return (x % 9) + 1;
}
