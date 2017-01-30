#ifndef __GAME_H__
#define __GAME_H__

#include "board.h"
#include "player.h"
#include "list.h"

typedef struct {
    IBOARD iboard;
    PLAYER player;
    ELIST elist;
} GAME, *IGAME;

IGAME game_new(IBOARD iboard, PLAYER player);

ELIST game_next_event_candidates(IGAME igame);
/* returns a event_list of what can happen next.
 * returned event_list SHOULD get freed when done using it.
 */

int game_is_end(IGAME igame);

int game_commit_event(IGAME igame, EVENT event);

void game_print_status(IGAME igame);

#endif /* __GAME_H__ */
