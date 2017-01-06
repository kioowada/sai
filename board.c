#include "board.h"
#include "dice.h"

#include <stdlib.h>
#include <stdio.h>

#define INDEX(iboard, w, h) ((h) * ((iboard)->width + 2) + (w))

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

void board_print_board(IBOARD iboard) {
    int w, h;

    for (h = 0; h < iboard->height+2; h++) {
        for (w = 0; w < iboard->width+2; w++) {
            CELL cell = iboard->cell[INDEX(iboard, w, h)];
            printf("(%2d,%2d,%2d) ", dice_type(cell), dice_top(cell), dice_south(cell));
        }
        printf("\n");
    }

}
