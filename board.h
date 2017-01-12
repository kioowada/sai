#ifndef __BOARD_H__
#define __BOARD_H__

#include "cell.h"

typedef unsigned int BOARD_TYPE;
#define BT_NORMAL 1
#define BT_ICE    2

typedef struct {
    BOARD_TYPE board_type;
    int width;
    int height;
    CELL cell[0];
} BOARD, *IBOARD;

IBOARD board_new(BOARD_TYPE type, int width, int height, CELL* initializer);

int board_dice_exists(IBOARD iboard, int w, int h);
CELL board_get_cell(IBOARD iboard, int w, int h);

void board_print_board(IBOARD iboard);

#endif /* __BOARD_H__ */
