
#ifndef __LEC_SEC__
#define __LEC_SEC__

#include <stdint.h>

/**************************************************************************************
 * affichage_section_contenu
 * parametres : FILE *elfFile, Elf64_Ehdr header, Elf64_Shdr sectHdr, int section
 * resultat : aucun
 * description : Affiche les adresses et le contenu en hexadecimal de la section donnee
 * effet de bord : aucun
 **************************************************************************************/
void affichage_section_contenu(FILE *elfFile, Elf32_Ehdr header, Elf32_Shdr sectHdr, int section);

/**************************************************************************************
 * affiche_section
 * parametres : FILE *elfFile, Elf64_Ehdr header, char *section 
 * resultat : aucun
 * description : Fonction transition entre les parametres donnes par l'utilisateur (arguments de ./readelf)
 *               et l'affichage du contenu de la section choisie (appel de affichage_section_contenu)
 * effet de bord : aucun
 **************************************************************************************/
void affiche_section(FILE *elfFile, Elf32_Ehdr header, char *section);

#endif