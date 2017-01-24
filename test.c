#include "dice.h"
#include "board.h"
#include "cell.h"
#include "game.h"
#include "player.h"


int main(void) {
    IBOARD iboard;
//    CELL initializer[] = {dice(DT_NORMAL, 3, 5), CELL_EMPTY, CELL_EMPTY, dice(DT_NORMAL, 2, 1), CELL_EMPTY, CELL_EMPTY};
    CELL initializer[] = {dice(DT_NORMAL, 3, 5), CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY};
    PLAYER player;
    IGAME igame;
    ELIST elist;

    iboard = board_new(BT_NORMAL, 2, 3, initializer);
    player.w = 1;
    player.h = 1;

    igame = game_new(iboard, player);
    game_print_status(igame);
    game_commit_event(igame, event_move(0, 1));
    game_print_status(igame);
    game_commit_event(igame, event_move(1, 0));
    game_print_status(igame);
    game_commit_event(igame, event_move(0, -1));
    game_print_status(igame);
    game_commit_event(igame, event_move(-1, 0));
    game_print_status(igame);
    

    return 0;
}
/*
int main(void) {
    IBOARD iboard;
    CELL initializer[] = {CELL_EMPTY, dice(DT_NORMAL, 6, 5), dice(DT_NORMAL, 2, 6), CELL_EMPTY, CELL_EMPTY, CELL_EMPTY};
    PLAYER player;
    IGAME igame;

    iboard = board_new(BT_NORMAL, 2, 3, initializer);
    player.w = 2;
    player.h = 1;

    igame = game_new(iboard, player);
    game_print_status(igame);
    game_commit_event(igame, event_move(-1, 0));
    game_print_status(igame);
    game_commit_event(igame, event_move(1, 0));
    game_print_status(igame);
    game_commit_event(igame, event_move(0, 1));
    game_print_status(igame);
    game_commit_event(igame, event_move(0, -1));
    game_print_status(igame);
    

    return 0;
}
*/

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
