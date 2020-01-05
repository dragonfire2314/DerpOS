;
;   Schedualer for DerpOS
;   Written by Tanner Kern
;

;   Data Structure
;   struct {
;       int esp
;       int ebp
;   }
;

;   Proccess data structures

extern _scheduler
global _asm_scheduler

_asm_addProcess:

    ret

_asm_scheduler:
    cli

    pushad

    mov al, 20h
    out 20h, al

    call _scheduler
    iretd

global _asm_scheduler_fin
_asm_scheduler_fin:
    pop eax

    jmp 0x8:_asm_long_jump_scheduler+0xC0000000

    iretd

_asm_long_jump_scheduler:
    popa

    push eax
    mov eax, esp
    add eax, 12
    mov esp, eax
    popf

    mov eax, [esp-12]
    mov [esp-4], eax

    sub esp, 16
    pop eax
    add esp, 8

    ret

    global _asm_scheduler_fin_kernel
_asm_scheduler_fin_kernel:
    pop eax

    jmp 0x28:_asm_long_jump_scheduler_kernel

    iretd

_asm_long_jump_scheduler_kernel:
    popa

    push eax
    mov eax, esp
    add eax, 12
    mov esp, eax
    popf

    mov eax, [esp-12]
    sub esp, 4
    mov [esp], eax

    sub esp, 16
    pop eax
    add esp, 8

    ret