#ifndef _BUBBLE_H_
#include "bubble.h"

#define BUBBLES_N       (CONFIG_OBJECT_BUBBLE_MAX)

#define PLAYER_H      (10)
#define BUBBLE_SHOT_H (10)
#define BUBBLE_SHOT_W (10)

#define BUBBLE_SPEED    (2.0)
#define BUBBLE_UP_SPEED (-1.0)    
#define SHOOT_DURATION  (60)
#define TOTAL_DURATION  (300)



void bubble_init(stBUBBLE* bubbles)
{
    for (int i = 0; i < BUBBLES_N; ++i) {
        bubbles[i].obj.is_active = false;
    }
}

// bubble
bool bubble_add(stPLAYER* player, stBUBBLE* bubbles)
{
    for (int i = 0; i < BUBBLES_N; ++i) 
    {
        if (bubbles[i].obj.is_active) continue;

        bubbles[i].obj.is_active = true;
        bubbles[i].obj.phy.pos.x = player->obj.phy.pos.x;
        bubbles[i].obj.phy.pos.y = player->obj.phy.pos.y + (PLAYER_H / 2) - (BUBBLE_SHOT_H / 2);

        if (player->obj.phy.look == eDIR_LOOK_RIGHT) {
            bubbles[i].obj.phy.speed.x = BUBBLE_SPEED;
        }
        else if (player->obj.phy.look == eDIR_LOOK_LEFT) {
            bubbles[i].obj.phy.speed.x = -BUBBLE_SPEED;
        }

        bubbles[i].obj.coll.tag = eOBJ_TAG_BUBBLE;
        bubbles[i].obj.coll.box.height = BUBBLE_SHOT_H;
        bubbles[i].obj.coll.box.width = BUBBLE_SHOT_W;
        bubbles[i].obj.coll.is_static = false;
        bubbles[i].obj.phy.is_gravity = false;
        bubbles[i].obj.phy.speed.y = 0;
        bubbles[i].frame = 0;
        bubbles[i].dur = TOTAL_DURATION; 
        bubbles[i].state = eBUBBLE_STATE_SHOOTING;

        return true;
    }
    return false;

}

void bubble_update(stBUBBLE* bubbles)
{
    for (int i = 0; i < BUBBLES_N; ++i)
    {
        if (!bubbles[i].obj.is_active) continue;

        if (bubbles[i].frame >= bubbles[i].dur) {
            bubbles[i].state = eBUBBLE_STATE_POP;
            bubbles[i].obj.is_active = false;
            continue;
        }

        if (bubbles[i].state == eBUBBLE_STATE_SHOOTING) {
            if (bubbles[i].frame >= SHOOT_DURATION) {
                bubbles[i].state = eBUBBLE_STATE_FLOAT;
                bubbles[i].obj.phy.speed.x = 0;
                bubbles[i].obj.phy.speed.y = BUBBLE_UP_SPEED;
            }
        }
    }
}

void bubble_update_frame(stBUBBLE* bubbles) 
{

    for (int i = 0; i < BUBBLES_N; i++) {
        if (!bubbles[i].obj.is_active) continue;

        bubbles[i].obj.phy.pos.x += bubbles[i].obj.phy.speed.x;
        bubbles[i].obj.phy.pos.y += bubbles[i].obj.phy.speed.y;
        bubbles[i].frame++;
    }
}

#endif // !_BUBBLE_H_