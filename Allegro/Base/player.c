#include <allegro5/allegro5.h>

#include "player.h"
#include "bubble.h"
#include "collision.h"

// --- player ---

// fix it later
const static double GRAVITY = 1.0;
const static double JUMP_SPEED = 8.0;
const static double PLAYER_SPEED = 3.0;

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
	switch (allegro_key) {
	case ALLEGRO_KEY_LEFT:
	{
		player->obj.phy.speed.x = -PLAYER_SPEED;
		player->obj.phy.look = eDIR_LOOK_LEFT;
		if (!player->is_jump) player->state = ePLAYER_STATE_MOVE;
	}
	break;
	case ALLEGRO_KEY_RIGHT:
	{
		player->obj.phy.speed.x = PLAYER_SPEED;
		player->obj.phy.look = eDIR_LOOK_RIGHT;
		if (!player->is_jump) player->state = ePLAYER_STATE_MOVE;
	}
	break;
	case ALLEGRO_KEY_UP:
	{
		if (player->is_jump == true)
			break;

		player->obj.phy.speed.y = -JUMP_SPEED;
		player->is_jump = true;
		player->state = ePLAYER_STATE_MOVE;
	}
	break;
	case ALLEGRO_KEY_SPACE:
	{
		if (bubble_add(player)) {
			player->shot_timer = 60;
			player->state = ePLAYER_STATE_ATTACK;
		}
	}
	break;
	default:
		break;
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
#if 0
	if (player.lives < 0) {
		player.state = ePLAYER_STATE_DEAD;
		player->obj.is_active = false;
		return;
	}
#endif

	if (player->obj.phy.speed.x == 0 && !player->is_jump && player->state != ePLAYER_STATE_ATTACK) {
		player->state = ePLAYER_STATE_IDLE;
	}

	if (player->obj.phy.is_gravity) {
		//player->obj.phy.speed.y += GRAVITY;
	}

	player->obj.phy.pos.x += player->obj.phy.speed.x;
	player->obj.phy.pos.y += player->obj.phy.speed.y;

	if (player->shot_timer > 0) player->shot_timer--;
	if (player->invincible_timer > 0) player->invincible_timer--;

	if (player->is_jump == true) {
		player->obj.phy.speed.y += 1;
	}
	player->obj.phy.speed.x = 0;
}

#if (DEBUG_PLAYER == 1)
void player_debug(stPLAYER* player)
{
	printf("\n[PLAYER DATA]\
			\n\tSTAT: { STAT: %d, IS_JUMP: %d, IS_MOVE: %d }\
			\n\tPOS: { LOOK: %d, X: %f, Y: %f }\n",
		player->state, player->is_jump, player->obj.phy.is_move,
		player->obj.phy.look, player->obj.phy.pos.x, player->obj.phy.pos.y);
}
#endif
