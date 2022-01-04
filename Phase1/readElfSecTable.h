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

/***************************************
 * find_type                      
 * parametres : long num, char *sh_type 
 * resultat : aucun                        
 * description : associe à sh_type la valeur textuelle correspondant a la valeur de num,
 *               num etant le type d'une section 
 * effet de bord : remplissage de la valeur de sh_type               
 ***************************************/
void find_type(long num, char *sh_type);

/***************************************
 * revstr                      
 * parametres : char *str 
 * resultat : aucun                        
 * description : Transforme une chaine de caracteres donnee en parametre en sa valeur inverse/mirroir
 *               (Exemple : Noel -> leoN)
 * effet de bord : modification de str              
 ***************************************/
void revstr(char *str);

/***************************************
 * find_flags                      
 * parametres : char *tab, int n
 * resultat : aucun                        
 * description : Dans une section, des instructions sont lues et des actions sont effectuees 
 *               Des fanions sont associees a ces actions. (Exemple des fanions : ZNCV)
 *               Cette fonction remplit le tableau tab avec la valeur des fanions
 * effet de bord : remplissage de tab          
 ***************************************/
void find_flags(char *tab, int n);

/***************************************
 * affiche_section_table                      
 * parametres : FILE *elfFile, Elf64_Ehdr header
 * resultat : aucun                        
 * description : Affichage de la table des sections et des informations sur chacune d'entre elles
 * effet de bord : aucun         
 ***************************************/
void affiche_section_table(FILE *elfFile, Elf64_Ehdr header);

#endif
