#include "collision.h"

#define MAP_Y_MAX (10)
#define MAP_Y_MIN (220)

// Need to Optimization
static bool AABB_to_AABB(stOBJECT* object1, stOBJECT* object2)
{
    double a_max_x = object1->phy.pos.x + object1->coll.box.width;
    double a_max_y = object1->phy.pos.y + object1->coll.box.height;
    double b_max_x = object2->phy.pos.x + object2->coll.box.width;
    double b_max_y = object2->phy.pos.y + object2->coll.box.height;

    double a_min_x = object1->phy.pos.x;
    double a_min_y = object1->phy.pos.y;

    double b_min_x = object2->phy.pos.x;
    double b_min_y = object2->phy.pos.y;

    if (a_max_x < b_min_x || a_min_x > b_max_x) return false;
    if (a_max_y < b_min_y || a_min_y > b_max_y) return false;
    return true;
}

void Collide_Object_Tile(stOBJECT* object, stTILE* tile) {
    double obj_w = object->coll.box.width;
    double obj_h = object->coll.box.height;
    double obj_x = object->phy.pos.x;
    double obj_y = object->phy.pos.y;
    double obj_speed_x = object->phy.speed.x;
    double obj_speed_y = object->phy.speed.y;

    double next_x = obj_x + obj_speed_x;
    double next_y = obj_y + obj_speed_y;

    double tile_l = tile->obj.phy.pos.x;
    double tile_r = tile->obj.phy.pos.x + tile->obj.coll.box.width;
    double tile_t = tile->obj.phy.pos.y;
    double tile_b = tile->obj.phy.pos.y + tile->obj.coll.box.height;

    bool is_y_overlap = (obj_y + obj_h > tile_t) && (obj_y < tile_b);
    bool is_x_overlap = (obj_x + obj_w > tile_l) && (obj_x < tile_r);

    /* Check Y Position */
    if (is_x_overlap) {

        // map out
        if (obj_speed_y < 0) {
            if (next_y < MAP_Y_MAX) {
                object->phy.speed.y = 0;
                object->phy.pos.y = MAP_Y_MAX;
            }
        }
        /* check gravity */
        if (obj_speed_y >= 0) {
            double curr_bottom = obj_y + obj_h;
            double next_bottom = next_y + obj_h;

            if (next_y > MAP_Y_MIN) {
                object->phy.speed.y = 0;
                object->phy.pos.y = MAP_Y_MIN;
            }

            if (curr_bottom <= tile_t && next_bottom >= tile_t) {
                object->phy.pos.y = tile_t - obj_h;
                object->phy.speed.y = 0;
                if (object->coll.is_static == false) {
                    object->phy.is_jump = false;
                }

                if (object->coll.tag == eOBJ_TAG_PLAYER) {
                    stPLAYER* p = (stPLAYER*)object;
                }
                else if (object->coll.tag == eOBJ_TAG_ENEMY) {
                    stENEMY* e = (stENEMY*)object;
                }
                else if (object->coll.tag == eOBJ_TAG_BUBBLE) {
                    stBUBBLE* b = (stBUBBLE*)object;
                }
            }
        }
    }

    /* Check X Position */
    if (is_y_overlap) {
        if (obj_speed_x > 0) {  // right
            if (obj_x + obj_w <= tile_l && next_x + obj_w >= tile_l) {
                object->phy.pos.x = tile_l - obj_w;
                object->phy.speed.x = 0;

                if (object->coll.tag == eOBJ_TAG_PLAYER) {
                    stPLAYER* p = (stPLAYER*)object;
                }
                else if (object->coll.tag == eOBJ_TAG_ENEMY) {
                    stENEMY* e = (stENEMY*)object;
                }
                else if (object->coll.tag == eOBJ_TAG_BUBBLE) {
                    stBUBBLE* b = (stBUBBLE*)object;
                    b->state = eBUBBLE_STATE_FLOAT;
                    b->obj.phy.speed.x = 0;
                    b->obj.phy.speed.y = BUBBLE_UP_SPEED;
                }
            }
        }
        else if (obj_speed_x < 0) { // left
            if (obj_x >= tile_r && next_x <= tile_r) {
                object->phy.pos.x = tile_r;
                object->phy.speed.x = 0;
                if (object->coll.tag == eOBJ_TAG_PLAYER) {
                    stPLAYER* p = (stPLAYER*)object;
                }
                else if (object->coll.tag == eOBJ_TAG_ENEMY) {
                    stENEMY* e = (stENEMY*)object;
                }
                else if (object->coll.tag == eOBJ_TAG_BUBBLE) {
                    stBUBBLE* b = (stBUBBLE*)object;
                    b->state = eBUBBLE_STATE_FLOAT;
                    b->obj.phy.speed.x = 0;
                    b->obj.phy.speed.y = BUBBLE_UP_SPEED;
                }
            }
        }
    }
}

void Collide_Enemy_Player(stOBJECT* object, stPLAYER* player) {

    if (player->state == ePLAYER_STATE_DEAD || player->invincible_timer > 0) {
        return;
    }

    if (object->coll.tag == eOBJ_TAG_ENEMY) {

        if (AABB_to_AABB(object, &(player->obj))) {
            stENEMY* e = (stENEMY*)object;

            if (e->state == eENEMY_STATE_TRAPPED) {
                e->state = eENEMY_STATE_DEAD;
                e->obj.is_active = false;
            }
            else {
                player->lives--;

                if (player->lives <= 0) {
                    player->state = ePLAYER_STATE_DEAD;
                    player->obj.is_active = false;
                }
                else {
                    player->invincible_timer = CONFIG_PHYSICS_PLAYER_INV_TIME * CONFIG_SYSTEM_FRAME; // 2s
                }
            }
        }
    }

    if (object->coll.tag == eOBJ_TAG_ENEMY_ATTACK) {
        if (AABB_to_AABB(object, &(player->obj))) {

            player->lives--;

            if (player->lives <= 0) {
                player->state = ePLAYER_STATE_DEAD;
                player->obj.is_active = false;
            }
            else {
                player->invincible_timer = CONFIG_PHYSICS_PLAYER_INV_TIME * CONFIG_SYSTEM_FRAME; // 2s
            }

            object->is_active = false;
        }
    }
}

void Collide_Object_Bubble(stOBJECT* object, stBUBBLE* bubble) {

    if (object->coll.tag == eOBJ_TAG_PLAYER) {
        if (AABB_to_AABB(object, &(bubble->obj))) {
            if (bubble->state == eBUBBLE_STATE_FLOAT) {
                if (object->phy.speed.y > 0) {
                    object->phy.is_jump = false;
                }
            }
        }
    }
    else if (object->coll.tag == eOBJ_TAG_ENEMY) {
        if (bubble->state == eBUBBLE_STATE_SHOOTING) {
            if (AABB_to_AABB(object, &(bubble->obj))) {
                stENEMY* e = (stENEMY*)object;

                if (e->state != eENEMY_STATE_TRAPPED) {
                    e->state = eENEMY_STATE_TRAPPED;
                    bubble->obj.is_active = false;
                }

            }
        }
    }
}
