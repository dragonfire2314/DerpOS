#include "elf.h"
#include "../../io/serial.h"
#include "../../core/mmu.h"
#include "../../paging.h"

#define SHT_NULL	          0x0	   
#define SHT_PROGBITS          0x1	   
#define SHT_SYMTAB	          0x2	   
#define SHT_STRTAB	          0x3	   
#define SHT_RELA	          0x4	   
#define SHT_HASH	          0x5	   
#define SHT_DYNAMIC	          0x6	   
#define SHT_NOTE	          0x7	   
#define SHT_NOBITS	          0x8	   
#define SHT_REL	              0x9	
#define SHT_SHLIB	          0x0A   
#define SHT_DYNSYM            0x0B 
#define SHT_INIT_ARRAY	      0x0E       
#define SHT_FINI_ARRAY        0x0F     
#define SHT_PREINIT_ARRAY     0x10        
#define SHT_GROUP	          0x11   
#define SHT_SYMTAB_SHNDX      0x12       
#define SHT_NUM               0x13         

struct ELF_FileHeader
{
    char e_ident[0x10];
    uint16 e_type;
    uint16 e_machine;
    uint32 e_version;
    uint32 e_entry;
    uint32 e_phoff;
    uint32 e_shoff;
    uint32 e_flags;
    uint16 e_ehsize;
    uint16 e_phentsize;
    uint16 e_phunt;
    uint16 e_shentsize;
    uint16 e_shunt;
    uint16 e_shstrndx;
} __attribute__ ((packed));

struct ELF_ProgramHeader
{
    uint32 p_type;
    uint32 p_offset;
    uint32 p_vaddr;
    uint32 p_paddr;
    uint32 p_filesz;
    uint32 p_memsz;
    uint32 p_flags;
    uint32 p_align;
} __attribute__ ((packed));

struct ELF_SectionHeader
{
    uint32 sh_name;
    uint32 sh_type;
    uint32 sh_flags;
    uint32 sh_addr;
    uint32 sh_offset;
    uint32 sh_size;
    uint32 sh_link;
    uint32 sh_info;
    uint32 sh_addralign;
    uint32 sh_entsize;
} __attribute__ ((packed));

void elf_copy_program(uint8* elf_data, uint8* dest, uint32 processID)
{
    ELF_FileHeader* fileHead = (ELF_FileHeader*)elf_data;
    ELF_ProgramHeader* progHead = (ELF_ProgramHeader*)(fileHead->e_phoff + ((uint32)elf_data));

    //Grab sring section where names are stored
    uint8* sections = elf_data + fileHead->e_shoff;

    serial_write_int((int)fileHead->e_shoff);

    ELF_SectionHeader* string_section = (ELF_SectionHeader*)(((uint8*)sections) + (fileHead->e_shstrndx * sizeof(ELF_SectionHeader)));

    serial_write_int((int)string_section);
    
    if (string_section->sh_type != SHT_STRTAB) return; //Check that the sring tab is corret //TODO - graceful exit

    uint8* strings = elf_data + string_section->sh_offset;

    ELF_SectionHeader* section = (ELF_SectionHeader*)sections;

    //memcpy(dest, elf_data + 0x1000, 0x300);

    for (int i = 0; i < fileHead->e_shunt; i++) {
            
        //Check if is a valid header
        if (section->sh_type == SHT_NULL) serial_write_string("Section NULL: ");
        if (section->sh_type == SHT_PROGBITS) {
            
            serial_write_string("Section Name: ");
            serial_write_string((char*)(strings + section->sh_name));
            serial_write_string("\r\n");

            //Allocate the required pages
            uint8* mem = (uint8*)alloc_large_memory((section->sh_size / 0x1000) + 1);
            //Load section into mem
            memcpy(mem, elf_data + section->sh_offset, section->sh_size);
            //Place section where is belongs relative to dest
            pagecpy(processID, (uint32*)section->sh_addr, mem);

            free_large(mem);
            /* serial_write_string("section->sh_offset: ");
            serial_write_int(section->sh_offset);
            serial_write_string("section->sh_size: ");
            serial_write_int(section->sh_size);
            serial_write_string("section->sh_addr: ");
            serial_write_int(section->sh_addr);
            serial_write_string("section->sh_type: ");
            serial_write_int(section->sh_type); */
        }

        section = (ELF_SectionHeader*)((((uint8*)section) + sizeof(ELF_SectionHeader)));
    }

    //Look through all sections
    //Load data into sections as need be
}

void elf_load_program(uint8* elf_data, uint8* dest) 
{
    
}