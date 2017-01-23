#include "board.h"
#include "dice.h"
#include "player.h" // for POINT

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define INDEX(iboard, w, h) ((h) * ((iboard)->width + 2) + (w))

////////////////////////////////////////////////////////////////////////////////////////////
// Internal Types
////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    int dummy;
    int top;
    int length;
    POINT points[0];
} CONNECTION, *ICONNECTION;

////////////////////////////////////////////////////////////////////////////////////////////
// Internal Declaration
////////////////////////////////////////////////////////////////////////////////////////////
CELL _board_get_target_cell(IBOARD iboard, int w, int h, int dir);
CELL _board_get_subject_cell(IBOARD iboard, int w, int h, int dir);
POINT _board_get_target_point(IBOARD iboard, int w, int h, int dir);
POINT _board_get_subject_point(IBOARD iboard, int w, int h, int dir);
int _board_set_cell(IBOARD iboard, int w, int h, CELL cell);
int _board_dir_to_dice_dir(int dir);


unsigned char* _board_state_operation_new(IBOARD iboard);
int _board_state_operation_is_changed(IBOARD iboard, unsigned char *so, int w, int h);
int _board_state_operation_change(IBOARD iboard, unsigned char *so, int w, int h);
void _board_state_operation_free(unsigned char *so);

ICONNECTION _board_get_connection(IBOARD iboard, int w, int h);
int _board_do_get_connection(IBOARD iboard, int w, int h, int top, int count, unsigned char *mask);
int _board_do_get_connection_impl(IBOARD iboard, int w, int h, int dw, int dh, int top, int count, unsigned char *mask);

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
            return 0;
        case CELL_INVALID:
            return -1;
        default:
            return -1;
    }

    return -1;
}

int board_roll_dice(IBOARD iboard, int w, int h, int dir) {
    CELL subject_cell = _board_get_subject_cell(iboard, w, h, dir);
    DICE current_dice = (DICE)board_get_cell(iboard, w, h); // FIXME
    POINT subject_point;

    switch (subject_cell) {
        case CELL_EMPTY:
            subject_point = _board_get_subject_point(iboard, w, h, dir);
            current_dice = dice_roll(current_dice, _board_dir_to_dice_dir(dir));
            _board_set_cell(iboard, w, h, CELL_EMPTY);
            _board_set_cell(iboard, subject_point.w, subject_point.h, (CELL)current_dice);
            return 0;
        case CELL_INVALID:
            return -1;
        default:
            return -1;
    }

    return -1;
}

// when last_event is
//  - move, check dice connection around target or subject cell, and then do the state_change
//      when target has a dice, then subject is empty
//      when subject has a dice, then target is empty
//  - state_change, no dice connection check is needed
ELIST board_get_internal_events(IBOARD iboard, EVENT last_event) {
    int w, h;
    ELIST elist = elist_new();
    unsigned char *so = _board_state_operation_new(iboard);
    CELL current_cell;
    DICE current_dice;
    int dice_state, i;
    ICONNECTION iconn;

    for (h = 1; h <= iboard->height; h++) {
        for (w = 1; w <= iboard->width; w++) {
            // if cell already updated, skip
            // if cell == empty, skip
            // if cell == existing dice, check connection
            // if cell == vanishing dice, goto next state
            printf("%d,%d,0x%08x\n", w, h, board_get_cell(iboard, w, h));

            if (_board_state_operation_is_changed(iboard, so, w, h)) {
                printf(" do nothing because already changed\n");
                continue;
            }

            current_cell = board_get_cell(iboard, w, h);
            if (current_cell == CELL_EMPTY) {
                printf(" skip because empty\n");
                continue;
            }

            if (dice_is_valid_dice((DICE)current_cell)) {
                current_dice = (DICE)current_cell;
                dice_state = dice_status(current_dice);
                switch (dice_state) {
                    case DS_SUBMERGED:
                        // move to DS_GONE state
                        elist_append(event_dice_state_change(w, h, DS_GONE), &elist);
                        _board_state_operation_change(iboard, so, w, h);
                        break;
                    case DS_GONE:
                        // move to EMPTY
                        elist_append(event_dice_vanish(w, h), &elist);
                        _board_state_operation_change(iboard, so, w, h);
                        break;
                    case DS_SOLID:
                        // check connection
                        printf(" check connection\n");
                        iconn = _board_get_connection(iboard, w, h);
                        printf(" iconn got:%p\n", iconn);
                        printf("top=%d,length=%d\n", iconn->top, iconn->length);
                        printf("%d,%d,%d,%d\n", iconn->points[0].w, iconn->points[0].h, iconn->points[1].w, iconn->points[1].h);
                        if ((iconn->top != 1) && (iconn->length >= iconn->top)) {
                            for (i = 0; i < iconn->length; i++) {
                                elist_append(event_dice_state_change(iconn->points[i].w, iconn->points[i].h, DS_SUBMERGED), &elist);
                                _board_state_operation_change(iboard, so, iconn->points[i].w, iconn->points[i].h);
                            }
                        }
                        break;
                }

            }
        }
    }

    _board_state_operation_free(so);
    return elist;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Internal Definition
////////////////////////////////////////////////////////////////////////////////////////////

ICONNECTION _board_get_connection(IBOARD iboard, int w, int h) {
    int count, ww, hh, i, top;
    unsigned char *mask; // TODO
    ICONNECTION iconn;

    mask = (unsigned char*)malloc(256);
    memset(mask, 0, 256); // TODO
    top = dice_top((DICE)board_get_cell(iboard, w, h));

    count = _board_do_get_connection(iboard, w, h, top, 0, mask);

    iconn = (ICONNECTION)malloc(sizeof(CONNECTION) + sizeof(POINT) * count);
    printf("sizeof(CONNECTION)=%lu\n", sizeof(CONNECTION));
    printf("malloced %lu\n", sizeof(CONNECTION) + sizeof(POINT) * count);
    iconn->length = count;
    iconn->top = top;
    for (i = 0; i < 256; i++) {
        if (mask[i]) {
            printf("[%d,%d:%d]", count, mask[i], i);
            count--;
            iconn->points[count].w = 181;
            iconn->points[count].h = 222;
//            iconn->points[count].w = i % (iboard->width + 2);
//            iconn->points[count].h = (i - iconn->points[count].w) / (iboard->width + 2);
            printf("->(%d,%d)@%p\n", iconn->points[count].w, iconn->points[count].h, &iconn->points[count]);
        }
    }
    printf("length=%d,top=%d", iconn->length, iconn->top);

    free(mask);
    return iconn;
}

int _board_do_get_connection(IBOARD iboard, int w, int h, int top, int count, unsigned char *mask) {
    int dw, dh;
    CELL cell;
    DICE dice;
    printf("  w=%d,h=%d,top=%d,count=%d\n", w, h, top, count);

    mask[INDEX(iboard, w, h)] = 1;

    dw = 0;
    for (dh = -1; dh <= 1; dh += 2) {
        count = _board_do_get_connection_impl(iboard, w, h, dw, dh, top, count, mask);
    }
    dh = 0;
    for (dw = -1; dw <= 1; dw += 2) {
        count = _board_do_get_connection_impl(iboard, w, h, dw, dh, top, count, mask);
    }

    return count;
}

int _board_do_get_connection_impl(IBOARD iboard, int w, int h, int dw, int dh, int top, int count, unsigned char *mask) {
    CELL cell;
    DICE dice;
    printf("   (%d,%d) mask=%d, cell=0x%08x\n", w + dw, h+ dh, mask[INDEX(iboard, w+dw,h+dh)], board_get_cell(iboard, w+dw, h+dh));
    if (mask[INDEX(iboard, w + dw, h + dh)]) {
        printf("    skip because masked\n");
        return count;
    }
    cell = board_get_cell(iboard, w + dw, h + dh);
    switch (cell) {
        case CELL_EMPTY:
            printf("    skip because empty\n");
            return count;
        case CELL_INVALID:
            printf("    skip because invalid\n");
            return count;
        default:
            dice = (DICE)cell;
            if (!dice_is_valid_dice(dice)) {
                printf("    skip because invalid dice\n");
                return count;
            }

            if (dice_top(dice) == top) {
                mask[INDEX(iboard, w + dw, h + dh)] = 1;
                return _board_do_get_connection(iboard, w + dw, h + dh, top, count + 1, mask);
            } else {
                // no connection
                return count;
            }
    }

    return count;
}

unsigned char* _board_state_operation_new(IBOARD iboard) {
    unsigned char *ret;

    ret = calloc((iboard->width + 2) * (iboard->height + 2), sizeof(unsigned char));

    memset(ret, 0, (iboard->width + 2) * (iboard->height + 2) * sizeof(unsigned char));
    return ret;
}

int _board_state_operation_is_changed(IBOARD iboard, unsigned char *so, int w, int h) {
    return so[INDEX(iboard, w, h)] != 0;
}

int _board_state_operation_change(IBOARD iboard, unsigned char *so, int w, int h) {
    so[INDEX(iboard, w, h)] = 1;
    return 0;
}

void _board_state_operation_free(unsigned char *so) {
    free(so);
}

// target: 進行方向 2 マス先
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

// target: 進行方向 1 マス先
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

int _board_dir_to_dice_dir(int dir) {
    switch (dir) {
        case DIR_NORTH:
            return DD_NORTH;
        case DIR_SOUTH:
            return DD_SOUTH;
        case DIR_EAST:
            return DD_EAST;
        case DIR_WEST:
            return DD_WEST;
    }
    return -1;
}
