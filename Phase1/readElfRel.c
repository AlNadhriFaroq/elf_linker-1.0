#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readElfRel.h"

/* Fichier principal de l'etape 5 : affichage des tables de reimplementation */

void affichage_rel(FILE *elfFile, uint32_t offset, uint32_t entrees)
{
	for (uint32_t i = 0; i < entrees; i++)
	{
		Elf32_Rel r;
		fread(&r, 1, sizeof(r), elfFile);
		printf("%x, %x\n", r.r_info, r.r_offset);
	}
}

void affichage_rela(FILE *elfFile, uint32_t offset, uint32_t entrees)
{
	Elf32_Rela r;
	fseek(elfFile, offset, SEEK_SET);
	for (uint32_t i = 0; i < entrees; i++)
	{
		//fseek(elfFile, offset + i * sizeof(r), SEEK_SET);
		fread(&r, 1, sizeof(r), elfFile);
		printf("%012x  %012x      type      %012d %012x + %x\n", r.r_offset, r.r_info, ELF32_R_TYPE(r.r_info), ELF32_R_SYM(r.r_info), r.r_addend);		
	}
}

/*
	find_type: trouve les sections REL ou RELA.
*/
TYPE find_type_reimplatation(int num)
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

void affiche_reimplantation_table(FILE *elfFile, Elf32_Ehdr header)
{
	Elf32_Shdr sectHdr;
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
		
		sh_name = sectNames + sectHdr.sh_name;
		if (find_type_reimplatation(sectHdr.sh_type) == REL)
		{
			uint32_t entrees =  (sectHdr.sh_size / sectHdr.sh_entsize);
			printf("REL\n");
			printf("Relocation section '%s' at offset 0x%x contains %d entries:\n", sh_name, sectHdr.sh_offset, entrees);
			printf("  Offset          Info           Type           Sym. Value    Sym. Name + Addend\n");
			affichage_rel(elfFile, sectHdr.sh_offset, entrees);
			printf("\n");
		}
		else if (find_type_reimplatation(sectHdr.sh_type) == RELA)
		{
			uint32_t entrees =  (sectHdr.sh_size / sectHdr.sh_entsize);
			printf("Relocation section '%s' at offset 0x%x contains %d entries:\n", sh_name, sectHdr.sh_offset, entrees);
			printf("  Offset          Info           Type           Sym. Value    Sym. Name + Addend\n");
			affichage_rela(elfFile, sectHdr.sh_offset, entrees);
			printf("\n");
		}
		else
		{
			continue;
		}   
	}

	free(sectNames);
}
