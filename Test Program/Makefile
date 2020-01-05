Project: lib usercode link copy format_drive load_to_drive
	pause

build: lib usercode link copy


lib:
	gcc -m32 -c lib/derpOS.c -o derpOS.o -m32 -ffreestanding -nostdlib -nostdinc
	gcc -m32 -c lib/memory.c -o memory.o -m32 -ffreestanding -nostdlib -nostdinc

usercode:
	gcc -m32 -c test.c -o test.o -m32 -ffreestanding -nostdlib -nostdinc

link:
	ld -T link.ld -o fin test.o sys.o memory.o derpOS.o

copy:
	objcopy -O elf32-i386 fin fin.bin
	objcopy -O binary fin.bin output.bin

format_drive:
	DerpFat.exe "C:\Users\tanne\VirtualBox VMs\DerpOS\formatedTest.vhd" 3 output.bin

load_to_drive:
	DerpFat.exe "C:\Users\tanne\VirtualBox VMs\DerpOS\formatedTest.vhd" 1 output.bin