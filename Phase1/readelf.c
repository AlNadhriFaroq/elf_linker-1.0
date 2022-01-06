#include <stdio.h>
#include <stdlib.h>
#include "readElfHeader.h"
#include "readElfSecTable.h"
#include "readElfLecSect.h"
#include "readElfRel.h"

void help()
{
	printf("Usage: readelf <option> elf-file(s)\n");
	printf(" Display information about the contents of ELF format files\n");
	printf(" Options are:\n");
	printf("  -a                 Equivalent to: -h -S -s -r\n");
	printf("  -h                 Display the ELF file header\n");
	printf("  -S                 Display the sections' header\n");
	printf("  -s                 Display the symbol table\n");
	printf("  -r                 Display the relocations (if present)\n");
	printf("  -x <number|name>   Dump the contents of section <number|name> as bytes\n");
	printf("  -H                 Display this information\n");
	exit(1);
}

int main(int argc, char *argv[])
{
	FILE *elfFile;

	// Ouverture du fichier ELF en fonction des parametres entrees
	if (argc == 3 && argv[1][1] != 'x' && argv[1][2] == '\0')
	{
		elfFile = fopen(argv[2], "rb");
	}
	else if (argc == 4 && argv[1][1] == 'x' && argv[1][2] == '\0')
	{
		elfFile = fopen(argv[3], "rb");
	}
	else
	{
		help();
	}

	if (elfFile == NULL)
	{
		printf("readelf: Error: File open error!\n");
		exit(1);
	}

	// Lecture de l'entete du fichier
	Elf64_Ehdr header;
	fread(&header, 1, sizeof(header), elfFile); 

	// Verification que le fichier lu est bien un fichier ELF
	if (!(header.e_ident[0] == 0x7f && header.e_ident[1] == 'E' &&
		header.e_ident[2] == 'L' && header.e_ident[3] == 'F'))
	{
		printf("readelf: Error: Not an ELF file - it has the wrong magic bytes at the start\n");
		fclose(elfFile);
		exit(1);
	}

	// Execution de la fonction correspondant a l'option entree en parametre
	switch (argv[1][1])
	{
	case 'a':
		affiche_header(header);
		affiche_section_table(elfFile, header);
		printf("affiche_symbol_table\n");
		affiche_reimplantation_table(elfFile, header);
		break;
	case 'h':
		affiche_header(header);
		break;
	case 'S':
		affiche_section_table(elfFile, header);
		break;
	case 'x':
		affiche_section(elfFile, header, argv[2]);
		break;
	case 's':
		affiche_symboles(elfFile, header);
		break;
	case 'r':
		affiche_reimplantation_table(elfFile, header);
		break;
	default:
		fclose(elfFile);
		printf("readelf: Error: Unknown flag %s\n", argv[1]);
		help();
	}

	fclose(elfFile);
	return 0;
}
