#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <stdbool.h>

/*  */
typedef enum {
	eOBJ_TAG_TILE = 0,
	eOBJ_TAG_PLAYER,
	eOBJ_TAG_ENEMY,
	eOBJ_TAG_BUBBLE,
	eOBJ_TAG_ENEMY_ATTACK,
	eOBJ_TAG_MAX
} eOBJ_TAG;

typedef struct {
	double height;
	double width;
} stCOLLISION_BOX;

typedef struct {
	bool			is_static;
	eOBJ_TAG		tag;
	stCOLLISION_BOX box;
} stCOLLISION;

typedef enum {
	eDIR_LOOK_RIGHT = 0,
	eDIR_LOOK_LEFT,
	eDIR_LOOK_UP,
	eDIR_LOOK_DOWN,
	eDIR_LOOK_MAX
} eDIR_LOOK;

typedef struct {
	double y;
	double x;
} stPOSITION;

typedef struct {
	bool is_gravity;
	bool is_jump;

	eDIR_LOOK	look;
	stPOSITION	pos;
	stPOSITION	speed;
} stPHYSICS;

typedef struct {
	int is_active; /* unused */
	bool is_move;
} stRENDER;

typedef struct {
	/* Enable Object */
	bool		is_active;

	/* Collision */
	stCOLLISION coll;

	/* Physics */
	stPHYSICS	phy;

	/* Render */
	stRENDER	rend;
} stOBJECT;

#endif