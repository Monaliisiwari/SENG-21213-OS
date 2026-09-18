[BITS 32]

global process_switch

process_switch:

    ; EAX = ESP of the process to resume.
    mov esp, eax

    ; Restore saved DS.
    pop eax

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Restore EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX.
    popa

    ; Restore EIP, CS and EFLAGS.
    iret