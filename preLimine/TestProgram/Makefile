Project: lib usercode link move

build: lib usercode link move


lib: .FORCE
.FORCE:
	nasm -f elf32 lib/syscalls.asm -o syscall.o
	g++ -m32 -c lib/derpOS.cpp -o derpOS.o -m32 -ffreestanding -nostdlib -nostdinc
	g++ -m32 -c lib/memory.cpp -o memory.o -m32 -ffreestanding -nostdlib -nostdinc
	g++ -m32 -c lib/graphics/window.cpp -o window.o -m32 -ffreestanding -nostdlib -nostdinc

usercode:
	g++ -m32 -c test.cpp -o test.o -m32 -ffreestanding -nostdlib -nostdinc

link:
	ld -T link.ld -o fin test.o syscall.o memory.o derpOS.o window.o

move:
	objcopy -O elf32-i386 fin fin.bin
	objcopy -O binary fin.bin output.bin
	copy /y fin.bin ..\ISO\TERMINAL

format_drive:
	DerpFat.exe "C:\Users\tanne\VirtualBox VMs\DerpOS\formatedTest.vhd" 3 output.bin

load_to_drive:
	DerpFat.exe "C:\Users\tanne\VirtualBox VMs\DerpOS\formatedTest.vhd" 1 output.bin