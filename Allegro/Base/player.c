#include <allegro5/allegro5.h>

#include "player.h"
#include "bubble.h"
#include "collision.h"

// --- player ---

// fix it later
const double GRAVITY = 1.0;
const double JUMP_SPEED = 15.0;
const double PALYER_SPEED = 3.0;

void init_player(stPLAYER* player)
{
	*player = (stPLAYER){
		.obj.coll.box.height = PLAYER_H, // fix it later
		.obj.coll.box.width = PLAYER_W,
		.obj.coll.is_static = false,
		.obj.coll.tag = eOBJ_TAG_PLAYER,
		.obj.phy.look = eDIR_LOOK_RIGHT,
		.obj.phy.speed.x = 0.0,
		.obj.phy.speed.y = 0.0,
		.obj.phy.pos.x = 10.0, // init pos, fix it later
		.obj.phy.pos.y = 220.0,
		.state = ePLAYER_STATE_IDLE,
		.shot_timer = 0,
		.lives = 3,
		.invincible_timer = 0,
		.is_jump = false,
		.obj.is_active = true,
		.obj.phy.is_gravity = true,
		.obj.phy.is_move = false,
		//.obj.rend,
	};
}

void player_update_input(stPLAYER* player, int allegro_key, unsigned char flag)
{
#if 0
	if (player.lives < 0) {
		player.state = ePLAYER_STATE_DEAD;
		player->obj.is_active = false;
	}
	return;
#endif
	if (flag & KEY_DOWN) {
		if (allegro_key == ALLEGRO_KEY_LEFT) {
			player->obj.phy.speed.x = -PALYER_SPEED;
			player->obj.phy.look = eDIR_LOOK_LEFT;
			if (!player->is_jump) player->state = ePLAYER_STATE_MOVE;
		}
		else if (allegro_key == ALLEGRO_KEY_RIGHT) {
			player->obj.phy.speed.x = PALYER_SPEED;
			player->obj.phy.look = eDIR_LOOK_RIGHT;
			if (!player->is_jump) player->state = ePLAYER_STATE_MOVE;
		}
	}
	else {
		if (allegro_key == ALLEGRO_KEY_LEFT || allegro_key == ALLEGRO_KEY_RIGHT) {
			player->obj.phy.speed.x = 0;
		}
	}

	if (flag & KEY_SEEN) {
		if (allegro_key == ALLEGRO_KEY_UP && !player->is_jump) {
			player->obj.phy.speed.y = -JUMP_SPEED;
			player->is_jump = true;
			player->state = ePLAYER_STATE_JUMP;
		}
		if (allegro_key == ALLEGRO_KEY_SPACE && !player->shot_timer) {
			if (bubble_add(player)) {
				player->shot_timer = 60;
				player->state = ePLAYER_STATE_ATTACK;
			}
		}
	}

	//if (allegro_key == ALLEGRO_KEY_DOWN)      downward jump implemented later
	//    ship.y += SHIP_SPEED;  
	//else
	//{
	//	if (shots_collide(true, ship.x, ship.y, SHIP_W, SHIP_H)) // enemy or projectile collide
	//	{
	//	    player.lives--;
	//	    player.invincible_timer = 180;
	//	}
	//}
}

void player_update_frame(stPLAYER* player) {
	if (!player->obj.is_active || player->state == ePLAYER_STATE_DEAD) {
		return;
	}

	if (player->obj.phy.speed.x == 0 && !player->is_jump && player->state != ePLAYER_STATE_ATTACK) {
		player->state = ePLAYER_STATE_IDLE;
	}

	if (player->obj.phy.is_gravity) {
		player->obj.phy.speed.y += GRAVITY;
	}
	player->obj.phy.pos.x += player->obj.phy.speed.x;
	player->obj.phy.pos.y += player->obj.phy.speed.y;

	if (player->shot_timer > 0) player->shot_timer--;
	if (player->invincible_timer > 0) player->invincible_timer--;
}

#if (DEBUG_PLAYER == 1)
void player_debug(stPLAYER* player)
{
	printf("\n[PLAYER DATA]\n\tSTAT: { %d }\n\tPOS: { X: %f, Y: %f }\n",
		player->state, player->obj.phy.pos.x, player->obj.phy.pos.y);
}
#endif
