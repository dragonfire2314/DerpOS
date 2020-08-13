# DerpOS
An operating system written entirely in C and assembly using syslinux for the stage 1 and stage 2 bootloaders.

(Code was recently changed to c++ although only structs from c++ were used, it can and should be reverted to c).

DerpOS was originally created as a project for a operating systems college course, but quickly went beyond the original goals. After creating a simple kernel and console with basic input and a simple vga character output for the class, many other features were added.

### Features
- Basic Round Robin Scheduling
- Programming loading form hardrive
- Basic harddisk implementation
- Procces GDT
- IDT
- Memory Managment
- Pagging
- Process Isolation
- Syscall from external process;

### Planned Features
- Kernel Console
- More Syscalls
- Basic pixel graphics
- Dynamic memory allocation

### Credit
The code for DerpOS was written entirly by Tanner Kern over the course of 2 years on and off.
