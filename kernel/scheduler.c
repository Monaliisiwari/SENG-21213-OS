#include <scheduler.h>
#include <process.h>

extern pcb_t *current_process;
extern pcb_t *ready_queue;

void scheduler_init(void) {
    /*
     * Process and ready-queue initialization is handled
     * by process_init().
     */
}

void scheduler_tick(void) {
    if (ready_queue == NULL) {
        return;
    }

    /*
     * Put the currently running process at the end of
     * the ready queue.
     */
    if (current_process != NULL &&
        current_process->state == PROCESS_RUNNING) {

        current_process->state = PROCESS_READY;

        pcb_t *last = ready_queue;

        while (last->next != NULL) {
            last = last->next;
        }

        last->next = current_process;
        current_process->next = NULL;
    }

    /*
     * Select the next process from the front of the queue.
     */
    current_process = ready_queue;
    ready_queue = ready_queue->next;

    current_process->next = NULL;
    current_process->state = PROCESS_RUNNING;
}

void schedule(void) {
    scheduler_tick();
}
