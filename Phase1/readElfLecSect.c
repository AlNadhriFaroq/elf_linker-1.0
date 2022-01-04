#include "readElfSecTable.h"
#include "readElfHeader.h"
#include "readElfLecSect.h"
#include <ctype.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void find_section_int(FILE *elfFile, Elf64_Ehdr header, Elf64_Shdr sectHdr, int section){
    uint8_t tab;
    int j = 1;
    fseek(elfFile, sectHdr.sh_offset, SEEK_SET);
    
    printf("  0x%08lx ", sectHdr.sh_addr);
    for(int i = 0; i < sectHdr.sh_size; i++){
        fread(&tab, 1, sizeof(tab), elfFile);
        printf("%02x", tab);
        if((i+1) % 4 == 0){
            printf(" ");
        }
        if (((i+1) % 16 == 0) && (i+1 != sectHdr.sh_size)){
            printf("\n  0x%08lx ", sectHdr.sh_addr + 16 * j);
            j++;
        }
    }
    printf("\n\n");
}

void affichage_section(FILE *elfFile, Elf64_Ehdr header, char *section)
{
    int number = 1, i = 0;
    Elf64_Shdr sectHdr;
    char *sectNames = NULL;

    fseek(elfFile, header.e_shoff + header.e_shstrndx * header.e_shentsize, SEEK_SET);
    fread(&sectHdr, 1, sizeof(sectHdr), elfFile);
    sectNames = malloc(sectHdr.sh_size);
    fseek(elfFile, sectHdr.sh_offset, SEEK_SET);
    fread(sectNames, 1, sectHdr.sh_size, elfFile);
    char *sh_name = "";

    for ( int i = 0; i< strlen(section); i++){
        if (!isdigit(section[i])){
            number = 0;
        }
    }

    switch (number)
    {
    case 1:
        fseek(elfFile, header.e_shoff + atoi(section) * sizeof(sectHdr), SEEK_SET);
        fread(&sectHdr, 1, sizeof(sectHdr), elfFile);
        sh_name = sectNames + sectHdr.sh_name;
        printf("\nHex dump of section '%s':\n", sh_name);
        find_section_int(elfFile, header, sectHdr, atoi(section));
        break;
    
    default:

        while(i < header.e_shnum)
        {
            
            char *sh_name = "";
            fseek(elfFile, header.e_shoff + i * sizeof(sectHdr), SEEK_SET);
            fread(&sectHdr, 1, sizeof(sectHdr), elfFile);
            sh_name = sectNames + sectHdr.sh_name;
            if(strcmp(section, sh_name) == 0){
                printf("\nHex dump of section '%s':\n", sh_name);
                find_section_int(elfFile, header, sectHdr, i);
                break;
            }      
            i++;      
        }
        if(i == header.e_shnum ){
            printf("readelf: Warning: Section '%s' was not dumped because it does not exist!\n", section);
        }
        break;
    }
  free(sectNames);
}