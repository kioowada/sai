#include "list.h"

#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Internal Declaration
////////////////////////////////////////////////////////////////////////////////////////////
int _elist_reallocate(IELIST ielist, int length);

////////////////////////////////////////////////////////////////////////////////////////////
// Exported Definition
////////////////////////////////////////////////////////////////////////////////////////////

ELIST elist_new() {
    ELIST elist;

    elist.num = 0;
    elist.length = ELIST_DEFAULT_ALLOCATE_LENGTH;
    elist.list = (EVENT*)calloc(sizeof(EVENT), elist.length);

    return elist;
}

void elist_free(ELIST elist) {
    if(elist.list) { free(elist.list); }
}

int elist_append(EVENT event, IELIST ielist) {
    if (ielist->num == ielist->length) {
        if (_elist_reallocate(ielist, 2 * ielist->length) < 0) {
            return -1;
        }
    }

    ielist->list[ielist->num++] = event;
    return 0;
}

EVENT elist_nth(unsigned int n, IELIST ielist) {
    return ielist->list[n];
}

int elist_concat(IELIST ielist1, IELIST ielist2) {
    int i;
    for (i = 0; i < ielist2->num; i++) {
        elist_append(elist_nth(i, ielist2), ielist1);
    }
    return 0;
}

void elist_print(IELIST ielist) {
    int i;
    EVENT event;

    for (i = 0; i < ielist->num; i++) {
        event = elist_nth(i, ielist);
        printf("%d: 0x%08x\n", i, event);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
// Internal Definition
////////////////////////////////////////////////////////////////////////////////////////////

int _elist_reallocate(IELIST ielist, int length) {
    // TODO reallocate
    return 0;
}
