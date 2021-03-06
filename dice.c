#include "dice.h"


////////////////////////////////////////////////////////////////////////////////////////////
// Internal Declaration
////////////////////////////////////////////////////////////////////////////////////////////
int _dice_is_valid_type(DICE_TYPE type);
int _dice_is_valid_status(DICE_STATUS status);
int _dice_is_valid_num(int num);

////////////////////////////////////////////////////////////////////////////////////////////
// Exported Definition
////////////////////////////////////////////////////////////////////////////////////////////
DICE dice(int type, int top, int south) {
    return dice_ws(type, top, south, DS_SOLID);
}

DICE dice_ws(int type, int top, int south, int status) {
    if (_dice_is_valid_type(type) && _dice_is_valid_num(top)
            && _dice_is_valid_num(south) && _dice_is_valid_status(status)) {
        return (type<<8) | (top<<4) | south;
    }
    return -1;
}

DICE dice_roll(DICE dice, int dir) {
    DICE_TYPE type = dice_type(dice);

    if (!dice_rollable(dice)) {
        return -1;
    }

    int status = dice_status(dice);
    switch (dir) {
        case DD_SOUTH:
            return dice_ws(type, dice_north(dice), dice_top(dice), status);
        case DD_NORTH:
            return dice_ws(type, dice_south(dice), dice_bottom(dice), status);
        case DD_EAST:
            return dice_ws(type, dice_west(dice), dice_south(dice), status);
        case DD_WEST:
            return dice_ws(type, dice_east(dice), dice_south(dice), status);
    }
    return -1;
}

int dice_is_valid_dice(DICE dice) {
    return _dice_is_valid_type(dice_type(dice)) && _dice_is_valid_num(dice_top(dice)) && _dice_is_valid_num(dice_south(dice));
}

/*
 *  1
 *  2
 * 463
 *  5
 */

DICE_TYPE dice_type(DICE dice) {
    return (dice>>8)&0xf;
}

int dice_top(DICE dice) {
    return (dice&0xf0)>>4;
}

int dice_bottom(DICE dice) {
    return 7 - dice_top(dice);
}

int dice_south(DICE dice) {
    return (dice&0xf);
}

int dice_north(DICE dice) {
    return 7 - dice_south(dice);
}

int dice_west(DICE dice) {
    int top = dice_top(dice);
    int south = dice_south(dice);

    if ( ((top==4)&&(south==2)) || ((top==2)&&(south==3)) || ((top==3)&&(south==5)) || ((top==5)&&(south==4)) ) {
        return 6;
    }
    if ( ((top==2)&&(south==4)) || ((top==3)&&(south==2)) || ((top==5)&&(south==3)) || ((top==4)&&(south==5)) ) {
        return 1;
    }
    if ( ((top==1)&&(south==2)) || ((top==2)&&(south==6)) || ((top==6)&&(south==5)) || ((top==5)&&(south==1)) ) {
        return 4;
    }
    if ( ((top==2)&&(south==1)) || ((top==6)&&(south==2)) || ((top==5)&&(south==6)) || ((top==1)&&(south==5)) ) {
        return 3;
    }
    if ( ((top==4)&&(south==6)) || ((top==6)&&(south==3)) || ((top==3)&&(south==1)) || ((top==1)&&(south==4)) ) {
        return 5;
    }
    if ( ((top==6)&&(south==4)) || ((top==3)&&(south==6)) || ((top==1)&&(south==3)) || ((top==4)&&(south==1)) ) {
        return 5;
    }

    return -1; // dummy
}

int dice_status(DICE dice) {
    return (dice>>16)&0xf;
}

DICE dice_change_status(DICE dice, DICE_STATUS status) {
    if (!_dice_is_valid_status(status)) {
        return (DICE)-1;
    }

    return (dice&0xffff0fff) | (status<<16);
}

int dice_east(DICE dice) {
    return 7 - dice_west(dice);
}

int dice_rollable(DICE dice) {
    return dice_type(dice) != DT_STEAL;
}

int dice_pushable(DICE dice) {
    return (dice_type(dice) != DT_STEAL) && (dice_type(dice) != DT_ROCK);
}

int dice_push_motion(DICE dice) {
    if (dice_pushable(dice)) {
        if (dice_type(dice) == DT_ROCK) {
            return DM_ROTATE;
        } else {
            return DM_SLIDE;
        }
    }

    return -1; // dummy
}

int dice_slide_inf(DICE dice) {
    return dice_type(dice) == DT_ICE;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Internal Definition
////////////////////////////////////////////////////////////////////////////////////////////

int _dice_is_valid_type(DICE_TYPE type) {
    return (type==DT_NORMAL) || (type==DT_ROCK) || (type==DT_ICE) || (type==DT_ROCK) || (type==DT_STEAL);
}

int _dice_is_valid_num(int num) {
    return (num >= 1) && (num <= 6);
}

int _dice_is_valid_status(DICE_STATUS status) {
    return (status==DS_SOLID) || (status==DS_SUBMERGED) || (status==DS_GONE);
}
