#ifndef _BUBBLE_H_
#include "bubble.h"
#include "object.h"
#include "player.h"

#define BUBBLES_N 128
stBUBBLE bubbles[BUBBLES_N];

#define PLAYER_H 10
#define SHOTS_N 10
#define BUBBLE_SHOT_H 10
#define BUBBLE_SHOT_W 10



void bubble_init(stBUBBLE* bubbles)
{
    for (int i = 0; i < BUBBLES_N; i++)
        bubbles[i].used = false;
}

// bubble
bool bubble_add(stPLAYER* player, stBUBBLE* bubbles)
{

    for (int i = 0; i < SHOTS_N; i++)
    {
        if (bubbles[i].used) continue;

        bubbles[i].x = player->obj.phy.pos.x;
        bubbles[i].y = player->obj.phy.pos.y + (PLAYER_H / 2) - (BUBBLE_SHOT_H / 2); // center pos
        bubbles[i].dx = 0;
        bubbles[i].dy = 0;

        bubbles[i].frame = 0;   // 0 frame used
        bubbles[i].used = true; // buble used
        bubbles[i].dur = 300; // 5s
        bubbles[i].state = eBUBBLE_STATE_SHOOTING;
        return true;
    }
    return false;
}

void bubble_update(stPLAYER* player, stBUBBLE* bubbles)
{
    for (int i = 0; i < BUBBLES_N; i++)
    {
        if (!bubbles[i].used)
            continue;

        if (bubbles[i].frame >= bubbles[i].dur) {

        }

        if (player->obj.phy.look == eDIR_LOOK_RIGHT) {
            bubbles[i].dx += 5;
        }
        else if (player->obj.phy.look == eDIR_LOOK_LEFT) {
            bubbles[i].dx -= 5;
        }

        bubbles[i].frame++; // 프레임 증가
    }
}

//bool bubble_collide(bool ship, int x, int y, int w, int h)
//{
//    for (int i = 0; i < BUBBLES_N; i++)
//    {
//        if (!bubbles[i].used)
//            continue;
//
//        int sw, sh;
//        if (ship)
//        {
//            //sw = ALIEN_SHOT_W;
//            //sh = ALIEN_SHOT_H;
//        }
//        else
//        {
//            sw = BUBBLE_SHOT_W;
//            sh = BUBBLE_SHOT_H;
//        }
//
//        //if (collide(x, y, x + w, y + h, bubbles[i].x, bubbles[i].y, bubbles[i].x + sw, bubbles[i].y + sh))
//        //{
//        //    bubbles[i].used = false;
//        //    return true;
//        //}
//    }
//
//    return false;
//}

//void bubble_draw()
//{
//    for (int i = 0; i < BUBBLES_N; i++)
//    {
//        if (!bubbles[i].used)
//            continue;
//
//        int frame_display = (bubbles[i].frame / 2) % 2;
//
//        al_draw_bitmap(sprites.ship_shot[frame_display], bubbles[i].x, bubbles[i].y, 0);
//    }
//}



#endif // !_BUBBLE_H_