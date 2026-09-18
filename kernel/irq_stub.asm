[BITS 32]

global irq0_stub
extern irq0_handler
extern process_switch

irq0_stub:

    ; Save all general-purpose registers.
    pusha

    ; Save the interrupted process's data segment.
    mov ax, ds
    push eax

    ; Use the kernel data segment while running C code.
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Pass the address of the saved interrupt frame.
    push esp
    call irq0_handler
    add esp, 4

    ; irq0_handler returns the selected process ESP in EAX.
    ;
    ; Do not return normally from this interrupt.
    ; process_switch will restore the selected context
    ; and finish with iret.
    jmp process_switch