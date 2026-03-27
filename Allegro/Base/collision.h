#ifndef __COLLISION_H__
#define __COLLISION_H__
#include "object.h"
#include "player.h"
#include "enemy.h"
#include "bubble.h"
#include "map.h"


/************************************************/
/*          Global Function Declaration         */
/************************************************/
static bool AABB_to_AABB(stOBJECT* object1, stOBJECT* object2);
void Collide_Object_Tile(stOBJECT* object, stTILE* tile);
void Collide_Enemy_Player(stOBJECT* object, stPLAYER* player);
void Collide_Object_Bubble(stOBJECT* object, stBUBBLE* bubble);

#endif
