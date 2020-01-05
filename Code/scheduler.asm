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

    ;Switch segment registers to 8
    mov dword EAX, 0x30
	mov word DS, EAX

    mov dword EAX, 0x10
	mov word SS, EAX
	;mov word ES, EAX
	;mov word FS, EAX
	;mov word GS, EAX
	;mov word SS, EAX

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