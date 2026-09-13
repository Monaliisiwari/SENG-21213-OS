global process_switch
[BITS 32]

process_switch:
    push ebp
    push ebx
    push esi
    push edi

    mov ecx, [esp + 20]   ; old_sp_ptr
    mov edx, [esp + 24]   ; new_sp

    mov [ecx], esp
    mov esp, edx

    pop edi
    pop esi
    pop ebx
    pop ebp
    ret
