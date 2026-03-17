global _ata_disk_wait
global _ata_drq_wait
global _ata_getData
global _ata_writeData
global _atapi_get2ksector

;%define ATA_STATUS 177h

_ata_drq_wait:
    pusha
    xor al, al
    mov dx, 1F7h
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
    mov dx, 1F7h
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

_atapi_get2ksector:
;; get data from port
	mov edi, [esp+4]

	mov     dx, 01F0h
	;mov     edi, 1000000h
	mov     ecx, 2048
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