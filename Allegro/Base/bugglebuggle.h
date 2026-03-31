#ifndef __BUGGLEBUGGLE_H__
#define __BUGGLEBUGGLE_H__

/************************************************/
/*               Macro Definition               */
/************************************************/
/* Debug Config */
#define DEBUG_PLAYER							(1)
#define DEBUG_STAGE								(1)

/* System Config */
#define CONFIG_SYSTEM_FRAME						(60)
#define CONFIG_SYSTEM_LOADING_DELAY				(1 * CONFIG_SYSTEM_FRAME)
#define CONFIG_SYSTEM_PLAYER_NAME_MAX			(12)

/* Game Config */
#define CONFIG_GAME_PLAYER_HEART_MAX			(3)

/* MAP Config */
#define CONFIG_MAP_X_MAX						(36)
#define CONFIG_MAP_Y_MAX						(24)

/* Physics Config */
#define CONFIG_PHYSICS_GRAVITY_FORCE			(0.25)
#define CONFIG_PHYSICS_JUMP_FORCE				(4.2)
#define CONFIG_PHYSICS_PLAYER_INV_TIME			(2)

/* Object Config */
#define CONFIG_OBJECT_PLAYER_MAX				(1)
#define CONFIG_OBJECT_BUBBLE_MAX				(20)
#define CONFIG_OBJECT_ENEMY_MAX					(10)
#define CONFIG_OBJECT_ENEMY_ATTACK_MAX			(10)
#define CONFIG_OBJECT_MAP_MAX					(CONFIG_MAP_X_MAX * CONFIG_MAP_Y_MAX)

/* Collision config */
#define CONFIG_COLLISION_TILE_SIZE				(10)

typedef enum {
	eMAIN_STATE_WAIT = 0,
	eMAIN_STATE_START,
	eMAIN_STATE_MAX
} eMAIN_STATE;

typedef enum {
	eGAME_STATE_MAIN = 0,
	eGAME_STATE_INGAME,
	eGAME_STATE_NAME,
	eGAME_STATE_SCORE,
	eGAME_STATE_END,
	eGAME_STATE_MAX
} eGAME_STATE;

typedef enum {
	eGAME_STAGE_NONE = 0,
	eGAME_STAGE_1,
	eGAME_STAGE_2,
	eGAME_STAGE_3,
	eGAME_STAGE_MAX
} eGAME_STAGE;

#endif
