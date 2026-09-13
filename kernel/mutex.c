#include <types.h>
#include <kernel/mutex.h>

void mutex_init(mutex_t *lock) {
    if (lock) {
        lock->locked = 0;
    }
}

