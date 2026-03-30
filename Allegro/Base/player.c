#include <allegro5/allegro5.h>

#include "player.h"
#include "collision.h"
#include "game_manager.h"

// --- player ---

// fix it later
const static double PLAYER_SPEED = 3.0;
#define REND_DURATION		(30)
#define BUBBLE_COOL_TIME	(60)


void init_player(stPLAYER* player)
{
	*player = (stPLAYER){
		.obj.coll.box.height = CONFIG_COLLISION_TILE_SIZE, // fix it later
		.obj.coll.box.width = CONFIG_COLLISION_TILE_SIZE,
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
		.shot_timer = REND_DURATION,
		.lives = CONFIG_GAME_PLAYER_HEART_MAX,
		.invincible_timer = 0,
		.obj.is_active = true,
		.obj.phy.is_gravity = true,
		.obj.phy.is_jump = false,
		.attack_timer = 0,
		//.obj.rend,
	};
}

void player_update_input(stPLAYER* player, int allegro_key, unsigned char flag)
{
	if (!player->obj.is_active) {
		return;
	}

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
		if (player->obj.phy.is_jump == true)
			break;

		player->obj.phy.speed.y = - CONFIG_PHYSICS_JUMP_FORCE;
		player->obj.phy.is_jump = true;
		player->state = ePLAYER_STATE_MOVE;
	}
	break;
	case ALLEGRO_KEY_DOWN:
	{
		player->obj.phy.pos.y += 3;
		player->obj.phy.is_jump = false;
		player->state = ePLAYER_STATE_MOVE;
	}
	break;
	case ALLEGRO_KEY_SPACE:
	{
		if (player->shot_timer <= 0 && bubble_add(player, GAME_MANAGER_GetBubble())) {
			player->shot_timer = BUBBLE_COOL_TIME;
			player->attack_timer = REND_DURATION;
		}
	}
	break;
	default:
		break;
	}
}

void player_update_frame(stPLAYER* player) {
#if 01
	if (!player->obj.is_active) {
		return;
	}
#endif
	// TODO: Need To Modify
#if 01
	if ((player->obj.phy.speed.x == 0)) {
		player->state = ePLAYER_STATE_IDLE;
	}
#endif

	if (player->attack_timer > 0) {
		player->state = ePLAYER_STATE_ATTACK;
		player->attack_timer--;
	}
	else {
		player->state = ePLAYER_STATE_IDLE;
	}
		
	if (player->shot_timer > 0) player->shot_timer--;
	if (player->invincible_timer > 0) player->invincible_timer--;
}

#if (DEBUG_PLAYER == 1)
void player_debug(stPLAYER* player)
{
	printf("\n[PLAYER DATA]\
			\n\tSTAT: { STAT: %d, IS_JUMP: %d, IS_MOVE: %d }\
			\n\tPOS: { LOOK: %d, X: %f, Y: %f }\n",
		player->state, player->obj.phy.is_jump, player->obj.rend.is_move,
		player->obj.phy.look, player->obj.phy.pos.x, player->obj.phy.pos.y);
}
#endif
