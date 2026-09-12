#include "process.h"

static pcb_t pcb_pool[MAX_PROCESSES];
pcb_t *ready_queue = 0;
pcb_t *current_process = 0;
static uint32_t next_pid = 1;

void process_init(void) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        pcb_pool[i].state = TERMINATED;
        pcb_pool[i].pid = 0;
    }
    ready_queue = 0;
    current_process = 0;
}

pcb_t *process_create(void (*entry)(void)) {
    pcb_t *p = 0;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (pcb_pool[i].state == TERMINATED) {
            p = &pcb_pool[i];
            break;
        }
    }
    if (!p) return 0;
    
    p->pid = next_pid++;
    p->state = READY;
    p->esp = (uint32_t)&p->stack[STACK_SIZE / 4 - 1];
    p->eip = (uint32_t)entry;
    p->next = 0;
    
    if (!ready_queue) {
        ready_queue = p;
    } else {
        pcb_t *temp = ready_queue;
        while (temp->next) temp = temp->next;
        temp->next = p;
    }
    return p;
}

void process_yield(void) {
    // Context switch stub
}

void process_exit(void) {
    if (current_process) {
        current_process->state = TERMINATED;
    }
    process_yield();
}
