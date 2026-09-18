#include <kernel/process.h>
#include <kernel/pmm.h>

#define MAX_PROCESSES 10
#define STACK_SIZE 4096

static pcb_t process_table[MAX_PROCESSES];

pcb_t *current_process = NULL;
pcb_t *ready_queue = NULL;

static int next_pid = 1;

void process_init(void) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = 0;
        process_table[i].state = PROCESS_TERMINATED;
        process_table[i].esp = 0;
        process_table[i].kernel_stack = 0;
        process_table[i].next = NULL;
    }

    current_process = NULL;
    ready_queue = NULL;
    next_pid = 1;
}

int process_create(void (*entry)(void)) {
    if (entry == NULL) {
        return -1;
    }

    int idx = -1;

    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state == PROCESS_TERMINATED) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        return -1;
    }

    pcb_t *pcb = &process_table[idx];

    void *stack_frame = pmm_alloc_frame();

    if (stack_frame == 0) {
        return -1;
    }

    pcb->pid = next_pid++;
    pcb->state = PROCESS_READY;
    pcb->next = NULL;

    pcb->kernel_stack = (uint32_t)stack_frame + STACK_SIZE;

    uint32_t *stack = (uint32_t *)pcb->kernel_stack;

    *(--stack) = (uint32_t)entry;
    *(--stack) = 0;
    *(--stack) = 0;
    *(--stack) = 0;
    *(--stack) = 0;

    pcb->esp = (uint32_t)stack;

    /* Add the new process to the ready queue. */
    if (ready_queue == NULL) {
        ready_queue = pcb;
    } else {
        pcb_t *last = ready_queue;

        while (last->next != NULL) {
            last = last->next;
        }

        last->next = pcb;
    }

    return pcb->pid;
}

void process_yield(void) {
    /* Scheduling is currently driven by the timer interrupt. */
}