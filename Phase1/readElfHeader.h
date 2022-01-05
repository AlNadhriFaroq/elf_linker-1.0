#ifndef __HEADER_H__
#define __HEADER_H__

#include <stdint.h>

// Documentation chapitre 1-4 Figure 1-3
#define EI_NIDENT 16
typedef struct {
	unsigned char e_ident[EI_NIDENT]; 
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint64_t e_entry;
	uint64_t e_phoff;
	uint64_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
} Elf64_Ehdr;

/**************************************************************************
 * affiche_header                      
 * parametres : struct Elf64_Ehdr   
 * resultat : aucun                        
 * description : Transforme la structure donnee en paramètre en un bloc de
 *               texte lisible detaillant toutes les specificites du header
 * effet de bord : aucun                
 **************************************************************************/
void affiche_header(Elf64_Ehdr header);

#endif
