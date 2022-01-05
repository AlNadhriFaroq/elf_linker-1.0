#ifndef __LEC_SEC__
#define __LEC_SEC__

#include <stdint.h>

/**************************************************************************
 * affichage_section_contenu                      
 * parametres : FILE *elfFile, Elf64_Ehdr header, Elf64_Shdr sectHdr, int section
 * resultat : aucun                        
 * description : Affiche le contenu de la section choisie par le parametre section
 *               et les adresses des octets lus
 * effet de bord : aucun                
 **************************************************************************/
void affichage_section_contenu(FILE *elfFile, Elf64_Ehdr header, Elf64_Shdr sectHdr, int section);

/**************************************************************************
 * affichage_section                      
 * parametres : FILE *elfFile, Elf64_Ehdr header, char *section 
 * resultat : aucun                        
 * description : Fonction transition entre les parametres donnes par l'utilisateur (arguments de ./readelf)
 *               et l'affichage du contenu de la section choisie (appel de affichage_section_contenu)
 * effet de bord : aucun                
 **************************************************************************/
void affichage_section(FILE *elfFile, Elf64_Ehdr header, char *section);

#endif