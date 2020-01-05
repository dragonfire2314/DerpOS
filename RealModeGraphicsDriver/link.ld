OUTPUT_FORMAT(pei-i386)
ENTRY(_start)
SECTIONS
 {
   . = 0x0;
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
 }