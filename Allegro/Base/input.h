#ifndef __COLLISION_H__
#define __COLLISION_H__

#include <allegro5/allegro5.h>

#define KEY_SEEN     1
#define KEY_DOWN     2

void init_keyboard(ALLEGRO_EVENT_QUEUE* queue);
void keyboard_update(ALLEGRO_EVENT* event);
void send_input(void);

#endif
