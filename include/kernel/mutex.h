#ifndef KERNEL_MUTEX_H
#define KERNEL_MUTEX_H

typedef struct mutex {
    int locked;
} mutex_t;

void mutex_init(mutex_t *lock);

#endif
