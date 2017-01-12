#include "game.h"
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Internal Declaration
////////////////////////////////////////////////////////////////////////////////////////////
CELL _game_get_current_cell(IGAME igame);


////////////////////////////////////////////////////////////////////////////////////////////
// Exported Definition
////////////////////////////////////////////////////////////////////////////////////////////
IGAME game_new(IBOARD iboard, PLAYER player) {
    IGAME igame;

    igame = (IGAME)malloc(sizeof(GAME));
    igame->iboard = iboard;
    igame->player = player;
    igame->elist  = elist_new();

    return igame;
}

int game_commit_event(IGAME igame, EVENT event) {
    CELL now_cell;

    now_cell = _game_get_current_cell(igame);
    switch (now_cell) {
        case CELL_EMPTY:
            // TODO just move on ground, or push dice
            break;
        case CELL_INVALID:
            // do nothing
            break;
        default:
            // TODO just move on dice, or roll dice
            break;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Internal Definition
////////////////////////////////////////////////////////////////////////////////////////////
CELL _game_get_current_cell(IGAME igame) {
    IBOARD iboard = igame->iboard;
    IPLAYER iplayer = &igame->player;
    return board_get_cell(iboard, iplayer->w, iplayer->h);
}
