#include "readElfSym.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void affichage_dynsym(FILE *elfFile, uint64_t offset, uint64_t entrees){
    fseek(elfFile, offset, SEEK_SET);
    Elf64_Sym sym;
    printf("   Num:    Value          Size Type    Bind   Vis      Ndx Name\n");
    for (int i = 0; i < entrees; i++){
        fread(&sym, 1, sizeof(sym), elfFile);
        printf("    %d:   %016lx     %lu       %d       %d        %lu \n", i, sym.st_value, sym.st_size, ELF64_ST_TYPE(sym.st_info), ELF64_ST_BIND(sym.st_info), sym.st_name);
    }
}

void affichage_symtab(FILE *elfFile, uint64_t offset, uint64_t entrees){
    assert(fseek(elfFile, offset, SEEK_SET) == 0);
    printf("l'adresse de la table des symboles est %lx\n", offset);
    
    Elf64_Sym sym;
    printf("   Num:    Value          Size Type    Bind   Vis      Ndx Name\n");
    for (int i = 0; i < entrees; i++){
        assert(fread(&sym, 1, sizeof(sym), elfFile) != 0);
        //printf("%d\n", ELF64_ST_TYPE(sym.st_info));
        printf("    %2d:   %016lx     %lu       %d       %d        %lu \n", i, sym.st_value, sym.st_size, ELF64_ST_TYPE(sym.st_info), ELF64_ST_BIND(sym.st_info), sym.st_name);
    }
}


TYPE_SYMB find_type_symbole(long num)
{
    switch (num)
    {
    case 2:
        return SYMTAB;
    case 11:
        return DYNSYM;
    default:
        return OTHER_SYMB;
    }
}

void affiche_symboles(FILE *elfFile, Elf64_Ehdr header)
{
    Elf64_Shdr sectHdr;
    char *sectNames = NULL;

    fseek(elfFile, header.e_shoff + header.e_shstrndx * header.e_shentsize, SEEK_SET);
    fread(&sectHdr, 1, sizeof(sectHdr), elfFile);
    sectNames = malloc(sectHdr.sh_size);
    fseek(elfFile, sectHdr.sh_offset, SEEK_SET);
    fread(sectNames, 1, sectHdr.sh_size, elfFile);

    // main loop
    for (int i = 0; i < header.e_shnum; i++)
    {
        char *sh_name = "";

        // Lecture de la sectHdr de la table du fichier
        fseek(elfFile, header.e_shoff + i * sizeof(sectHdr), SEEK_SET);
        fread(&sectHdr, 1, sizeof(sectHdr), elfFile);
        //
        
        sh_name = sectNames + sectHdr.sh_name;

        if (find_type_symbole(sectHdr.sh_type) == DYNSYM){
            uint64_t entrees =  (sectHdr.sh_size / sectHdr.sh_entsize);
            printf("Symbol table '%s' contains %lu entries:\n", sh_name, entrees);
            affichage_dynsym(elfFile, sectHdr.sh_offset, entrees);
            printf("\n");
        }
        else if (find_type_symbole(sectHdr.sh_type) == SYMTAB){
            uint64_t entrees =  (sectHdr.sh_size / sectHdr.sh_entsize);
            printf("Symbol table '%s' contains %lu entries:\n", sh_name, entrees);
            affichage_symtab(elfFile, sectHdr.sh_offset, entrees);
            printf("\n");
        }
        else{
            continue;
        }   
    }

    free(sectNames);
}
