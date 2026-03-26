#include "player.h"
#include "collision.h"
#include "map.h"

void OneWay_Jump(stPLAYER* player, stTILE* tile) {

    // x range
    double player_left = player->obj.phy.pos.x;
    double player_right = player->obj.phy.pos.x + player->obj.coll.box.width;
    double tile_left = tile->obj.phy.pos.x;
    double tile_right = tile->obj.phy.pos.x; + tile->obj.coll.box.width;
    
    // overlap
    bool is_x_overlap = (player_right > tile_left) && (player_left < tile_right);

    if (is_x_overlap) {

        // y range
        double current_bottom = player->obj.phy.pos.y + player->obj.coll.box.height;
        double previous_bottom = current_bottom - player->obj.phy.speed.y;
        double tile_top = tile->obj.phy.pos.y;

        // falling, ground
        if (player->obj.phy.speed.y >= 0) {

            if (previous_bottom <= tile_top && current_bottom >= tile_top) {

                // landing
                player->obj.phy.pos.y = tile_top - player->obj.coll.box.height;
                player->obj.phy.speed.y = 0;
                if (player->is_jump) {
                    player->is_jump = false;
                    player->state = ePLAYER_STATE_IDLE;
                }
            }
        }
    }
}


void Collide_Player_Wall(stPLAYER* player, stTILE* tile) {


    double player_top = player->obj.phy.pos.y;
    double player_bottom = player->obj.phy.pos.y + player->obj.coll.box.height;
    double tile_top = tile->obj.phy.pos.y;
    double tile_bottom = tile->obj.phy.pos.y + tile->obj.coll.box.height;

    bool is_y_overlap = (player_bottom >= tile_top) && (player_top <= tile_bottom);

    if (is_y_overlap) {

        double curr_left = player->obj.phy.pos.x;
        double curr_right = curr_left + player->obj.coll.box.width;
        double prev_left = curr_left - player->obj.phy.speed.x;
        double prev_right = prev_left + player->obj.coll.box.width;

        double tile_left = tile->obj.phy.pos.x;
        double tile_right = tile->obj.phy.pos.x + tile->obj.coll.box.width;

        if (player->obj.phy.speed.x > 0 && prev_right < curr_right) { // right
            if (prev_right <= tile_left) {
                player->obj.phy.pos.x = tile_left - player->obj.coll.box.width;
                player->obj.phy.speed.x = 0;
            }
        }
        else if (player->obj.phy.speed.x < 0 && prev_left > curr_left) { // left
            if (prev_left >= tile_right) {
                player->obj.phy.pos.x = tile_right;
                player->obj.phy.speed.x = 0;
            }

        }
    }
}
