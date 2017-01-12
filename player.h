#ifndef __PLAYER_H__
#define __PLAYER_H__

typedef struct {
    int w, h;
} PLAYER, *IPLAYER;


#define PLAYER_MOVE_NORTH(player) (player).h--
#define PLAYER_MOVE_SOUTH(player) (player).h++
#define PLAYER_MOVE_WEST(player) (player).w--
#define PLAYER_MOVE_EAST(player) (player).w++

#endif /* __PLAYER_H__ */
