#ifndef _TIMER_H
#define _TIMER_H

#include "base.h"

#define PIT_FREQUENCY 1193180
#define TIMER_FREQUENCY 100

void timer_set(u32 freq_timer);

#endif //_TIMER_H