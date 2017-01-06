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

void board_print_board(IBOARD iboard);

#endif /* __BOARD_H__ */
