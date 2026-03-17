nasm -f elf32 system.asm -o sys.o

gcc -m32 -c test.c -o test.o -m32 -ffreestanding -nostdlib -nostdinc

ld -T link.ld -o fin sys.o test.o

objcopy -O elf32-i386 fin fin.bin

pause