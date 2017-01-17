#include "event.h"

////////////////////////////////////////////////////////////////////////////////////////////
// Internal Declaration
////////////////////////////////////////////////////////////////////////////////////////////
EVENT_PARAM_RET _event_get_param_dice_state_change(EVENT event);
EVENT_PARAM_RET _event_get_param_move(EVENT event);

////////////////////////////////////////////////////////////////////////////////////////////
// Exported Definition
////////////////////////////////////////////////////////////////////////////////////////////

EVENT event_move(int8_t w, int8_t h) {
    return (ET_PLAYER_MOVE<<24) | ((w&0xff)<<8) | (h&0xff);
}

EVENT event_dice_state_change(int w, int h, int state) {
    return (ET_DICE_STATE_CHANGE<<24) | ((state&0xff)<<16) | ((w&0xff)<<8) | (h&0xff);
}

int event_type(EVENT event) {
    return event>>24 & 0xff;
}

int event_is_move(EVENT event) {
    return ((event>>24) | ET_PLAYER_MOVE) == ET_PLAYER_MOVE;
}

int event_is_dice_state_change(EVENT event) {
    return ((event>>24) | ET_DICE_STATE_CHANGE) == ET_DICE_STATE_CHANGE;
}

EVENT_PARAM_RET event_get_param(EVENT event) {
    EVENT_PARAM_RET null_ret;
    switch (event_type(event)) {
        case ET_PLAYER_MOVE:
            return _event_get_param_move(event);
        case ET_DICE_STATE_CHANGE:
            return _event_get_param_dice_state_change(event);
    }

    null_ret.result_type = -1;
    return null_ret;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Internal Definition
////////////////////////////////////////////////////////////////////////////////////////////

EVENT_PARAM_RET _event_get_param_move(EVENT event) {
    int8_t w, h;
    EVENT_PARAM_RET ret;

    w = (event>>8) & 0xff;
    h = event & 0xff;

    ret.result_type = ET_PLAYER_MOVE;
    ret.param.move_param.w = w;
    ret.param.move_param.h = h;

    return ret;
}

EVENT_PARAM_RET _event_get_param_dice_state_change(EVENT event) {
    int w, h, state;
    EVENT_PARAM_RET ret;

    state = (event>>16) & 0xff;
    w = (event>>8) & 0xff;
    h = event & 0xff;

    ret.result_type = ET_DICE_STATE_CHANGE;
    ret.param.dice_state_change_param.state = state;
    ret.param.dice_state_change_param.w = w;
    ret.param.dice_state_change_param.h = h;

    return ret;
}
