#ifndef _MAIN_H_
#define _MAIN_H_

#include "kal_event.h"

enum {
    EVENT_BUTTON_PUSH = _KAL_EVENT_USER_START,
    EVENT_ANIMATION_TICK,
    EVENT_REFRESH,
    EVENT_QUANTITY,
    EVENT_ALL = KAL_EVENT_ALL,
};

#endif // _MAIN_H_
