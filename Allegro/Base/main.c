#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>

#include "bugglebuggle.h"
#include "util.h"
#include "keyboard.h"
#include "player.h"
#include "render.h"
#include "game_manager.h"

/************************************************/
/*         Local Function Declaration           */
/************************************************/
/* Init Modules */
ALLEGRO_EVENT_QUEUE* init_queue(void);
ALLEGRO_TIMER* init_timer(ALLEGRO_EVENT_QUEUE* queue);
void routine_ingame(void);

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
    bool redraw = true;
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
    init_player(GAME_MANAGER_GetPlayer(0));

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
            if (game_state == eGAME_STATE_INGAME) {
                routine_ingame();
            }

            redraw = true;
            frames++;
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        keyboard_update(&event);

        if (redraw && al_is_event_queue_empty(queue))
        {
            render_draw(game_state);
            redraw = false;
        }
    }

    deinit_render();

    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

/************************************************/
/*          Local Function Definition           */
/************************************************/
static void routine_ingame(void)
{
    /* Set Player & Enemy State, Direction, Delta Pos */
    keyboard_processing_ingame();
    /* Calculate Interaction & State */
    GAME_MANAGER_CheckCollision();
    /* Apply Calculated Physics */
    GAME_MANAGER_UpdatePhysics();
    /* Apply Object Status */
    GAME_MANAGER_UpdateObject();
    /* Update Rendering */
    render_update_ingame();
}

static ALLEGRO_EVENT_QUEUE* init_queue(void)
{
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    return queue;
}

static ALLEGRO_TIMER* init_timer(ALLEGRO_EVENT_QUEUE* queue)
{
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");
    al_register_event_source(queue, al_get_timer_event_source(timer));

    return timer;
}
