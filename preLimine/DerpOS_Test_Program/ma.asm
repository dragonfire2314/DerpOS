bits 32

global _pix

_pix:
   ; mov   ax, [esp + 4]    
	;mov   bx, [esp + 4 + 4]
   ; mov   cx, [esp + 4 + 4 + 4]
   ; mov   dx, [esp + 4 + 4 + 4 + 4]
   ; push  ax 
   ; push  bx  
   ; push  cx  
   ; push  dx
   ; mov bx, [esp + 4]
   ; mov [0x0], bx
   ; mov bx, [esp + 8]
   ; mov [0x0], bx
   ; mov bx, [esp + 12]
   ; mov [0x0], bx
   ; mov bx, [esp + 16]
   ; mov [0x0], bx
    int 34
    ret