#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include <stdbool.h>
#include "object.h"
#include "player.h"

#define BUBBLE_SPEED    (2.0)
#define BUBBLE_UP_SPEED (-1.0)    

typedef enum _bubble_state {

    eBUBBLE_STATE_SHOOTING = 0, 
    eBUBBLE_STATE_FLOAT,       
    eBUBBLE_STATE_POP,
    eBUBBLE_STATE_MAX,

}eBUBBLE_STATE;


typedef struct _bubble
{
    stOBJECT obj;
    int frame;        
    int dur;          
    eBUBBLE_STATE state;
} stBUBBLE;

void bubble_init(stBUBBLE* bubbles);
bool bubble_add(stPLAYER* player, stBUBBLE* bubbles);
void bubble_update(stBUBBLE* bubbles);
void bubble_update_frame(stBUBBLE* bubbles);


#endif
