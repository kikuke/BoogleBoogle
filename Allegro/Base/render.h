#ifndef __RENDER_H__
#define __RENDER_H__
#include "bugglebuggle.h"

#define BUFFER_W 320
#define BUFFER_H 240

#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

#define TEXT_X (100)
#define TEXT_Y (215)
#define FLAG_0 (0)

void init_render(ALLEGRO_EVENT_QUEUE* queue);
void render_draw_main(void);
void render_draw_ingame(void);
void render_draw_score(void);
void deinit_render();

void disp_pre_draw();
void disp_post_draw();

#endif
