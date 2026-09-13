#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <process.h>
void scheduler_init(void);
void scheduler_tick(void);
void schedule(void);

#endif
