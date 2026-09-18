#include "idt.h"
#include "vga.h"
#include <kernel/scheduler.h>
#include <kernel/process.h>

static struct idt_entry idt[256];
static struct idt_ptr   idtp;

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__("outb %0, %1" : : "a"(val), "Nd"(port));
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

extern void irq0_stub(void);

/*
 * Called from irq0_stub().
 *
 * frame points to the saved DS value at the bottom of
 * the interrupt context frame.
 *
 * Returns the ESP of the process that should resume.
 */

uint32_t irq0_handler(uint32_t *frame) {

    if (current_process != NULL) {
        current_process->esp = (uint32_t)frame;
    }

    scheduler_tick();

    outb(0x20, 0x20);

    if (current_process != NULL) {
        return current_process->esp;
    }

    return (uint32_t)frame;
}
void pic_remap(void) {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    /*
     * Enable only IRQ0 on the master PIC.
     */
    outb(0x21, 0xFE);
    outb(0xA1, 0xFF);
}

void idt_init(void) {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (uint32_t)&idt;

    pic_remap();

    /*
     * IRQ0 -> interrupt vector 32.
     */
    idt_set_gate(32, (uint32_t)irq0_stub, 0x08, 0x8E);

    __asm__ __volatile__("lidt %0" : : "m"(idtp));
}

void pit_init(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);

    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}
