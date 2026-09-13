bits 32
global irq0_stub
extern irq0_handler

irq0_stub:
    pusha
    mov ax, ds
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Pass ESP (pointing to the saved register structure) as an argument to irq0_handler
    push esp
    call irq0_handler
    add esp, 4

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    iret
