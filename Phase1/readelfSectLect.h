#ifndef __LEC_SEC__
#define __LEC_SEC__

#include <stdint.h>
#include "readelfOptions.h"
#include "readelfHeader.h"
#include "readelfSectTable.h"


/*******************************************************************************
 * trier_sections
 * parametres : Options *opt, SectionsList liste
 * resultat : aucun
 * description : Renomme, trie et supprime les doublons de Opt.sectList
 * effet de bord : Modification de Opt.sectList
 *******************************************************************************/
void trier_sections(Options *opt, SectionsList liste);


/*******************************************************************************
 * lire_section
 * parametres : FILE *elfFile, Section sect
 * resultat : aucun
 * description : Lit dans le fichier elfFile le contenu de la section sect
 *               et complete la dataTab de la structure sect dediee.
 * effet de bord : aucun
 *******************************************************************************/
void lire_section(FILE *elfFile, Section Sect);


/*******************************************************************************
 * lire_sections
 * parametres : FILE *elfFile, SectionsList liste 
 * resultat : aucun
 * description : Lit dans le fichier elfFile le contenu de toutes les sections.
 * effet de bord : aucun
 *******************************************************************************/
void lire_sections(FILE *elfFile, SectionsList liste);


/*******************************************************************************
 * afficher_section
 * parametres : Section sect
 * resultat : aucun
 * description : Affiche de facon lisible le contenu de la section sect donnee.
 * effet de bord : aucun
 *******************************************************************************/
void afficher_section(Section sect);


/*******************************************************************************
 * afficher_sections
 * parametres : SectionsList liste_sections, Options opt
 * resultat : aucun
 * description : Affiche le contenu des sections de liste_sections presentes
 *               dans dans la liste de sections entree.
 * effet de bord : aucun
 *******************************************************************************/
void afficher_sections(SectionsList liste_sections, Options opt);


/*******************************************************************************
 * ecrire_section
 * parametres : FILE *outFile, Section sect
 * resultat : aucun
 * description : Ecrit dans le fichier outFile le contenu d'une section definie
 *               dans la structure sect.
 * effet de bord : Ecrit dans le fichier
 *******************************************************************************/
void ecrire_section(FILE *outFile, Section Sect);


/*******************************************************************************
 * ecrire_sections
 * parametres : FILE *outFile, SectionsList liste_sections , uint32_t e_shoff
 * resultat : aucun
 * description : Ecrit dans le fichier outFile les contenus des sections de la
 *               table des sections definie dans la structure liste.
 * effet de bord : Ecrit dans le fichier
 *******************************************************************************/
void ecrire_sections(FILE *outFile, SectionsList liste_sections, uint32_t e_shoff);

#endif