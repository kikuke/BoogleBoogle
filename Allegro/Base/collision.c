#include "collision.h"


void Collide_Object_Tile(stOBJECT* object, stTILE* tile) {

    double obj_w = object->coll.box.width;
    double obj_h = object->coll.box.height;

    double next_x = object->phy.pos.x + object->phy.speed.x;
    double next_y = object->phy.pos.y + object->phy.speed.y;

    double tile_l = tile->obj.phy.pos.x;
    double tile_r = tile->obj.phy.pos.x + tile->obj.coll.box.width;
    double tile_t = tile->obj.phy.pos.y;
    double tile_b = tile->obj.phy.pos.y + tile->obj.coll.box.height;

    //bool is_x_overlap = (obj_right > tile_left) && (obj_left < tile_right);
    //bool is_y_overlap = (obj_bottom > tile_top) && (obj_top < tile_bottom);

    bool is_y_overlap = (object->phy.pos.y + obj_h >= tile_t) && (object->phy.pos.y <= tile_b);
    bool is_x_overlap = (object->phy.pos.x + obj_w >= tile_l) && (object->phy.pos.x <= tile_r);

    // wall
    if (is_y_overlap) {
        if (object->phy.speed.x > 0) {  // right
            if (object->phy.pos.x + obj_w <= tile_l && next_x + obj_w >= tile_l) {
                object->phy.pos.x = tile_l - obj_w; 
                object->phy.speed.x = 0;   
                if (object->coll.tag == eOBJ_TAG_PLAYER) {
                    stPLAYER* p = (stPLAYER*)object;
                    p->state = ePLAYER_STATE_IDLE;
                }
                else if (object->coll.tag == eOBJ_TAG_ENEMY) {
                    stENEMY* e = (stENEMY*)object;
                    e->state = eENEMY_STATE_IDLE;
                }
            }
        }
        else if (object->phy.speed.x < 0) { // left
            if (object->phy.pos.x >= tile_r && next_x <= tile_r) {
                object->phy.pos.x = tile_r;
                object->phy.speed.x = 0;
                if (object->coll.tag == eOBJ_TAG_PLAYER) {
                    stPLAYER* p = (stPLAYER*)object;
                    p->state = ePLAYER_STATE_IDLE;
                }
                else if (object->coll.tag == eOBJ_TAG_ENEMY) {
                    stENEMY* e = (stENEMY*)object;
                    e->state = eENEMY_STATE_IDLE;
                }
            }
        }
    }

    // one_way_jump
    if (is_x_overlap) {
        if (object->phy.speed.y >= 0) {
            double curr_bottom = object->phy.pos.y + obj_h;
            double next_bottom = next_y + obj_h;

            if (curr_bottom <= tile_t && next_bottom >= tile_t) {
                object->phy.pos.y = tile_t - obj_h; 
                object->phy.speed.y = 0; 

                if (object->coll.tag == eOBJ_TAG_PLAYER) {
                    stPLAYER* p = (stPLAYER*)object;
                    p->is_jump = false;
                    p->state = ePLAYER_STATE_IDLE;
                }
                else if (object->coll.tag == eOBJ_TAG_ENEMY) {
                    stENEMY* e = (stENEMY*)object;
                    e->state = eENEMY_STATE_IDLE;
                }
            }
        }
    }
}