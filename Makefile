
DerpOS: compile_asm compile_cpp link makeTerminal objcopy build-iso

compile_asm:
	nasm -f elf32 Code\kernel.asm -o Compiled\kasm.o

compile_cpp:
	g++ -m32 -c Code\kernel.cpp -o Compiled\kernel.o -ffreestanding -Wno-write-strings
	g++ -m32 -c Code\scheduler.cpp -o Compiled\scheduler.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\core\mmu.cpp -o Compiled\mmu.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\paging.cpp -o Compiled\paging.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\interrupts.cpp -o Compiled\interrupts.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\io\serial.cpp -o Compiled\serial.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\disk\ata.cpp -o Compiled\ata.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\disk\drivers\atapi_driver.cpp -o Compiled\atapi_driver.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\global\string\cstring.cpp -o Compiled\cstring.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\graphics\vgaText.cpp -o Compiled\vgaText.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\kernelLog.cpp -o Compiled\kernelLog.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\util\elf\elf.cpp -o Compiled\elf.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\core\syscall.cpp -o Compiled\syscall.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\graphics\vbe.cpp -o Compiled\vbe.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\graphics\windowManager.cpp -o Compiled\windowManager.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\graphics\draw\shapes.cpp -o Compiled\shapes.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\graphics\screen.cpp -o Compiled\screen.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\desktop\desktop.cpp -o Compiled\desktop.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings
	g++ -m32 -c Code\util\string.cpp -o Compiled\string.o -ffreestanding -nostdlib -nostdinc -Wno-write-strings

link:
	ld -T link.ld -o Compiled\kernel \
	Compiled\kasm.o \
	Compiled\kernel.o \
	Compiled\mmu.o \
	Compiled\paging.o \
	Compiled\serial.o \
	Compiled\scheduler.o \
	Compiled\interrupts.o \
	Compiled\ata.o \
	Compiled\atapi_driver.o \
	Compiled\cstring.o \
	Compiled\vgaText.o \
	Compiled\kernelLog.o \
	Compiled\elf.o \
	Compiled\syscall.o \
	Compiled\vbe.o \
	Compiled\windowManager.o \
	Compiled\shapes.o \
	Compiled\screen.o \
	Compiled\desktop.o \
	Compiled\string.o \
	-build-id=none

objcopy:
	objcopy -O elf32-i386 Compiled\kernel kernel.bin
	objcopy -O elf32-i386 Compiled\kernel kernel.elf
	copy /y kernel.bin ISO\kernel.bin

build-iso:
	Tools\ISO9660Generator.exe 4 "%CD%\DerpOS.iso" "%CD%\ISO\isolinux-debug.bin" true "%CD%\ISO"

makeTerminal:
	make -C .\TestProgram\

clean:
	del Compiled\*.o
	del kernel.bin
	del kernel.elf
	del DerpOS.iso
	pause