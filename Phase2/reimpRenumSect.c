#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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


void supprimer_section(SectionsList *liste, int i)
{
	// Liberer la memoire de la section i
	free(liste->sectTab[i].dataTab);
	//decalage de toutes les sections suivantes la secion i
	for (int j = i; j < liste->nb_sect - 1; j++)
	{
		liste->sectTab[j] = liste->sectTab[j + 1];
	}
	liste->nb_sect--;
}


void renumeroter_sections(Elf32_Ehdr *header, SectionsList * liste)
{
	int taille_supp = 0;
	int align=0;
	for (int i = 0; i < liste->nb_sect; i++)
	{
		// modifier l'offset de tous les sections selon la taille de sections supprimees
		liste->sectTab[i].header.sh_offset -= taille_supp;

		//bien aligné l'offset pour qu'il soit un mult de 4
		align= liste->sectTab[i].header.sh_offset % 4 ;
		if (align != 0)
		{
			liste->sectTab[i].header.sh_offset += (4 - align);
		}
			if (liste->sectTab[i].header.sh_type == SH_REL || liste->sectTab[i].header.sh_type == SH_RELA)
			{
				// calculer la taille totale des sections supprimees
				taille_supp += liste->sectTab[i].header.sh_size;
				// supprimer une setcion

				supprimer_section(liste, i);
				// prendre en compte section suivante
				i--;
			}

	}

	//tester en affichant toutes les sections apres modification
	/*
	for (int i = 0; i < liste->nb_sect; i++)
	{
		if (strlen(liste->sectTab[i].name) > 17)
		{
			liste->sectTab[i].name[17] = '\0';
		}
		printf("  [%2d] %-17s %-15s %08x %06x ", i,
			   liste->sectTab[i].name,
			   liste->sectTab[i].type,
			   liste->sectTab[i].header.sh_addr,
			   liste->sectTab[i].header.sh_offset);
		printf("%06x %02x %3s %2d  %2d %2d\n",
			   liste->sectTab[i].header.sh_size,
			   liste->sectTab[i].header.sh_entsize,
			   liste->sectTab[i].flag,
			   liste->sectTab[i].header.sh_link,
			   liste->sectTab[i].header.sh_info,
			   liste->sectTab[i].header.sh_addralign);
	}
	*/

	///////////////////////////////////////////////////////
		// modifier le nombre de section dans le header
		header->e_shnum = liste->nb_sect;
		// modifier les indices de sections dans le header
		header->e_shstrndx = liste->nb_sect - 1;
	}
