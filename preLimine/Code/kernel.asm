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

scheduler_memory_start:
	TIMES 16384 db 0

Kernel_Stack_End2:
	TIMES 65535 db 0
Kernel_Stack_Start2:

GDT_Contents:
db 0, 0, 0, 0, 0, 0, 0, 0			
db 255, 255, 0, 0, 0, 0x9A, 0xCF, 0    ;Ring 0 Code (No high half mapping)
db 255, 255, 0, 0, 0, 0x92, 0xCF, 0	   ;Ring 0 Data (No high half mapping)
db 255, 255, 0, 0, 0, 0xFA, 0xCF, 0	   ;Ring 3 Code
db 255, 255, 0, 0, 0, 0xF2, 0xCF, 0    ;Ring 3 Data
db 255, 255, 0, 0, 0, 0x9A, 0xC3, 0xC0 ;Kernel Code (High Half mapping)
db 255, 255, 0, 0, 0, 0x92, 0xC3, 0xC0 ;Kernel Data (High Half mapping)
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
_longJump:									;Long jump for loading into high half kernal
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

	jmp 0x28:_high_half_kmain			;C code for high half kernal mapping
	ret

_highKernelAsm:
	call _high_half_kmain
	ret

global _testFunc
_testFunc:
	int 35
	ret

global _defualt_ISR
_defualt_ISR:			;Default ISR - Resets IRQs then rets
	cli
	mov al, 20h
    out 20h, al
	iret

%include "Code/scheduler.asm"
%include "Code/asm/ata.asm"
%include "Code/asm/irq.asm"
%include "Code/asm/int32.asm"

extern _syscall
global _syscall_int
_syscall_int:
	cli
	pushad

	;Copy values to new temp stack
	;mov eax, [esp+48]
	;mov [0xC0FFFF00], eax
	;mov eax, [esp+52]
	;mov [0xC0FFFF04], eax
	;mov eax, [esp+56]
	;mov [0xC0FFFF08], eax

	mov ebx, [esp+48]
	mov ecx, [esp+52]

	mov eax, 0x30
	mov ds, eax
	mov ss, eax

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

extern _breakpoint_handler
global _breakpoint
_breakpoint:
	cli
	pushad

	call _breakpoint_handler

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

global _load_idt

;;Paging Stuff
_Pagging_CR0_CR3_Setup:

	;mov eax, 0h
	
	mov eax, 780000h ;;780000h is the pageDirectory address
 	mov cr3, eax
 
 	mov eax, cr0
 	or eax, 0x80000000
 	mov cr0, eax
	
	ret


global _write_port
global _read_port

global _read_port_w
global _write_port_w

_read_port_w:
	mov edx, [esp + 4]
			
	in ax, dx
	ret

_write_port_w:
	mov   edx, [esp + 4]    
	mov   ax, [esp + 4 + 4]  
	out   dx, ax  
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
	ret

_keyboard_handler:                 
	;call    _keyboard_handler_main
	mov al, 20h
    out 20h, al
	iretd	

_load_gdt:
	mov edx, [esp + 4]
	lgdt [edx]
	sti
	ret



