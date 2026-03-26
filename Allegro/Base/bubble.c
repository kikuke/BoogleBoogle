#ifndef _BUBBLE_H_
#include "bubble.h"
#include "object.h"
#include "player.h"

#define BUBBLES_N 128
stBUBBLE bubbles[BUBBLES_N];

#define SHIP_H 1
#define SHOTS_N 1
#define SHIP_SHOT_H 1
#define SHIP_SHOT_W 1



void bubble_init()
{
    for (int i = 0; i < BUBBLES_N; i++)
        bubbles[i].used = false;
}

// ����
bool bubble_add(stPLAYER player)
{

    for (int i = 0; i < SHOTS_N; i++)
    {
        if (bubbles[i].used) continue;

        bubbles[i].x = player.obj.phy.pos.x;
        bubbles[i].y = player.obj.phy.pos.y + (SHIP_H / 2) - (SHIP_SHOT_H / 2); // center pos


        bubbles[i].frame = 0;   // 0 frame used
        bubbles[i].used = true; // buble used
        bubbles[i].dur = 300; // 5s
        bubbles[i].state = eBUBBLE_STATE_SHOOTING;
        return true;
    }
    return false;
}

void bubble_update()
{
    for (int i = 0; i < BUBBLES_N; i++)
    {
        if (!bubbles[i].used)
            continue;

        // look, left: x-=, right: x+=
        // bubbles[i].y -= 5;
        if (bubbles[i].y < -SHIP_SHOT_H) // ĳ���� ũ�� �ޱ�
        {
            bubbles[i].used = false;
            continue;
        }

        bubbles[i].frame++; // ������ ����
    }
}

bool bubble_collide(bool ship, int x, int y, int w, int h)
{
    for (int i = 0; i < BUBBLES_N; i++)
    {
        if (!bubbles[i].used)
            continue;

        int sw, sh;
        if (ship)
        {
            //sw = ALIEN_SHOT_W;
            //sh = ALIEN_SHOT_H;
        }
        else
        {
            sw = SHIP_SHOT_W;
            sh = SHIP_SHOT_H;
        }

        //if (collide(x, y, x + w, y + h, bubbles[i].x, bubbles[i].y, bubbles[i].x + sw, bubbles[i].y + sh))
        //{
        //    bubbles[i].used = false;
        //    return true;
        //}
    }

    return false;
}

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