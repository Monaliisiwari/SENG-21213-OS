#include <kernel/process.h>
#include <kernel/pmm.h>
#include <stddef.h>

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
        process_table[i].next = NULL;
    }
    current_process = NULL;
}

int process_create(void (*entry)(void)) {
    int idx = -1;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state == PROCESS_TERMINATED) {
            idx = i;
            break;
        }
    }
    if (idx == -1) return -1;

    pcb_t *pcb = &process_table[idx];
    pcb->pid = next_pid++;
    pcb->state = PROCESS_READY;
    pcb->next = NULL;

    // Allocate a kernel stack using PMM
    void *stack_frame = pmm_alloc_frame();
    uint32_t stack_top = (uint32_t)stack_frame + STACK_SIZE;
    pcb->kernel_stack = stack_top;

    // Set up initial stack frame for process_switch (edi, esi, ebx, ebp, ret_addr)
    uint32_t *stack = (uint32_t *)stack_top;
    
    // Push initial entry point as the return address for the first switch
    *(--stack) = (uint32_t)entry; 
    
    // Push initial registers saved by process_switch (edi, esi, ebx, ebp)
    *(--stack) = 0; // edi
    *(--stack) = 0; // esi
    *(--stack) = 0; // ebx
    *(--stack) = 0; // ebp

    pcb->esp = (uint32_t)stack;

    return pcb->pid;
}

void process_yield(void) {
    // Cooperative scheduling placeholder
}
