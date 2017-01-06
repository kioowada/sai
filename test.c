#include "dice.h"
#include "board.h"
#include "cell.h"

int main(void) {
    IBOARD iboard;
    CELL initializer[] = {CELL_EMPTY, CELL_INVALID, dice(DT_NORMAL, 1, 2), CELL_EMPTY};

    iboard = board_new(BT_NORMAL, 2, 2, initializer);

    board_print_board(iboard);
    return 0;
}
