#include "process.h"

extern pcb_t *ready_queue;
extern pcb_t *current_process;

void scheduler_tick(void) {
    if (!ready_queue) return;
    
    if (!current_process) {
        current_process = ready_queue;
        current_process->state = RUNNING;
        ready_queue = ready_queue->next;
        current_process->next = 0;
        return;
    }
    
    if (current_process->state == RUNNING) {
        current_process->state = READY;
        pcb_t *temp = ready_queue;
        if (!temp) {
            ready_queue = current_process;
        } else {
            while (temp->next) temp = temp->next;
            temp->next = current_process;
        }
        current_process = 0;
    }
    
    current_process = ready_queue;
    current_process->state = RUNNING;
    ready_queue = ready_queue->next;
    current_process->next = 0;
}
