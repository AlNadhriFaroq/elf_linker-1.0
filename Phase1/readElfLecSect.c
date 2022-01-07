#include <ctype.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readElfSecTable.h"
#include "readElfHeader.h"
#include "readElfLecSect.h"

/* Fichier principal de l'etape 3 : affichage du contenu d'une section */

/*	affichage_section_contenu(FILE *elfFile, Elf64_Ehdr header, Elf64_Shdr sectHdr, int section)
		Pour la section choisie, affiche son contenu en hexadecimal et l'adresse des octets lus
*/
void affichage_section_contenu(FILE *elfFile, Elf32_Ehdr header, Elf32_Shdr sectHdr, int section)
{
	// tab est la variable lecteur de l'octet courant
	uint8_t tab;
	int j = 1;

	// Placement du curseur au debut de la section determinee par le parametre section
	fseek(elfFile, sectHdr.sh_offset, SEEK_SET);

	// Affichage de l'adresse du debut du contenu de la section
	printf("  0x%08x ", sectHdr.sh_addr);

	// Parcours des octets du contenu de la section
	for(int i = 0; i < sectHdr.sh_size; i++)
	{
		// Affichage de l'octet courant
		fread(&tab, 1, sizeof(tab), elfFile);
		printf("%02x", tab);

		// Affichage d'un espace tous les quatre octets
		if((i+1) % 4 == 0 && !((i+1) % 16 == 0))
		{
			printf(" ");
		}

		// Affichage d'un saut de ligne tous les 16 octets et de l'adresse de l'octet courant
		if (((i+1) % 16 == 0) && (i+1 != sectHdr.sh_size))
		{
			printf("\n  0x%08x ", sectHdr.sh_addr + 16 * j);
			j++;
		}
	}
	printf("\n\n");
}

/*	affiche_section(FILE *elfFile, Elf64_Ehdr header, char *section)
		Fonction d'affichage d'une section en fonction des parametres donnes par l'utilisateur
*/
void affiche_section(FILE *elfFile, Elf32_Ehdr header, char *section)
{
	int number = 1, i = 0, sec;
	Elf32_Shdr sectHdr;
	char *sectNames = NULL;
	char *sh_name = "";

	// Placement du curseur au debut de la premiere section
	fseek(elfFile, header.e_shoff + header.e_shstrndx * header.e_shentsize, SEEK_SET);

	// Lecture de l'en-tete de la premiere section
	fread(&sectHdr, 1, sizeof(sectHdr), elfFile);

	// Allocation dans sectNames du nom de la premiere section
	sectNames = malloc(sectHdr.sh_size);
	fseek(elfFile, sectHdr.sh_offset, SEEK_SET);
	fread(sectNames, 1, sectHdr.sh_size, elfFile);

	// Test verifiant si le contenu du parametre section est une valeur numerique ou textuelle
	for ( int i = 0; i< strlen(section); i++)
	{
		if (!isdigit(section[i]))
		{
			number = 0;
		}
	}

	// Utilisation du booleen number selon que l'utilisateur entre le numero de la section ou son nom
	switch (number)
	{

	// cas ou l'utilisateur rentre la valeur numerique de la section    
	case 1:

		sec = atoi(section);
        
		// Si la valeur de section est trop grande alors on renvoie une erreur
		if (sec >= header.e_shnum)
		{
			printf("readelf: Warning: Section %d was not dumped because it does not exist!\n", sec);
			exit(0);
		}

		// Placement du curseur de lecture a la section courante
		fseek(elfFile, header.e_shoff + sec * sizeof(sectHdr), SEEK_SET);
		fread(&sectHdr, 1, sizeof(sectHdr), elfFile);
		sh_name = sectNames + sectHdr.sh_name;

		// Affichage de la section
		printf("\nHex dump of section '%s':\n", sh_name);
		affichage_section_contenu(elfFile, header, sectHdr, sec);
		break;

	// cas ou l'utilisateur rentre la valeur textuelle de la section
	default:

		// Parcours de toutes les sections
		while(i < header.e_shnum)
		{
			// Placement du curseur de lecture a la section courante
			char *sh_name = "";
			fseek(elfFile, header.e_shoff + i * sizeof(sectHdr), SEEK_SET);
			fread(&sectHdr, 1, sizeof(sectHdr), elfFile);
			sh_name = sectNames + sectHdr.sh_name;

			// Comparaison entre le nom de la section courante et le nom de section donne par l'utilisateur
			if(strcmp(section, sh_name) == 0)
			{
				// Affichage de la section
				printf("\nHex dump of section '%s':\n", sh_name);
				affichage_section_contenu(elfFile, header, sectHdr, i);
				break;
			}
			i++;
		}

		// Si le nom de la section n'existe pas alors on renvoie une erreur
		if(i == header.e_shnum )
		{
			printf("readelf: Warning: Section '%s' was not dumped because it does not exist!\n", section);
			exit(0);
		}
		break;
	}

	free(sectNames);
}