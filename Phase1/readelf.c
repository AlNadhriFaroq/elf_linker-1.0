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
	// Lecture et interpretation des options entrees
	options Opt = read_options(argc, argv);
	liste_sections liste_sec;

	// Parcours des fichiers a afficher
	for (int i = 0 ; i < Opt.nb_file ; i++)
	{
		// Ouverture du fichier courant
		FILE *elfFile;
		elfFile = fopen(Opt.fileList[i], "rb");
		
		// Cas ou le fichier presente une erreur
		if (elfFile == NULL)
		{
			printf("readelf: ERROR: «%s»: File open error!\n", Opt.fileList[i]);
		}
		
		// Cas ou le fichier s'est ouvert normalement
		else
		{
			// Lecture de l'entete du fichier
			Elf32_Ehdr header;
			fread(&header, 1, sizeof(header), elfFile); 

			// Verification que le fichier lu est bien un fichier ELF
			if (!(header.e_ident[0] == 0x7f && header.e_ident[1] == 'E' &&
				header.e_ident[2] == 'L' && header.e_ident[3] == 'F'))
			{
				printf("readelf: Error: Not an ELF file - it has the wrong magic bytes at the start\n");
				fclose(elfFile);
				exit(1);
			}

			liste_sec = lecture_section_table(elfFile, header);
			// Execution des fonctions correspondants aux options entrees en parametre
			if (Opt.h)
			{
				affiche_header(header);
			}
			else if (Opt.S)
			{
				affiche_section_table(liste_sec, header.e_shoff);
			}
			else if (Opt.s)
			{
				affiche_symboles(elfFile, header);
			}
			else if (Opt.r)
			{
				affiche_reimplantation_table(elfFile, header);
			}
			else if (Opt.nb_sec > 0)
			{
				// Parcours des sections a afficher
				for (int s = 0 ; s < Opt.nb_sec ; s++)
				{
					affiche_section(elfFile, header, Opt.secList[s]);
				}
			}

			fclose(elfFile);
		}
	}
	
	return 0;
}
