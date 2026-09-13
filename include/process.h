#ifndef PROCESS_H
#define PROCESS_H

#include "types.h"

typedef enum {
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED,
    PROCESS_TERMINATED,
    READY = PROCESS_READY,
    RUNNING = PROCESS_RUNNING
} process_state_t;

typedef struct pcb {
    int pid;
    process_state_t state;
    uint32_t esp;
    uint32_t kernel_stack;
    struct pcb *next;
} pcb_t;

void process_init(void);
int process_create(void (*entry)(void));
void process_yield(void);

extern pcb_t *current_process;
extern pcb_t *ready_queue;

#endif
