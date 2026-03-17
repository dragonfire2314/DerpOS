bits 32

section .text
MultibootSignature dd 464367618
MultibootFlags dd 3
MultibootChecksum dd -464367621
MultibootGraphicsRuntime_VbeModeInfoAddr dd 2147483647
MultibootGraphicsRuntime_VbeControlInfoAddr dd 2147483647
MultibootGraphicsRuntime_VbeMode dd 2147483647
MultibootInfo_Memory_High dd 0
MultibootInfo_Memory_Low dd 0

MultibootInfo_Structure dd 0

Kernel_Stack_End:
	TIMES 65535 db 0
Kernel_Stack_Start:

GDT_Contents:
db 0, 0, 0, 0, 0, 0, 0, 0			
db 255, 255, 0, 0, 0, 0x9A, 0xCF, 0	
db 255, 255, 0, 0, 0, 0x92, 0xCF, 0	
db 255, 255, 0, 0, 0, 0xFA, 0xCF, 0	
db 255, 255, 0, 0, 0, 0xF2, 0xCF, 0
db 255, 255, 0, 0, 0, 0x9A, 0xC3, 0xC0
db 255, 255, 0, 0, 0, 0x92, 0xC3, 0xC0
;16Bit
db 255, 255, 0, 0, 0, 0x9B, 0, 0	
db 255, 255, 0, 0, 0, 0x93, 0, 0

GDT_Pointer db 71, 0, 0, 0, 0, 0

savcr0:
	dd 0

idt_real:
	dw 0x3ff		; 256 entries, 4b each = 1K
	dd 0

extern _kmain
extern _high_half_kmain
global _keyboard_handler
global _timer_time
global _put_pixel

global start
start:
	;Code to get to protected mode
	cli
	mov dword ECX, 0x2BADB002
	cmp ECX, EAX
	jne HandleNoMultiboot
	
	mov dword [MultibootInfo_Structure], EBX
	add dword EBX, 0x4
	mov dword EAX, [EBX]
	mov dword [MultibootInfo_Memory_Low], EAX
	add dword EBX, 0x4
	mov dword EAX, [EBX]
	mov dword [MultibootInfo_Memory_High], EAX
	mov dword EAX, CR0
	or EAX, 1
	mov dword CR0, EAX

	mov dword ESP, Kernel_Stack_Start

	mov dword [GDT_Pointer + 2], GDT_Contents
	mov dword EAX, GDT_Pointer
	lgdt [EAX]
	
	mov dword EAX, 0x10
	mov word DS, EAX
	mov word ES, EAX
	mov word FS, EAX
	mov word GS, EAX
	mov word SS, EAX

	jmp 0x8:Boot_FlushCsGDT
Boot_FlushCsGDT:

	call _kmain

	jmp Halt
HandleNoMultiboot:
	mov dword EAX, 0x4F	
	mov dword EBX, 0xB8000 
	mov dword ECX, 2000	
	.ColourOutput:
	mov byte [EBX], 0
	mov byte [EBX+1], AL
	add EBX, 2
	loop .ColourOutput

Halt:
	cli	
	hlt			
	jmp Halt	

global _longJump
_longJump:
	mov dword EAX, 0x30
	mov word DS, EAX
	mov word ES, EAX
	mov word FS, EAX
	mov word GS, EAX
	mov word SS, EAX

	;mov eax, 0x30
	;mov ds, eax
	;mov ss, eax

	mov dword [GDT_Pointer + 2], GDT_Contents+0xC0000000
	mov dword EAX, GDT_Pointer
	lgdt [EAX]

	jmp 0x28:_high_half_kmain
	ret

_highKernelAsm:
	call _high_half_kmain
	ret

global _testFunc
_testFunc:
	int 35
	ret

global _defualt_ISR
_defualt_ISR:
	iret

extern _syscall
global _syscall_int
_syscall_int:
	cli
	pushad

	;Copy values to new temp stack
	mov eax, [esp+48]
	mov [0xC0FFFF00], eax
	mov eax, [esp+52]
	mov [0xC0FFFF04], eax
	mov eax, [esp+56]
	mov [0xC0FFFF08], eax

	mov eax, 0x30
	mov ds, eax
	mov ss, eax
	mov es, eax
	mov fs, eax
	mov gs, eax

	mov eax, esp
	mov esp, 0x00FFFF00
	push eax

	call _syscall

	pop eax
	mov esp, eax

	mov eax, 0x10
	mov ds, eax
	mov ss, eax
	mov es, eax
	mov fs, eax
	mov gs, eax

	popad
	iret

[global _read_cr0]
_read_cr0:
	mov eax, cr0
	retn

[global _write_cr0]
_write_cr0:
	;push ebp
	;mov ebp, esp
	mov eax, [esp+4]
	mov cr0,  eax
	;pop ebp
	retn

[global _read_cr3]
_read_cr3:
	mov eax, cr3
	retn

[global _write_cr3]
_write_cr3:
	;push ebp
	;mov ebp, esp
	mov eax, [esp+4]
	mov cr3, eax
	;pop ebp
	retn

;Scheduler
%include "Code/scheduler.asm"
%include "Code/realMode.asm"
%include "Code/int32.asm"


vg_stuff:
	call mode13h

	nop

	jmp $

	ret



mode13h:
	mov ah, 0
	mov al, 0x13
	int 10h
	ret

global _Pic
global _load_idt

extern _Debug
extern _Debug2

global _ata_disk_wait
global _ata_drq_wait
global _ata_getData
global _ata_writeData

_Pic:
	;call _Debug
	;Schedualer
	;Step one make a copy of the flags, cs, and eip
	push eax
	
	;eFlags
	mov eax, [esp+12]
	mov [21000h], eax
	;CS
	mov eax, [esp+8]
	mov [20FFCh], eax
	;EIP
	mov eax, _UserSpace
	mov [20FF8h], eax
	

	pop eax
	;Step 2 push all registers
	;Step 3 switch esp
	mov esp, 20FF8h
	mov ebp, 2100fh

	iretd

_UserSpace:
	call _Debug
	hlt
	ret

global _write_port
global _read_port

_ata_drq_wait:
    pusha
    xor al, al
    mov dx, 01F7h
.loop:   
    in  al, dx
    test al, 008h
    jz .loop
.end:
    popa
    ret

_ata_disk_wait:
    pusha
    xor ax, ax
    mov dx, 01F7h
.loop:
    in  al, dx
    and al, 0C0h
    cmp al, 040h
    jne .loop
.end:
    popa
    ret

_ata_getData:
;; get data from port
	mov edi, [esp+4]

	mov     dx, 01F0h
	;mov     edi, 1000000h
	mov     ecx, 256
	rep     insw
	ret

_ata_writeData:
;; Send data to port
	xor cx, cx 			;Sets cx to 0
	mov dx, 01F0h		;Sets port for write
	mov ebx, 1000200h	;where the data is read from
.ata_write_loop:
	
	mov ax, [ebx] 		;loads write value as 16 bit
	out dx, ax			;Outputs 16 bit value

	inc cx				;incraments cx and ebx
	inc ebx
	inc ebx
	cmp cx, 256			;Counts cycles
	jle .ata_write_loop

;;Paging Stuff
_Pagging_CR0_CR3_Setup:

	;mov eax, 0h
	
	mov eax, 780000h ;;780000h is the pageDirectory address
 	mov cr3, eax
 
 	mov eax, cr0
 	or eax, 0x80000000
 	mov cr0, eax
	
	ret








_read_port:
	mov edx, [esp + 4]
			
	in al, dx
	ret

_write_port:
	mov   edx, [esp + 4]    
	mov   al, [esp + 4 + 4]  
	out   dx, al  
	ret

_load_idt:
	mov edx, [esp + 4]
	lidt [edx] 
	sti		
	ret

_keyboard_handler:                 
	;call    _keyboard_handler_main
	iretd	

_load_gdt:
	mov edx, [esp + 4]
	lgdt [edx]
	sti
	ret

_put_pixel:
	;call _put_pixel_main
	iretd

_timer_time:
	;call _timer_set_main
	call _Debug2
	iretd

_startInt:
	sti
	ret

_stopInt:
	cli
	ret

_test:
	call 0x1001000
	ret 