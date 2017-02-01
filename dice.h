#ifndef __DICE_H__
#define __DICE_H__

/*
 *  1
 *  2
 * 463
 *  5
 */

typedef unsigned int DICE;
/* xxxx xxxx ssss TTTT tttt ssss;
 *  xxxx: unused
 *  ssss: status
 *  TTTT: type
 *  tttt: top
 *  ssss: south
 */

typedef unsigned int DICE_TYPE;

#define DT_NORMAL 0
#define DT_WOOD   1
#define DT_ICE    2
#define DT_ROCK   3
#define DT_STEAL  4
/* dice types
 *
 * DT_NORMAL: rollable, slides once when pushed
 * DT_WOOD  : rollable, rotate when pushed
 * DT_ICE   : rollable, slides 'til hits something when pushed
 *            when DT_ICE hits to DT_ICE, the target dice starts to slide
 * DT_ROCK  : rollable, unpushable
 * DT_STEAL : unrollable, unpushable
 */

typedef unsigned int DICE_STATUS;

#define DS_SOLID     0
#define DS_SUBMERGED 1
#define DS_GONE      2
/* dice status
 *
 * DS_SOLID     : the dice exists and is not going to vanish
 * DS_SUBMARGED : the dice is now vanishing but still exists
 * DS_GONE      : the dice is now vanishing and doesn't have its existence
 */

#define DD_SOUTH 0
#define DD_NORTH 1
#define DD_EAST  2
#define DD_WEST  3
/* direction */

#define DM_SLIDE  1
#define DM_ROTATE 2
/* what happens if the dice is pushed */

DICE dice(int type, int top, int south);
DICE dice_ws(int type, int top, int south, int status);
/* define a new dice */

DICE dice_roll(DICE dice, int dir);

int dice_is_valid_dice(DICE dice);

DICE_TYPE dice_type(DICE dice);
int dice_top(DICE dice);
int dice_bottom(DICE dice);
int dice_south(DICE dice);
int dice_north(DICE dice);
int dice_west(DICE dice);
int dice_east(DICE dice);

int dice_status(DICE dice);
DICE dice_change_status(DICE dice, DICE_STATUS status);

int dice_rollable(DICE dice);
int dice_pushable(DICE dice);
int dice_push_motion(DICE dice);
int dice_slide_inf(DICE dice);

#endif /* __DICE_H__ */
