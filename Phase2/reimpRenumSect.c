#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../Phase1/readelfHeader.h"
#include "../Phase1/readelfSectTable.h"
#include "reimpRenumSect.h"


/*	supprimer_section(SectionsList *liste, int i)
		Supprime la section i
*/
uint32_t supprimer_section(SectionsList *liste, int i)
{
	// Adresse et taille de la section a supprimer
	uint32_t offset = liste->sectTab[i].header.sh_offset;
	uint32_t size = liste->sectTab[i].header.sh_size;
	
	// Liberation de la memoire du contenu de la section i
	free(liste->sectTab[i].dataTab);
	
	// Parcours des sections a partir de celle a supprimer 
	for (int j = i; j < liste->nb_sect - 1; j++)
	{
		// Decalage de toutes les sections suivants la section a supprimer
		liste->sectTab[j] = liste->sectTab[j + 1];
		
		// Decalage de l'adresse de la section decalee si celle_ci se situe apres celle supprimee
		if (liste->sectTab[j].header.sh_offset > offset)
		{
			liste->sectTab[j].header.sh_offset -= size;
		}
	}

	// Reallocation de la liste des sections
	liste->nb_sect--;
	liste->sectTab = realloc(liste->sectTab, sizeof(Section) * liste->nb_sect);

	// Parcours des sections
	for (int j = 0; j < liste->nb_sect - 1; j++)
	{
		// Decrementation des sh_link pour les sections decalees
		if (liste->sectTab[j].header.sh_link > i)
		{
			liste->sectTab[j].header.sh_link--;
		}
	}
	
	// Renvoie de la taille de la section supprimee
	return size;
}


/*	renumeroter_sections(SectionsList * liste, Elf32_Ehdr *header)
		Supprime toute les section de type SH_REL et SH_RELA
*/
void renumeroter_sections(SectionsList * liste, Elf32_Ehdr *header)
{
	int i = 0;
	
	// Parcours des sections
	while (i < liste->nb_sect)
	{
		// Cas ou la section est de type SH_RELA ou SH_REL
		if (liste->sectTab[i].header.sh_type == 9 || liste->sectTab[i].header.sh_type == 4)
		{
			// Suppression de la section
			uint32_t size = supprimer_section(liste, i);
			
			// Modification de l'adresse de la table de sections dans le header
			header->e_shoff -= size;
			
			// Decrementation du nombre de section dans le header
			header->e_shnum--;
		}
		
		// Cas ou la section est celle contenant les noms des sections
		else if (strcmp(liste->sectTab[i].name, ".shstrtab") == 0)
		{
			// Changement de son index dans le header
			header->e_shstrndx = i;
			i++;
		}
		else
		{
			i++;
		}
	}
}

