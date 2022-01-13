#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../Phase1/readelfHeader.h"
#include "../Phase1/readelfSectTable.h"
#include "../Phase1/readelfSymbTable.h"
#include "reimpRenumSect.h"


/*	supprimer_section(SectionsList *liste, int i)
		Supprime la section i
*/
uint32_t supprimer_section(SectionsList *liste_sect, SymbolesList *liste_symb, int i)
{
	// Adresse et taille de la section a supprimer
	uint32_t offset = liste_sect->sectTab[i].header.sh_offset;
	uint32_t size = liste_sect->sectTab[i].header.sh_size;
	
	// Liberation de la memoire du contenu de la section i
	free(liste_sect->sectTab[i].dataTab);
	
	// Parcours des sections a partir de celle a supprimer 
	for (int j = i; j < liste_sect->nb_sect - 1; j++)
	{
		// Decalage de toutes les sections suivants la section a supprimer
		liste_sect->sectTab[j] = liste_sect->sectTab[j + 1];
		
		// Decalage de l'adresse de la section decalee si celle_ci se situe apres celle supprimee
		if (liste_sect->sectTab[j].header.sh_offset > offset)
		{
			liste_sect->sectTab[j].header.sh_offset -= size;
		}
	}

	// Reallocation de la liste des sections
	liste_sect->nb_sect--;
	liste_sect->sectTab = realloc(liste_sect->sectTab, sizeof(Section) * liste_sect->nb_sect);

	// Parcours des sections
	for (int j = 0; j < liste_sect->nb_sect - 1; j++)
	{
		// Decrementation des sh_link pour les sections decalees
		if (liste_sect->sectTab[j].header.sh_link > i)
		{
			liste_sect->sectTab[j].header.sh_link--;
		}
	}
	
	// Correction des numeros de section dans la table des symboles
	for (int j = 0; j < liste_symb->nb_symb; j++)
	{
		if (liste_symb->symbTab[j].symb.st_shndx > i)
		{
			liste_symb->symbTab[j].symb.st_shndx--;
		}
	}
	
	// Coreection des numeros de section dans la table des symboles dynamiques
	for (int j = 0; j < liste_symb->nb_symbDyn; j++)
	{
		if (liste_symb->symbDynTab[j].symb.st_shndx > i)
		{
			liste_symb->symbDynTab[j].symb.st_shndx--;
		}
	}
	
	// Renvoie de la taille de la section supprimee
	return size;
}


/*	renumeroter_sections(SectionsList * liste, Elf32_Ehdr *header)
		Supprime toute les section de type SH_REL et SH_RELA
*/
void renumeroter_sections(SectionsList * liste_sect, SymbolesList *liste_symb, Elf32_Ehdr *header)
{
	int i = 0;
	
	// Parcours des sections
	while (i < liste_sect->nb_sect)
	{
		// Cas ou la section est de type SH_RELA ou SH_REL
		if (liste_sect->sectTab[i].header.sh_type == 9 || liste_sect->sectTab[i].header.sh_type == 4)
		{
			// Suppression de la section
			uint32_t size = supprimer_section(liste_sect, liste_symb, i);
			
			// Modification de l'adresse de la table de sections dans le header
			header->e_shoff -= size;
			
			// Decrementation du nombre de section dans le header
			header->e_shnum--;
		}
		
		// Cas ou la section est celle contenant les noms des sections
		else if (strcmp(liste_sect->sectTab[i].name, ".shstrtab") == 0)
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

