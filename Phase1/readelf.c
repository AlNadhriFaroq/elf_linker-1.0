#include <stdio.h>
#include <stdlib.h>
#include "readelfOptions.h"
#include "readelfHeader.h"
#include "readelfSectTable.h"
#include "readelfSectLect.h"
#include "readelfSymbTable.h"
#include "readelfReimpTable.h"

int main(int argc, char *argv[])
{
	// Lecture et interpretation des options entrees
	Options opt = lire_options(argc, argv);

	// Parcours des fichiers a afficher
	for (int i = 0 ; i < opt.nb_file ; i++)
	{
		// Ouverture du fichier courant
		FILE *elfFile;
		elfFile = fopen(opt.fileList[i], "rb");
		
		// Cas ou le fichier presente une erreur
		if (elfFile == NULL)
		{
			printf("readelf: ERROR: «%s»: File open error!\n", opt.fileList[i]);
		}
		
		// Cas ou le fichier s'est ouvert normalement
		else
		{
			// Lecture de l'entete du fichier
			Elf32_Ehdr header = lire_entete(elfFile);

			// Verification que le fichier lu est bien un fichier ELF
			if (!est_fichier_elf(header))
			{
				printf("readelf: Error: Not an ELF file - it has the wrong magic bytes at the start\n");
				fclose(elfFile);
				exit(1);
			}
			
			// Affichage de l'en-tete si l'option '-h' est activee
			if (opt.h)
			{
				afficher_entete(header);
			}
			
			// Lecture du fichier
			SectionsList liste_sections = lire_sections_table(elfFile, header);
			lire_sections(elfFile, liste_sections);
			SymbolesList liste_symboles = lire_symboles_table(liste_sections);
			RelocTable table_reimp = lire_reimp_table(liste_sections, liste_symboles);
			
			// Affichage de la table des sections si l'option '-S' est activee
			if (opt.S)
			{
				afficher_sections_table(liste_sections, header.e_shoff);
			}
			
			// Lecture et affichage de la table de reimplantation si l'option '-r' est activee
			if (opt.r)
			{
				afficher_reimp_table(table_reimp);
			}
			
			// Lecture et affichage de la table des symboles si l'option '-s' est activee
			if (opt.s)
			{
				afficher_symboles_table(liste_symboles);
			}
			
			// Affichage du contenu des sections si l'option '-x' est activee
			if (opt.nb_sect > 0)
			{
				// Renommage, tri et suppression des doublons de Opt.sectList
				trier_sections(&opt, liste_sections);	
				
				// Affichage du contenu des sections voulues
				afficher_sections(liste_sections, opt);
			}
			
			supprimer_sections_table(liste_sections);
			supprimer_symboles_table(liste_symboles);
			supprimer_reimp_table(table_reimp);
			fclose(elfFile);
		}
	}
	
	return 0;
}
