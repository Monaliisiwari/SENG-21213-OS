#include <scheduler.h>
#include <stdint.h>

// Inline assembly to send End-Of-Interrupt (EOI) to the PIC
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd" (port));
}

void timer_handler(void *regs) {
    (void)regs;

    // Acknowledge the interrupt to the PIC so further timer ticks aren't blocked
    outb(0x20, 0x20);

    // Trigger the round-robin scheduler preemption
    scheduler_tick();
}
