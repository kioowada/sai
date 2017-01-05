#ifndef __DICE_H__
#define __DICE_H__

/*
 *  1
 *  2
 * 463
 *  5
 */

typedef unsigned int DICE;
/* xxxx xxxx xxxx TTTT tttt ssss;
 *  xxxx: unused
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

#define DD_SOUTH 0
#define DD_NORTH 1
#define DD_EAST  2
#define DD_WEST  3
/* direction */

#define DM_SLIDE  1
#define DM_ROTATE 2
/* what happens if the dice is pushed */

DICE dice(int type, int top, int south);
/* define a new dice */

DICE_TYPE dice_type(DICE dice);
int dice_top(DICE dice);
int dice_bottom(DICE dice);
int dice_south(DICE dice);
int dice_north(DICE dice);
int dice_west(DICE dice);
int dice_east(DICE dice);

int dice_rollable(DICE dice);
int dice_pushable(DICE dice);
int dice_push_motion(DICE dice);
int dice_slide_inf(DICE dice);

#endif /* __DICE_H__ */
