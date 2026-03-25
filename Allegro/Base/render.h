#ifndef __RENDER_H__
#define __RENDER_H__

#define BUFFER_W 320
#define BUFFER_H 240

#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

void init_render(ALLEGRO_EVENT_QUEUE* queue);
void render_update(void);
void render_draw(void);
void deinit_render();

void disp_pre_draw();
void disp_post_draw();

void test_disp(float x, float y);
void test_scale_disp(float sx, float sy, float sw, float sh,
    float dx, float dy, float dw, float dh, int flags);

#endif
