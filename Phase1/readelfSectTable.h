#ifndef __SECTION_H__
#define __SECTION_H__

#include <stdio.h>
#include "readelfHeader.h"


// Structure pour stocker l'en-tete d'une section. Documentation chapitre 1-10 Figure 1-8
typedef struct
{
	uint32_t sh_name;
	uint32_t sh_type;
	uint32_t sh_flags;
	uint32_t sh_addr;
	uint32_t sh_offset;
	uint32_t sh_size;
	uint32_t sh_link;
	uint32_t sh_info;
	uint32_t sh_addralign;
	uint32_t sh_entsize;
} Elf32_Shdr;

// Structure pour stocker les informations d'une section : nom, type, flags, donnees et en-tete
typedef struct
{
	char name[30];
	char type[25];
	char flag[6];
	uint8_t *dataTab;
	Elf32_Shdr header;
} Section;

// Structure pour stocker une liste de sections
typedef struct
{
	uint32_t nb_sect;
	Section *sectTab;
} SectionsList;


/*******************************************************************************
 * lire_sections_table
 * parametres : FILE *elfFile, Elf64_Ehdr header
 * resultat : SectionsList
 * description : Lit dans le fichier elfFile les informations sur les sections
 *               et les renvoie sous la forme d'une structure dediee.
 * effet de bord : aucun
 *******************************************************************************/
SectionsList lire_sections_table(FILE *elfFile, Elf32_Ehdr header);


/*******************************************************************************
 * afficher_sections_table
 * parametres : SectionsList liste, uint32_t offset
 * resultat : aucun
 * description : Affiche de facon lisible les informations contenu dans la
 *               structures liste donnee.
 * effet de bord : aucun
 *******************************************************************************/
void afficher_sections_table(SectionsList liste, uint32_t offset);


/*******************************************************************************
 * ecrire_sections_table
 * parametres : FILE *outFile, SectionsList liste
 * resultat : aucun
 * description : Ecrit dans le fichier outFile le contenu de la table des
 *               sections definie dans la structure header.
 * effet de bord : Ecriture dans le fichier
 *******************************************************************************/
void ecrire_sections_table(FILE *outFile, SectionsList liste);


/*******************************************************************************
 * supprimer_sections_table
 * parametres : SectionsList liste
 * resultat : aucun
 * description : Supprime et libere la memoire de la structure liste donnee.
 * effet de bord : Libere de l'espace en memoire 
 *******************************************************************************/
void supprimer_sections_table(SectionsList liste);

#endif