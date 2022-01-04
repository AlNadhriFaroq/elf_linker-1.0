#ifndef __LEC_SEC__
#define __LEC_SEC__

#include<stdint.h>

void find_section_int(FILE *elfFile, Elf64_Ehdr header, Elf64_Shdr sectHdr, int section);

void affichage_section(FILE *elfFile, Elf64_Ehdr header, char *section);


#endif