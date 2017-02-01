#include "game.h"
#include "dice.h"
#include <stdlib.h>
#include <stdio.h>

#define INDEX(iboard, w, h) ((h) * ((iboard)->width + 2) + (w))
////////////////////////////////////////////////////////////////////////////////////////////
// Internal Declaration
////////////////////////////////////////////////////////////////////////////////////////////
CELL _game_get_current_cell(IGAME igame);

int _game_commit_event_move(IGAME igame, EP_MOVE param);
CELL _game_get_move_target_cell(IGAME igame, EP_MOVE param);
int _game_move_player(IGAME igame, EP_MOVE param);
int _game_push_dice(IGAME igame, EP_MOVE param);
int _game_roll_dice(IGAME igame, EP_MOVE param);
int _game_ep_move_to_dir(EP_MOVE param);
int _game_ignite_internal_event(IGAME igame, EVENT last_event);

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

int game_is_end(IGAME igame) {
    return !board_is_any_dice_alive(igame->iboard);
}

int game_commit_event(IGAME igame, EVENT event) {
    EVENT_PARAM_RET eparam;
    switch (event_type(event)) {
        case ET_PLAYER_MOVE:
            eparam = event_get_param(event);
            _game_commit_event_move(igame, eparam.param.move_param);
            break;
        default:
            return -1;
    }
    elist_append(event, &igame->elist);
    return _game_ignite_internal_event(igame, event);
}

void game_print_status(IGAME igame) {
    int w, h, player_here;
    IBOARD iboard = igame->iboard;
    IPLAYER iplayer = &igame->player;

    for (h = 0; h < iboard->height+2; h++) {
        for (w = 0; w < iboard->width+2; w++) {
            CELL cell = iboard->cell[INDEX(iboard, w, h)];
            player_here = (iplayer->w == w) && (iplayer->h == h);
            printf("(%s%2d,%2d,%2d,%2d) ", player_here?"P":" ", dice_status(cell), dice_type(cell), dice_top(cell), dice_south(cell));
        }
        printf("\n");
    }

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
            // just move on ground, or push dice
            target_cell = _game_get_move_target_cell(igame, param);
            switch (target_cell) {
                case CELL_EMPTY:
                    _game_move_player(igame, param);
                    break;
                case CELL_INVALID:
                    break;
                default:
                    if(_game_push_dice(igame, param) == 0) {
                        _game_move_player(igame, param);
                    }
            }
            break;
        case CELL_INVALID:
            // do nothing
            break;
        default:
            // TODO just move on dice, or roll dice
            target_cell = _game_get_move_target_cell(igame, param);
            switch (target_cell) {
                case CELL_EMPTY:
                    if (_game_roll_dice(igame, param) == 0) {
                        _game_move_player(igame, param);
                    }
                    break;
                case CELL_INVALID:
                    break;
                default:
                    _game_move_player(igame, param);
            }
            break;
    }

    return 0;
}

int _game_ignite_internal_event(IGAME igame, EVENT last_event) {
    int i;
    EVENT event;
    ELIST ignited_events = board_get_internal_events(igame->iboard, last_event);

    for (i = 0; i < ignited_events.num; i++) {
        event = elist_nth(i, &ignited_events);
        board_apply_event(igame->iboard, event);
    }

    elist_concat(&igame->elist, &ignited_events);

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
    int dir = _game_ep_move_to_dir(param);
    int ret;
    if (ret = board_push_dice(igame->iboard, igame->player.w, igame->player.h, dir),
            ret < 0) {
        return ret;
    } else {
        return 0;
    }
}

int _game_ep_move_to_dir(EP_MOVE param) {
    if (param.w == 1) {
        return DIR_EAST;
    } else if (param.w == -1) {
        return DIR_WEST;
    } else if (param.h == 1) {
        return DIR_SOUTH;
    } else if (param.h == -1) {
        return DIR_NORTH;
    }

    return -1;
}

int _game_roll_dice(IGAME igame, EP_MOVE param) {
    int dir = _game_ep_move_to_dir(param);
    int ret;
    if (ret = board_roll_dice(igame->iboard, igame->player.w, igame->player.h, dir),
            ret < 0) {
        return ret;
    } else {
        return 0;
    }
}
