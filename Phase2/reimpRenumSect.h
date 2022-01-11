#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../Phase1/readelfHeader.h"
#include "../Phase1/readelfSectTable.h"
#include "../Phase1/readelfSectLect.h"

#define SH_RELA 4
#define SH_REL 9

typedef struct
{
	uint32_t p_type;
	uint32_t p_offset;
	uint32_t p_vaddr;
	uint32_t p_paddr;
	uint32_t p_filesz;
	uint32_t p_memsz;
	uint32_t p_flags;
	uint32_t p_align;
} Elf32_Phdr;


/*******************************************************************************
 * renumeroter_sections
 * parametres : Elf32_Ehdr header, SectionsList liste
 * resultat : aucun
 * description : Modifie les structures donnees selon l'etape 6.
 * effet de bord : Modification de hedaer et liste
 *******************************************************************************/
void renumeroter_sections(Elf32_Ehdr header, SectionsList liste);
