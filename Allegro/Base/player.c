#include <allegro5/allegro5.h>

#include "player.h"
#include "bubble.h"
#include "collision.h"

// --- player ---

// fix it later
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
#if 0
		.obj.phy.pos.x = 10.0, // init pos, fix it later
		.obj.phy.pos.y = 220.0,
#else
		.obj.phy.pos.x = 15.0, // init pos, fix it later
		.obj.phy.pos.y = 210.0,
#endif
		.state = ePLAYER_STATE_IDLE,
		.shot_timer = 0,
		.lives = 3,
		.invincible_timer = 0,
		.is_jump = false,
		.obj.is_active = true,
		.obj.phy.is_gravity = true,
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
		player->state = ePLAYER_STATE_MOVE;
	}
	break;
	case ALLEGRO_KEY_RIGHT:
	{
		player->obj.phy.speed.x = PLAYER_SPEED;
		player->obj.phy.look = eDIR_LOOK_RIGHT;
		player->state = ePLAYER_STATE_MOVE;
	}
	break;
	case ALLEGRO_KEY_UP:
	{
		if (player->is_jump == true)
			break;

		player->obj.phy.speed.y = - CONFIG_PHYSICS_JUMP_FORCE;
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
}

void player_update_frame(stPLAYER* player) {
#if 0
	if (player.lives < 0) {
		player.state = ePLAYER_STATE_DEAD;
		player->obj.is_active = false;
		return;
	}
#endif
	// TODO: Need To Modify
	if ((player->obj.phy.speed.x == 0)/* && (player->obj.phy.speed.y == 0)*/) {
		player->state = ePLAYER_STATE_IDLE;
	}

	if (player->shot_timer > 0) player->shot_timer--;
	if (player->invincible_timer > 0) player->invincible_timer--;
}

#if (DEBUG_PLAYER == 1)
void player_debug(stPLAYER* player)
{
	printf("\n[PLAYER DATA]\
			\n\tSTAT: { STAT: %d, IS_JUMP: %d}\
			\n\tPOS: { LOOK: %d, X: %f, Y: %f }\n",
		player->state, player->is_jump,
		player->obj.phy.look, player->obj.phy.pos.x, player->obj.phy.pos.y);
}
#endif
