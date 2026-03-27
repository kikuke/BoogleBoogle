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

#define FLAG_0 (0)
#define MAP (28)
#define SCALE (10)
#define CHARACTER (48)
#define HEART (30)
#define SPRITE_PLAYER_MAX (5)
#define SPRITE_PLAYER_STAY (2)
#define SPRITE_PLAYER_ATTACK (4)
#define BUBBLE (55)
#define BUBBLE_POP (70)
#define SPRITE_ENEMY_MAX (5)
#define SPRITE_ENEMY_STAY (2)
#define SPRITE_ENEMY_EASY (50)
#define SPRITE_ENEMY_HARD (47)
#define ENEMY_THROW (21)

typedef struct 
{
    int idx;
    ALLEGRO_BITMAP* left[SPRITE_PLAYER_MAX];
    ALLEGRO_BITMAP* right[SPRITE_PLAYER_MAX];
    ALLEGRO_BITMAP* stay[SPRITE_PLAYER_STAY];
    ALLEGRO_BITMAP* left_attack[SPRITE_PLAYER_ATTACK];
    ALLEGRO_BITMAP* right_attack[SPRITE_PLAYER_ATTACK];
    ALLEGRO_BITMAP* curr_move;
} stSPRITE_PLAYER;

typedef struct 
{
    int idx;
    ALLEGRO_BITMAP* left[SPRITE_ENEMY_MAX];
    ALLEGRO_BITMAP* right[SPRITE_ENEMY_MAX];
    ALLEGRO_BITMAP* stay[SPRITE_ENEMY_STAY];
    ALLEGRO_BITMAP* curr_move;
} stSPRITE_ENEMY_EASY;

typedef struct
{
    int idx;
    ALLEGRO_BITMAP* left[SPRITE_ENEMY_MAX];
    ALLEGRO_BITMAP* right[SPRITE_ENEMY_MAX];
    ALLEGRO_BITMAP* stay[SPRITE_ENEMY_STAY];
    ALLEGRO_BITMAP* curr_move;
    ALLEGRO_BITMAP* throw;
} stSPRITE_ENEMY_HARD;

typedef struct 
{
    ALLEGRO_BITMAP* bubble_idle;
    ALLEGRO_BITMAP* bubble_pop;
} stSPRITE_BUBBLE;

typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;
    ALLEGRO_BITMAP* _title_sheet;
    ALLEGRO_BITMAP* heart;
    ALLEGRO_BITMAP* map;
    stSPRITE_PLAYER player;
    stSPRITE_ENEMY_EASY enemy_easy;
    stSPRITE_ENEMY_HARD enemy_hard;
    stSPRITE_BUBBLE bubble;
} SPRITES;

/************************************************/
/*         Local Function Declaration           */
/************************************************/
void disp_pre_draw();
void disp_post_draw();
void render_heart();
void test_disp(float x, float y);
void test_scale_disp(float dx, float dy, float dw, float dh, int flags);

void heart_scale_disp(float dx, float dy, float dw, float dh, int flags);
void test_render_heart(int heart_cnt);

//map
void map_scale_disp(float dx, float dy, float dw, float dh, int flags);
void render_map(stTILE* tiles, size_t tile_len);

//player
void render_player_move(stPLAYER* player);
void character_scale_disp(ALLEGRO_BITMAP* sprite, float px, float py, float dw, float dh, int flags);

//enemy
void enemy_easy_scale_disp(ALLEGRO_BITMAP* sprite, float px, float py, float dw, float dh, int flags);
void enemy_hard_scale_disp(ALLEGRO_BITMAP* sprite, float px, float py, float dw, float dh, int flags);
void render_enemy_easy_move(stENEMY* enemy);
void render_enemy_hard_move(stENEMY* enemy);
void render_enemy_throw_attack(stOBJECT* enemy_throw);
void enemy_throw_scale_disp(ALLEGRO_BITMAP* sprite, float px, float py, float dw, float dh, int flags);
static ALLEGRO_DISPLAY* disp;
static ALLEGRO_BITMAP* buffer;

/************************************************/
/*         Local Variable Declaration           */
/************************************************/
SPRITES sprites;

  

    

/************************************************/
/*          Global Function Definition          */
/************************************************/
void render_update_ingame(void)
{

#if 0
    fx_update();
    hud_update();
#endif
}

void render_draw(eGAME_STATE state)
{
#if (DEBUG_PLAYER == 1)
    player_debug(GAME_MANAGER_GetPlayer(0));
#endif

    disp_pre_draw();
    al_clear_to_color(al_map_rgb(0, 0, 0));
    switch (state) {
    case eGAME_STATE_MAIN:
        
        break;

    case eGAME_STATE_INGAME:

        
        render_map(GAME_MANAGER_GetMap(), CONFIG_MAP_Y_MAX * CONFIG_MAP_X_MAX);
        test_render_heart(3);
        //render_heart();
        test_scale_disp(30, 30, SCALE, SCALE, 0);

        render_player_move(GAME_MANAGER_GetPlayer(0));
        render_enemy_throw_attack(GAME_MANAGER_GetEnemyAttacks());
        stENEMY* enemies = GAME_MANAGER_GetEnemy();
        for (int i = 0; i < CONFIG_OBJECT_ENEMY_MAX; ++i) {
            stENEMY* enemy = enemies + i;
            if (enemy->obj.is_active == true) {
                render_enemy_easy_move(enemy);
            }
        }
        break;
    }
    //if (state == eGAME_STATE_MAIN) {
    //    disp_pre_draw();
    //    al_clear_to_color(al_map_rgb(0, 0, 0));
    //}
    //if (state == eGAME_STATE_INGAME) {

    //    disp_pre_draw();
    //    al_clear_to_color(al_map_rgb(0, 0, 0));
    //    render_map(GAME_MANAGER_GetMap(), CONFIG_MAP_Y_MAX * CONFIG_MAP_X_MAX);
    //    test_render_heart(3);
    //    //render_heart();
    //    //test_scale_disp(30, 30, SCALE, SCALE, 0);

    //    render_player_move(GAME_MANAGER_GetPlayer(0));

    //    enemy_throw_attack(GAME_MANAGER_GetEnemyAttacks());
    //    //character_scale_disp(10, 220, SCALE, SCALE, 0);
    //}
    
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
    al_draw_bitmap(sprites.enemy_easy.left[0], x, y, 0);
}
static void test_scale_disp(float dx, float dy, float dw, float dh, int flags) {
    al_draw_scaled_bitmap(sprites.player.stay[1], 0, 0, CHARACTER, CHARACTER, dx, dy, dw, dh, flags);
}

static void map_scale_disp(float dx, float dy, float dw, float dh, int flags) {
    al_draw_scaled_bitmap(sprites.map,0,0, MAP, MAP, dx, dy, dw, dh, flags);
}

static void character_scale_disp(ALLEGRO_BITMAP *sprite, float px, float py, float dw, float dh, int flags) {
    al_draw_scaled_bitmap(sprite, 0, 0, CHARACTER, CHARACTER, px, py, dw, dh, flags);
}

static void enemy_easy_scale_disp(ALLEGRO_BITMAP* sprite, float px, float py, float dw, float dh, int flags) {
    al_draw_scaled_bitmap(sprite, 0, 0, SPRITE_ENEMY_EASY, SPRITE_ENEMY_EASY, px, py, dw, dh, flags);
}

static void enemy_hard_scale_disp(ALLEGRO_BITMAP* sprite, float px, float py, float dw, float dh, int flags) {
    al_draw_scaled_bitmap(sprite, 0, 0, SPRITE_ENEMY_HARD, SPRITE_ENEMY_HARD, px, py, dw, dh, flags);
}
static void enemy_throw_scale_disp(ALLEGRO_BITMAP* sprite, float px, float py, float dw, float dh, int flags) {
    al_draw_scaled_bitmap(sprite, 0, 0, ENEMY_THROW, ENEMY_THROW, px, py, dw, dh, flags);
}

static void heart_scale_disp(float dx, float dy, float dw, float dh, int flags) {
    al_draw_scaled_bitmap(sprites.heart, 0, 0, HEART, HEART, dx, dy, dw, dh, flags);;
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
    //sprites._title_sheet = al_load_bitmap("Resource/Bubble_Bobble_sprite_2.png");
    must_init(sprites._sheet, "spritesheet");
    //must_init(sprites._title_sheet, "spritesheet");

    //map
    sprites.map = sprite_grab(8, 1578, MAP, MAP);

    //p_move
    sprites.player.stay[0] = sprite_grab(268, 143, CHARACTER, CHARACTER);
    sprites.player.stay[1] = sprite_grab(332, 143, CHARACTER, CHARACTER);
    sprites.player.left[0] = sprite_grab(270, 16, CHARACTER, CHARACTER);
    sprites.player.left[1] = sprite_grab(206, 16, CHARACTER, CHARACTER);
    sprites.player.left[2] = sprite_grab(141, 16, CHARACTER, CHARACTER);
    sprites.player.left[3] = sprite_grab(76, 16, CHARACTER, CHARACTER);
    sprites.player.left[4] = sprite_grab(10, 16, CHARACTER, CHARACTER);
    sprites.player.right[0]= sprite_grab(334, 16, CHARACTER, CHARACTER);
    sprites.player.right[1] = sprite_grab(400, 16, CHARACTER, CHARACTER);
    sprites.player.right[2] = sprite_grab(465, 16, CHARACTER, CHARACTER);
    sprites.player.right[3] = sprite_grab(528, 16, CHARACTER, CHARACTER);
    sprites.player.right[4] = sprite_grab(590, 16, CHARACTER, CHARACTER);

    //p_attack
    sprites.player.left_attack[0] = sprite_grab(268,335, CHARACTER, CHARACTER);
    sprites.player.left_attack[1] = sprite_grab(204, 335, CHARACTER, CHARACTER);
    sprites.player.left_attack[2] = sprite_grab(140, 335, CHARACTER, CHARACTER);
    sprites.player.left_attack[3] = sprite_grab(74, 335, CHARACTER, CHARACTER);
    sprites.player.right_attack[0] = sprite_grab(332, 335, CHARACTER, CHARACTER);
    sprites.player.right_attack[1] = sprite_grab(397, 335, CHARACTER, CHARACTER);
    sprites.player.right_attack[2] = sprite_grab(463, 335, CHARACTER, CHARACTER);
    sprites.player.right_attack[3] = sprite_grab(528, 335, CHARACTER, CHARACTER);

    //enemy_easy
    sprites.enemy_easy.left[0] = sprite_grab(14, 528,SPRITE_ENEMY_EASY, SPRITE_ENEMY_EASY);
    sprites.enemy_easy.left[1] = sprite_grab(78, 528, SPRITE_ENEMY_EASY, SPRITE_ENEMY_EASY);
    sprites.enemy_easy.left[2] = sprite_grab(142, 528, SPRITE_ENEMY_EASY, SPRITE_ENEMY_EASY);
    sprites.enemy_easy.left[3] = sprite_grab(206, 528, SPRITE_ENEMY_EASY, SPRITE_ENEMY_EASY);
    sprites.enemy_easy.left[4] = sprite_grab(270, 528, SPRITE_ENEMY_EASY, SPRITE_ENEMY_EASY);
    
    sprites.enemy_easy.right[0] = sprite_grab(589, 528, SPRITE_ENEMY_EASY, SPRITE_ENEMY_EASY);
    sprites.enemy_easy.right[1] = sprite_grab(530, 528, SPRITE_ENEMY_EASY, SPRITE_ENEMY_EASY);
    sprites.enemy_easy.right[2] = sprite_grab(467, 528, SPRITE_ENEMY_EASY, SPRITE_ENEMY_EASY);
    sprites.enemy_easy.right[3] = sprite_grab(402, 528, SPRITE_ENEMY_EASY, SPRITE_ENEMY_EASY);
    sprites.enemy_easy.right[4] = sprite_grab(332, 528, SPRITE_ENEMY_EASY, SPRITE_ENEMY_EASY);

    //enemy_hard
    sprites.enemy_hard.throw = sprite_grab(50, 659, ENEMY_THROW, ENEMY_THROW);

    sprites.enemy_hard.left[0] = sprite_grab(16, 1309, SPRITE_ENEMY_HARD, SPRITE_ENEMY_HARD);
    sprites.enemy_hard.left[1] = sprite_grab(80, 1309, SPRITE_ENEMY_HARD, SPRITE_ENEMY_HARD);
    sprites.enemy_hard.left[2] = sprite_grab(144, 1309, SPRITE_ENEMY_HARD, SPRITE_ENEMY_HARD);
    sprites.enemy_hard.left[3] = sprite_grab(208, 1309, SPRITE_ENEMY_HARD, SPRITE_ENEMY_HARD);
    sprites.enemy_hard.left[4] = sprite_grab(272, 1309, SPRITE_ENEMY_HARD, SPRITE_ENEMY_HARD);

    sprites.enemy_hard.right[0] = sprite_grab(591, 1309, SPRITE_ENEMY_HARD, SPRITE_ENEMY_HARD);
    sprites.enemy_hard.right[1] = sprite_grab(525, 1309, SPRITE_ENEMY_HARD, SPRITE_ENEMY_HARD);
    sprites.enemy_hard.right[2] = sprite_grab(461, 1309, SPRITE_ENEMY_HARD, SPRITE_ENEMY_HARD);
    sprites.enemy_hard.right[3] = sprite_grab(397, 1309, SPRITE_ENEMY_HARD, SPRITE_ENEMY_HARD);
    sprites.enemy_hard.right[4] = sprite_grab(333, 1309, SPRITE_ENEMY_HARD, SPRITE_ENEMY_HARD);


    //bubble
    sprites.bubble.bubble_idle = sprite_grab(135,1467,BUBBLE ,BUBBLE);
    sprites.bubble.bubble_pop = sprite_grab(416, 1460, BUBBLE_POP, BUBBLE_POP);
    //hud
    sprites.heart = sprite_grab(8, 1436, HEART, HEART);
}


void sprites_deinit()
{
    al_destroy_bitmap(sprites.map);

    al_destroy_bitmap(sprites.player.left[0]);
    al_destroy_bitmap(sprites.player.left[1]);
    al_destroy_bitmap(sprites.player.left[2]);
    al_destroy_bitmap(sprites.player.left[3]);
    al_destroy_bitmap(sprites.player.left[4]);
    al_destroy_bitmap(sprites.player.right[0]);
    al_destroy_bitmap(sprites.player.right[1]);
    al_destroy_bitmap(sprites.player.right[2]);
    al_destroy_bitmap(sprites.player.right[3]);
    al_destroy_bitmap(sprites.player.right[4]);

    al_destroy_bitmap(sprites.player.left_attack[0]);
    al_destroy_bitmap(sprites.player.left_attack[1]);
    al_destroy_bitmap(sprites.player.left_attack[2]);
    al_destroy_bitmap(sprites.player.left_attack[3]);
    al_destroy_bitmap(sprites.player.right_attack[0]);
    al_destroy_bitmap(sprites.player.right_attack[1]);
    al_destroy_bitmap(sprites.player.right_attack[2]);
    al_destroy_bitmap(sprites.player.right_attack[3]);

    al_destroy_bitmap(sprites.heart);

    al_destroy_bitmap(sprites.bubble.bubble_idle);
    al_destroy_bitmap(sprites.bubble.bubble_pop);

    al_destroy_bitmap(sprites.enemy_easy.left[0]);
    al_destroy_bitmap(sprites.enemy_easy.left[1]);
    al_destroy_bitmap(sprites.enemy_easy.left[2]);
    al_destroy_bitmap(sprites.enemy_easy.left[3]);
    al_destroy_bitmap(sprites.enemy_easy.left[4]);

    al_destroy_bitmap(sprites.enemy_easy.right[0]);
    al_destroy_bitmap(sprites.enemy_easy.right[1]);
    al_destroy_bitmap(sprites.enemy_easy.right[2]);
    al_destroy_bitmap(sprites.enemy_easy.right[3]);
    al_destroy_bitmap(sprites.enemy_easy.right[4]);


    al_destroy_bitmap(sprites.enemy_hard.left[0]);
    al_destroy_bitmap(sprites.enemy_hard.left[1]);
    al_destroy_bitmap(sprites.enemy_hard.left[2]);
    al_destroy_bitmap(sprites.enemy_hard.left[3]);
    al_destroy_bitmap(sprites.enemy_hard.left[4]);

    al_destroy_bitmap(sprites.enemy_hard.right[0]);
    al_destroy_bitmap(sprites.enemy_hard.right[1]);
    al_destroy_bitmap(sprites.enemy_hard.right[2]);
    al_destroy_bitmap(sprites.enemy_hard.right[3]);
    al_destroy_bitmap(sprites.enemy_hard.right[4]);



    al_destroy_bitmap(sprites.enemy_hard.throw);

    al_destroy_bitmap(sprites._sheet);
}

static void render_main_title() {

}

static void render_map(stTILE  *tiles, size_t tile_len) {
    
    for (int i = 0; i < tile_len; ++i) {
        float x = tiles[i].obj.phy.pos.x;
        float y = tiles[i].obj.phy.pos.y;
        map_scale_disp(x, y, SCALE, SCALE, FLAG_0);
    }
}
static void render_player_move(stPLAYER* player) {
    if (player == NULL) return;

    static int time = 0;

    switch (player->obj.rend.is_move)
    {
    case true:
        time++;
        sprites.player.idx = (time/6) % SPRITE_PLAYER_MAX;;

        switch (player->obj.phy.look)
        {
        case eDIR_LOOK_LEFT:
            sprites.player.curr_move = sprites.player.left[sprites.player.idx];
            break;
        case eDIR_LOOK_RIGHT:
            sprites.player.curr_move = sprites.player.right[sprites.player.idx];
            break;
        }
        break;
    case false:
        time = 0;
        switch (player->obj.phy.look)
        {
        case eDIR_LOOK_LEFT:
            sprites.player.curr_move = sprites.player.stay[0];
            break;
        case eDIR_LOOK_RIGHT:
            sprites.player.curr_move = sprites.player.stay[1];
            break;
        }
        break;

    }
    character_scale_disp(sprites.player.curr_move, player->obj.phy.pos.x, player->obj.phy.pos.y, SCALE, SCALE, FLAG_0);

}

static void render_enemy_easy_move(stENEMY* enemy) {
    if (enemy == NULL) return;

    static int time = 0;

    switch (enemy->obj.rend.is_move)
    {
    case true:
        time++;
        sprites.enemy_easy.idx = (time / 6) % SPRITE_PLAYER_MAX;;

        switch (enemy->obj.phy.look)
        {
        case eDIR_LOOK_LEFT:
            sprites.enemy_easy.curr_move = sprites.enemy_easy.left[sprites.enemy_easy.idx];
            break;
        case eDIR_LOOK_RIGHT:
            sprites.enemy_easy.curr_move = sprites.enemy_easy.right[sprites.enemy_easy.idx];
            break;
        }
        break;
    case false:
        time = 0;
        switch (enemy->obj.phy.look)
        {
        case eDIR_LOOK_LEFT:
            sprites.enemy_easy.curr_move = sprites.enemy_easy.left[0];
            break;
        case eDIR_LOOK_RIGHT:
            sprites.enemy_easy.curr_move = sprites.enemy_easy.right[0];
            break;
        }
        break;

    }
    enemy_easy_scale_disp(sprites.enemy_easy.curr_move, enemy->obj.phy.pos.x, enemy->obj.phy.pos.y, SCALE, SCALE, FLAG_0);

}

static void render_enemy_hard_move(stENEMY* enemy) {
    if (enemy == NULL) return;

    static int time = 0;

    switch (enemy->obj.rend.is_move)
    {
    case true:
        time++;
        sprites.enemy_easy.idx = (time / 6) % SPRITE_PLAYER_MAX;;

        switch (enemy->obj.phy.look)
        {
        case eDIR_LOOK_LEFT:
            sprites.enemy_hard.curr_move = sprites.enemy_hard.left[sprites.enemy_hard.idx];
            break;
        case eDIR_LOOK_RIGHT:
            sprites.enemy_hard.curr_move = sprites.enemy_hard.right[sprites.enemy_hard.idx];
            break;
        }
        break;
    case false:
        time = 0;
        switch (enemy->obj.phy.look)
        {
        case eDIR_LOOK_LEFT:
            sprites.enemy_hard.curr_move = sprites.enemy_hard.left[0];
            break;
        case eDIR_LOOK_RIGHT:
            sprites.enemy_hard.curr_move = sprites.enemy_hard.right[0];
            break;
        }
        break;

    }
    enemy_hard_scale_disp(sprites.enemy_hard.curr_move, enemy->obj.phy.pos.x, enemy->obj.phy.pos.y, SCALE, SCALE, FLAG_0);

}

static void render_enemy_throw_attack(stOBJECT* enemy_throw) {
    if (enemy_throw == NULL) return;
    for (int i = 0; i < CONFIG_OBJECT_ENEMY_ATTACK_MAX; ++i) {
        if (enemy_throw[i].rend.is_active) {
            enemy_throw_scale_disp(sprites.enemy_hard.throw, enemy_throw[i].phy.pos.x, enemy_throw[i].phy.pos.y, SCALE, SCALE, FLAG_0);
        }
    }
    

}


static void test_render_heart(int heart_cnt) {
    if (heart_cnt >= 1) {
        heart_scale_disp(0, 0, SCALE, SCALE, FLAG_0);
        
    }
    if (heart_cnt >= 2) {
        heart_scale_disp(10, 0, SCALE, SCALE, FLAG_0);
    }
    if (heart_cnt >= 3) {

        heart_scale_disp(20, 0, SCALE, SCALE, FLAG_0);
    }
    
}
static void render_heart() {
   
    heart_scale_disp(0, 0, SCALE, SCALE, FLAG_0);
    heart_scale_disp(10, 0, SCALE, SCALE, FLAG_0);
    heart_scale_disp(20, 0, SCALE, SCALE, FLAG_0);
    

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
