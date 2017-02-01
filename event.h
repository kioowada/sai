#ifndef __EVENT_H__
#define __EVENT_H__

#include <inttypes.h>

typedef unsigned int EVENT;
/* xccc tttt pppp pppp pppp pppp pppp pppp
 *
 *  ccc:  usually 0. 1 when the event is triggered by the previous event
 *  tttt: event type
 *  pppp: parameter
 *
 *  NOTE: `c' is not used (implemented) now, because it is not needed unless
 *  you want to show the replay of the game.
 */

#define ET_PLAYER_MOVE       1
/*
 * DUPLICATED: MUST NOT USE ( I MEAN THIS IS USELESS)
 * player moved
 *
 * [parameter]
 *  xxxx xxxx wwww wwww hhhh hhhh
 *  www/hhh: move distance (usually one)
 */
#define ET_DICE_STATE_CHANGE 2
/*
 * dice state changed (mainly vanishing)
 *
 * [parameter]
 *  ssss ssss wwww wwww hhhh hhhh
 */

#define ET_DICE_VANISH 3
/*
 * dice vanished completely
 * [parameter]
 *  xxxx xxxx wwww wwww hhhh hhhh
 *  www/hhh: which position
 */

#define ET_DICE_PUSH 4
/*
 * push dice
 *
 * [oarameter]
 *  xxxx xxdd wwww wwww hhhh hhhh
 */

#define ET_DICE_ROLL 5
/*
 * roll dice
 *
 * [oarameter]
 *  xxxx xxdd wwww wwww hhhh hhhh
 */

typedef struct {
    int8_t w, h;
} EP_MOVE;

typedef struct {
    int8_t w, h;
} EP_DICE_VANISH;

typedef struct {
    unsigned char w, h, state;
} EP_DICE_STATE_CHANGE;

typedef struct {
    int result_type; // -1 when invalid
    union {
        EP_MOVE move_param;
        EP_DICE_STATE_CHANGE dice_state_change_param;
        EP_DICE_VANISH vanish_param;
    } param;
} EVENT_PARAM_RET;

EVENT event_move(int8_t w, int8_t h);
EVENT event_dice_state_change(int w, int h, int state);
EVENT event_dice_vanish(int8_t w, int8_t h);

int event_type(EVENT event);

int event_is_move(EVENT event);
int event_is_dice_state_change(EVENT event);
int event_is_dice_vanish(EVENT event);

EVENT_PARAM_RET event_get_param(EVENT event);


#endif /* __EVENT_H__ */
