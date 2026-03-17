;Tanner Kern

global _irq14

extern _irq14_handler

_irq14:
  pusha
  call _irq14_handler
  popa
  iret