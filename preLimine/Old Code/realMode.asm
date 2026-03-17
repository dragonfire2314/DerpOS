;   x86 vm loader and reaturn
;   by Tanner Kern

global _v86RealMode
_v86RealMode:
    ;I THING THIS IS THE ORDER
    ;es 
    mov eax, 0
    push eax
    ;ds
    mov eax, 0
    push eax
    ;fs
    mov eax, 0
    push eax
    ;gs
    mov eax, 0
    push eax

    ;Correct order
    ;ss
    mov eax, 0
    push eax
    ;esp
    mov eax, 0x0000FFFC
    push eax
    ;eflags
    pushfd
    or dword [esp], (1<<17)
    ;cs
    mov eax, 0
    push eax
    ;eip
    mov eax, 0x00007E00
    push eax

    iret

