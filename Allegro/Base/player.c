#include <allegro5/allegro5.h>
#include "player.h"
#include "bubble.h"

// --- player ---

#define PALYER_SPEED 3 // fix it later
#define SHIP_W 1
#define SHIP_H 1
#define SHIP_W 1
#define SHIP_W 1


static stPLAYER player;

void init_player(void)
{
    player = (stPLAYER){
        .obj.coll.box.height = 2, // fix it later
        .obj.coll.box.width = 2,
        .obj.coll.is_static = false,
        .obj.coll.tag = eOBJ_TAG_PLAYER,
        .obj.phy.look = eDIR_LOOK_RIGHT,
        .obj.phy.speed.x = PALYER_SPEED,
        .obj.phy.speed.y = PALYER_SPEED,
        .obj.phy.pos.x = 10, // init pos, fix it later
        .obj.phy.pos.y = 10,
        .state = ePLAYER_STATE_IDLE,
        .shot_timer = 0,
        .lives = 3,
        .invincible_timer = 0,
        //.obj.rend,
    };
}

void player_update(int allegro_key, unsigned char flag)
{
    if (player.lives < 0)
        return;

    if (allegro_key == ALLEGRO_KEY_LEFT) {
        player.obj.phy.pos.x -= player.obj.phy.speed.x;
        player.state = ePLAYER_STATE_MOVE;
    }
    if (allegro_key == ALLEGRO_KEY_RIGHT)
        player.obj.phy.pos.x += player.obj.phy.speed.x;
    if (allegro_key == ALLEGRO_KEY_UP)
        player.obj.phy.pos.y -= player.obj.phy.speed.y; // jump func logic
    //if (allegro_key == ALLEGRO_KEY_DOWN)      downward jump implemented later
    //    ship.y += SHIP_SPEED;  

    if (player.invincible_timer)
        player.invincible_timer--;
    else
    {
        //if (shots_collide(true, ship.x, ship.y, SHIP_W, SHIP_H)) // enemy or projectile collide
        //{
        //    player.lives--;
        //    player.invincible_timer = 180;
        //}
    }

    if (player.shot_timer)
        player.shot_timer--;
    else if (allegro_key == ALLEGRO_KEY_SPACE)
    {
        if (bubble_add(player))
            player.shot_timer = 60; // 1s delay
    }
}

void player_draw()
{
    if (player.lives < 0)
        return;
    if (((player.invincible_timer / 2) % 3) == 1) // blinked
        return;

    //al_draw_bitmap(sprites.ship, ship.x, ship.y, 0);
}

#if (DEBUG_PLAYER == 1)
void player_debug(void)
{
    printf("\n[PLAYER DATA]\n\tSTAT: { %d }\n\tPOS: { X: %f, Y: %f }\n",
        player.state, player.obj.phy.pos.x, player.obj.phy.pos.y);
}
#endif
