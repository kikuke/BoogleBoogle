#ifndef __BUGGLEBUGGLE_H__
#define __BUGGLEBUGGLE_H__

/************************************************/
/*               Macro Definition               */
/************************************************/
/* System Config */
#define ENABLE_RENDER							(0)
#define DEBUG_PLAYER							(1)
#define CONFIG_GAME_FRAME						(60)

/* MAP Config */
#define CONFIG_MAP_X_MAX						(36)
#define CONFIG_MAP_Y_MAX						(24)

/* Physics Config */
#define CONFIG_PHYSICS_GRAVITY_FORCE			(1.0)
#define CONFIG_PHYSICS_JUMP_FORCE				(8.0)
#define CONFIG_PHYSICS_PLAYER_INV_TIME			(1)

/* Object Config */
#define CONFIG_OBJECT_PLAYER_MAX				(1)
#define CONFIG_OBJECT_BUBBLE_MAX				(10)
#define CONFIG_OBJECT_ENEMY_MAX					(10)
#define CONFIG_OBJECT_ENEMY_ATTACK_MAX			(10)
#define CONFIG_OBJECT_MAP_MAX					(CONFIG_MAP_X_MAX * CONFIG_MAP_Y_MAX)
#define CONFIG_OBJECT_COLLISION_TILE_SIZE		(10)

/* Collision config */
#define PLAYER_W 10
#define PLAYER_H 10

typedef enum {
	eGAME_STATE_MAIN = 0,
	eGAME_STATE_INGAME,
	eGAME_STATE_END
} eGAME_STATE;

typedef enum {
	eGAME_STAGE_1 = 0,
	eGAME_STAGE_2,
	eGAME_STAGE_3,
	eGAME_STAGE_MAX
} eGAME_STAGE;

#endif
