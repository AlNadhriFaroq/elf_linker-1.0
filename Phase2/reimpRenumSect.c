#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../Phase1/readelfHeader.h"
#include "../Phase1/readelfSectTable.h"
#include "../Phase1/readelfSectLect.h"
#include "reimpRenumSect.h"

#define SH_RELA 4
#define SH_REL 9

/*
  A faire:
  - supprimer sh_rela et sh_rel section
  - supprimer sh_rela et sh_rel entete de la section header 
  - renumeroter les section et modfier les offset des sections
*/


void supprimer_section(SectionsList liste, int i)
{
	// Liberer la memoire de la section i
	free(liste.sectTab[i].dataTab);

	//decalage de toutes les sections suivantes la secion i
	for (int j = i; j < liste.nb_sect - 1; i++){
		liste.sectTab[j] = liste.sectTab[j+1];
	}
	liste.nb_sect--;
}


void renumeroter_sections(Elf32_Ehdr header, SectionsList liste)
{
	int taille_supp = 0;
	
	for (int i = 0; i < liste.nb_sect; i++)
	{
		// probleme liste.sectTab[i].header
		printf("size of section = %x\n", liste.sectTab[i].header.sh_size);
		if (liste.sectTab[i].header.sh_type == SH_REL || liste.sectTab[i].header.sh_type == SH_RELA)
		{
			//calculer la taille totale des sections supprimees
			taille_supp += liste.sectTab[i].header.sh_size;
			// supprimer une setcion
			supprimer_section(liste, i);
		}

		//modifier l'offset de tous les sections selon la taille de sections supprimees
		liste.sectTab[i].header.sh_offset -= taille_supp;
	}

	//modifier le nombre de section dans le header 
	header.e_shnum = liste.nb_sect;
}
