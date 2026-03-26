#ifndef __BUBBLE_H__
#define __BUBBLE_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum _bubble_state {

    eBUBBLE_STATE_SHOOTING = 0, // 발사하는도중 -> 이떄 충돌시 적을 버블에 가둠
    eBUBBLE_STATE_FLOAT,        // 일정거리 이동 or 벽과 충돌시, 버블이 커지며 공중에 떠오름
    eBUBBLE_STATE_MAX,

}eBUBBLE_STATE;


typedef struct _bubble
{
    int x, y, dx, dy; // 위치와 속도
    int frame;        // 몇 프레임동안 유지? 
    int dur;          // 유지 시간
    bool used;        // 사용하는지
    eBUBBLE_STATE state; // 상태
}stBUBBLE;

void shots_init();
bool bubble_add();


#endif
