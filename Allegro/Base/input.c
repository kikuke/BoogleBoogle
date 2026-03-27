#include "bugglebuggle.h"
#include "input.h"
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

void send_input(void)
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
