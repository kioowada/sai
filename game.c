#include "game.h"
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Internal Declaration
////////////////////////////////////////////////////////////////////////////////////////////
CELL _game_get_current_cell(IGAME igame);

int _game_commit_event_move(IGAME igame, EP_MOVE param);
CELL _game_get_move_target_cell(IGAME igame, EP_MOVE param);
int _game_move_player(IGAME igame, EP_MOVE param);
int _game_push_dice(IGAME igame, EP_MOVE param);

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
    EVENT_PARAM_RET eparam;
    switch (event_type(event)) {
        case ET_PLAYER_MOVE:
            eparam = event_get_param(event);
            return _game_commit_event_move(igame, eparam.param.move_param);
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Internal Definition
////////////////////////////////////////////////////////////////////////////////////////////
CELL _game_get_current_cell(IGAME igame) {
    IBOARD iboard = igame->iboard;
    IPLAYER iplayer = &igame->player;
    return board_get_cell(iboard, iplayer->w, iplayer->h);
}

int _game_commit_event_move(IGAME igame, EP_MOVE param) {
    CELL now_cell, target_cell;

    now_cell = _game_get_current_cell(igame);
    switch (now_cell) {
        case CELL_EMPTY:
            // TODO just move on ground, or push dice
            target_cell = _game_get_move_target_cell(igame, param);
            switch (target_cell) {
                case CELL_EMPTY:
                    _game_move_player(igame, param);
                    break;
                case CELL_INVALID:
                    break;
                default:
                    _game_push_dice(igame, param);
            }
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

CELL _game_get_move_target_cell(IGAME igame, EP_MOVE param) {
    IPLAYER iplayer = &igame->player;
    return board_get_cell(igame->iboard, iplayer->w + param.w, iplayer->h + param.h);
}

int _game_move_player(IGAME igame, EP_MOVE param) {
    igame->player.w += param.w;
    igame->player.h += param.h;
    return 0;
}

int _game_push_dice(IGAME igame, EP_MOVE param) {
    // TODO
    return 0;
}
