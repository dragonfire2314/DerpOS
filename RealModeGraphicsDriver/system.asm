bits 16

section .text

extern _Usermain
global start
start:
    call _Usermain
    ret