#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../Phase1/readelfHeader.h"
#include "../Phase1/readelfSectTable.h"
#include "../Phase1/readelfSectLect.h"
#include "../Phase1/readelfSymbTable.h"
#include "reimpCorrectSymb.h"


void corriger_symboles(SectionsList *liste_sect, SymbolesList *liste_symb)
{
	// Parcours des symboles
	for (int i = 0; i < liste_symb->nb_symb; i++)
	{
		// Symboles courant
		Symbole symbole = liste_symb->symbTab[i]; 
		
		// Cas d'un symboles a modifier
		if (symbole.symb.st_shndx != 65521 && symbole.symb.st_shndx != 0 && ELF32_ST_TYPE(symbole.symb.st_info) == 3)
		{
			// Recupration des donnees
			uint32_t index = liste_symb->symbTab[i].symb.st_shndx;
			uint32_t offset = liste_sect->sectTab[index].header.sh_offset;
			
			// Modifications
			liste_sect->sectTab[index].header.sh_addr = offset;
			liste_symb->symbTab[i].symb.st_value = offset;
		}
	}
}
