#include <stddef.h>
#include <scheduler.h>
#include <process.h>

extern void process_switch(uint32_t *old_esp, uint32_t new_esp);

extern pcb_t *current_process;
extern pcb_t *ready_queue;

void scheduler_init(void) {
}

void scheduler_tick(void) {
    if (ready_queue == NULL) return;

    pcb_t *old_process = current_process;

    if (current_process && current_process->state == RUNNING) {
        current_process->state = READY;
        pcb_t *last = ready_queue;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = current_process;
        current_process->next = NULL;
    }

    current_process = ready_queue;
    ready_queue = ready_queue->next;
    current_process->state = RUNNING;

    if (old_process != current_process && old_process != NULL) {
        process_switch(&old_process->esp, current_process->esp);
    }
}

void schedule(void) {
    scheduler_tick();
}
