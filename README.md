# 🎮 BuggleBuggle — 완전한 API 레퍼런스 가이드

> **프로젝트:** BuggleBuggle — Allegro5 기반 C언어 2D 플랫폼 아케이드 게임  
> **버전:** v1.0 | **언어:** C (C99) | **의존성:** Allegro5, SQLite3

---

## 📑 Table of Contents

- [🎮 BuggleBuggle — 완전한 API 레퍼런스 가이드](#-bugglebuggle--완전한-api-레퍼런스-가이드)
  - [📑 Table of Contents](#-table-of-contents)
  - [시스템 아키텍처 및 실행 흐름도](#시스템-아키텍처-및-실행-흐름도)
    - [전체 게임 상태 흐름](#전체-게임-상태-흐름)
    - [게임 상태 전이 다이어그램](#게임-상태-전이-다이어그램)
    - [프레임당 업데이트 순서 (INGAME)](#프레임당-업데이트-순서-ingame)
  - [전역 설정 상수](#전역-설정-상수)
    - [시스템 설정](#시스템-설정)
    - [게임 설정](#게임-설정)
    - [물리 설정](#물리-설정)
    - [오브젝트 풀 설정](#오브젝트-풀-설정)
    - [열거형 (Enums)](#열거형-enums)
  - [핵심 데이터 구조](#핵심-데이터-구조)
    - [파생 구조체 계층](#파생-구조체-계층)
  - [Main Loop \& Game Manager](#main-loop--game-manager)
    - [`main`](#main)
    - [`GAME_MANAGER_UpdateState`](#game_manager_updatestate)
    - [`GAME_MANAGER_SetGameState`](#game_manager_setgamestate)
    - [`GAME_MANAGER_GetGameStage`](#game_manager_getgamestage)
    - [`GAME_MANAGER_SetGameStage_Next`](#game_manager_setgamestage_next)
    - [`GAME_MANAGER_UpdateStage`](#game_manager_updatestage)
    - [`GAME_MANAGER_GetScore`](#game_manager_getscore)
    - [`GAME_MANAGER_IsLoading`](#game_manager_isloading)
    - [`GAME_MANAGER_GetPlayer`](#game_manager_getplayer)
    - [`GAME_MANAGER_GetBubble`](#game_manager_getbubble)
    - [`GAME_MANAGER_GetEnemy`](#game_manager_getenemy)
    - [`GAME_MANAGER_GetEnemyAttacks`](#game_manager_getenemyattacks)
    - [`GAME_MANAGER_GetMap`](#game_manager_getmap)
    - [`GAME_MANAGER_CheckCollision`](#game_manager_checkcollision)
    - [`GAME_MANAGER_UpdatePhysics`](#game_manager_updatephysics)
    - [`GAME_MANAGER_UpdateObject`](#game_manager_updateobject)
  - [Keyboard 모듈](#keyboard-모듈)
    - [키 상태 플래그](#키-상태-플래그)
    - [`init_keyboard`](#init_keyboard)
    - [`keyboard_update`](#keyboard_update)
    - [`keyboard_processing_main`](#keyboard_processing_main)
    - [`keyboard_processing_ingame`](#keyboard_processing_ingame)
    - [`keyboard_processing_name`](#keyboard_processing_name)
    - [`keyboard_processing_score`](#keyboard_processing_score)
    - [`keboard_get_name`](#keboard_get_name)
  - [Physics \& Collision](#physics--collision)
    - [`stOBJECT_UpdatePhysics`](#stobject_updatephysics)
    - [`Collide_Object_Tile`](#collide_object_tile)
    - [`Collide_Enemy_Player`](#collide_enemy_player)
    - [`Collide_Object_Bubble`](#collide_object_bubble)
  - [Map 모듈](#map-모듈)
    - [맵 데이터 구조체](#맵-데이터-구조체)
    - [`stTILE_init`](#sttile_init)
    - [`map_init_stage`](#map_init_stage)
    - [`MAP_1_GetData`](#map_1_getdata)
    - [`MAP_2_GetData`](#map_2_getdata)
    - [`MAP_3_GetData`](#map_3_getdata)
  - [Player 모듈](#player-모듈)
    - [`stPLAYER` 구조체](#stplayer-구조체)
    - [`init_player`](#init_player)
    - [`player_update_input`](#player_update_input)
    - [`player_update_frame`](#player_update_frame)
  - [Enemy 모듈](#enemy-모듈)
    - [`stENEMY` 구조체](#stenemy-구조체)
    - [적 타입별 특성](#적-타입별-특성)
    - [`Enemy_Create`](#enemy_create)
    - [`Enemy_Update` / `Enemy_Update_ALL`](#enemy_update--enemy_update_all)
    - [`Enemy_ChangeState`](#enemy_changestate)
    - [`Enemy_GetCurrentState`](#enemy_getcurrentstate)
    - [`Enemy_UpdateIdle`](#enemy_updateidle)
    - [`Enemy_UpdateMove`](#enemy_updatemove)
    - [`Enemy_UpdateAttack`](#enemy_updateattack)
    - [`Enemy_UpdateTrapped`](#enemy_updatetrapped)
    - [`Enemy_ToPlayer_Ground`](#enemy_toplayer_ground)
    - [`Enemy_ToPlayer_Fly`](#enemy_toplayer_fly)
    - [`Enemy_Throw`](#enemy_throw)
    - [`Throw_Create`](#throw_create)
    - [`Throw_Update` / `Throw_Update_ALL`](#throw_update--throw_update_all)
    - [`Throw_MoveTowardPlayer`](#throw_movetowardplayer)
  - [Bubble 모듈](#bubble-모듈)
    - [버블 상태 생명주기](#버블-상태-생명주기)
    - [버블 속도 상수](#버블-속도-상수)
    - [`bubble_init`](#bubble_init)
    - [`bubble_add`](#bubble_add)
    - [`bubble_update`](#bubble_update)
    - [`bubble_update_frame`](#bubble_update_frame)
  - [Score 모듈](#score-모듈)
    - [관련 구조체](#관련-구조체)
    - [`Score_Add`](#score_add)
    - [`Score_Get`](#score_get)
    - [`Score_Print`](#score_print)
  - [Util 모듈](#util-모듈)
    - [`must_init`](#must_init)
    - [`util_rand_i`](#util_rand_i)
    - [`util_rand_f`](#util_rand_f)

---

## 시스템 아키텍처 및 실행 흐름도

### 전체 게임 상태 흐름

```
[프로그램 시작]
      │
      ▼
  main()
  ├─ al_init()
  ├─ init_keyboard()
  └─ init_render()
      │
      ▼
  al_wait_for_event()  ◄──────────────────────────────┐
      │                                                │
      ▼                                                │
  GAME_MANAGER_UpdateState()                           │
      │                                                │
  ┌───┴─────────────────────────────────┐              │
  │       현재 게임 상태 분기            │              │
  └───┬────────┬──────────┬─────────────┘              │
      │        │          │                            │
   MAIN     INGAME      NAME / SCORE / END             │
      │        │                                       │
      │        ▼                                       │
      │   routine_ingame()                             │
      │   ├─ keyboard_processing_ingame()              │
      │   ├─ GAME_MANAGER_CheckCollision()             │
      │   │     ├─ Collide_Object_Tile()  (모든 오브젝트)
      │   │     ├─ Collide_Enemy_Player() (적 ↔ 플레이어)
      │   │     └─ Collide_Object_Bubble()(버블 ↔ 오브젝트)
      │   ├─ GAME_MANAGER_UpdatePhysics()              │
      │   │     ├─ stOBJECT_UpdatePhysics() (플레이어)  │
      │   │     ├─ stOBJECT_UpdatePhysics() (모든 적)   │
      │   │     └─ bubble_update()                     │
      │   ├─ GAME_MANAGER_UpdateObject()               │
      │   │     ├─ player_update_frame()               │
      │   │     ├─ bubble_update_frame()               │
      │   │     ├─ Enemy_Update_ALL()                  │
      │   │     └─ Throw_Update_ALL()                  │
      │   ├─ render_draw_ingame()                      │
      │   └─ GAME_MANAGER_UpdateStage()                │
      │         ├─ 모든 적 처치 → SetGameStage_Next()   │
      │         └─ 플레이어 사망 → SetGameState(NAME)   │
      │                                                │
      └────────────────── 다음 이벤트 대기 ────────────┘
```

### 게임 상태 전이 다이어그램

```
          SPACE 입력
[MAIN] ──────────────► [INGAME]
  ▲                       │
  │     SPACE 입력         │ 모든 적 처치 또는 플레이어 사망
  │                       ▼
  │ SPACE 입력          [NAME]  ── 이름 입력 완료 ──► [SCORE]
  └──────────────────────────────────────────────────────┘
```

### 프레임당 업데이트 순서 (INGAME)

```
┌──────────────────────────────────────────────────┐
│  매 프레임 (1/60초) 처리 순서                      │
│                                                  │
│  1. keyboard_processing_ingame()  ← 입력 처리     │
│  2. CheckCollision()              ← 충돌 판정     │
│  3. UpdatePhysics()               ← 물리 적용     │
│  4. UpdateObject()                ← 상태 갱신     │
│  5. render_draw_ingame()          ← 화면 출력     │
│  6. UpdateStage()                 ← 스테이지 판정 │
└──────────────────────────────────────────────────┘
```

---

## 전역 설정 상수

> 📄 **파일:** `bugglebuggle.h`  
> 전체 프로젝트에서 공유하는 시스템 설정값과 열거형을 정의합니다.

### 시스템 설정

| 상수 | 값 | 설명 |
|---|---|---|
| `CONFIG_SYSTEM_FRAME` | `60` | 목표 FPS |
| `CONFIG_SYSTEM_LOADING_DELAY` | `60` (1 × FRAME) | 상태 전환 시 로딩 딜레이 (프레임 수) |
| `CONFIG_SYSTEM_PLAYER_NAME_MAX` | `12` | 플레이어 이름 최대 글자 수 |

### 게임 설정

| 상수 | 값 | 설명 |
|---|---|---|
| `CONFIG_GAME_PLAYER_HEART_MAX` | `3` | 플레이어 초기 목숨 수 |
| `CONFIG_MAP_X_MAX` | `36` | 맵 가로 타일 수 |
| `CONFIG_MAP_Y_MAX` | `24` | 맵 세로 타일 수 |

### 물리 설정

| 상수 | 값 | 설명 |
|---|---|---|
| `CONFIG_PHYSICS_GRAVITY_FORCE` | `0.25` | 중력 가속도 (픽셀/프레임²) |
| `CONFIG_PHYSICS_JUMP_FORCE` | `4.2` | 점프 초기 속도 (픽셀/프레임) |
| `CONFIG_PHYSICS_PLAYER_INV_TIME` | `2` | 피격 후 무적 시간 (초) |

> 💡 **밸런스 노트:** `GRAVITY = 0.25`, `JUMP_FORCE = 4.2`로 튜닝된 수치입니다. 중력이 너무 강하면 조작감이 둔해지고, 너무 약하면 허공을 떠다니는 느낌이 납니다. 현재 값은 약 17프레임 상승 → 17프레임 하강의 포물선 궤도를 만들어 자연스러운 점프감을 구현합니다.

### 오브젝트 풀 설정

| 상수 | 값 | 설명 |
|---|---|---|
| `CONFIG_OBJECT_PLAYER_MAX` | `1` | 최대 플레이어 수 |
| `CONFIG_OBJECT_BUBBLE_MAX` | `10` | 최대 동시 버블 수 |
| `CONFIG_OBJECT_ENEMY_MAX` | `10` | 최대 동시 적 수 |
| `CONFIG_OBJECT_ENEMY_ATTACK_MAX` | `10` | 최대 동시 투사체 수 |
| `CONFIG_OBJECT_MAP_MAX` | `864` (36 × 24) | 최대 타일 수 |
| `CONFIG_COLLISION_TILE_SIZE` | `10` | 타일 1칸의 픽셀 크기 |

### 열거형 (Enums)

```c
typedef enum { eMAIN_STATE_WAIT = 0, eMAIN_STATE_START } eMAIN_STATE;

typedef enum {
    eGAME_STATE_MAIN = 0,
    eGAME_STATE_INGAME,
    eGAME_STATE_NAME,
    eGAME_STATE_SCORE,
    eGAME_STATE_END
} eGAME_STATE;

typedef enum {
    eGAME_STAGE_NONE = 0,
    eGAME_STAGE_1, eGAME_STAGE_2, eGAME_STAGE_3,
    eGAME_STAGE_MAX
} eGAME_STAGE;
```

---

## 핵심 데이터 구조

> 📄 **파일:** `object.h`  
> 게임 내 모든 오브젝트의 공통 기반 구조체입니다.

```c
// 충돌 박스
typedef struct { double height; double width; } stCOLLISION_BOX;

// 충돌 컴포넌트
typedef struct {
    bool            is_static;   // true = 정적(타일), false = 동적
    eOBJ_TAG        tag;         // TILE / PLAYER / ENEMY / BUBBLE / ENEMY_ATTACK
    stCOLLISION_BOX box;
} stCOLLISION;

// 위치 / 속도 공용 구조체
typedef struct { double x; double y; } stPOSITION;

// 물리 컴포넌트
typedef struct {
    bool       is_gravity;  // 중력 적용 여부
    bool       is_jump;     // 공중에 있는지 여부
    eDIR_LOOK  look;        // 바라보는 방향
    stPOSITION pos;         // 현재 위치
    stPOSITION speed;       // 현재 속도 (프레임당 픽셀)
} stPHYSICS;

// 렌더링 컴포넌트
typedef struct {
    int  is_active;  // 렌더링 활성화 여부
    bool is_move;    // 이동 중 여부 (애니메이션 트리거)
} stRENDER;

// 기반 오브젝트 (모든 게임 오브젝트가 이를 포함)
typedef struct {
    bool        is_active;
    stCOLLISION coll;
    stPHYSICS   phy;
    stRENDER    rend;
} stOBJECT;
```

### 파생 구조체 계층

```
stOBJECT
├── stPLAYER  (obj + state, shot_timer, lives, invincible_timer, attack_timer)
├── stENEMY   (obj + type, state, state_timer, trapped_timer, throw_timer, is_angry)
├── stBUBBLE  (obj + frame, dur, state)
└── stTILE    (obj 래핑)
```

> ⚠️ **중요:** `stPLAYER`, `stENEMY`, `stBUBBLE`은 모두 `stOBJECT`를 첫 번째 멤버로 가집니다. 이 덕분에 `(stOBJECT*)` 캐스팅을 통해 태그 기반 다형성이 구현됩니다.

---

## Main Loop & Game Manager

> 📄 **파일:** `main.c` / `game_manager.h` / `game_manager.c`  
> 프로그램의 진입점과 게임 전체 상태를 제어하는 중앙 허브입니다. 모든 오브젝트 리소스(플레이어, 버블, 적, 맵)를 소유하고, 충돌·물리·오브젝트 업데이트의 통합 진입점 역할을 합니다.

---

### `main`

```c
int main(void);
```

**Description**  
프로그램의 진입점입니다. Allegro5 서브시스템을 초기화하고, 60FPS 타이머 기반의 메인 이벤트 루프를 실행합니다.

**초기화 순서**

| 단계 | 함수 | 설명 |
|---|---|---|
| 1 | `al_init()` | Allegro5 코어 초기화 |
| 2 | `al_create_event_queue()` | 이벤트 큐 생성 |
| 3 | `al_create_timer(1.0/60)` | 60FPS 타이머 생성 |
| 4 | `init_keyboard()` | 키보드 서브시스템 등록 |
| 5 | `init_render()` | 렌더러 및 디스플레이 초기화 |

**Logic & Notes**
- `ALLEGRO_EVENT_TIMER` 이벤트마다 `routine_game()`을 호출해 게임 로직을 처리합니다.
- `ALLEGRO_EVENT_DISPLAY_CLOSE` 시 루프를 종료합니다.
- `keyboard_update()`는 이벤트 큐 처리 **후** 호출해야 키 상태가 정확히 갱신됩니다.

---

### `GAME_MANAGER_UpdateState`

```c
eGAME_STATE GAME_MANAGER_UpdateState(void);
```

**Description**  
매 프레임 호출되는 게임 상태 머신(State Machine)의 핵심입니다. 로딩 딜레이를 처리하고, `INGAME` 상태에서는 스테이지 전환 플래그를 확인해 다음 스테이지를 로드합니다.

**Return Value**

| 값 | 설명 |
|---|---|
| `eGAME_STATE` | 현재 게임 상태 (호출자가 분기에 사용) |

**Logic & Notes**
- `GAME_MANAGER_UpdateLoading()` (내부 static 함수)를 먼저 호출해 로딩 딜레이를 처리합니다.
- 모든 스테이지(`eGAME_STAGE_MAX`) 클리어 시 자동으로 `eGAME_STATE_NAME`으로 전환됩니다.
- 반드시 `main` 이벤트 루프의 **최우선**으로 호출해야 합니다.

---

### `GAME_MANAGER_SetGameState`

```c
void GAME_MANAGER_SetGameState(eGAME_STATE state);
```

**Description**  
게임 상태를 변경하고 로딩 플래그(`is_loading = true`)를 활성화합니다. 직접 멤버 접근 대신 이 함수를 통해 상태를 전환해야 합니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `state` | `eGAME_STATE` | 전환할 게임 상태 |

**Return Value:** 없음 (`void`)

---

### `GAME_MANAGER_GetGameStage`

```c
eGAME_STAGE GAME_MANAGER_GetGameStage(void);
```

**Description**  
현재 진행 중인 스테이지 번호를 반환합니다.

**Return Value**

| 값 | 설명 |
|---|---|
| `eGAME_STAGE_NONE` | 인게임 전 또는 초기화 상태 |
| `eGAME_STAGE_1` ~ `eGAME_STAGE_3` | 현재 진행 중인 스테이지 |

---

### `GAME_MANAGER_SetGameStage_Next`

```c
void GAME_MANAGER_SetGameStage_Next(void);
```

**Description**  
다음 스테이지 로드를 예약합니다. 내부 `flag_next_stage`를 `true`로 설정하며, 실제 전환은 다음 `UpdateState()` 호출 시 처리됩니다.

**Return Value:** 없음 (`void`)

> 💡 **설계 의도:** 충돌/업데이트 도중 즉시 스테이지를 전환하면 배열 순회 중 상태가 바뀌는 문제가 생길 수 있습니다. 플래그 방식을 통해 프레임 경계에서 안전하게 전환합니다.

---

### `GAME_MANAGER_UpdateStage`

```c
void GAME_MANAGER_UpdateStage(void);
```

**Description**  
매 프레임 스테이지 진행 정보를 갱신합니다. 남은 적 수를 계산하고, 클리어 또는 게임 오버 조건을 판정합니다.

**Return Value:** 없음 (`void`)

**판정 로직**

| 조건 | 결과 |
|---|---|
| `enemy_remain <= 0` | `SetGameStage_Next()` 호출 (스테이지 클리어) |
| `is_player_dead == true` | `SetGameState(NAME)` 호출 (게임 오버) |

**Logic & Notes**
- `enemy_el[type]`은 **처치된** 적 수입니다 (`max - 현재 남은 수`).
- `stage_frame`은 해당 스테이지에서 경과한 프레임 수를 카운트합니다. 점수 계산에는 사용되지 않지만 스코어 화면 표시에 활용됩니다.

---

### `GAME_MANAGER_GetScore`

```c
int GAME_MANAGER_GetScore(void);
```

**Description**  
모든 스테이지의 처치 기록을 합산해 총점을 계산하여 반환합니다.

**Return Value**

| 값 | 설명 |
|---|---|
| `int` | 계산된 총점 |

**점수 계산 공식**

```
총점 = Σ (스테이지 0 ~ MAX) {
    + BASIC 처치 수  × 100점
    + THROW 처치 수  × 200점
    + (SCORE 상태일 때만) 잔여 목숨 수 × 500점
}
```

> ⚠️ **주의:** `eGAME_STATE_SCORE` 상태에서만 잔여 목숨 보너스가 합산됩니다. 다른 상태에서 호출하면 목숨 보너스 없이 처치 점수만 반환됩니다.

---

### `GAME_MANAGER_IsLoading`

```c
bool GAME_MANAGER_IsLoading(void);
```

**Description**  
현재 로딩 딜레이가 진행 중인지 여부를 반환합니다. `routine_ingame()` 등에서 게임 로직을 건너뛰는 가드로 사용됩니다.

**Return Value:** `true` = 로딩 중, `false` = 로딩 완료

---

### `GAME_MANAGER_GetPlayer`

```c
stPLAYER* GAME_MANAGER_GetPlayer(int player_id);
```

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `player_id` | `int` | 플레이어 인덱스. 현재 `0`만 유효 |

**Return Value:** 해당 `stPLAYER` 포인터

---

### `GAME_MANAGER_GetBubble`

```c
stBUBBLE* GAME_MANAGER_GetBubble(void);
```

**Return Value:** 버블 오브젝트 풀 배열의 포인터 (`stBUBBLE[CONFIG_OBJECT_BUBBLE_MAX]`)

---

### `GAME_MANAGER_GetEnemy`

```c
stENEMY* GAME_MANAGER_GetEnemy(void);
```

**Return Value:** 적 오브젝트 풀 배열의 포인터 (`stENEMY[CONFIG_OBJECT_ENEMY_MAX]`)

---

### `GAME_MANAGER_GetEnemyAttacks`

```c
stOBJECT* GAME_MANAGER_GetEnemyAttacks(void);
```

**Return Value:** 투사체 오브젝트 풀 배열의 포인터 (`stOBJECT[CONFIG_OBJECT_ENEMY_ATTACK_MAX]`)

---

### `GAME_MANAGER_GetMap`

```c
stTILE* GAME_MANAGER_GetMap(void);
```

**Return Value:** 맵 타일 배열의 포인터 (`stTILE[CONFIG_OBJECT_MAP_MAX]`)

---

### `GAME_MANAGER_CheckCollision`

```c
void GAME_MANAGER_CheckCollision(void);
```

**Description**  
매 프레임 모든 충돌 쌍을 순서대로 검사하고 충돌 응답을 처리합니다.

**Return Value:** 없음 (`void`)

**처리 순서 및 복잡도**

```
순서  │ 대상                         │ 복잡도
──────┼──────────────────────────────┼─────────────
 1    │ 모든 타일 × 모든 플레이어     │ O(T × P)
 2    │ 모든 타일 × 모든 적           │ O(T × E)
 3    │ 모든 타일 × 모든 버블         │ O(T × B)
 4    │ 모든 적    × 플레이어[0]      │ O(E)
 5    │ 모든 투사체 × 플레이어[0]     │ O(A)
 6    │ 모든 버블   × 플레이어[0]     │ O(B)
 7    │ 모든 적    × 모든 버블        │ O(E × B)
```

> ⚠️ **성능 주의:** 7번 항목은 적×버블 이중 루프로 O(E×B) = O(100)입니다. 현재 풀 크기에서는 문제없지만, 오브젝트 수를 대폭 늘릴 경우 공간 분할(Spatial Partitioning) 최적화가 필요합니다.

---

### `GAME_MANAGER_UpdatePhysics`

```c
void GAME_MANAGER_UpdatePhysics(void);
```

**Description**  
플레이어 및 모든 활성 적에 물리 업데이트를 적용하고, 버블 상태를 갱신합니다.

**Return Value:** 없음 (`void`)

> ⚠️ **호출 순서:** 반드시 `CheckCollision()` **이후**에 호출해야 합니다. 충돌 해소 없이 물리를 적용하면 오브젝트가 타일을 통과할 수 있습니다.

---

### `GAME_MANAGER_UpdateObject`

```c
void GAME_MANAGER_UpdateObject(void);
```

**Description**  
플레이어 프레임 상태, 버블 위치, 적 AI, 투사체를 업데이트합니다. 게임 루프의 마지막 논리 단계입니다.

**Return Value:** 없음 (`void`)

**내부 호출 순서**

| 순서 | 호출 | 설명 |
|---|---|---|
| 1 | `player_update_frame()` | 플레이어 상태·타이머 갱신 |
| 2 | `bubble_update_frame()` | 버블 위치·프레임 카운터 갱신 |
| 3 | `Enemy_Update_ALL()` | 적 AI 전체 갱신 |
| 4 | `Throw_Update_ALL()` | 투사체 전체 갱신 |

---

## Keyboard 모듈

> 📄 **파일:** `keyboard.h` / `keyboard.c`  
> Allegro5 키 이벤트를 수신해 비트 플래그로 관리하고, 게임 상태별 입력을 처리합니다.

### 키 상태 플래그

| 플래그 | 값 | 의미 |
|---|---|---|
| `KEY_SEEN` | `1` | 이번 프레임에 이벤트가 발생했음 |
| `KEY_DOWN` | `2` | 현재 눌려 있는 상태 |

> 💡 **비트 마스크 설계:** `key[code]` 값이 `KEY_SEEN | KEY_DOWN (= 3)`이면 이번 프레임에 처음 눌린 것, `KEY_DOWN (= 2)`만이면 이전부터 계속 눌려 있는 것입니다. `KEY_SEEN`은 매 타이머 이벤트마다 초기화됩니다.

---

### `init_keyboard`

```c
void init_keyboard(ALLEGRO_EVENT_QUEUE* queue);
```

**Description**  
Allegro 키보드 서브시스템을 초기화하고 이벤트 큐에 등록합니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `queue` | `ALLEGRO_EVENT_QUEUE*` | 키보드 이벤트를 등록할 이벤트 큐 |

**Return Value:** 없음 (`void`)

---

### `keyboard_update`

```c
void keyboard_update(ALLEGRO_EVENT* event);
```

**Description**  
이벤트를 분석해 `key[]` 배열의 비트 상태를 갱신하고, 이름 입력 모드(`is_input`)일 때 눌린 키를 문자로 변환해 `player_name.data`에 저장합니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `event` | `ALLEGRO_EVENT*` | 처리할 Allegro 이벤트 |

**Return Value:** 없음 (`void`)

**처리 이벤트 목록**

| 이벤트 타입 | 동작 |
|---|---|
| `ALLEGRO_EVENT_TIMER` | 모든 키의 `KEY_SEEN` 비트 클리어 (프레임 경계) |
| `ALLEGRO_EVENT_KEY_DOWN` | 해당 키에 `KEY_SEEN \| KEY_DOWN` 설정; 이름 입력 모드면 문자 저장 |
| `ALLEGRO_EVENT_KEY_UP` | 해당 키의 `KEY_DOWN` 비트 클리어 |

**Logic & Notes**
- 이름 입력 시 `keycode + 'A' - 1` 변환을 사용합니다. Allegro의 키코드(`ALLEGRO_KEY_A = 1`)가 1부터 시작하기 때문입니다.

---

### `keyboard_processing_main`

```c
eMAIN_STATE keyboard_processing_main(void);
```

**Description**  
메인 화면에서의 입력을 처리합니다. 스페이스 바 입력 여부를 반환합니다.

**Return Value**

| 값 | 조건 |
|---|---|
| `eMAIN_STATE_START` | 스페이스 바가 눌려 있을 때 |
| `eMAIN_STATE_WAIT` | 그 외 |

---

### `keyboard_processing_ingame`

```c
void keyboard_processing_ingame(void);
```

**Description**  
인게임 중 방향키, 스페이스(버블 발사) 입력을 처리해 `player_update_input()`을 호출합니다.

**Return Value:** 없음 (`void`)

**Logic & Notes**
- 하강(`KEY_DOWN`)은 `KEY_SEEN` 플래그가 있을 때만 처리합니다. 꾹 누름으로 연속 하강하는 것을 방지합니다.
- 나머지 키는 `key[code]`가 0이 아니면(눌려 있으면) 매 프레임 입력을 전달합니다.

---

### `keyboard_processing_name`

```c
bool keyboard_processing_name(void);
```

**Description**  
이름 입력 화면에서 키 입력을 수신합니다. 스페이스 바를 누르면 입력을 종료합니다.

**Return Value:** `true` = 입력 완료 (스페이스로 확인), `false` = 입력 중

---

### `keyboard_processing_score`

```c
bool keyboard_processing_score(void);
```

**Description**  
스코어 화면에서 스페이스 바 입력 여부를 반환합니다.

**Return Value:** `true` = 스페이스 바 눌림 (`KEY_SEEN` 기준), `false` = 그 외

---

### `keboard_get_name`

```c
const char* keboard_get_name(void);
```

**Description**  
입력된 플레이어 이름 문자열의 포인터를 반환합니다.

**Return Value:** 이름 문자열 (`const char*`, 정적 배열이므로 해제 불필요)

> ⚠️ **주의:** 함수명에 오타(`keyboard` → `keboard`)가 있습니다. 하위 호환성을 위해 수정하지 않도록 합니다.

---

## Physics & Collision

> 📄 **파일:** `physics.h` / `physics.c` / `collision.h` / `collision.c`  
> 중력·속도 적용 물리 연산과 AABB 기반 충돌 판정·응답 엔진입니다.

---

### `stOBJECT_UpdatePhysics`

```c
void stOBJECT_UpdatePhysics(stOBJECT* obj);
```

**Description**  
오브젝트에 매 프레임 물리 법칙을 적용합니다. 속도를 위치에 더하고, 중력을 수직 속도에 누적하며, 프레임 종료 후 수평 속도를 0으로 초기화합니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `obj` | `stOBJECT*` | 물리를 적용할 오브젝트 |

**Return Value:** 없음 (`void`)

**물리 공식**

```
// 위치 갱신
pos.x += speed.x
pos.y += speed.y

// 중력 누적 (활성화된 오브젝트에만)
if (is_gravity && is_active):
    speed.y += GRAVITY  // += 0.25 (프레임당 가속)

// 렌더링 이동 플래그
rend.is_move = (speed.x != 0)

// 수평 속도 소비 (입력이 없으면 멈춤)
speed.x = 0
```

**밸런스 설계 근거**

| 수치 | 값 | 설계 이유 |
|---|---|---|
| `GRAVITY` | `0.25` px/frame² | 60FPS 기준 낙하감이 너무 빠르지 않으면서 플랫폼 게임다운 무게감 제공 |
| `JUMP_FORCE` | `4.2` px/frame | 최고점 도달까지 약 17프레임, 총 점프 높이 약 35px (타일 3.5칸) |

**Logic & Notes**
- `speed.x`는 **소비형**: 매 프레임 0이 됩니다. 지속 이동을 위해 `keyboard_processing_ingame()`이 매 프레임 속도를 재설정합니다.
- `speed.y`는 **누적형**: 충돌 시스템이 착지를 감지하면 직접 0으로 초기화합니다.

---

### `Collide_Object_Tile`

```c
void Collide_Object_Tile(stOBJECT* object, stTILE* tile);
```

**Description**  
동적 오브젝트(플레이어/적/버블)와 정적 타일 간의 충돌을 예측 위치(`next_pos`) 기반으로 판정하고 겹침을 해소합니다. Y축(수직)과 X축(수평) 충돌을 독립적으로 처리합니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `object` | `stOBJECT*` | 충돌을 확인할 동적 오브젝트 |
| `tile` | `stTILE*` | 충돌 대상 정적 타일 |

**Return Value:** 없음 (`void`)

**충돌 해소 알고리즘 (AABB Sweep)**

```
예측 위치: next_x = pos.x + speed.x
           next_y = pos.y + speed.y

[Y축 처리] - X가 겹칠 때만 수행
  ▲ 위로 이동 & next_y < MAP_Y_MAX(10)
    → speed.y = 0, pos.y = 10  (천장 고정)

  ▼ 아래로 이동 중
    · next_y > MAP_Y_MIN(220)
      → speed.y = 0, pos.y = 220  (하단 경계 고정)
    · curr_bottom ≤ tile_top AND next_bottom ≥ tile_top
      → pos.y = tile_top - obj_height  (타일 위에 착지)
      → speed.y = 0, is_jump = false

[X축 처리] - Y가 겹칠 때만 수행
  ► 오른쪽 이동 & 타일 왼쪽 면 통과 예정
    → pos.x = tile_left - obj_width
    → speed.x = 0
    → (버블이면) state = FLOAT, speed.y = -0.8

  ◄ 왼쪽 이동 & 타일 오른쪽 면 통과 예정
    → pos.x = tile_right
    → speed.x = 0
    → (버블이면) state = FLOAT, speed.y = -0.8
```

**Logic & Notes**
- 맵 경계 상수: 상단 `MAP_Y_MAX = 10`, 하단 `MAP_Y_MIN = 220`
- 버블이 벽에 닿으면 자동으로 `FLOAT` 상태로 전환되어 위로 올라갑니다.

---

### `Collide_Enemy_Player`

```c
void Collide_Enemy_Player(stOBJECT* object, stPLAYER* player);
```

**Description**  
적(또는 적 투사체)과 플레이어 간의 AABB 충돌을 처리합니다. 적 상태에 따라 처치 또는 플레이어 피해로 분기합니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `object` | `stOBJECT*` | 적(`eOBJ_TAG_ENEMY`) 또는 투사체(`eOBJ_TAG_ENEMY_ATTACK`) |
| `player` | `stPLAYER*` | 충돌 대상 플레이어 |

**Return Value:** 없음 (`void`)

**충돌 결과 의사결정표**

```
플레이어 상태 == DEAD OR invincible_timer > 0 ?
  → 충돌 무시 (return)

object.tag == eOBJ_TAG_ENEMY:
  충돌(AABB) 있음?
    enemy.state == TRAPPED?
      → 적 즉시 처치 (state=DEAD, is_active=false)
    그 외:
      → player.lives--
      player.lives <= 0?
        → 플레이어 사망 (state=DEAD, is_active=false)
      그 외:
        → invincible_timer = 2초 × 60프레임

object.tag == eOBJ_TAG_ENEMY_ATTACK:
  충돌(AABB) 있음?
    → player.lives-- (동일 처리)
    → object.is_active = false  (투사체 소멸)
```

---

### `Collide_Object_Bubble`

```c
void Collide_Object_Bubble(stOBJECT* object, stBUBBLE* bubble);
```

**Description**  
오브젝트와 버블 간의 충돌을 처리합니다. 태그에 따라 플레이어 착지 보조 또는 적 포획으로 분기합니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `object` | `stOBJECT*` | 충돌 대상 오브젝트 (플레이어 또는 적) |
| `bubble` | `stBUBBLE*` | 충돌 대상 버블 |

**Return Value:** 없음 (`void`)

**태그별 처리**

| 오브젝트 태그 | 버블 상태 | 충돌 결과 |
|---|---|---|
| `eOBJ_TAG_PLAYER` | `FLOAT` | 플레이어 하강 중이면 `is_jump = false` (버블 위에 착지 허용) |
| `eOBJ_TAG_ENEMY` | `SHOOTING` | 적 `TRAPPED` 전환, 버블 비활성화 |
| `eOBJ_TAG_ENEMY` | `FLOAT` / `POP` | 무시 (이미 발사 단계 아님) |

---

## Map 모듈

> 📄 **파일:** `map.h` / `map.c` / `map_1.c` / `map_2.c` / `map_3.c`  
> 타일 초기화, 스테이지 데이터 로드, 각 스테이지별 레이아웃 및 적 배치를 담당합니다.

### 맵 데이터 구조체

```c
// 단일 타일 (stOBJECT 래핑)
typedef struct { stOBJECT obj; } stTILE;

// 스테이지 내 적 배치 데이터
typedef struct {
    bool        is_active;
    eENEMY_TYPE type;
    stPOSITION  pos;
} stSTAGE_ENEMY_DATA;

// 스테이지 전체 데이터 (맵 레이아웃 + 적 배치)
typedef struct {
    int (*stage)[CONFIG_MAP_X_MAX];  // 24×36 정수 배열 포인터
    stSTAGE_ENEMY_DATA* enemies;
} stSTAGE_DATA;
```

---

### `stTILE_init`

```c
stTILE* stTILE_init(stTILE* tile, stPOSITION* pos);
```

**Description**  
단일 타일을 지정 위치에 활성화하여 초기화합니다. 타일은 항상 정적(`is_static = true`) 오브젝트입니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `tile` | `stTILE*` | 초기화할 타일 포인터 |
| `pos` | `stPOSITION*` | 타일의 좌상단 위치 |

**Return Value:** 초기화된 타일 포인터 (입력값 동일)

**고정 초기화 값**

| 필드 | 값 |
|---|---|
| `is_active` | `true` |
| `coll.is_static` | `true` |
| `coll.tag` | `eOBJ_TAG_TILE` |
| `coll.box` | `10 × 10` px |

---

### `map_init_stage`

```c
void map_init_stage(stTILE* map, stENEMY* enemy, stSTAGE_DATA* data);
```

**Description**  
스테이지 데이터를 기반으로 맵 타일 배열과 적 풀을 일괄 초기화합니다. 스테이지 전환마다 호출됩니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `map` | `stTILE*` | 초기화할 맵 타일 배열 |
| `enemy` | `stENEMY*` | 초기화할 적 풀 배열 |
| `data` | `stSTAGE_DATA*` | 스테이지 레이아웃 및 적 배치 데이터 |

**Return Value:** 없음 (`void`)

**초기화 순서**

```
1. init_player(player[0])           ← 플레이어 위치·상태 리셋
2. memset(enemy, 0, ...)            ← 적 풀 전체 초기화
3. Enemy_Create(...)                ← data->enemies 기반으로 적 생성
4. 타일 배열 순회 (i=0..Y, j=0..X):
   · data->stage[i][j] != 0 → stTILE_init()
   · data->stage[i][j] == 0 → tile.is_active = false
```

**Logic & Notes**
- 타일 좌표 공식: `pos.x = j × 10`, `pos.y = i × 10`
- 맵 배열에서 `1`은 타일, `0`은 빈 공간입니다.

---

### `MAP_1_GetData`

```c
stSTAGE_DATA* MAP_1_GetData(void);
```

**Description**  
스테이지 1의 정적 데이터 포인터를 반환합니다.

**스테이지 1 구성**

| 항목 | 내용 |
|---|---|
| 레이아웃 | 좌우 벽 + 상하 바닥/천장, 중간에 4개 수평 플랫폼 (8행마다 반복, 가운데 통로 있음) |
| 적 구성 | `BASIC` × 6마리 (지상 추적형) |
| 적 배치 좌표 | `(90,190)`, `(130,160)`, `(150,130)`, `(100,220)`, `(120,100)`, `(150,700)` |

---

### `MAP_2_GetData`

```c
stSTAGE_DATA* MAP_2_GetData(void);
```

**Description**  
스테이지 2의 정적 데이터 포인터를 반환합니다.

**스테이지 2 구성**

| 항목 | 내용 |
|---|---|
| 레이아웃 | 불규칙한 플랫폼 배치 (계단형, 중앙 돌출 플랫폼 포함) |
| 적 구성 | `THROW` × 3마리 (공중 투사체형) |
| 적 배치 좌표 | `(300,220)`, `(80,80)`, `(160,140)` |

---

### `MAP_3_GetData`

```c
stSTAGE_DATA* MAP_3_GetData(void);
```

**Description**  
스테이지 3의 정적 데이터 포인터를 반환합니다.

**스테이지 3 구성**

| 항목 | 내용 |
|---|---|
| 레이아웃 | 좌우 비대칭 구조물, 상단 중앙 고립 플랫폼 |
| 적 구성 | `THROW` × 3마리 + `BASIC` × 3마리 (혼합) |
| 적 배치 좌표 | THROW: `(300,70)`, `(300,100)`, `(300,120)` / BASIC: `(160,140)`, `(80,70)`, `(230,70)` |

---

## Player 모듈

> 📄 **파일:** `player.h` / `player.c`  
> 플레이어 초기화, 입력 처리, 프레임 단위 상태 갱신을 담당합니다.

### `stPLAYER` 구조체

```c
typedef struct {
    stOBJECT      obj;             // 기반 오브젝트 (위치, 물리, 충돌)
    ePLAYER_STATE state;           // 현재 상태 (IDLE/MOVE/ATTACK/DEAD)
    int           shot_timer;      // 버블 발사 쿨타임 카운터
    int           lives;           // 남은 목숨 수
    int           invincible_timer;// 무적 프레임 카운터
    int           attack_timer;    // 공격 애니메이션 유지 카운터
} stPLAYER;
```

---

### `init_player`

```c
void init_player(stPLAYER* player);
```

**Description**  
`stPLAYER`를 기본값으로 초기화합니다. 스테이지 전환 시 `map_init_stage()` 내부에서 자동으로 호출됩니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `player` | `stPLAYER*` | 초기화할 플레이어 포인터 |

**Return Value:** 없음 (`void`)

**초기화 주요 값**

| 필드 | 초기값 | 설명 |
|---|---|---|
| `pos.x / pos.y` | `15.0 / 210.0` | 스테이지 시작 위치 |
| `lives` | `CONFIG_GAME_PLAYER_HEART_MAX (3)` | 최대 목숨 |
| `phy.is_gravity` | `true` | 중력 즉시 적용 |
| `phy.is_jump` | `false` | 지상에서 시작 |
| `coll.box` | `10 × 10` px | 타일 크기와 동일 |

---

### `player_update_input`

```c
void player_update_input(stPLAYER* player, int allegro_key, unsigned char flag);
```

**Description**  
Allegro 키 코드를 받아 플레이어의 속도·방향·상태를 즉시 갱신합니다. `keyboard_processing_ingame()`에서 호출됩니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `player` | `stPLAYER*` | 입력을 적용할 플레이어 |
| `allegro_key` | `int` | Allegro 키 코드 |
| `flag` | `unsigned char` | `KEY_SEEN`, `KEY_DOWN` 비트 플래그 |

**Return Value:** 없음 (`void`)

**키 매핑표**

| 키 | speed.x | speed.y | look | state | 특이사항 |
|---|---|---|---|---|---|
| `KEY_LEFT` | `-2.0` | — | `LEFT` | `MOVE` | — |
| `KEY_RIGHT` | `+2.0` | — | `RIGHT` | `MOVE` | — |
| `KEY_UP` | — | `-4.2` | — | `MOVE` | `is_jump == false`일 때만 |
| `KEY_DOWN` | — | — | — | `MOVE` | `pos.y += 3`, `is_jump = false` |
| `KEY_SPACE` | — | — | — | — | `shot_timer ≤ 0`일 때 `bubble_add()` 호출 |

**Logic & Notes**
- `player->obj.is_active == false`이면 즉시 반환 (사망 상태 보호)
- 버블 발사 성공 시 `shot_timer = 40`, `attack_timer = 20` 설정

---

### `player_update_frame`

```c
void player_update_frame(stPLAYER* player);
```

**Description**  
매 프레임 플레이어의 상태와 타이머를 갱신합니다. 공격 애니메이션 유지, 쿨타임·무적 타이머 감소를 처리합니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `player` | `stPLAYER*` | 갱신할 플레이어 |

**Return Value:** 없음 (`void`)

**상태 전이 로직**

```
attack_timer > 0 ?
  → state = ATTACK, attack_timer--
그 외:
  → state = IDLE

shot_timer > 0  → shot_timer--
inv_timer > 0   → invincible_timer--
```

---

## Enemy 모듈

> 📄 **파일:** `enemy.h` / `enemy.c`  
> 적 캐릭터의 생성, 상태 머신 기반 AI 업데이트, 투사체 관리를 담당합니다.

### `stENEMY` 구조체

```c
typedef struct {
    stOBJECT    obj;            // 기반 오브젝트
    eENEMY_TYPE type;           // BASIC / THROW / BOSS
    eENEMY_STATE state;         // 현재 AI 상태
    int         state_timer;    // 상태 내 경과 프레임
    int         trapped_timer;  // 버블 탈출 카운트다운
    int         throw_timer;    // 투사체 발사 쿨타임
    bool        is_angry;       // 분노 상태 (버블 탈출 후)
} stENEMY;
```

### 적 타입별 특성

| 타입 | 이동 방식 | 공격 | 기본 속도 | 분노 속도 |
|---|---|---|---|---|
| `BASIC` | 지상 추적 + 확률적 점프 | 없음 (접촉 피해) | 1.2 px/f | 1.4 px/f |
| `THROW` | 공중 부유 (중력 무시) | 투사체 발사 | 0.8 px/f | 1.2 px/f |
| `BOSS` | 공중 부유 | 없음 (현재) | 0.8 px/f | 1.2 px/f |

---

### `Enemy_Create`

```c
stENEMY* Enemy_Create(stENEMY* enemy_pool, eENEMY_TYPE type, int x, int y);
```

**Description**  
오브젝트 풀에서 비활성화 슬롯을 찾아 새 적을 초기화하고 반환합니다 (Object Pooling 패턴).

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `enemy_pool` | `stENEMY*` | 적 오브젝트 풀 |
| `type` | `eENEMY_TYPE` | 적 타입 |
| `x`, `y` | `int` | 초기 좌표 |

**Return Value:** 초기화된 `stENEMY*`, 슬롯 없으면 `NULL`

---

### `Enemy_Update` / `Enemy_Update_ALL`

```c
void Enemy_Update(stENEMY* e, stOBJECT* p, stOBJECT* t);
void Enemy_Update_ALL(stENEMY* enemy, stPLAYER* player, stOBJECT* throw);
```

**Description**  
`Enemy_Update`는 단일 적의 상태 머신을 구동합니다. `Enemy_Update_ALL`은 풀 전체에 대해 순차 호출합니다.

**상태 머신 구조**

```
[IDLE]
  └─► Enemy_UpdateIdle() → 즉시 MOVE 전환

[MOVE]
  └─► Enemy_UpdateMove()
        ├─ BASIC → Enemy_ToPlayer_Ground()
        └─ THROW/BOSS → Enemy_ToPlayer_Fly()

[ATTACK]
  └─► Enemy_UpdateAttack() (THROW 타입만 의미 있음)

[TRAPPED]
  └─► Enemy_UpdateTrapped() → 카운트다운 후 MOVE 복귀 + angry=true

[DEAD]
  └─► 처리 없음
```

> 💡 **참고:** `THROW` 타입은 상태와 무관하게 `Enemy_UpdateAttack()`을 매 프레임 추가 호출합니다. 이동 중에도 투사체를 발사할 수 있는 이유입니다.

---

### `Enemy_ChangeState`

```c
void Enemy_ChangeState(stENEMY* e, eENEMY_STATE newState);
```

**Description**  
적의 상태를 변경하고 `state_timer`를 0으로 리셋합니다. 동일 상태 전환은 무시됩니다.

---

### `Enemy_GetCurrentState`

```c
eENEMY_STATE Enemy_GetCurrentState(stENEMY* e);
```

**Return Value:** 현재 상태, `e == NULL`이면 `eENEMY_STATE_MAX`

---

### `Enemy_UpdateIdle`

```c
void Enemy_UpdateIdle(stENEMY* e);
```

**Description**  
`IDLE` 상태 핸들러. 즉시 `MOVE`로 전환합니다. 현재 대기 로직 없음.

---

### `Enemy_UpdateMove`

```c
void Enemy_UpdateMove(stENEMY* e, stOBJECT* p);
```

**Description**  
`MOVE` 상태 핸들러. 타입에 따라 지상(`BASIC`) 또는 공중(`THROW`/`BOSS`) AI를 호출합니다.

---

### `Enemy_UpdateAttack`

```c
void Enemy_UpdateAttack(stENEMY* e, stOBJECT* p, stOBJECT* t);
```

**Description**  
`THROW` 타입 전용 투사체 발사 타이머 관리. 쿨타임마다 `Enemy_Throw()`를 호출합니다.

**발사 쿨타임 계산**

```
쿨타임 = 180 + (Get_RandNum_1_to_9() × 15) 프레임
       = 3.0초 ~ 4.5초 (60FPS 기준)
```

> 💡 **난수 조절:** `Get_RandNum_1_to_9()`은 XorShift 알고리즘을 사용해 의사 난수를 생성합니다. `rand()` 대신 사용해 시드 의존성을 없애고, 1~9의 균일한 분포로 발사 간격에 예측 불가능한 변화를 줍니다.

---

### `Enemy_UpdateTrapped`

```c
void Enemy_UpdateTrapped(stENEMY* e);
```

**Description**  
버블에 갇힌 상태의 카운트다운을 관리합니다. `trapped_timer`가 0이 되면 `MOVE` 복귀 + `is_angry = true`.

**Logic & Notes**
- 탈출 후 `trapped_timer`는 타입별 기본값으로 재설정됩니다 (현재 모두 300프레임 = 5초).
- 분노 상태에서는 이동 속도가 증가합니다 (`BASIC`: 1.4, `THROW`: 1.2).

---

### `Enemy_ToPlayer_Ground`

```c
void Enemy_ToPlayer_Ground(stENEMY* enemy, stOBJECT* target);
```

**Description**  
`BASIC` 타입의 지상 추적 AI. 수평 이동 + 조건부 점프로 플레이어를 추적합니다.

**이동 로직**

```
벽 감지: state_timer > 1 AND rend.is_move == false
  → 방향(look) 반전

방향에 따라 speed.x = ±cur_speed

점프 조건 (모두 충족 시):
  1. MOVE 상태이면서 is_jump == false
  2. target.pos.y < e.pos.y - 10  (플레이어가 10px 이상 위)
  3. state_timer % 60 == 0         (1초마다 판정)
  4. Get_RandNum_1_to_9() ≤ 5     (약 55% 확률)
  → speed.y = -4.2, is_jump = true
```

---

### `Enemy_ToPlayer_Fly`

```c
void Enemy_ToPlayer_Fly(stENEMY* enemy, stOBJECT* player);
```

**Description**  
`THROW`/`BOSS` 타입의 공중 부유 AI. 중력 무시, 수평 이동 + 수직 진동으로 이동합니다.

**이동 로직**

```
is_gravity = false (강제)
수평: look에 따라 speed.x = ±cur_speed
수직: speed.y가 0이면 -cur_speed로 초기화 (위로 이동 시작)
     pos.y ≤ 0이면 speed.y = +cur_speed (방향 반전)
```

---

### `Enemy_Throw`

```c
void Enemy_Throw(stENEMY* e, stOBJECT* p, stOBJECT* throw_pool);
```

**Description**  
적의 현재 위치에 투사체를 생성하고 플레이어 방향으로 즉시 발사합니다.

---

### `Throw_Create`

```c
stOBJECT* Throw_Create(stOBJECT* throw_pool, int x, int y);
```

**Description**  
투사체 풀에서 비활성 슬롯을 찾아 초기화합니다.

**Return Value:** 초기화된 `stOBJECT*`, 슬롯 없으면 `NULL`

---

### `Throw_Update` / `Throw_Update_ALL`

```c
void Throw_Update(stOBJECT* throw, stOBJECT* target_player);
void Throw_Update_ALL(stENEMY* enemy, stPLAYER* player, stOBJECT* throw);
```

**Description**  
투사체의 위치를 속도만큼 갱신하고, 맵 경계 밖으로 나가면 비활성화합니다.

---

### `Throw_MoveTowardPlayer`

```c
void Throw_MoveTowardPlayer(stOBJECT* throw, stOBJECT* target_player);
```

**Description**  
벡터 정규화를 통해 투사체를 플레이어 방향으로 일정 속력으로 발사합니다.

**벡터 정규화 공식**

```
dx = target.pos.x - throw.pos.x
dy = target.pos.y - throw.pos.y
dist = sqrt(dx² + dy²)

if dist > 0:
    speed.x = (dx / dist) × THROW_SPEED (2.5)
    speed.y = (dy / dist) × THROW_SPEED (2.5)
```

> ⚠️ **주의:** 이 함수는 생성 시 1회 호출해 속도를 설정하는 용도입니다. 이후 이동은 `Throw_Update()`가 담당합니다. 중복 호출 시 이동이 두 배가 됩니다.

---

## Bubble 모듈

> 📄 **파일:** `bubble.h` / `bubble.c`  
> 게임의 핵심 메커니즘인 버블의 발사, 상태 변화, 이동을 담당합니다.

### 버블 상태 생명주기

```
발사(bubble_add)
    │
    ▼
[SHOOTING] ──── 30프레임 ────► [FLOAT]
    │                             │
    │ 벽 충돌                     │ 300프레임 경과
    ▼                             ▼
[FLOAT]                        [POP]
                                  │
                                  │ 330프레임 경과
                                  ▼
                             is_active = false
```

### 버블 속도 상수

| 상수 | 값 | 설명 |
|---|---|---|
| `BUBBLE_SPEED` | `3.0` px/f | 발사 초기 수평 속도 |
| `BUBBLE_UP_SPEED` | `-0.8` px/f | FLOAT 상태의 상승 속도 |

---

### `bubble_init`

```c
void bubble_init(stBUBBLE* bubbles);
```

**Description**  
버블 풀 전체를 비활성화 상태로 초기화합니다. 게임 시작 시 1회 호출합니다.

---

### `bubble_add`

```c
bool bubble_add(stPLAYER* player, stBUBBLE* bubbles);
```

**Description**  
플레이어의 위치와 방향을 기반으로 버블을 풀에서 생성합니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `player` | `stPLAYER*` | 버블을 발사하는 플레이어 |
| `bubbles` | `stBUBBLE*` | 버블 오브젝트 풀 |

**Return Value:** `true` = 생성 성공, `false` = 풀 가득 참

**초기화 값**

| 필드 | 값 |
|---|---|
| `pos.x` | `player.pos.x` |
| `pos.y` | `player.pos.y + 5 - 5` (플레이어 중심 높이 맞춤) |
| `speed.x` | `±3.0` (방향에 따라) |
| `speed.y` | `0` |
| `is_gravity` | `false` |
| `dur` | `300` 프레임 |
| `state` | `SHOOTING` |

---

### `bubble_update`

```c
void bubble_update(stBUBBLE* bubbles);
```

**Description**  
버블 풀 전체의 상태(State)를 갱신합니다. 경과 프레임 수에 따라 `SHOOTING → FLOAT → POP → 비활성화` 전환을 처리합니다.

**상태 전환 조건**

| 조건 | 전환 |
|---|---|
| `SHOOTING` + `frame >= 30` | `FLOAT` 전환, `speed.x = 0`, `speed.y = -0.8` |
| `frame >= 300` | `POP` 전환 |
| `frame >= 330` | `is_active = false` |

---

### `bubble_update_frame`

```c
void bubble_update_frame(stBUBBLE* bubbles);
```

**Description**  
모든 활성 버블의 위치를 현재 속도만큼 직접 갱신하고 `frame` 카운터를 증가시킵니다.

> 💡 **호출 관계:** `bubble_update()`는 **상태**를 결정하고, `bubble_update_frame()`은 **위치**를 갱신합니다. 두 함수는 항상 같은 프레임에 순서대로 호출해야 합니다.

---

## Score 모듈

> 📄 **파일:** `score.h` / `score.c`  
> SQLite3를 이용한 리더보드 저장·조회와 스코어 화면 렌더링을 담당합니다.

### 관련 구조체

```c
// 리더보드 한 줄
typedef struct {
    int  rank;
    char player_name[CONFIG_SYSTEM_PLAYER_NAME_MAX]; // 최대 12자
    int  score;
} stBOARD;

// 스테이지 통계 (점수 계산에 사용)
typedef struct {
    bool is_player_dead;
    int  player_lives;
    int  enemy_remain;
    int  enemy_el[eENEMY_TYPE_MAX];   // 처치된 적 수 (타입별)
    int  enemy_max[eENEMY_TYPE_MAX];  // 최대 적 수 (타입별)
    int  stage_frame;                 // 경과 프레임
} stSTAGE_INFO;
```

---

### `Score_Add`

```c
void Score_Add(ALLEGRO_FONT* font, const char* name, int score);
```

**Description**  
플레이어 이름과 점수를 `leaderboard.db`에 저장합니다. 테이블이 없으면 자동 생성합니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `font` | `ALLEGRO_FONT*` | DB 오류 발생 시 화면 출력에 사용 |
| `name` | `const char*` | 저장할 플레이어 이름 |
| `score` | `int` | 저장할 점수 |

**Return Value:** 없음 (`void`)

**Logic & Notes**
- Prepared Statement(`sqlite3_bind_*`)를 사용해 SQL Injection을 방지합니다.
- DB 파일 위치: 실행 디렉터리의 `leaderboard.db`
- `CREATE TABLE IF NOT EXISTS`로 최초 실행 시 자동 스키마 생성

---

### `Score_Get`

```c
stBOARD* Score_Get(void);
```

**Description**  
데이터베이스에서 상위 10개 점수를 조회해 정적 배열에 저장하고 반환합니다.

**Return Value:** `leaderboard[1..10]`이 채워진 정적 배열 포인터 (해제 불필요)

**SQL 쿼리**

```sql
SELECT name, score FROM Ranking
ORDER BY score DESC, name ASC
LIMIT 10;
```

> ⚠️ **주의:** 반환 배열의 유효 인덱스는 `[1]`~`[10]`입니다. `[0]`은 사용되지 않습니다.

---

### `Score_Print`

```c
void Score_Print(ALLEGRO_FONT* font, stSTAGE_INFO* stage_info);
```

**Description**  
스코어 화면 전체를 렌더링합니다. 플레이어 이름, 처치 현황, 경과 시간, 총점, 리더보드 상위 10위를 출력합니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `font` | `ALLEGRO_FONT*` | 텍스트 렌더링 폰트 |
| `stage_info` | `stSTAGE_INFO*` | 전체 스테이지 통계 배열 |

**Return Value:** 없음 (`void`)

**Logic & Notes**
- `disp_pre_draw()` / `disp_post_draw()`를 내부에서 호출합니다.
- 경과 시간: 전 스테이지 `stage_frame` 합산 ÷ `CONFIG_SYSTEM_FRAME (60)` = 초 단위

---

## Util 모듈

> 📄 **파일:** `util.h` / `util.c`  
> 초기화 보조, 난수 생성 등 프로젝트 전반에서 사용하는 유틸리티 함수입니다.

---

### `must_init`

```c
void must_init(bool test, const char* description);
```

**Description**  
초기화 함수의 반환값을 검사하고, 실패 시 오류 메시지를 출력하고 `exit(1)`로 프로세스를 종료합니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `test` | `bool` | 초기화 성공 여부 (성공이 `true`) |
| `description` | `const char*` | 오류 메시지에 포함할 설명 |

**Return Value:** 없음 (`void`, 실패 시 프로세스 종료)

**사용 예시**

```c
must_init(al_init(), "allegro");
must_init(al_install_keyboard(), "keyboard");
```

---

### `util_rand_i`

```c
int util_rand_i(int lo, int hi);
```

**Description**  
`[lo, hi)` 범위의 균일한 정수 난수를 반환합니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `lo` | `int` | 범위 하한 (포함) |
| `hi` | `int` | 범위 상한 (미포함) |

**Return Value:** `lo` 이상 `hi` 미만의 정수

---

### `util_rand_f`

```c
float util_rand_f(float lo, float hi);
```

**Description**  
`[lo, hi)` 범위의 균일한 부동소수점 난수를 반환합니다.

**Parameters**

| 이름 | 타입 | 설명 |
|---|---|---|
| `lo` | `float` | 범위 하한 (포함) |
| `hi` | `float` | 범위 상한 (미포함) |

**Return Value:** `lo` 이상 `hi` 미만의 `float`

> ⚠️ **참고:** `util_rand_i` / `util_rand_f`는 표준 `rand()`를 래핑합니다. 적 AI 내부의 `Get_RandNum_1_to_9()`는 별도의 XorShift 알고리즘을 사용해 시드 독립성을 확보합니다.

---

*이 문서는 `bugglebuggle` 프로젝트의 전체 소스 코드 분석을 바탕으로 작성되었습니다.*  
*마지막 수정: 2026년 3월*
