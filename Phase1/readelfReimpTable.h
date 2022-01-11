#ifndef __REL_H__
#define __REL_H__

#include <stdint.h>
#include "readelfHeader.h"
#include "readelfSectTable.h"
#include "readelfSymbTable.h"

#define ELF32_R_SYM(i) ((i) >> 16)
#define ELF32_R_TYPE(i) ((unsigned char)(i))
#define ELF32_R_INFO(s, t) (((s) << 8) + (unsigned char)(t))


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

typedef struct
{
	char type[30];
	Elf32_Rel rel;
} Rel;

typedef struct
{
	char type[30];
	Elf32_Rela rela;
} Rela;

typedef struct
{
	char name[30];
	uint32_t offset;
	int nb_reloc;
	int type; // 0=rel - 1=rela
	Rel *relTab;
	Rela *relaTab;
} RelocList;

typedef struct
{
	int nb_list;
	RelocList *tab;
} RelocTable;


Rela lire_rela(Section sect, int i);

Rel lire_rel(Section sect, int i);

RelocList lire_rela_liste(Section sect);

RelocList lire_rel_liste(Section sect);

RelocTable lire_reimp_table(SectionsList liste);

void afficher_reimp_table(RelocTable table_reimp);

// void ecrire_reimp_table(SectionList liste_sect, RelocTable table_reimp);

void supprimer_reimp_table(RelocTable table_reimp);

#endif