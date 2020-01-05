nasm -f elf32 ma.asm -o ma.o

gcc -m32 -c main.c -o main.o -ffreestanding -nostdlib -nostdinc

ld -T link.ld -o sample main.o ma.o -build-id=none

objcopy -O elf32-i386 sample sample.bin

objcopy -O elf32-i386 sample sample.elf

pause