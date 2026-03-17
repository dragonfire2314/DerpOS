nasm -f elf32 lib/syscalls.asm -o sys.o

gcc -m32 -c test.c -o test.o -m32 -ffreestanding -nostdlib -nostdinc
gcc -m32 -c lib/derpOS.c -o derpOS.o -m32 -ffreestanding -nostdlib -nostdinc
gcc -m32 -c lib/memory.c -o memory.o -m32 -ffreestanding -nostdlib -nostdinc

ld -T link.ld -o fin test.o sys.o memory.o derpOS.o

objcopy -O elf32-i386 fin fin.bin
objcopy -O binary fin.bin output.bin

DerpFat.exe "C:\Users\tanne\VirtualBox VMs\DerpOS\formatedTest.vhd" 3 output.bin

DerpFat.exe "C:\Users\tanne\VirtualBox VMs\DerpOS\formatedTest.vhd" 1 output.bin

pause