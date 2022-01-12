#ifndef __REL_H__
#define __REL_H__

#include <stdint.h>
#include "readelfHeader.h"
#include "readelfSectTable.h"
#include "readelfSymbTable.h"

#define ELF32_R_SYM(i)     ((i) >> 8)
#define ELF32_R_TYPE(i)    ((unsigned char)(i))
#define ELF32_R_INFO(s, t) (((s) << 8) + (unsigned char)(t))


// Structures pour stocker une reimplantation. Documentation chapitre 1-22 Figure 1-19
typedef struct
{
	uint32_t r_offset;
	uint32_t r_info; // Elf32_word
} Elf32_Rel;

typedef struct
{
	uint32_t r_offset;
	uint32_t r_info;  // Elf32_word
	int32_t r_addend; // Elf32_Sword
} Elf32_Rela;

// Structures pour stocker les informations d'une reimplantation : type, nom
typedef struct
{
	uint32_t value;
	char type[30];
	char name[50];
	Elf32_Rel rel;
} Rel;

typedef struct
{
	uint32_t value;
	char type[30];
	char name[30];
	Elf32_Rela rela;
} Rela;

// Structure pour stocker une liste de reimplantation
typedef struct
{
	char name[30];
	uint32_t offset;
	int nb_reloc;
	int type; // 0=rel - 1=rela
	Rel *relTab;
	Rela *relaTab;
} RelocList;

// Structure pour stocker une liste (table) de liste de reimplantation reimplantation
typedef struct
{
	int nb_list;
	RelocList *tab;
} RelocTable;


/*******************************************************************************
 * lire_rela_liste
 * parametres : Section sect, SectionsList liste_sect, SymbolesList liste_symb
 * resultat : RelocList
 * description : Lit dans la section sect une liste de reimplantation et la
 *               renvoie sous la forme d'une structure dediee.
 * effet de bord : aucun
 *******************************************************************************/
RelocList lire_rela_liste(Section sect, SectionsList liste_sect, SymbolesList liste_symb);


/*******************************************************************************
 * lire_rel_liste
 * parametres : Section sect, SectionsList liste_sect, SymbolesList liste_symb
 * resultat : RelocList
 * description : Lit dans la section sect une liste de reimplantation et la
 *               renvoie sous la forme d'une structure dediee.
 * effet de bord : aucun
 *******************************************************************************/
RelocList lire_rel_liste(Section sect, SectionsList liste_sect, SymbolesList liste_symb);


/*******************************************************************************
 * lire_reimp_table
 * parametres : SectionsList liste_sect, SymbolesList liste_symb
 * resultat : RelocTable
 * description : Lit depuis le contenu des sections une table de reimplantation
 *               et la renvoie sous la forme d'une structure dediee.
 * effet de bord : aucun
 *******************************************************************************/
RelocTable lire_reimp_table(SectionsList liste_sect, SymbolesList liste_symb);


/*******************************************************************************
 * afficher_reimp_table
 * parametres : RelocTable table_reimp
 * resultat : aucun
 * description : Affiche de facon lisible les informations de la table de
 *               reimplantation donnee.
 * effet de bord : aucun
 *******************************************************************************/
void afficher_reimp_table(RelocTable table_reimp);


/*******************************************************************************
 * ecrire_reimp_table
 * parametres : SectionsList liste_sect, RelocTable table_reimp
 * resultat : aucun
 * description : Ecrit dans le contenu des sections de liste_sec le contenu de
 *               la table des symboles definie dans la structure liste_symb.
 * effet de bord : Modification de contenu de sections de liste_sect
 *******************************************************************************/
void ecrire_reimp_table(SectionsList liste_sect, RelocTable table_reimp);


/*******************************************************************************
 * supprimer_reimp_table
 * parametres : RelocTable table_reimp
 * resultat : aucun
 * description : Libere l'espace memoire de la structure donnee.
 * effet de bord : Liberation de memoire
 *******************************************************************************/
void supprimer_reimp_table(RelocTable table_reimp);


#endif

