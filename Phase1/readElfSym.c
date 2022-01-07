#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readElfSym.h"

void find_type_sym(uint32_t value, char *type)
{
	switch (value)
	{
	case 0:
		strcpy(type, "NOTYPE");
		break;
	case 1:
		strcpy(type, "OBJECT");
		break;
	case 2:
		strcpy(type, "FUNC");
		break;
	case 3:
		strcpy(type, "SECTION");
		break;
	case 4:
		strcpy(type, "FILE");
		break;
	case 5:
		strcpy(type, "COMMON");
		break;
	case 6:
		strcpy(type, "TLS");
		break;
	case 10:
		strcpy(type, "LOOS");
		break;   
	case 12:
		strcpy(type, "HIOS");
		break;
	case 13:
		strcpy(type, "LOPROC");
		break;
	case 15:
		strcpy(type, "HIPROC");
		break;
	default:
		break;
	}
}


void affichage_dynsym(FILE *elfFile, uint64_t offset, uint64_t entrees, uint64_t taille_entree)
{
	fseek(elfFile, offset, SEEK_SET);
	Elf64_Sym sym;
	printf("   Num:	Value		  Size Type	Bind   Vis	  Ndx Name\n");
	for (int i = 0; i < entrees; i++)
	{
		fread(&sym, 1, sizeof(sym), elfFile);
		printf("	%d:   %016lx	 %lu	   %d	   %d		%lu \n", i, sym.st_value, sym.st_size, ELF32_ST_TYPE(sym.st_info), ELF32_ST_BIND(sym.st_info), sym.st_name);
	}
}


void affichage_symtab(FILE *elfFile, uint64_t offset, uint64_t entrees, uint64_t taille_entree)
{
	assert(fseek(elfFile, offset, SEEK_SET) == 0);
	printf("l'adresse de la table des symboles est %lx\n", offset);
	Elf32_Sym sym;
	int tmp;
	printf("   Num:	Value			   Size	 Type	Bind	   Ndx Name\n");
	for (int i = 0; i < entrees; i++)
	{
		char type[10];
		assert(fread(&tmp, 1, taille_entree - sizeof(sym), elfFile) != 0);
		assert(fread(&sym, 1, sizeof(sym), elfFile) != 0);
		find_type_sym(ELF32_ST_TYPE(sym.st_name), type);
		printf("	%2d:   %016x	 %u	   %s	   %x		%x \n", i, sym.st_value, sym.st_size, type, ELF32_ST_BIND(sym.st_name), sym.st_name);
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

		if (find_type_symbole(sectHdr.sh_type) == DYNSYM)
		{
			uint64_t entrees =  (sectHdr.sh_size / sectHdr.sh_entsize);
			printf("Symbol table '%s' contains %lu entries:\n", sh_name, entrees);
			affichage_dynsym(elfFile, sectHdr.sh_offset, entrees, sectHdr.sh_entsize);
			printf("\n");
		}
		else if (find_type_symbole(sectHdr.sh_type) == SYMTAB)
		{
			uint64_t entrees =  (sectHdr.sh_size / sectHdr.sh_entsize);
			printf("Symbol table '%s' contains %lu entries:\n", sh_name, entrees);
			affichage_symtab(elfFile, sectHdr.sh_offset, entrees, sectHdr.sh_entsize);
			printf("\n");
		}
		else
		{
			continue;
		}   
	}

	free(sectNames);
}
