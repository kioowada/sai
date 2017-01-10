#ifndef __EVENT_LIST_H__
#define __EVENT_LIST_H__

#include "event.h"

typedef struct {
    EVENT *list;
    int length; // allocated length
    int num;    // number of properties
} ELIST, *IELIST;

#define ELIST_DEFAULT_ALLOCATE_LENGTH 1024

ELIST elist_new();
void elist_free(ELIST elist);

int elist_append(EVENT event, IELIST ielist);
/* append num to the tail */

EVENT elist_nth(unsigned int n, IELIST ielist);
/* get the n-th number. the first is counted as 0 */


#endif /* __EVENT_LIST_H__ */
