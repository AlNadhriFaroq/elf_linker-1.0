#ifndef SYMB_TABLE
#define SYMB_TABLE

#include <stdint.h>
#include "readelfSectTable.h"
#include "readelfHeader.h"

#define ELF32_ST_BIND(i) ((i) >> 4)
#define ELF32_ST_TYPE(i) ((i)&0xf)
#define ELF32_ST_INFO(b, t) (((b) << 4) + ((t)&0xf))
#define ELF32_ST_VISIBILITY(o) ((o)&0x3)

typedef enum
{
	DYNSYM,
	SYMTAB,
	STRTAB,
	OTHER_SYMB
} TYPE_SYMB;


typedef struct
{
	uint32_t st_name;
	uint32_t st_value;
	uint32_t st_size;
	unsigned char st_info;
	unsigned char st_other;
	uint16_t st_shndx;
} Elf32_Sym;

typedef struct
{
	int nb_symboles;
	Elf32_Sym *symb;
} table_sym;

TYPE_SYMB find_type_symbole(int num);

void find_visibility(int num,char *vis);

void affiche_symboles(FILE *elfFile, Elf32_Ehdr header);

void affichage_dynsym(FILE *elfFile, uint32_t offset, uint32_t entrees, uint32_t taille_entree);

void affichage_symtab(FILE *elfFile, uint32_t offset, uint32_t entrees, uint32_t taille_entree);

void affiche_table_sym(SectionsList liste, FILE *elfFile);

void find_name_symb(Section strtab, char *name, uint32_t debut_mot);

#endif