#ifndef __BUGGLEBUGGLE_H__
#define __BUGGLEBUGGLE_H__

/* MAP Config */
#define CONFIG_MAP_X_MAX				(30)
#define CONFIG_MAP_Y_MAX				(20)

/* Object Config */
#define CONFIG_OBJECT_PLAYER_MAX		(1)
#define CONFIG_OBJECT_BUBBLE_MAX		(10)
#define CONFIG_OBJECT_ENEMY_MAX			(10)
#define CONFIG_OBJECT_ENEMY_ATTACK_MAX	(10)
#define CONFIG_OBJECT_MAP_MAX			(CONFIG_MAP_X_MAX * CONFIG_MAP_Y_MAX)
#define KEY_SEEN     1
#define KEY_DOWN     2

#define ENABLE_RENDER	(0)

#define DEBUG_PLAYER	(1)

typedef enum {
	eGAME_STAGE_1 = 0,
	eGAME_STAGE_2,
	eGAME_STAGE_3,
	eGAME_STAGE_MAX
} eGAME_STAGE;

#endif
