#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readelfReimpTable.h"
#include "readelfSectTable.h"

/* Fichier principal de l'etape 5 : affichage des tables de reimplentation */


/*	lire_type(int num, char *type)
		Traduit l'entier num en chaine de caractere
*/
static void lire_type(int num, char *type)
{
	switch (num)
	{
	case 0:
		strcpy(type, "R_386_NONE");
		break;
	case 1:
		strcpy(type, "R_386_32");
		break;
	case 2:
		strcpy(type, "R_386_PC32");
		break;
	case 3:
		strcpy(type, "R_386_GOT32");
		break;
	case 4:
		strcpy(type, "R_386_PLT32");
		break;
	case 5:
		strcpy(type, "R_386_COPY");
		break;
	case 6:
		strcpy(type, "R_386_GLOB_DAT");
		break;
	case 7:
		strcpy(type, "R_386_JMP_SLOT");
		break;
	case 8:
		strcpy(type, "R_386_RELATIVE");
		break;
	case 9:
		strcpy(type, "R_386_GOTOFF");
		break;
	case 10:
		strcpy(type, "R_386_GOTPC");
		break;
	case 11:
		strcpy(type, "R_386_32PLT");
		break;
	case 20:
		strcpy(type, "R_386_16");
		break;
	case 21:
		strcpy(type, "R_386_PC16");
		break;
	case 22:
		strcpy(type, "R_386_8");
		break;
	case 23:
		strcpy(type, "R_386_PC8");
		break;
	case 38:
		strcpy(type, "R_386_SIZE32");
		break;
	default:
		strcpy(type, "UNKOWN");
	}
}


Rela lire_rela(Section sect, int i)
{
	Rela reimp = {"", {0, 0, 0}};
	
	// Lecture de reimp.rela dans la section sect entree
	reimp.rela.r_offset = *((uint32_t *) &(sect.dataTab[i*12  ]));
	reimp.rela.r_info   = *((uint32_t *) &(sect.dataTab[i*12+4])); 
	reimp.rela.r_addend = *((int32_t  *) &(sect.dataTab[i*12+8])); 
	
	// Traduction de l'info en texte lisible
	lire_type(ELF32_R_TYPE(reimp.rela.r_info), reimp.type);
	
	// Gestion de la negation de r_addend
	if (reimp.rela.r_addend & ((uint32_t)1 << 31))
	{
		reimp.rela.r_addend = - reimp.rela.r_addend;
	}
		
	return reimp;
}


Rel lire_rel(Section sect, int i)
{
	Rel reimp = {"", {0, 0}};
	
	// Lecture de reimp.rela dans la section sect entree
	reimp.rel.r_offset = *((uint32_t *) &(sect.dataTab[i*8  ]));
	reimp.rel.r_info   = *((uint32_t *) &(sect.dataTab[i*8+4])); 
	
	// Traduction de l'info en texte lisible
	lire_type(ELF32_R_TYPE(reimp.rel.r_info), reimp.type);
	
	return reimp;
}


RelocList lire_rela_liste(Section sect)
{
	int nb_reloc = sect.header.sh_size / sect.header.sh_entsize;
	RelocList liste_reimp = {"", sect.header.sh_offset, nb_reloc, 1, NULL, NULL};
	strcpy(liste_reimp.name, sect.name);
	liste_reimp.relaTab = malloc(sizeof(Rela) * nb_reloc);
	
	for (int i = 0; i < nb_reloc; i++)
	{
		liste_reimp.relaTab[i] = lire_rela(sect, i);
	}
	
	return liste_reimp;
}


RelocList lire_rel_liste(Section sect)
{
	int nb_reloc = sect.header.sh_size / sect.header.sh_entsize;
	RelocList liste_reimp = {"", sect.header.sh_offset, nb_reloc, 0, NULL, NULL};
	strcpy(liste_reimp.name, sect.name);
	liste_reimp.relTab = malloc(sizeof(Rel) * nb_reloc);

	for (int i = 0; i < nb_reloc; i++)
	{
		liste_reimp.relTab[i] = lire_rel(sect, i);
	}
	
	return liste_reimp;
}


RelocTable lire_reimp_table(SectionsList liste_sect)
{
	RelocTable table_reimp = {0, NULL};
	table_reimp.tab = malloc(sizeof(RelocList) * 0);

	for (int i = 0; i < liste_sect.nb_sect; i++)
	{
		Section sect = liste_sect.sectTab[i];
		if (sect.header.sh_type == 4)
		{
			RelocList liste_reimp = lire_rela_liste(sect);
			table_reimp.nb_list++;
			table_reimp.tab = realloc(table_reimp.tab, sizeof(RelocList) * table_reimp.nb_list);
			table_reimp.tab[table_reimp.nb_list - 1] = liste_reimp;
		}
		else if (sect.header.sh_type == 9)
		{
			RelocList liste_reimp = lire_rel_liste(sect);
			table_reimp.nb_list++;
			table_reimp.tab = realloc(table_reimp.tab, sizeof(RelocList) * table_reimp.nb_list);
			table_reimp.tab[table_reimp.nb_list - 1] = liste_reimp;
		}
	}
	
	return table_reimp;
}


void afficher_reimp_table(RelocTable table_reimp)
{
	// Parcours des liste de reimplantation
	for (int i = 0; i < table_reimp.nb_list; i++)
	{
		RelocList liste_reimp = table_reimp.tab[i];
		
		if (liste_reimp.nb_reloc == 1)
		{
			printf("\nRelocation section '%s' at offset 0x%x contains 1 entry:\n",
				   liste_reimp.name, liste_reimp.offset);
		}
		else
		{
			printf("\nRelocation section '%s' at offset 0x%x contains %d entries:\n",
				   liste_reimp.name, liste_reimp.offset, liste_reimp.nb_reloc);
		}
		
		// Cas d'une liste de reimplantation de type rela
		if (liste_reimp.type)
		{
			printf("Offset             Info           Type           Sym. index          + Addend \n");
			
			// Parcours des reimplantations
			for (int j = 0; j < liste_reimp.nb_reloc; j++)
			{
				Rela reimp = liste_reimp.relaTab[j];
				
				if (reimp.rela.r_addend < 0)
				{
					printf("%06x    %06x      %-15s  %08x    - %x\n",
						   reimp.rela.r_offset, reimp.rela.r_info, reimp.type,
						   ELF32_R_SYM(reimp.rela.r_info), reimp.rela.r_addend);
				}
				else{
					printf("%06x    %06x      %-15s  %08x     + %x\n",
						   reimp.rela.r_offset, reimp.rela.r_info, reimp.type,
						   ELF32_R_SYM(reimp.rela.r_info), reimp.rela.r_addend);
				}
			}
		}
		
		// Cas d'une liste de reimplantation de type rel
		else
		{
			printf(" Offset     Info    Type            Sym.Value  Sym. Name\n");
			
			// Parcours des reimplantations
			for (int j = 0; j < liste_reimp.nb_reloc; j++)
			{
				Rel reimp = liste_reimp.relTab[j];
				printf("%08x  %08x %-17s %08x   %s\n",
					   reimp.rel.r_offset, reimp.rel.r_info, reimp.type,
					   ELF32_R_SYM(reimp.rel.r_info), "");
			}
		}
	}
}


// void ecrire_reimp_table(SectionsList liste_sect, RelocTable table_reimp)


void supprimer_reimp_table(RelocTable table_reimp)
{
	// Parcours des listes de reimplantations
	for (int i = 0; i < table_reimp.nb_list; i++)
	{
		if (table_reimp.tab[i].type)
		{
			free(table_reimp.tab[i].relaTab);
		}
		else
		{
			free(table_reimp.tab[i].relTab);
		}
	}
	
	free(table_reimp.tab);
}
