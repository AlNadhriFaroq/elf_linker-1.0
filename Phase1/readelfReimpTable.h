#ifndef __REL_H__
#define __REL_H__

#include <stdint.h>
#include "readelfSectTable.h"
#include "readelfHeader.h"
#include "readelfSymbTable.h"

#define ELF32_R_SYM(i) ((i) >> 16)
#define ELF32_R_TYPE(i) ((unsigned char)(i))
#define ELF32_R_INFO(s, t) (((s) << 8) + (unsigned char)(t))

typedef enum
{
	RELA,
	REL,
	OTHER
} TYPE;

typedef struct
{
	uint32_t r_offset;
	uint32_t r_info; // Elf32_word
} Elf32_Rel;

typedef struct
{
	uint32_t r_offset;
	uint32_t r_info;  // Elf32_word
	int32_t r_addend; // Elf32_Sword
} Elf32_Rela;

void find_type_relocation_table(int num,char *type);

TYPE find_type_reimplantation(int num);

// void affiche_reimplantation_table2(FILE *elfFile, Elf32_Ehdr header);

// void affichage_rel(FILE *elfFile, uint32_t offset, uint32_t entrees);

// void affichage_rela(FILE *elfFile, uint32_t offset, uint32_t entrees);

void affiche_reimplantation_table(SectionsList liste, FILE *elfFile, uint16_t type_file);

#endif