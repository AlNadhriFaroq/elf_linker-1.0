#include <stdio.h>
#include <stdlib.h>
#include "readElfHeader.h"
#include "readElfSecTable.h"
#include "readElfLecSect.h"
#include "readElfSym.h"
#include "readElfRel.h"
#include "options.h"

int main(int argc, char *argv[])
{
	// Lecture des options entrees
	options Opt = read_options(argc, argv);

	// Ouverture du premier fichier
	FILE *elfFile;
	elfFile = fopen(Opt.fileList[0], "rb");
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

	// Execution des fonctions correspondantes aux options entrees en parametre
	if (Opt.h)
	{
		affiche_header(header);
	}
	if (Opt.S)
	{
		affiche_section_table(elfFile, header);
	}
	if (Opt.s)
	{
		affiche_symboles(elfFile, header);
	}
	if (Opt.r)
	{
		affiche_reimplantation_table(elfFile, header);
	}
	if (Opt.nb_sec > 0)
	{
		for (int i = 0 ; i < Opt.nb_sec ; i++)
		{
			affiche_section(elfFile, header, Opt.secList[i]);
		}
	}

	fclose(elfFile);
	return 0;
}
