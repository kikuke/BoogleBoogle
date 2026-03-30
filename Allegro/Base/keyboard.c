#include "bugglebuggle.h"
#include "keyboard.h"
#include "game_manager.h"
#include "player.h"

/************************************************/
/*         Local Variable Declaration           */
/************************************************/
typedef struct {
    bool is_input;
    int idx;
    char data[CONFIG_SYSTEM_PLAYER_NAME_MAX];
} stPLAYER_NAME;
/************************************************/
/*         Local Variable Definition           */
/************************************************/
static unsigned char key[ALLEGRO_KEY_MAX];
static stPLAYER_NAME player_name;

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
        if (player_name.is_input) {
            // TODO: Need to check bound
            player_name.data[player_name.idx++] = event->keyboard.keycode + 'A' - 1;
            player_name.data[player_name.idx] = 0;
        }
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
            if (iKeyInput == ALLEGRO_KEY_DOWN && !(key[iKeyInput] & KEY_SEEN))
                break;

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

bool keyboard_processing_name(void)
{
    player_name.is_input = true;

    if (key[ALLEGRO_KEY_SPACE] & KEY_SEEN) {
        player_name.data[player_name.idx - 1] = 0;
        player_name.is_input = false;
        player_name.idx = 0;
    }
    return !player_name.is_input;
}

bool keyboard_processing_score(void)
{
    return key[ALLEGRO_KEY_SPACE] & KEY_SEEN;
}

const char* keboard_get_name(void)
{
    return player_name.data;
}

#if (DEBUG_STAGE == 1)
bool test_next_stage_input(void)
{
    return key[ALLEGRO_KEY_N] & KEY_SEEN;
}
#endif
