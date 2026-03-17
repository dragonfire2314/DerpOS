bits 32

section .text

extern _Usermain
global start
start:
    call _Usermain
    ret

global _sys1
global _sys0x10_graphics_init

_sys1:
    int 35
    ret

_sys0x10_graphics_init:
    int 35
    ret