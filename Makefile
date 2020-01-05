
DerpOS: compile_asm compile_cpp link objcopy build-iso 

compile_asm:
	nasm -f elf32 Code\kernel.asm -o Compiled\kasm.o

compile_cpp:
	gcc -m32 -c Code\kernel.c -o Compiled\kernel.o -ffreestanding
	gcc -m32 -c Code\core\mmu.c -o Compiled\mmu.o -ffreestanding -nostdlib -nostdinc
	gcc -m32 -c Code\paging.c -o Compiled\paging.o -ffreestanding -nostdlib -nostdinc
	gcc -m32 -c Code\scheduler.c -o Compiled\scheduler.o -ffreestanding -nostdlib -nostdinc
	gcc -m32 -c Code\graphics_temp.c -o Compiled\graphics_temp.o -ffreestanding -nostdlib -nostdinc
	gcc -m32 -c Code\interrupts.c -o Compiled\interrupts.o -ffreestanding -nostdlib -nostdinc
	gcc -m32 -c Code\drive.c -o Compiled\dirve.o -ffreestanding
	gcc -m32 -c Code\core\syscall.c -o Compiled\syscall.o -ffreestanding -nostdlib -nostdinc
	gcc -m32 -c Code\graphics\vbe.c -o Compiled\vbe.o -ffreestanding -nostdlib -nostdinc

link:
	ld -T link.ld -o Compiled\kernel Compiled\kasm.o Compiled\kernel.o Compiled\syscall.o Compiled\vbe.o Compiled\mmu.o Compiled\paging.o Compiled\graphics_temp.o Compiled\scheduler.o Compiled\interrupts.o Compiled\dirve.o -build-id=none

objcopy:
	objcopy -O elf32-i386 Compiled\kernel kernel.bin
	objcopy -O elf32-i386 Compiled\kernel kernel.elf
	copy /y kernel.bin ISO\kernel.bin

build-iso:
	Tools\ISO9660Generator.exe 4 "%CD%\DerpOS.iso" "%CD%\ISO\isolinux-debug.bin" true "%CD%\ISO"

clean:
	del Compiled\*.o
	del kernel.bin
	del kernel.elf
	del DerpOS.iso
	pause