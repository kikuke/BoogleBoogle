
#include "score.h"
#include "bugglebuggle.h"
#include "sqlite3.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "render.h"
#include "render.c"


void Score_Add(stSCORE* score_manager) {
    sqlite3* db;
    char* err_msg = 0;
    int rc;

    rc = sqlite3_open("leaderboard.db", &db);
    if (rc) {
        al_draw_textf(font, al_map_rgb(255, 255, 255), TEXT_X, TEXT_Y, FLAG_0, "Can't open DB errmsg : %s", sqlite3_errmsg(db));
        return 1;
    }

    char* sql_create = "CREATE TABLE IF NOT EXISTS Ranking("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "score INTEGER NOT NULL);";
    sqlite3_exec(db, sql_create, 0, 0, &err_msg);

    sqlite3_stmt* res;
    const char* sql_step_insert = "INSERT INTO Ranking (name, score) VALUES (?, ?);";

    sqlite3_prepare_v2(db, sql_step_insert, -1, &res, 0);

    sqlite3_bind_text(res, 1, score_manager->player_name, -1, SQLITE_STATIC);
    sqlite3_bind_int(res, 2, score_manager->score);

    sqlite3_step(res) == SQLITE_DONE;

    sqlite3_finalize(res);
    sqlite3_close(db);

}

stBOARD* Score_Get(stSCORE* score_manager, stBOARD* leaderboard) {
    sqlite3* db;
    char* err_msg = 0;
    int rc;

    rc = sqlite3_open("leaderboard.db", &db);

    sqlite3_stmt* res;
    char* sql_select = "SELECT name, score FROM Ranking ORDER BY score DESC, namw ASC LIMIT 10;";

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
            *board->player_name = sqlite3_column_text(res, 0);
            board->score = sqlite3_column_int(res, 1);

            dx += 20;
            i++;
        }
    }

    sqlite3_finalize(res);
    sqlite3_close(db);

    return leaderboard;
}

void Score_Print(stSCORE* score_manager, stBOARD* leaderboard) {

    disp_pre_draw();
    al_clear_to_color(al_map_rgb(0, 0, 0));

    int dx = 0;
    for (int i = 1; i<11 ; i++, dx+=20) {
        al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 20 + dx, 0,
            "%2d. %-12s : %6d\n\n", leaderboard[i].rank, leaderboard[i].player_name, leaderboard[i].score);
    }

    disp_post_draw();
}


void Score_Test() {

    disp_pre_draw();
    al_clear_to_color(al_map_rgb(0, 0, 0));

    sqlite3* db;
    char* err_msg = 0;
    int rc;

    rc = sqlite3_open("leaderboard.db", &db);
    if (rc) {
        al_draw_textf(font, al_map_rgb(255, 255, 255), TEXT_X, TEXT_Y, FLAG_0, "Can't open DB errmsg : %s", sqlite3_errmsg(db));
        return 1;
    }

    char* sql_create = "CREATE TABLE IF NOT EXISTS Ranking("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "score INTEGER NOT NULL);";
    sqlite3_exec(db, sql_create, 0, 0, &err_msg);
    char* sql_insert = "INSERT INTO Ranking (name, score) VALUES ('test1', 4);"
        "INSERT INTO Ranking (name, score) VALUES ('test2', 3);"
        "INSERT INTO Ranking (name, score) VALUES ('test3', 2);"
        "INSERT INTO Ranking (name, score) VALUES ('test4', 4);"
        "INSERT INTO Ranking (name, score) VALUES ('test5', 1);";
    sqlite3_exec(db, sql_insert, 0, 0, &err_msg);

    // call top 10 members
    sqlite3_stmt* res;
    char* sql_select = "SELECT name, score FROM Ranking ORDER BY score DESC, namw ASC LIMIT 10;";

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
                "%2d. %-12s : %6d\n\n", rank++, name, score);
            dx += 20;
        }
    }
    else {
        al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 0, 0,
            "Failed search data err : %s\n\n", sqlite3_errmsg(db));
    }

    // close DB
    sqlite3_finalize(res);
    sqlite3_close(db);

    disp_post_draw();
}




