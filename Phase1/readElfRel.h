#ifndef __REL_H__
#define __REL_H__

#include<stdint.h>
#include "readElfSecTable.h"
#include "readElfHeader.h"

#define ELF32_R_SYM(i)	((i)>>8)
#define ELF32_R_TYPE(i)   ((unsigned char)(i))
#define ELF32_R_INFO(s,t) (((s)<<8)+(unsigned char)(t))

#define ELF64_R_SYM(i)    ((i)>>32)
#define ELF64_R_TYPE(i)   ((i)&0xffffffffL)
#define ELF64_R_INFO(s,t) (((s)<<32)+((t)&0xffffffffL)

typedef enum {RELA, 
              REL,
              OTHER
}TYPE;

typedef struct {
	uint32_t r_offset;
	uint32_t r_info;   // Elf32_word
} Elf32_Rel;

typedef struct {
	uint32_t r_offset;
	uint32_t r_info;   // Elf32_word
	int32_t r_addend; // Elf32_Sword
} Elf32_Rela;

typedef struct {
	uint64_t r_offset;
	uint64_t r_info;   // Elf64_Xword
} Elf64_Rel;

typedef struct {
	uint64_t r_offset; 
	uint64_t r_info;   // Elf64_Xword
	uint64_t r_addend; // Elf64_Sxword
} Elf64_Rela;

TYPE find_type_reimplatation(long num);

void affiche_tables_entree_reimplantation(FILE *elfFile, Elf64_Ehdr header);

void affichage_rel(FILE *elfFile, uint64_t offset, uint64_t entrees);

void affichage_rela(FILE *elfFile, uint64_t offset, uint64_t entrees);

#endif