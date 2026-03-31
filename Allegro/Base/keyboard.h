#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stdbool.h>
#include <allegro5/allegro5.h>
#include "bugglebuggle.h"      // eMAIN_STATE Á¤ÀÇ

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
eMAIN_STATE keyboard_processing_main(void);
void keyboard_processing_ingame(void);
bool keyboard_processing_name(void);
bool keyboard_processing_score(void);

const char* keyboard_get_name(void);

#if (DEBUG_STAGE == 1)
bool test_next_stage_input(void);
#endif

#endif
