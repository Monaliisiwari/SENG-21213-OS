#include "vga.h"
#include "keyboard.h"
#include "idt.h"
#include "../include/types.h"
#include <kernel/pmm.h>
#include <kernel/vmm.h>
#include <kernel/process.h>
#include <kernel/scheduler.h>

void task_a(void) {
    while (1) {
        vga_puts("A");
        for (volatile int i = 0; i < 50000; i++);
    }
}

void task_b(void) {
    while (1) {
        vga_puts("B");
        for (volatile int i = 0; i < 50000; i++);
    }
}

void kernel_main(void) {
    vga_init();
    kb_init();

    vga_clear(VGA_BLACK);
    vga_puts("Starting multitasking...\n");

    process_init();
    scheduler_init();

    process_create(task_a);
    process_create(task_b);

    idt_init();
    pit_init(100);

    asm volatile("sti");

    while (1) {
        asm volatile("hlt");
    }
}
