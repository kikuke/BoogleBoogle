#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <allegro5/allegro5.h>

/************************************************/
/*               Macro Definition               */
/************************************************/
/* Key Bitfield */
#define KEY_SEEN     1
#define KEY_DOWN     2

/************************************************/
/*          Global Function Declaration         */
/************************************************/
void init_keyboard(ALLEGRO_EVENT_QUEUE* queue);
void keyboard_update(ALLEGRO_EVENT* event);
void keyboard_processing_ingame(void);

#endif
