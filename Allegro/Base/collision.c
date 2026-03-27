#include "collision.h"

// Need to Optimization
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
        /* check gravity */
        if (obj_speed_y >= 0) {
            double curr_bottom = obj_y + obj_h;
            double next_bottom = next_y + obj_h;

            if (curr_bottom <= tile_t && next_bottom >= tile_t) {
                object->phy.pos.y = tile_t - obj_h;
                object->phy.speed.y = 0;
                if (object->coll.is_static == false) {
                    object->phy.is_jump = false;
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
            }
        }
    }
}
