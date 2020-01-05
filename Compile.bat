nasm -f elf32 Code\kernel.asm -o Compiled\kasm.o

gcc -m32 -c Code\kernel.c -o Compiled\kc.o -ffreestanding -nostdlib -nostdinc
gcc -m32 -c Code\interrupts.c -o Compiled\interrupts.o -ffreestanding -nostdlib -nostdinc
gcc -m32 -c Code\vga.c -o Compiled\vga.o -ffreestanding -nostdlib -nostdinc
gcc -m32 -c Code\math.c -o Compiled\math.o -ffreestanding -nostdlib -nostdinc
gcc -m32 -c Code\mmu.c -o Compiled\mmu.o -ffreestanding -nostdlib -nostdinc

gcc -m32 -c Code\paging.c -o Compiled\paging.o -ffreestanding -nostdlib -nostdinc
gcc -m32 -c Code\scheduler.c -o Compiled\scheduler.o -ffreestanding -nostdlib -nostdinc
gcc -m32 -c Code\syscall.c -o Compiled\syscall.o -ffreestanding -nostdlib -nostdinc
gcc -m32 -c Code\floppy.c -o Compiled\floppy.o -ffreestanding -nostdlib -nostdinc
gcc -m32 -c Code\drive.c -o Compiled\drive.o -ffreestanding -nostdlib -nostdinc
gcc -m32 -c Code\vbe.c -o Compiled\vbe.o -ffreestanding -nostdlib -nostdinc
gcc -m32 -c Code\graphics.c -o Compiled\graphics.o -ffreestanding -nostdlib -nostdinc

g++ -m32 -c Code\desktop.cpp -o Compiled\desktop.o -ffreestanding -nostdlib -nostdinc

ld -T link.ld -o Compiled\kernel Compiled\kasm.o Compiled\kc.o Compiled\interrupts.o Compiled\vga.o Compiled\paging.o  Compiled\scheduler.o Compiled\math.o Compiled\linkedList.o Compiled\mmu.o Compiled\syscall.o Compiled\floppy.o Compiled\drive.o Compiled\vbe.o Compiled\graphics.o Compiled\desktop.o -build-id=none

objcopy -O elf32-i386 Compiled\kernel kernel.bin

objcopy -O elf32-i386 Compiled\kernel kernel.elf

copy /y kernel.bin ISO\kernel.bin



Tools\ISO9660Generator.exe 4 "%CD%\DerpOS.iso" "%CD%\ISO\isolinux-debug.bin" true "%CD%\ISO"

pause