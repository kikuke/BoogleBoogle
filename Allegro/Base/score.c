#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "score.h"
#include "bugglebuggle.h"
#include "sqlite3.h"
#include "render.h"
#include "keyboard.h"

int stage_timer_arr[STAGE_NUM_TOT] = { 0 };	// [ s1, s2, s3 ]
int enemy_num_arr[3] = { 0 };			// [ Basic, Throw, Boss ]

stBOARD leaderboard[11];

void Score_Add(ALLEGRO_FONT* font, const char *name, int score) {
    sqlite3* db;
    char* err_msg = 0;
    int rc;

    rc = sqlite3_open("leaderboard.db", &db);
    if (rc) {
        al_draw_textf(font, al_map_rgb(255, 255, 255), TEXT_X, TEXT_Y, FLAG_0, "Can't open DB errmsg : %s", sqlite3_errmsg(db));
        return;
    }

    char* sql_create = "CREATE TABLE IF NOT EXISTS Ranking("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "score INTEGER NOT NULL);";
    sqlite3_exec(db, sql_create, 0, 0, &err_msg);

    sqlite3_stmt* res;
    const char* sql_step_insert = "INSERT INTO Ranking (name, score) VALUES (?, ?);";

    sqlite3_prepare_v2(db, sql_step_insert, -1, &res, 0);

    sqlite3_bind_text(res, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_int(res, 2, score);

    if (sqlite3_step(res) != SQLITE_DONE) {

    }

    sqlite3_finalize(res);
    sqlite3_close(db);

}

stBOARD* Score_Get(void) {
    sqlite3* db;
    char* err_msg = 0;
    int rc;

    rc = sqlite3_open("leaderboard.db", &db);

    sqlite3_stmt* res;
    char* sql_select = "SELECT name, score FROM Ranking ORDER BY score DESC, name ASC LIMIT 10;";

    // ready sql
    rc = sqlite3_prepare_v2(db, sql_select, -1, &res, 0);

    if (rc == SQLITE_OK) {
        int rank = 1;
        int dx = 0;
        int i = 1;
        // read every row while sqlite3_step return SQLITE_ROW
        while (sqlite3_step(res) == SQLITE_ROW) {
            stBOARD* board = &leaderboard[i];
            board->rank = i;
            strcpy_s(board->player_name, CONFIG_SYSTEM_PLAYER_NAME_MAX, sqlite3_column_text(res, 0));
            board->score = sqlite3_column_int(res, 1);

            dx += 20;
            i++;
        }
    }

    sqlite3_finalize(res);
    sqlite3_close(db);

    return leaderboard;
}

void Score_Print(ALLEGRO_FONT* font, stSTAGE_INFO* stage_info) {

    disp_pre_draw();
    al_clear_to_color(al_map_rgb(0, 0, 0));

    int basic = 0;
    int fly = 0;
    int tot_time = 0;

    for (int i = 0; i < eGAME_STAGE_MAX; i++) {
        stSTAGE_INFO* info = &stage_info[i];
        basic += info->enemy_el[eENEMY_TYPE_BASIC];
        fly += info->enemy_el[eENEMY_TYPE_THROW];
        tot_time += info->stage_frame;
    }
    tot_time /= CONFIG_SYSTEM_FRAME;

    al_draw_textf(font, al_map_rgb(255, 255, 255), 110, 10, 0,
        "Score Board");

    al_draw_textf(font, al_map_rgb(255, 255, 255), 70, 25, 0,
        "Name: %-12s", keboard_get_name());

    al_draw_textf(font, al_map_rgb(255, 255, 255), 70, 40, 0,
        "Basic: %2d      Fly: %2d", basic, fly);

    al_draw_textf(font, al_map_rgb(255, 255, 255), 30, 55, 0,
        "Elapsed time: %5d Score: %5d", tot_time, 2000);

    al_draw_textf(font, al_map_rgb(255, 255, 255), 40, 80, 0,
        "Rank. Name%-12sScore"," ");

    al_draw_textf(font, al_map_rgb(255, 255, 255), 40, 90, 0,
        "-----------------------------");
    int dy = 0;
    for (int i = 1; i<11 ; i++, dy+=10) {
        al_draw_textf(font, al_map_rgb(255, 255, 255), 55, 100 + dy, 0,
            "%2d. %-12s : %6d", leaderboard[i].rank, leaderboard[i].player_name, leaderboard[i].score);
    }

    al_draw_textf(font, al_map_rgb(255, 255, 255), 70, 215, 0,
        "Press Space bar to Main");

    disp_post_draw();
}


void Score_Test(ALLEGRO_FONT* font) {

    disp_pre_draw();
    al_clear_to_color(al_map_rgb(0, 0, 0));

    sqlite3* db;
    char* err_msg = 0;
    int rc;

    rc = sqlite3_open("leaderboard.db", &db);
    if (rc) {
        al_draw_textf(font, al_map_rgb(255, 255, 255), TEXT_X, TEXT_Y, FLAG_0, "Can't open DB errmsg : %s", sqlite3_errmsg(db));
        return;
    }

    char* sql_create = "CREATE TABLE IF NOT EXISTS Ranking("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "score INTEGER NOT NULL);";
    sqlite3_exec(db, sql_create, 0, 0, &err_msg);
    char* sql_insert = "INSERT INTO Ranking (name, score) VALUES ('test2', 6);"
        "INSERT INTO Ranking (name, score) VALUES ('test2', 3);"
        "INSERT INTO Ranking (name, score) VALUES ('test3', 2);"
        "INSERT INTO Ranking (name, score) VALUES ('test4', 4);"
        "INSERT INTO Ranking (name, score) VALUES ('test5', 1);";
    sqlite3_exec(db, sql_insert, 0, 0, &err_msg);

    // call top 10 members
    sqlite3_stmt* res;
    char* sql_select = "SELECT name, score FROM Ranking ORDER BY score DESC, name ASC LIMIT 10;";

    // ready sql
    rc = sqlite3_prepare_v2(db, sql_select, -1, &res, 0);

    if (rc == SQLITE_OK) {
        int rank = 1;
        int dx = 0;
        // read every row while sqlite3_step return SQLITE_ROW
        while (sqlite3_step(res) == SQLITE_ROW) {
            const unsigned char* name = sqlite3_column_text(res, 0);
            int score = sqlite3_column_int(res, 1);

            al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 20 + dx, 0,
                "%2d. %-12s : %d", rank++, name, score);
            dx += 20;
        }
    }
    else {
        al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 0, 0,
            "F search data err : %s\n\n", sqlite3_errmsg(db));
    }

    // close DB
    sqlite3_finalize(res);
    sqlite3_close(db);

    disp_post_draw();
}
