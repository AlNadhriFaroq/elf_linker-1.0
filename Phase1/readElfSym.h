#ifndef __SYMB_TABLE__
#define __SYMB_TABLE__

#include<stdint.h>
#include "readElfSecTable.h"
#include "readElfHeader.h"

#define ELF32_ST_BIND(i) ((i)>>4)
#define ELF32_ST_TYPE(i) ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

#define ELF64_ST_BIND(i)   ((i)>>4)
#define ELF64_ST_TYPE(i)   ((i)&0xf)
#define ELF64_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

typedef enum {DYNSYM, 
              SYMTAB,
              OTHER_SYMB
}TYPE_SYMB;

typedef enum {STB_LOCAL,
			  STB_GLOBAL,
              STB_WEAK,
              STB_LOOS,
			  STB_HIOS,
			  STB_LOPROC,
			  STB_HIPROC
}SYM_BINDING;


typedef struct {
	uint32_t st_name;
	uint32_t st_value;
	uint32_t st_size;
	unsigned char st_info;
	unsigned char st_other;
	uint32_t st_shndx;
} Elf32_Sym;

typedef struct {
	uint64_t	st_name;
	unsigned char	st_info;
	unsigned char	st_other;
	uint64_t	st_shndx;
	uint64_t	st_value;
	uint64_t	st_size;
} Elf64_Sym;

TYPE_SYMB find_type_symbole(long num);

void affiche_symboles(FILE *elfFile, Elf64_Ehdr header);

void affichage_dynsym(FILE *elfFile, uint64_t offset, uint64_t entrees, uint64_t taille_entree);

void affichage_symtab(FILE *elfFile, uint64_t offset, uint64_t entrees, uint64_t taille_entree);

#endif
