#include <stdio.h>
#include <stdlib.h>
#include "../Phase1/readelfHeader.h"
#include "../Phase1/readelfSectTable.h"
#include "../Phase1/readelfSectLect.h"
#include "../Phase1/readelfSymbTable.h"
#include "../Phase1/readelfReimpTable.h"

/* Creer une copie conforme d'un fichier elf */


static uint8_t *lire_programs_table(FILE *elfFile, Elf32_Ehdr header, SectionsList liste)
{
	uint8_t *programs_table = NULL;
	int size = liste.sectTab[1].header.sh_offset - header.e_ehsize;
	
	if (size != 0)
	{
		programs_table = malloc(size);
		fread(programs_table, 1, size, elfFile);
	}
	
	return programs_table;
}


static void ecrire_programs_table(FILE *outFile, uint8_t *programs_table)
{
	if (programs_table != NULL)
	{
		fwrite(programs_table, 1, sizeof(programs_table), outFile);
		free(programs_table);
	}
}


int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Usage: writeelf elf-file out-file\n");
		exit(1);
	}
	
	// Ouverture du fichier source duquel on va lire les donnees
	FILE *elfFile = fopen(argv[1], "rb");
	if (elfFile == NULL)
	{
		printf("writeelf: ERROR: '%s': File open error!\n", argv[1]);
		exit(1);
	}

	// Lecture de l'entete du fichier elfFile
	Elf32_Ehdr header = lire_entete(elfFile);

	// Verification que le fichier lu est bien un fichier ELF
	if (!est_fichier_elf(header))
	{
		printf("writeelf: Error: Not an ELF file - it has the wrong magic bytes at the start\n");
		fclose(elfFile);
		exit(1);
	}
	
	// Ouverture du fichier destination dans lequel on va ecrire les donnees
	FILE *outFile = fopen(argv[2], "wb");
	if (outFile == NULL)
	{
		printf("writeelf: ERROR: '%s': File open error!\n", argv[2]);
		fclose(elfFile);
		exit(1);
	}
	
	// Lecture du fichier elfFile
	SectionsList liste_sections = lire_sections_table(elfFile, header);
	lire_sections(elfFile, liste_sections);
	uint8_t *programs_table = lire_programs_table(elfFile, header, liste_sections);
	//struct = lire_symboles_table(elfFile, liste_sections); // etape 4
	//struct = lire_reimp_table(elfFile, liste_sections); // etape 5
	
	// Ecriture dans le fichier outFile
	//ecrire_symb_tables(outFile, struct); // etape 4
	//ecrire_reimp_table(outFile, struct); // etape 5
	ecrire_entete(outFile, header);
	ecrire_programs_table(outFile, programs_table);
	ecrire_sections(outFile, liste_sections, header.e_shoff);
	ecrire_sections_table(outFile, liste_sections);
	
	supprimer_sections_table(liste_sections);
	fclose(elfFile);
	fclose(outFile);
	
	return 0;
}
