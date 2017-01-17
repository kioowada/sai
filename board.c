#include "board.h"
#include "dice.h"
#include "player.h" // for POINT

#include <stdlib.h>
#include <stdio.h>

#define INDEX(iboard, w, h) ((h) * ((iboard)->width + 2) + (w))

////////////////////////////////////////////////////////////////////////////////////////////
// Exported Definition
////////////////////////////////////////////////////////////////////////////////////////////
CELL _board_get_target_cell(IBOARD iboard, int w, int h, int dir);
CELL _board_get_subject_cell(IBOARD iboard, int w, int h, int dir);
POINT _board_get_target_point(IBOARD iboard, int w, int h, int dir);
POINT _board_get_subject_point(IBOARD iboard, int w, int h, int dir);
int _board_set_cell(IBOARD iboard, int w, int h, CELL cell);

////////////////////////////////////////////////////////////////////////////////////////////
// Exported Definition
////////////////////////////////////////////////////////////////////////////////////////////
IBOARD board_new(BOARD_TYPE type, int width, int height, CELL* initializer) {
    IBOARD ret;
    int w, h;

    ret = (IBOARD)malloc(sizeof(BOARD) + sizeof(CELL)*(width+2)*(height+2));
    if (ret == NULL) {
        return NULL;
    }

    ret->board_type = type;
    ret->width = width;
    ret->height = height;

    for (h = 0; h < height+2; h++) {
        for (w = 0; w < width+2; w++) {
            int idx = h * (width+2) + w;
            int status = CELL_EMPTY;
            if ( (h==0) || (h==height+1) || (w==0) || (w==width+1) ) {
                status = CELL_INVALID;
            } else {
                status = initializer[(h-1) * width + (w-1)];
            }
            ret->cell[idx] = status;
        }
    }

    return ret;
}

int board_dice_exists(IBOARD iboard, int w, int h) {
    return dice_is_valid_dice(iboard->cell[INDEX(iboard, w, h)]);
}

CELL board_get_cell(IBOARD iboard, int w, int h) {
    return iboard->cell[INDEX(iboard, w, h)];
}

void board_print_board(IBOARD iboard) {
    int w, h;

    for (h = 0; h < iboard->height+2; h++) {
        for (w = 0; w < iboard->width+2; w++) {
            CELL cell = iboard->cell[INDEX(iboard, w, h)];
            printf("(%2d,%2d,%2d,%2d) ", dice_status(cell), dice_type(cell), dice_top(cell), dice_south(cell));
        }
        printf("\n");
    }

}

int board_push_dice(IBOARD iboard, int w, int h, int dir) {
    CELL target_cell = _board_get_target_cell(iboard, w, h, dir);
    DICE subject_dice = (DICE)_board_get_subject_cell(iboard, w, h, dir); // FIXME
    POINT target_point;
    POINT subject_point;

    printf("push dice: %d,%d -> %d\n", w, h, dir);
    printf("target_cell = 0x%08x, subject_dice = 0x%08x\n", target_cell, subject_dice);
    
    switch (target_cell) {
        case CELL_EMPTY:
            target_point = _board_get_target_point(iboard, w, h, dir);
            subject_point = _board_get_subject_point(iboard, w, h, dir);
            _board_set_cell(iboard, subject_point.w, subject_point.h, CELL_EMPTY);
            _board_set_cell(iboard, target_point.w, target_point.h, (CELL)subject_dice);
            break;
        case CELL_INVALID:
            break;
        default:
            break;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Internal Definition
////////////////////////////////////////////////////////////////////////////////////////////

CELL _board_get_target_cell(IBOARD iboard, int w, int h, int dir) {
    POINT target_point = _board_get_target_point(iboard, w, h, dir);
    return board_get_cell(iboard, target_point.w, target_point.h);
}

POINT _board_get_target_point(IBOARD iboard, int w, int h, int dir) {
    POINT ret;
    switch (dir) {
        case DIR_NORTH:
            h -= 2;
            break;
        case DIR_SOUTH:
            h += 2;
            break;
        case DIR_EAST:
            w += 2;
            break;
        case DIR_WEST:
            w -= 2;
            break;
        default:
            ret.w = -1;
            ret.h = -1;
            return ret;
    }

    ret.w = w;
    ret.h = h;
    return ret;
}

POINT _board_get_subject_point(IBOARD iboard, int w, int h, int dir) {
    POINT ret;
    switch (dir) {
        case DIR_NORTH:
            h -= 1;
            break;
        case DIR_SOUTH:
            h += 1;
            break;
        case DIR_EAST:
            w += 1;
            break;
        case DIR_WEST:
            w -= 1;
            break;
        default:
            ret.w = -1;
            ret.h = -1;
            return ret;
    }

    ret.w = w;
    ret.h = h;
    return ret;
}

CELL _board_get_subject_cell(IBOARD iboard, int w, int h, int dir) {
    POINT subject = _board_get_subject_point(iboard, w, h, dir);
    return board_get_cell(iboard, subject.w, subject.h);
}

int _board_set_cell(IBOARD iboard, int w, int h, CELL cell) {
    iboard->cell[INDEX(iboard, w, h)] = cell;
    return 0;
}
