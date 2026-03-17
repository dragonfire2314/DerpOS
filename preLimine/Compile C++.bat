nasm -f elf32 Code\kernel.asm -o Compiled\kasm.o

g++ -m32 -c Code\kernel.c -o Compiled\kc.o -ffreestanding -nostdlib -nostdinc

g++ -m32 -c Code\scheduler.c -o Compiled\scheduler.o -ffreestanding -nostdlib -nostdinc

g++ -m32 -c Code\interrupts.c -o Compiled\interrupts.o -ffreestanding -nostdlib -nostdinc

ld -T link.ld -o Compiled\kernel Compiled\kasm.o Compiled\kc.o Compiled\interrupts.o Compiled\scheduler.o -build-id=none

objcopy -O elf32-i386 Compiled\kernel kernel.bin

objcopy -O elf32-i386 Compiled\kernel kernel.elf

copy /y kernel.bin ISO\kernel.bin



Tools\ISO9660Generator.exe 4 "%CD%\DerpOS.iso" "%CD%\ISO\isolinux-debug.bin" true "%CD%\ISO"

pause