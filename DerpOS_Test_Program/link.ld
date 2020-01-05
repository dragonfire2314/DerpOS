OUTPUT_FORMAT(pei-i386)
ENTRY(_pmain)
SECTIONS
 {
   . = 0x0;
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
 }
