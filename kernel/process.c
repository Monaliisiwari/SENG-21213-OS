#include <kernel/process.h>
#define MAX_PROCESSES 10
#define STACK_SIZE 4096

static pcb_t process_table[MAX_PROCESSES];

/*
 * Each process gets its own kernel stack.
 * These are statically allocated so process stacks never overlap
 * with the kernel or low physical memory.
 */
static uint8_t process_stacks[MAX_PROCESSES][STACK_SIZE]
    __attribute__((aligned(16)));

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

    pcb->pid = next_pid++;
    pcb->state = PROCESS_READY;
    pcb->next = NULL;

    uint32_t stack_top =
        (uint32_t)(process_stacks[idx] + STACK_SIZE);

    pcb->kernel_stack = stack_top;

    /*
     * Build a fake interrupt frame.
     *
     * irq0_stub expects the stack to contain:
     *
     *   saved DS
     *   EDI
     *   ESI
     *   EBP
     *   saved ESP
     *   EBX
     *   EDX
     *   ECX
     *   EAX
     *   EIP
     *   CS
     *   EFLAGS
     *
     * When the scheduler first selects this process,
     * process_switch() restores this frame and iret enters
     * the process at its entry function.
     */

    uint32_t *stack = (uint32_t *)stack_top;

    *(--stack) = 0x202;              /* EFLAGS: interrupts enabled */
    *(--stack) = 0x08;               /* CS */
    *(--stack) = (uint32_t)entry;    /* EIP */

    *(--stack) = 0;                  /* EAX */
    *(--stack) = 0;                  /* ECX */
    *(--stack) = 0;                  /* EDX */
    *(--stack) = 0;                  /* EBX */
    *(--stack) = 0;                  /* saved ESP */
    *(--stack) = 0;                  /* EBP */
    *(--stack) = 0;                  /* ESI */
    *(--stack) = 0;                  /* EDI */

    *(--stack) = 0x10;               /* saved DS */

    pcb->esp = (uint32_t)stack;

    /* Add process to the ready queue. */
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
    /*
     * Preemptive scheduling is driven by the PIT timer interrupt.
     */
}
