#ifndef __SECTION_H__
#define __SECTION_H__

#include <stdio.h>
#include "readElfHeader.h"

// Documentation chapitre 1-10 Figure 1-8
typedef struct {
	uint32_t sh_name;
	uint32_t sh_type;
	uint64_t sh_flags;
	uint64_t sh_addr;
	uint64_t sh_offset;
	uint64_t sh_size;
	uint32_t sh_link;
	uint32_t sh_info;
	uint64_t sh_addralign;
	uint64_t sh_entsize;
} Elf64_Shdr;

// Structure utilisee pour recapituler le contenu d'une section, en y ajoutant le nom, le type et les flags
// Utilisee pour l'etape 2, puis l'etape 3 et 4
typedef struct{
	char name[20];
	char type[25];
	char flag[6];
	uint8_t *tabDonnee;
	Elf64_Shdr sec;
}section;

// Structure utilisee pour generer une liste des sections a partir de la struct section
// Utilisee pour l'etape 2, puis 3 et 4
typedef struct{
	uint64_t nb_sections;
	section *tab;
}liste_sections;

/*******************************************************************
 * find_type
 * parametres : long num, char *sh_type
 * resultat : aucun
 * description : associe a sh_type la valeur textuelle correspondant
 *               a la valeur de num, le type d'une section.
 * effet de bord : remplissage de la valeur de sh_type
 *******************************************************************/
void find_type(long num, char *sh_type);

/****************************************************************
 * revstr
 * parametres : char *str
 * resultat : aucun
 * description : Transforme une chaine de caracteres donnee en sa
 *               valeur inverse/miroir (Exemple : noel -> leon).
 * effet de bord : modification du contenu de str
 ****************************************************************/
void revstr(char *str);

/********************************************************************************
 * find_flags
 * parametres : char *tab, int n
 * resultat : aucun
 * description : Dans une section, des instructions sont lues et effectuees.
 *               Des fanions sont associes a ces diferrents types d'actions.
 *               Cette fonction remplit le tableau tab avec la valeur des fanions.
 * effet de bord : remplissage de tab
 *********************************************************************************/
void find_flags(char *tab, int n);

/**********************************************************
 * affiche_section_table
 * parametres : FILE *elfFile, Elf64_Ehdr header
 * resultat : aucun
 * description : Affichage de la table des sections et
                 des informations sur chacune d'entre elles
 * effet de bord : aucun
 **********************************************************/
void affiche_section_table(liste_sections liste, uint64_t offset);

/**********************************************************
 * lecture_section_table
 * parametres : FILE *elfFile, Elf64_Ehdr header
 * resultat : liste_sections
 * description : Affichage de la table des sections et
                 des informations sur chacune d'entre elles
 * effet de bord : aucun
 **********************************************************/
liste_sections lecture_section_table(FILE *elfFile, Elf64_Ehdr header);

//void lecture_donnees_section(FILE *elfFile, Elf64_Ehdr header, liste_sections liste);

#endif