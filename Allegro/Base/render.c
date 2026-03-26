#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>

#include "util.h"
#include "render.h"
#include "map.h"
#include "game_manager.h"
/************************************************/
/*         Local Structure Declaration          */
/************************************************/

#define MAP 28
#define SCALE 10
#define CHARACTER 48

typedef struct {
    ALLEGRO_BITMAP* left[5];
    ALLEGRO_BITMAP* right[5];
} stSPRITE_PLAYER;

typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;

    ALLEGRO_BITMAP* map;
    stSPRITE_PLAYER player;
} SPRITES;

/************************************************/
/*         Local Function Declaration           */
/************************************************/
void disp_pre_draw();
void disp_post_draw();

void test_disp(float x, float y);
void map_scale_disp(float dx, float dy, float dw, float dh, int flags);
void character_scale_disp(float px, float py, float dw, float dh, int flags);
void map_render(stTILE* tiles, size_t tile_len);
static ALLEGRO_DISPLAY* disp;
static ALLEGRO_BITMAP* buffer;

/************************************************/
/*         Local Variable Declaration           */
/************************************************/
SPRITES sprites;

/************************************************/
/*          Global Function Definition          */
/************************************************/
void render_update(void)
{

#if 0
    fx_update();
    hud_update();
#endif
}

void render_draw(void)
{
    stPLAYER* player = GAME_MANAGER_GetPlayer(0);
    disp_pre_draw();
    al_clear_to_color(al_map_rgb(0, 0, 0));

    map_render(GAME_MANAGER_GetMap(), CONFIG_MAP_Y_MAX * CONFIG_MAP_X_MAX);
    character_scale_disp(player->obj.phy.pos.x, player->obj.phy.pos.y, SCALE, SCALE, 0);
    //character_scale_disp(10, 220, SCALE, SCALE, 0);
#if 0

    stars_draw();
    aliens_draw();
    shots_draw();
    fx_draw();
    ship_draw();

    hud_draw();

#endif
    disp_post_draw();
}

void disp_init()
{
    // TEST
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    disp = al_create_display(DISP_W, DISP_H);
    must_init(disp, "display");

    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(buffer, "bitmap buffer");
}

void disp_deinit()
{
    al_destroy_bitmap(buffer);
    al_destroy_display(disp);
}

/************************************************/
/*          Local Function Definition           */
/************************************************/
static void disp_pre_draw()
{
    al_set_target_bitmap(buffer);
}

static void disp_post_draw()
{
    al_set_target_backbuffer(disp); // 스케일 업된 버퍼 지정
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);   // 스케일 업

    al_flip_display(); // 스케일 업된 버퍼를 실제 화면에 그림
}

static void test_disp(float x, float y)
{
    al_draw_bitmap(sprites.map, x, y, 0);
}

static void map_scale_disp(float dx, float dy, float dw, float dh, int flags) {
    al_draw_scaled_bitmap(sprites.map,0,0, MAP, MAP, dx, dy, dw, dh, flags);
}

static void character_scale_disp(float px, float py, float dw, float dh, int flags) {
    al_draw_scaled_bitmap(sprites.player.left[4], 0, 0, CHARACTER, CHARACTER, px, py, dw, dh, flags);
}

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h)
{
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
    must_init(sprite, "sprite grab");
    return sprite;
}

void sprites_init()
{
    sprites._sheet = al_load_bitmap("Resource/Bubble_Bobble_sprite.png");
    must_init(sprites._sheet, "spritesheet");

    sprites.map = sprite_grab(8, 1578, MAP, MAP);

    sprites.player.left[0] = sprite_grab(270, 16, CHARACTER, CHARACTER);
    sprites.player.left[1] = sprite_grab(206, 16, CHARACTER, CHARACTER);
    sprites.player.left[2] = sprite_grab(141, 16, CHARACTER, CHARACTER);
    sprites.player.left[3] = sprite_grab(76, 16, CHARACTER, CHARACTER);
    sprites.player.left[4] = sprite_grab(10, 16, CHARACTER, CHARACTER);
}


void sprites_deinit()
{
    al_destroy_bitmap(sprites.map);

    al_destroy_bitmap(sprites.player.left[0]);
    al_destroy_bitmap(sprites.player.left[1]);
    al_destroy_bitmap(sprites.player.left[2]);
    al_destroy_bitmap(sprites.player.left[3]);
    al_destroy_bitmap(sprites.player.left[4]);

    al_destroy_bitmap(sprites._sheet);
}

static void map_render(stTILE  *tiles, size_t tile_len) {
    
    for (int i = 0; i < tile_len; ++i) {
        float x = tiles[i].obj.phy.pos.x;
        float y = tiles[i].obj.phy.pos.y;
        map_scale_disp(x, y, SCALE, SCALE, 0);
    }
}
#if 0
// --- fx ---

typedef struct FX
{
    int x, y;
    int frame;
    bool spark;
    bool used;
} FX;

#define FX_N 128
FX fx[FX_N];

void fx_init()
{
    for (int i = 0; i < FX_N; i++)
        fx[i].used = false;
}

void fx_add(bool spark, int x, int y)
{/*
    if (!spark)
        al_play_sample(sample_explode[between(0, 2)], 0.75, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);*/

    for (int i = 0; i < FX_N; i++)
    {
        if (fx[i].used)
            continue;

        fx[i].x = x;
        fx[i].y = y;
        fx[i].frame = 0;
        fx[i].spark = spark;
        fx[i].used = true;
        return;
    }
}

void fx_update()
{
    for (int i = 0; i < FX_N; i++)
    {
        if (!fx[i].used)
            continue;

        fx[i].frame++;

        if ((!fx[i].spark && (fx[i].frame == (EXPLOSION_FRAMES * 2)))
            || (fx[i].spark && (fx[i].frame == (SPARKS_FRAMES * 2)))
            )
            fx[i].used = false;
    }
}

void fx_draw()
{
    for (int i = 0; i < FX_N; i++)
    {
        if (!fx[i].used)
            continue;

        int frame_display = fx[i].frame / 2;
        ALLEGRO_BITMAP* bmp =
            fx[i].spark
            ? sprites.sparks[frame_display]
            : sprites.explosion[frame_display]
            ;

        int x = fx[i].x - (al_get_bitmap_width(bmp) / 2);
        int y = fx[i].y - (al_get_bitmap_height(bmp) / 2);
        al_draw_bitmap(bmp, x, y, 0);
    }
}
#endif
// --- hud ---
#if 0
ALLEGRO_FONT* font;
long score_display;

void hud_init()
{
    font = al_create_builtin_font();
    must_init(font, "font");

    score_display = 0;
}

void hud_deinit()
{
    al_destroy_font(font);
}

void hud_update()
{
}

void hud_draw()
{
    al_draw_textf(
        font,
        al_map_rgb_f(1, 1, 1),
        1, 1,
        0,
        "%06ld",
        score_display
    );

    int spacing = LIFE_W + 1;
    /*
    for (int i = 0; i < ship.lives; i++)
        al_draw_bitmap(sprites.life, 1 + (i * spacing), 10, 0);

    if (ship.lives < 0)
        al_draw_text(
            font,
            al_map_rgb_f(1, 1, 1),
            BUFFER_W / 2, BUFFER_H / 2,
            ALLEGRO_ALIGN_CENTER,
            "G A M E  O V E R"
        );*/
}
#endif

void init_render(ALLEGRO_EVENT_QUEUE* queue)
{
    disp_init();

    must_init(al_init_image_addon(), "image");
    sprites_init();
#if 0
    hud_init();
#endif
    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_display_event_source(disp));
#if 0
    fx_init();
#endif
}

void deinit_render()
{
    disp_deinit();
}
