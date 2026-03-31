#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>

#include "bugglebuggle.h"
#include "util.h"
#include "keyboard.h"
#include "render.h"
#include "game_manager.h"

/************************************************/
/*         Local Function Declaration           */
/************************************************/
/* Init Modules */
ALLEGRO_EVENT_QUEUE* init_queue(void);
ALLEGRO_TIMER* init_timer(ALLEGRO_EVENT_QUEUE* queue);

/* Main Routine */
int routine_game(ALLEGRO_EVENT_QUEUE* queue, eGAME_STATE game_state);
/* Sub Routine */
void routine_main(ALLEGRO_EVENT_QUEUE* queue);
void routine_ingame(ALLEGRO_EVENT_QUEUE* queue);
void routine_name(ALLEGRO_EVENT_QUEUE* queue);
void routine_score(ALLEGRO_EVENT_QUEUE* queue);

/************************************************/
/*         Local Variable Declaration           */
/************************************************/

/************************************************/
/*          Global Function Definition          */
/************************************************/
int main()
{
    int frames;
    bool done = false;
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_TIMER* timer;
    eGAME_STATE game_state;

    /* Module initialization */
    must_init(al_init(), "allegro");

    queue = init_queue();
    timer = init_timer(queue);

    init_keyboard(queue);
    init_render(queue);

    /* TODO: Need to Modify */

    frames = 0;
    al_start_timer(timer);
    while (1)
    {
        al_wait_for_event(queue, &event);

        /* Check Game Status */
        game_state = GAME_MANAGER_UpdateState();

        /* Event Processing */
        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            {
                done = routine_game(queue, game_state);
                frames++;
            }
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            {
                done = true;
            }
            break;
        }

        if (done)
            break;

        keyboard_update(&event);
    }

    deinit_render();

    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

/************************************************/
/*          Local Function Definition           */
/************************************************/
static int routine_game(ALLEGRO_EVENT_QUEUE* queue, eGAME_STATE game_state)
{
    int done = false;

    switch (game_state) {
    case eGAME_STATE_MAIN:
        routine_main(queue);
        break;
    case eGAME_STATE_INGAME:
        routine_ingame(queue);
        break;
    case eGAME_STATE_NAME:
        routine_name(queue);
        break;
    case eGAME_STATE_SCORE:
        routine_score(queue);
        break;
    case eGAME_STATE_END:
        done = true;
        break;
    default:
        break;
    }

    return done;
}

static void routine_main(ALLEGRO_EVENT_QUEUE* queue)
{
    if (GAME_MANAGER_IsLoading() == true)
        return;

    if (keyboard_processing_main() == eMAIN_STATE_START) {
        GAME_MANAGER_SetGameStage_Next();
    }
    if (al_is_event_queue_empty(queue)) {
        render_draw_main();
    }
}

static void routine_ingame(ALLEGRO_EVENT_QUEUE* queue)
{
    if (GAME_MANAGER_IsLoading() == false) {
        /* Set Player & Enemy State, Direction, Delta Pos */
        keyboard_processing_ingame();
        /* Calculate Interaction & State */
        GAME_MANAGER_CheckCollision();
        /* Apply Calculated Physics */
        GAME_MANAGER_UpdatePhysics();
        /* Apply Object Status */
        GAME_MANAGER_UpdateObject();
    }

    /* Rendering */
    if (al_is_event_queue_empty(queue)) {
        render_draw_ingame();
    }
    /* Update Stage */
    GAME_MANAGER_UpdateStage();
    
#if (DEBUG_STAGE == 1)
    if (test_next_stage_input()) {
        GAME_MANAGER_SetGameStage_Next();
    }
#endif
}

static void routine_name(ALLEGRO_EVENT_QUEUE* queue)
{
    if (GAME_MANAGER_IsLoading() == true) {
        // Draw Gameover
        render_draw_game_end();
        return;
    }

    if (al_is_event_queue_empty(queue)) {
        render_draw_name();
    }

    if (keyboard_processing_name()) {
        GAME_MANAGER_SetGameState(eGAME_STATE_SCORE);
        render_draw_score();
    }

}

static void routine_score(ALLEGRO_EVENT_QUEUE* queue)
{
    if (GAME_MANAGER_IsLoading() == true)
        return;

    if (al_is_event_queue_empty(queue)) {
    }
    if (keyboard_processing_score()) {
        GAME_MANAGER_SetGameState(eGAME_STATE_MAIN);
    }
}

static ALLEGRO_EVENT_QUEUE* init_queue(void)
{
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    return queue;
}

static ALLEGRO_TIMER* init_timer(ALLEGRO_EVENT_QUEUE* queue)
{
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / CONFIG_SYSTEM_FRAME);
    must_init(timer, "timer");
    al_register_event_source(queue, al_get_timer_event_source(timer));

    return timer;
}
