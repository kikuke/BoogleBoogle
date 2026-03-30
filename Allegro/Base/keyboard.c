#include "bugglebuggle.h"
#include "keyboard.h"
#include "game_manager.h"
#include "player.h"

/************************************************/
/*         Local Variable Declaration           */
/************************************************/
static unsigned char key[ALLEGRO_KEY_MAX];

/************************************************/
/*          Global Function Definition          */
/************************************************/
void init_keyboard(ALLEGRO_EVENT_QUEUE* queue)
{
    must_init(al_install_keyboard(), "keyboard");
    al_register_event_source(queue, al_get_keyboard_event_source());
    memset(key, 0, sizeof(key));
}

void keyboard_update(ALLEGRO_EVENT* event)
{
    switch (event->type)
    {
    case ALLEGRO_EVENT_TIMER:
        for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
            key[i] &= ~KEY_SEEN;
        break;

    case ALLEGRO_EVENT_KEY_DOWN:
        key[event->keyboard.keycode] = KEY_SEEN | KEY_DOWN;
        break;
    case ALLEGRO_EVENT_KEY_UP:
        key[event->keyboard.keycode] &= ~KEY_DOWN;
        break;
    }
}

eMAIN_STATE keyboard_processing_main(void)
{
    return key[ALLEGRO_KEY_SPACE] ? eMAIN_STATE_START : eMAIN_STATE_WAIT;
}

void keyboard_processing_ingame(void)
{
    for (int iKeyInput = 0; iKeyInput < ALLEGRO_KEY_MAX; ++iKeyInput) {
        switch (iKeyInput) {
        case ALLEGRO_KEY_LEFT:
        case ALLEGRO_KEY_RIGHT:
        case ALLEGRO_KEY_UP:
        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_SPACE:
        {
            if (key[iKeyInput]) {
                player_update_input(GAME_MANAGER_GetPlayer(0), iKeyInput, key[iKeyInput]);
            }
        }
        break;
        default:
            break;
        }
    }
}

#if (DEBUG_STAGE == 1)
bool test_next_stage_input(void)
{
    return key[ALLEGRO_KEY_N] & KEY_SEEN;
}
#endif
