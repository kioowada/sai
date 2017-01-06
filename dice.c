#include "dice.h"


////////////////////////////////////////////////////////////////////////////////////////////
// Internal Declaration
////////////////////////////////////////////////////////////////////////////////////////////
int _dice_is_valid_type(DICE_TYPE type);
int _dice_is_valid_num(int num);

////////////////////////////////////////////////////////////////////////////////////////////
// Exported Definition
////////////////////////////////////////////////////////////////////////////////////////////
DICE dice(int type, int top, int south) {
    if (_dice_is_valid_type(type) && _dice_is_valid_num(top) && _dice_is_valid_num(south)) {
        return (type<<8) | (top<<4) | south;
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

