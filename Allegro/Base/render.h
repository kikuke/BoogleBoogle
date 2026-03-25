#ifndef __RENDER_H__
#define __RENDER_H__

#define BUFFER_W 320
#define BUFFER_H 240

#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

void render_init(ALLEGRO_EVENT_QUEUE* queue);

#endif
