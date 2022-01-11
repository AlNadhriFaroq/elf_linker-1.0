#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readelfReimpTable.h"
#include "readelfSectTable.h"

// /* Fichier principal de l'etape 5 : affichage des tables de reimplementation */

/*
	find_type: trouve les Sections REL ou RELA.

*/

void find_type_relocation_table(int num,char *type){
	switch (num)
	{
	case 0:
		strcpy(type, "R_386_NONE");
		break;
	case 1:
		strcpy(type, "R_386_32");
		break;
	case 2:
		strcpy(type, "R_386_PC32");
		break;
	case 3:
		strcpy(type, "R_386_GOT32");
		break;
	case 4:
		strcpy(type, "R_386_PLT32");
		break;
	case 5:
		strcpy(type, "R_386_COPY");
		break;
	case 6:
		strcpy(type, "R_386_GLOB_DAT");
		break;
	case 7:
		strcpy(type, "R_386_JMP_SLOT");
		break;
	case 8:
		strcpy(type, "R_386_RELATIVE");
		break;
	case 9:
		strcpy(type, "R_386_GOTOFF");
		break;
	case 10:
		strcpy(type, "R_386_GOTPC");
		break;
	case 11:
		strcpy(type, "R_386_32PLT");
		break;
	case 20:
		strcpy(type, "R_386_16");
		break;
	case 21:
		strcpy(type, "R_386_PC16");
		break;
	case 22:
		strcpy(type, "R_386_8");
		break;
	case 23:
		strcpy(type, "R_386_PC8");
		break;
	case 38:
		strcpy(type, "R_386_SIZE32");
		break;
	default:
		strcpy(type, "UNKOWN");
	}
}
TYPE find_type_reimplantation(int num)
{
	switch (num)
	{
	case 4:
		return RELA;
	case 9:
		return REL;
	default:
		return OTHER;
	}
}

void affichage_rela2(Section rela, FILE *elfFile)
{
	Elf32_Rela r;
	uint32_t neg;
	char type[30];
	int nb_reloc = rela.header.sh_size / rela.header.sh_entsize;
	fseek(elfFile, rela.header.sh_offset, SEEK_SET);
	printf("Relocation Section '%s' at offset 0x6d8 contains %d entries:\n", rela.name, nb_reloc);
	printf("Offset             Info           Type           Sym. index          + Addend \n");
	for (int i = 0; i < nb_reloc; i++)
	{
		fread(&r, sizeof(r), 1, elfFile);
		find_type_relocation_table(ELF32_R_TYPE(r.r_info), type);
		if (r.r_addend & ((uint32_t)1 << 31))
		{
			neg = -r.r_addend;
			printf("%06x    %06x      %-15s  %08x    - %x\n", r.r_offset, r.r_info, type, ELF32_R_SYM(r.r_info), neg);
		}
		else
		{
			neg = r.r_addend;
			printf("%06x    %06x      %-15s  %08x     + %x\n", r.r_offset, r.r_info, type, ELF32_R_SYM(r.r_info), neg);
		}
	}
	printf("\n");
}

void affichage_rel2(Section rel, FILE *elfFile)
{
	Elf32_Rel r;
	char type[30];
	int nb_reloc = rel.header.sh_size / rel.header.sh_entsize;
	fseek(elfFile, rel.header.sh_offset, SEEK_SET);
	printf("Relocation section '%s' at offset 0x6d8 contains %d entries:\n", rel.name, nb_reloc);
	printf(" Offset     Info    Type            Sym.Value  Sym. Name\n");
	for (int i = 0; i < nb_reloc; i++)
	{
		fread(&r, sizeof(r), 1, elfFile);
		find_type_relocation_table(ELF32_R_TYPE(r.r_info), type);
		printf(" %08x  %08x  %-15s %08x \n", r.r_offset, r.r_info, type, ELF32_R_SYM(r.r_info));
	}
	printf("\n");
}

void affiche_reimplantation_table(SectionsList liste, FILE *elfFile, uint16_t type_file)
{
	Section rela, rel;

	for (int i = 0; i < liste.nb_sect; i++)
	{
		if (find_type_reimplantation(liste.sectTab[i].header.sh_type) == RELA)
		{
			rela = liste.sectTab[i];
			affichage_rela2(rela, elfFile);
		}
		else if (find_type_reimplantation(liste.sectTab[i].header.sh_type) == REL)
		{
			rel = liste.sectTab[i];
			affichage_rel2(rel, elfFile);
		}
		else
		{
			continue;
		}
	}
}