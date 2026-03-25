#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>

#include "util.h"
#include "render.h"

ALLEGRO_DISPLAY* disp;
ALLEGRO_BITMAP* buffer;

void disp_init()
{
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

void disp_pre_draw()
{
    al_set_target_bitmap(buffer);
}

void disp_post_draw()
{
    al_set_target_backbuffer(disp);
    al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);

    al_flip_display();
}


// --- sprites ---

#define SHIP_W 12
#define SHIP_H 13

#define SHIP_SHOT_W 2
#define SHIP_SHOT_H 9

#define LIFE_W 6
#define LIFE_H 6

const int ALIEN_W[] = { 14, 13, 45 };
const int ALIEN_H[] = { 9, 10, 27 };

#define ALIEN_BUG_W      ALIEN_W[0]
#define ALIEN_BUG_H      ALIEN_H[0]
#define ALIEN_ARROW_W    ALIEN_W[1]
#define ALIEN_ARROW_H    ALIEN_H[1]
#define ALIEN_THICCBOI_W ALIEN_W[2]
#define ALIEN_THICCBOI_H ALIEN_H[2]

#define ALIEN_SHOT_W 4
#define ALIEN_SHOT_H 4

#define EXPLOSION_FRAMES 4
#define SPARKS_FRAMES    3


typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;

    ALLEGRO_BITMAP* ship;
    ALLEGRO_BITMAP* ship_shot[2];
    ALLEGRO_BITMAP* life;

    ALLEGRO_BITMAP* alien[3];
    ALLEGRO_BITMAP* alien_shot;

    ALLEGRO_BITMAP* explosion[EXPLOSION_FRAMES];
    ALLEGRO_BITMAP* sparks[SPARKS_FRAMES];

    ALLEGRO_BITMAP* powerup[4];
} SPRITES;
SPRITES sprites;

ALLEGRO_BITMAP* sprite_grab(int x, int y, int w, int h)
{
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
    must_init(sprite, "sprite grab");
    return sprite;
}

void sprites_init()
{
    sprites._sheet = al_load_bitmap("spritesheet.png");
    must_init(sprites._sheet, "spritesheet");

    sprites.ship = sprite_grab(0, 0, SHIP_W, SHIP_H);

    sprites.ship_shot[0] = sprite_grab(13, 0, SHIP_SHOT_W, SHIP_SHOT_H);
    sprites.ship_shot[1] = sprite_grab(16, 0, SHIP_SHOT_W, SHIP_SHOT_H);

    sprites.life = sprite_grab(0, 14, LIFE_W, LIFE_H);

    sprites.alien[0] = sprite_grab(19, 0, ALIEN_BUG_W, ALIEN_BUG_H);
    sprites.alien[1] = sprite_grab(19, 10, ALIEN_ARROW_W, ALIEN_ARROW_H);
    sprites.alien[2] = sprite_grab(0, 21, ALIEN_THICCBOI_W, ALIEN_THICCBOI_H);

    sprites.alien_shot = sprite_grab(13, 10, ALIEN_SHOT_W, ALIEN_SHOT_H);

    sprites.explosion[0] = sprite_grab(33, 10, 9, 9);
    sprites.explosion[1] = sprite_grab(43, 9, 11, 11);
    sprites.explosion[2] = sprite_grab(46, 21, 17, 18);
    sprites.explosion[3] = sprite_grab(46, 40, 17, 17);

    sprites.sparks[0] = sprite_grab(34, 0, 10, 8);
    sprites.sparks[1] = sprite_grab(45, 0, 7, 8);
    sprites.sparks[2] = sprite_grab(54, 0, 9, 8);

    sprites.powerup[0] = sprite_grab(0, 49, 9, 12);
    sprites.powerup[1] = sprite_grab(10, 49, 9, 12);
    sprites.powerup[2] = sprite_grab(20, 49, 9, 12);
    sprites.powerup[3] = sprite_grab(30, 49, 9, 12);
}

void sprites_deinit()
{
    al_destroy_bitmap(sprites.ship);

    al_destroy_bitmap(sprites.ship_shot[0]);
    al_destroy_bitmap(sprites.ship_shot[1]);

    al_destroy_bitmap(sprites.life);

    al_destroy_bitmap(sprites.alien[0]);
    al_destroy_bitmap(sprites.alien[1]);
    al_destroy_bitmap(sprites.alien[2]);

    al_destroy_bitmap(sprites.alien_shot);

    al_destroy_bitmap(sprites.explosion[0]);
    al_destroy_bitmap(sprites.explosion[1]);
    al_destroy_bitmap(sprites.explosion[2]);
    al_destroy_bitmap(sprites.explosion[3]);

    al_destroy_bitmap(sprites.sparks[0]);
    al_destroy_bitmap(sprites.sparks[1]);
    al_destroy_bitmap(sprites.sparks[2]);

    al_destroy_bitmap(sprites.powerup[0]);
    al_destroy_bitmap(sprites.powerup[1]);
    al_destroy_bitmap(sprites.powerup[2]);
    al_destroy_bitmap(sprites.powerup[3]);

    al_destroy_bitmap(sprites._sheet);
}


// --- audio ---

ALLEGRO_SAMPLE* sample_shot;
ALLEGRO_SAMPLE* sample_explode[2];

void audio_init()
{/*
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(128);

    sample_shot = al_load_sample("shot.flac");
    must_init(sample_shot, "shot sample");

    sample_explode[0] = al_load_sample("explode1.flac");
    must_init(sample_explode[0], "explode[0] sample");
    sample_explode[1] = al_load_sample("explode2.flac");
    must_init(sample_explode[1], "explode[1] sample");*/
}

void audio_deinit()
{/*
    al_destroy_sample(sample_shot);
    al_destroy_sample(sample_explode[0]);
    al_destroy_sample(sample_explode[1]);*/
}


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

// --- hud ---

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

void render_init(ALLEGRO_EVENT_QUEUE* queue)
{
    disp_init();

    audio_init();

    must_init(al_init_image_addon(), "image");
    sprites_init();

    hud_init();

    must_init(al_init_primitives_addon(), "primitives");

    must_init(al_reserve_samples(16), "reserve samples");

    al_register_event_source(queue, al_get_display_event_source(disp));

    fx_init();
}
