#include <stdlib.h>
#include <stdbool.h>

#include "bugglebuggle.h"
#include "physics.h"

const static double GRAVITY = CONFIG_PHYSICS_GRAVITY_FORCE;
const static double JUMP_SPEED = CONFIG_PHYSICS_JUMP_FORCE;

void stOBJECT_UpdatePhysics(stOBJECT* obj) {
	obj->phy.pos.x += obj->phy.speed.x;
	obj->phy.pos.y += obj->phy.speed.y;

	if (obj->phy.is_gravity && obj->is_active) {
		obj->phy.speed.y += GRAVITY;
	}

	/* Update Rendering */
	obj->rend.is_move = obj->phy.speed.x != 0;

	obj->phy.speed.x = 0;
}
