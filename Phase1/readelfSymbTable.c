#include "readelfSymbTable.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void find_ndx_symb(uint16_t ndx, char *name)
{
	switch (ndx)
	{
	case 0:
		strcpy(name, "UND");
		break;
	case 65521:
		strcpy(name, "ABS");
		break;
	default:
		sprintf(name, "%d", ndx);
		break;
	}
}

void find_bind_symb(int number, char *name)
{
	switch (number)
	{
	case 0:
		strcpy(name, "LOCAL");
		break;
	case 1:
		strcpy(name, "GLOBAL");
		break;
	case 2:
		strcpy(name, "WEAK");
		break;
	case 10:
		strcpy(name, "LOOS");
		break;
	case 12:
		strcpy(name, "HIOS");
		break;
	case 13:
		strcpy(name, "LOPROC");
		break;
	case 15:
		strcpy(name, "HIPROC");
		break;

	default:
		break;
	}
}

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
		strcpy(type, "Section");
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

TYPE_SYMB find_type_symbole(int num)
{
	switch (num)
	{
	case 2:
		return SYMTAB;
	case 11:
		return DYNSYM;
	case 3:
		return STRTAB;
	default:
		return OTHER_SYMB;
	}
}

void find_name_symb(Section strtab, char *name, uint32_t debut_mot)
{
	int i = 0;

	while (strtab.dataTab [debut_mot] != 0)
	{
		name[i] = strtab.dataTab [debut_mot];
		debut_mot++;
		i++;
	}
	name[i] = '\0';
}

void affiche_sym(Section strtab, Section symtab, Section dynsym, Section dynstr, uint8_t nb, FILE *elfFile)
{
	char type[10];
	char bind[10];
	char name[50];
	char ndx[16];
	Elf32_Sym sym;
	if (nb >= 1)
	{
		// printf("%lx\n", symtab.sec.sh_offset);
		fseek(elfFile, symtab.header.sh_offset, SEEK_SET);
		uint32_t nb_symbole = symtab.header.sh_size / symtab.header.sh_entsize;
		printf("Symbol table '%s' contains %d entries:\n", symtab.name, nb_symbole);
		printf("   Num:    Value               Size                   Type            Bind       Ndx Name\n");
		for (int i = 0; i < nb_symbole; i++)
		{
			fread(&sym, 1, sizeof(sym), elfFile);
			find_type_sym(ELF32_ST_TYPE(sym.st_info), type);
			find_bind_symb(ELF32_ST_BIND(sym.st_info), bind);
			find_name_symb(strtab, name, sym.st_name);
			find_ndx_symb(sym.st_shndx, ndx);
			printf("    %3d:   %08x      %-8d             %-8s       %-8s      %s   %s \n", i, sym.st_value, sym.st_size, type, bind, ndx, name);
		}
	}

	printf("\n");

	if (nb > 1)
	{
		fseek(elfFile, dynsym.header.sh_offset, SEEK_SET);
		uint32_t nb_symbole = dynsym.header.sh_size / dynsym.header.sh_entsize;
		printf("Symbol table '%s' contains %d entries:\n", dynsym.name, nb_symbole);
		printf("   Num:    Value               Size                   Type            Bind       Ndx Name\n");
		for (int i = 0; i < nb_symbole; i++)
		{
			fread(&sym, 1, sizeof(sym), elfFile);
			find_type_sym(ELF32_ST_TYPE(sym.st_info), type);
			find_bind_symb(ELF32_ST_BIND(sym.st_info), bind);
			find_name_symb(dynstr, name, sym.st_name);
			find_ndx_symb(sym.st_shndx, ndx);
			printf("    %3d:   %08x      %-8d             %-8s       %-8s      %s   %s \n", i, sym.st_value, sym.st_size, type, bind, ndx, name);
		}
	}
}

void affiche_table_sym(SectionsList liste, FILE *elfFile)
{
	Section strtab, symtab, dynsym, dynstr;
	uint8_t nb = 0;
	for (int i = 0; i < liste.nb_sect; i++)
	{
		if (find_type_symbole(liste.sectTab[i].header.sh_type) == DYNSYM)
		{
			dynsym = liste.sectTab[i];
			nb++;
		}
		else if (find_type_symbole(liste.sectTab[i].header.sh_type) == SYMTAB)
		{
			symtab = liste.sectTab[i];
			nb++;
		}
		else if (strcmp(liste.sectTab[i].name, ".strtab") == 0)
		{
			strtab = liste.sectTab[i];
		}
		else if (strcmp(liste.sectTab[i].name, ".dynstr") == 0)
		{
			dynstr = liste.sectTab[i];
		}
		else
		{
			continue;
		}
	}
	affiche_sym(strtab, symtab, dynsym, dynstr, nb, elfFile);
}
