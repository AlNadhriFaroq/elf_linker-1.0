#ifndef __SECTION_H__
#define __SECTION_H__
#include <stdio.h>
#include "readElfHeader.h"
typedef struct {
uint32_t sh_name;
uint32_t sh_type;
uint64_t sh_flags;
uint64_t sh_addr;
uint64_t sh_offset;
uint64_t sh_size;
uint32_t sh_link;
uint32_t sh_info;
uint64_t sh_addralign;
uint64_t sh_entsize;
} Elf64_Shdr;


void affiche_section_table(FILE *elfFile,Elf64_Ehdr header);
#endif