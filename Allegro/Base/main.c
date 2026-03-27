#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>

#include "bugglebuggle.h"
#include "util.h"
#include "player.h"
#include "render.h"
#include "game_manager.h"

/************************************************/
/*         Local Function Declaration           */
/************************************************/

/* Init Modules */
ALLEGRO_EVENT_QUEUE* init_queue(void);
ALLEGRO_TIMER* init_timer(ALLEGRO_EVENT_QUEUE* queue);

/* Input Processing */

/************************************************/
/*         Local Variable Declaration           */
/************************************************/
static int frames;

/************************************************/
/*          Global Function Definition          */
/************************************************/
int main()
{
    must_init(al_init(), "allegro");

    ALLEGRO_EVENT_QUEUE* queue = init_queue();
    ALLEGRO_TIMER* timer = init_timer(queue);

    init_keyboard(queue);
    init_render(queue);
    init_player(GAME_MANAGER_GetPlayer(0));

    frames = 0;

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while (1)
    {
        al_wait_for_event(queue, &event);

        GAME_MANAGER_SetStage(eGAME_STAGE_1);// Need To Condition

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            /* Set Player & Enemy State, Direction, Delta Pos */
            send_input();
            /* Calculate Interaction & State */
            GAME_MANAGER_CheckCollision();
            /* Apply Calculated Physics */
            GAME_MANAGER_UpdatePhysics();
            /* Apply Object Status */
            GAME_MANAGER_UpdateObject();

            render_update();

#if (DEBUG_PLAYER == 1)
            player_debug(GAME_MANAGER_GetPlayer(0));
#endif

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
            render_draw();
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
