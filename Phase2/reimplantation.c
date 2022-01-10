#include <stdio.h>
#include <stdlib.h>
#include "../Phase1/readelfHeader.h"
#include "../Phase1/readelfSectTable.h"
#include "../Phase1/readelfSectLect.h"
#include "../Phase1/readelfSymbTable.h"
#include "../Phase1/readelfReimpTable.h"
#include "reimpRenumSect.h"
#include "reimpCorrectSymb.h"
#include "reimpTypeAbs.h"
#include "reimpTypeJumpCall.h"
//#include "interfaceSimul.h"
//#include "producExec.h"

int main(int argc, char *argv[])
{
	// Lecture et interpretation des options entrees
	if (argc != 2)
	{
		printf("Usage: reimplantation elf-file\n");
		exit(1);
	}

	FILE *elfFile = fopen(argv[1], "rb");
	if (elfFile == NULL)
	{
		printf("reimplantation: ERROR: '%s': File open error!\n", argv[1]);
		exit(1);
	}
	
	// Lecture de l'en-tete du fichier
	Elf32_Ehdr header = lire_entete(elfFile);

	// Verification que le fichier lu est bien un fichier ELF
	if (!est_fichier_elf(header))
	{
		printf("reimplantation: Error: Not an ELF file - it has the wrong magic bytes at the start\n");
		fclose(elfFile);
		exit(1);
	}

	FILE *outFile = fopen("outFile", "wb");
	if (outFile == NULL)
	{
		printf("reimplantation: ERROR: Destination file open error!\n");
		exit(1);
	}
	
	// Lecture du fichier source elfFile
	SectionsList liste_sections = lire_sections_table(elfFile, header);
	lire_sections(elfFile, liste_sections);
	//struct = lire_symboles_table(elfFile, header); // etape 4
	//struct = lire_reimp_table(elfFile, header); // etape 5
	
	// Fonctions de modification des structures precedemment definies
	//renommer_sections(header, liste_sections); // etape 6
	//corriger_symboles(liste_sections); // etape 7
	//reimplanter_type_abs(header, liste_sections, struct); // etape 8
	//Reimplanter_type_jump_call(header, liste_sections, struct); // etape 9
	
	// Ecriture dans le fichier de destination outFile
	//ecrire_symb_table(liste_sections, struct); // etape 4
	//ecrire_reimp_table(liste_sections, struct); // etape 5
	ecrire_entete(outFile, header);
	// ecrire la table des programmes (ce qu'il y a entre la fin de l'en-tete et le debut du contenu de la premiere section
	ecrire_sections(outFile, liste_sections);
	ecrire_sections_table(outFile, liste_sections);
	
	supprimer_sections_table(liste_sections);
	fclose(elfFile);
	fclose(outFile);
	// ici : appels aux modules des etapes 10 et 11
	return 0;
}
