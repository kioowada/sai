#include "dice.h"
#include "board.h"
#include "cell.h"
#include "game.h"
#include "player.h"

int main(void) {
    IBOARD iboard;
    CELL initializer[] = {CELL_EMPTY, dice(DT_NORMAL, 6, 5), dice(DT_NORMAL, 2, 6), CELL_EMPTY};
    PLAYER player;
    IGAME igame;

    iboard = board_new(BT_NORMAL, 2, 2, initializer);
    player.w = 2;
    player.h = 1;

    igame = game_new(iboard, player);
    game_commit_event(igame, event_move(0, 1));

    board_print_board(iboard);
    return 0;
}

/*
 * NULL 6
 * 2 NULL
 */


/*
 *  1
 *  2
 * 463
 *  5
 */
