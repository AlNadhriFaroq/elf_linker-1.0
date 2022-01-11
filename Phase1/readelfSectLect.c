#include <ctype.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readelfOptions.h"
#include "readelfHeader.h"
#include "readelfSectTable.h"
#include "readelfSectLect.h"

/* Fichier principal de l'etape 3 : affichage du contenu d'une section */


/*	strComp(const void *chaine1, const void *chaine2)
		Compare les deux chaines donnees
*/
static int strComp(const void *chaine1, const void *chaine2)
{
    return strcmp(* (char * const *) chaine1, * (char * const *) chaine2);
}


/*	adrComp(const void *chaine1, const void *chaine2)
		Compare les deux adresses des sections donnees
*/
static int adrComp(const void *S1, const void *S2)
{
	Section *sect1 = (Section *) S1;
	Section *sect2 = (Section *) S2;
    uint32_t adr1 = sect1->header.sh_offset;
	uint32_t adr2 = sect2->header.sh_offset;
	if (adr1 == adr2)
	{
		return 0;
	}
	if (adr1 > adr2)
	{
		return 1;
	}
	if (adr1 < adr2)
	{
		return -1;
	}
	return -2;
}


/*	trier_sections(Options *Opt, SectionsList liste)
		Renomme, trie et supprime les doublons de Opt.sectList
*/
void trier_sections(Options *opt, SectionsList liste)
{
	/* Ronnomage des sections pour toutes les identifier par leur numero */
	
	int i = 0;
	// Parcours des sections donnees en entree du programme
	while (i < opt->nb_sect)
	{
		int is_number = 1;
		
		// Parcours des caracteres de la section courante
		for (int j = 0; j < strlen(opt->sectList[i]); j++)
		{
			if (!isdigit(opt->sectList[i][j]))
			{
				is_number = 0;
			}
		}
		
		// Cas ou la section courante est definie par son nom
		if (!is_number)
		{
			int j = 0;
			
			// Parcours de toutes les sections reelles
			while(j < liste.nb_sect)
			{
				// Comparaison entre les noms de sections
				if (strcmp(opt->sectList[i], liste.sectTab[j].name) == 0)
				{
					// Remplace le nom de la section courante par son numero
					snprintf(opt->sectList[i], 3, "%d", j);
					break;
				}
				j++;
			}
			
			// Cas ou le nom de la section n'existe pas, renvoie une erreur
			if(j == liste.nb_sect)
			{
				printf("readelf: Warning: Section '%s' was not dumped because it does not exist!\n", opt->sectList[i]);
				
				// Suppression de cette section dans la liste des sections a afficher
				for (int k = i; k < opt->nb_sect-1; k++)
				{
					opt->sectList[k] = opt->sectList[k+1];
				}
				opt->nb_sect--;
			}
			// Cas ou le nom de la section existe, rien a faire
			else
			{
				i++;
			}
		}
		// Cas ou la section courante est definie par son numero, rien a faire
		else
		{
			i++;
		}
	}
	
	/* Tri dans l'ordre croissant les numeros des sections */
	
	qsort(opt->sectList, opt->nb_sect, sizeof(char *), strComp);
	
	/* Suppression des sections en double */
	
	i = 1;
	// Parcours des sections donnees en entree du programme
	while (i < opt->nb_sect)
	{
		// Comparaison des numeros de la section courante avec la precedante
		if (strcmp(opt->sectList[i-1], opt->sectList[i]) == 0)
		{
			// Suppression de cette section dans la liste des sections a afficher
			for (int j = i; j < opt->nb_sect-1; j++)
			{
				opt->sectList[j] = opt->sectList[j+1];
			}
			opt->nb_sect--;
		}
		else
		{
			i++;
		}
	}
}


/*	lire_section(FILE *elfFile, Section sect)
		Lit le contenu d'une section
*/
void lire_section(FILE *elfFile, Section sect)
{
	// Placement du curseur au debut de la section
	fseek(elfFile, sect.header.sh_offset, SEEK_SET);

	// Lecture du contenu de la section
	fread(sect.dataTab, 1, sect.header.sh_size, elfFile);
}


/*	lire_sections(FILE *elfFile, SectionsList liste)
		Lit le contenu d'une liste de sections
*/
void lire_sections(FILE *elfFile, SectionsList liste)
{
	// Parcours des sections
	for (int i = 0; i < liste.nb_sect; i++)
	{
		lire_section(elfFile, liste.sectTab[i]);
	}
}


/*	afficher_section(Section sect)
		Affiche le contenu d'une section
*/
void afficher_section(Section sect)
{
	// Cas ou le contenu de la section est vide
	if (sect.header.sh_size == 0)
	{
		printf("Section '%s' has no data to dump.\n", sect.name);
	}
	
	// Cas ou le contenu de la section n'est pas vide
	else
	{
		// Affichage du nom de la section
		printf("\nHex dump of section '%s':\n", sect.name);
		
		// Decompte du nombre de ligne affichee
		int j = 1;

		// Affichage de l'adresse du debut du contenu de la section
		printf("  0x%08x ", sect.header.sh_addr);

		// Parcours des octets du contenu de la section
		for (int i = 0; i < sect.header.sh_size; i++)
		{
			// Affichage de l'octet courant
			printf("%02x", sect.dataTab[i]);

			// Affichage d'un espace tous les quatre octets
			if((i+1) % 4 == 0 && !((i+1) % 16 == 0))
			{
				printf(" ");
			}

			// Affichage d'un saut de ligne tous les 16 octets et de l'adresse de l'octet courant
			if (((i+1) % 16 == 0) && (i+1 != sect.header.sh_size))
			{
				printf("\n  0x%08x ", sect.header.sh_addr + 16 * j);
				j++;
			}
		}
		printf("\n\n");
	}
}


/*	afficher_sections(SectionsList liste_sections, Options Opt)
		Affiche le contenu d'une liste de sections
*/
void afficher_sections(SectionsList liste, Options opt)
{
	// parcours des sections demandees en entree du programme
	for (int i = 0; i < opt.nb_sect; i++)
	{
		// Recuperation du numero de la section courante
		int num_sect = atoi(opt.sectList[i]);
		
		// Cas ou cette section existe, affichage de son contenu
		if (num_sect < liste.nb_sect)
		{
			afficher_section(liste.sectTab[num_sect]);
		}
		// Cas ou cette section n'existe pas, renvoie une erreur
		else
		{
			printf("readelf: Warning: Section '%s' was not dumped because it does not exist!\n", opt.sectList[i]);
		}
	}
}


/*	ecrire_section(FILE *outFile, Section sect)
		Ecrit le contenu d'une section dans le fichier
*/
void ecrire_section(FILE *outFile, Section sect)
{
	fwrite(sect.dataTab, 1, sect.header.sh_size, outFile);
}


/*	ecrire_sections(FILE *outFile, SectionsList liste_sections, uint32_t e_shoff)
		Ecrit le contenu d'une liste de sections dans le fichier
*/
void ecrire_sections(FILE *outFile, SectionsList liste_sections, uint32_t e_shoff)
{
	// Copie de la liste des sections pour pouvoir la changer sans changer l'original
	SectionsList liste = liste_sections;
	liste.sectTab = malloc(sizeof(Section) * liste.nb_sect);
	liste.sectTab = memcpy(liste.sectTab, liste_sections.sectTab, sizeof(Section)*liste_sections.nb_sect);
	
	// Tri des sections dans l'ordre des adresses croissantes
	qsort(liste.sectTab, liste.nb_sect, sizeof(Section), adrComp);
	
	// Parcours des sections
	for (int i = 1; i < liste.nb_sect; i++)
	{
		// Ecriture du contenu de la section courante
		ecrire_section(outFile, liste.sectTab[i]);
		
		// Calcul de l'ecart avec la section suivante
		int ecart = 0;
		if (i+1 < liste.nb_sect)
		{
			ecart = liste.sectTab[i+1].header.sh_offset -(liste.sectTab[i].header.sh_offset + liste.sectTab[i].header.sh_size);
		}
		else if (i+1 == liste.nb_sect)
		{
			ecart = e_shoff -(liste.sectTab[i].header.sh_offset + liste.sectTab[i].header.sh_size);
		}
		
		// Cas ou il y a un ecart
		if (ecart != 0)
		{
			// Remplissage de cette ecart
			for (int j = 0; j < ecart; j++)
			{
				char *c = 0x0;
				fwrite(&c, 1, sizeof(char), outFile);
			}
		}
	}
	
	free(liste.sectTab);
}
