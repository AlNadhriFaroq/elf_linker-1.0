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
	char name[20];
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
 * revstr
 * parametres : char *str
 * resultat : aucun
 * description : Transforme une chaine de caracteres donnee en sa chaine
 *               inverse/miroir (Exemple : noel -> leon).
 * effet de bord : modification du contenu de str
 *******************************************************************************/
void revstr(char *str);


/*******************************************************************************
 * lire_type
 * parametres : long num, char *sh_type
 * resultat : aucun
 * description : Associe a sh_type la valeur textuelle correspondant a la valeur
 *               de num, le type d'une section.
 * effet de bord : remplissage de la valeur de sh_type
 *******************************************************************************/
void lire_type(long num, char *sh_type);


/*******************************************************************************
 * lire_flags
 * parametres : char *tab, int n
 * resultat : aucun
 * description : Remplit le tableau tab avec les flags de la section n.
 *               Ces flags correspondent aux differents type d'instructions lues
 *               et effectuees dans la section.
 * effet de bord : remplissage de tab
 *******************************************************************************/
void lire_flags(char *tab, int n);


/*******************************************************************************
 * lire_sections_table
 * parametres : FILE *elfFile, Elf64_Ehdr header
 * resultat : SectionsList
 * description : Lit dans le fichier elfFile les informations sur les sections
 *				 et les renvoie sous la forme d'une structure dediee.
 * effet de bord : aucun
 *******************************************************************************/
SectionsList lire_sections_table(FILE *elfFile, Elf32_Ehdr header);


/*******************************************************************************
 * afficher_sections_table
 * parametres : SectionsList liste, uint32_t offset
 * resultat : aucun
 * description : Affiche de facon lisible les informations contenu dans la
 *				 structures liste donnee.
 * effet de bord : aucun
 *******************************************************************************/
void afficher_sections_table(SectionsList liste, uint32_t offset);


/*******************************************************************************
 * supprimer_sections_table
 * parametres : SectionsList liste
 * resultat : aucun
 * description : Supprime et libere la memoire de la structure liste donnee.
 * effet de bord : Libere de l'espace en memoire 
 *******************************************************************************/
void supprimer_sections_table(SectionsList liste);

#endif