#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readelfSectTable.h"
#include "readelfSymbTable.h"

/* Fichier principal de l'etape 4 : Affichage de la table des symboles */


/*	lire_name(Section strSect, uint32_t st_name, char *name)
		Traduit l'entier st_name en une chaine de caracteres
*/
static void lire_name(Section strSect, uint32_t st_name, char *name)
{
	int i = 0;

	while (strSect.dataTab[st_name] != 0)
	{
		name[i] = strSect.dataTab[st_name];
		st_name++;
		i++;
	}
	name[i] = '\0';
}


/*	lire_type(int st_info, char *type)
		Traduit l'entier st_info en une chaine de caracteres
*/
static void lire_type(int st_info, char *type)
{
	switch (st_info)
	{
	case 0:
		strcpy(type, "NOTYPE");
		break;
	case 1:
		strcpy(type, "OBJECT");
		break;
	case 2:
		strcpy(type, "FUNC");
		break;
	case 3:
		strcpy(type, "SECTION");
		break;
	case 4:
		strcpy(type, "FILE");
		break;
	case 5:
		strcpy(type, "COMMON");
		break;
	case 6:
		strcpy(type, "TLS");
		break;
	case 10:
		strcpy(type, "LOOS");
		break;
	case 12:
		strcpy(type, "HIOS");
		break;
	case 13:
		strcpy(type, "LOPROC");
		break;
	case 15:
		strcpy(type, "HIPROC");
		break;
	default:
		break;
	}
}


/*	lire_bind(int st_info, char *bind)
		Traduit l'entier st_info en une chaine de caracteres
*/
static void lire_bind(int st_info, char *bind)
{
	switch (st_info)
	{
	case 0:
		strcpy(bind, "LOCAL");
		break;
	case 1:
		strcpy(bind, "GLOBAL");
		break;
	case 2:
		strcpy(bind, "WEAK");
		break;
	case 10:
		strcpy(bind, "LOOS");
		break;
	case 12:
		strcpy(bind, "HIOS");
		break;
	case 13:
		strcpy(bind, "LOPROC");
		break;
	case 15:
		strcpy(bind, "HIPROC");
		break;
	default:
		break;
	}
}


/*	lire_vis(int st_other, char *vis)
		Traduit l'entier st_other en une chaine de caracteres
*/
static void lire_vis(int st_other, char *vis)
{
	switch (st_other)
	{
	case 1:
		strcpy(vis,"INTERNAL");
		break;
	case 2:
		strcpy(vis, "HIDDEN");
		break;
	case 3:
		strcpy(vis,"PROTECTED");
		break;
	default:
		strcpy(vis, "DEFAULT");
		break;
	}
}


/*	lire_ndx(uint16_t st_shndx, char *ndx)
		Traduit l'entier st_shndx en une chaine de caracteres
*/
static void lire_ndx(uint16_t st_shndx, char *ndx)
{
	switch (st_shndx)
	{
	case 0:
		strcpy(ndx, "UND");
		break;
	case 65521:
		strcpy(ndx, "ABS");
		break;
	default:
		sprintf(ndx, "%d", st_shndx);
		break;
	}
}


/*	lire_symbole(Section symSect, Section strSect, int i)
		Lit dans a section dediee les informations du symbole i
*/
Symbole lire_symbole(Section symSect, Section strSect, int i)
{
	Symbole symbole = {"", "", "", "", "", {0}};
	
	// Lecture de symbole.symb dans la section symSect entree
	symbole.symb.st_name  = *((uint32_t *) &(symSect.dataTab[i*16   ]));
	symbole.symb.st_value = *((uint32_t *) &(symSect.dataTab[i*16+4 ])); 
	symbole.symb.st_size  = *((uint32_t *) &(symSect.dataTab[i*16+8 ])); 
	symbole.symb.st_info  = *((uint8_t  *) &(symSect.dataTab[i*16+12])); 
	symbole.symb.st_other = *((uint8_t  *) &(symSect.dataTab[i*16+13])); 
	symbole.symb.st_shndx = *((uint16_t *) &(symSect.dataTab[i*16+14]));
	
	// Traduction des informations precedentes en textes lisibles
	lire_name(strSect, symbole.symb.st_name, symbole.name);
	lire_type(ELF32_ST_TYPE(symbole.symb.st_info), symbole.type);
	lire_bind(ELF32_ST_BIND(symbole.symb.st_info), symbole.bind);
	lire_vis(ELF32_ST_VISIBILITY(symbole.symb.st_other), symbole.vis);
	lire_ndx(symbole.symb.st_shndx, symbole.ndx);

	return symbole;
}


/*	lire_symboles_table(SectionsList liste_sect)
		Lit la table des symboles
*/
SymbolesList lire_symboles_table(SectionsList liste_sect)
{
	Section strtab, symtab, dynsym, dynstr;
	SymbolesList liste_symb = {0, NULL, 0, NULL};
	
	// Recherche des sections contenant la table des symboles
	int nb = 0;
	for (int i = 0; i < liste_sect.nb_sect; i++)
	{
		Section sect = liste_sect.sectTab[i];
		if (sect.header.sh_type == 11)
		{
			dynsym = sect;
			nb++;
		}
		else if (sect.header.sh_type == 2)
		{
			symtab = sect;
			nb++;
		}
		else if (strcmp(sect.name, ".strtab") == 0)
		{
			strtab = sect;
		}
		else if (strcmp(sect.name, ".dynstr") == 0)
		{
			dynstr = sect;
		}
	}
	
	// Cas ou il existe une section de symboles dynamiques
	if (nb > 1)
	{
		// Calcul du nombre de symbole et allocation memoire de la liste des symboles
		liste_symb.nb_symbDyn = dynsym.header.sh_size / dynsym.header.sh_entsize;
		liste_symb.symbDynTab = malloc(sizeof(Symbole) * liste_symb.nb_symbDyn);

		// Parcours des symboles
		for (int i = 0; i < liste_symb.nb_symbDyn; i++)
		{
			liste_symb.symbDynTab[i] = lire_symbole(dynsym, dynstr, i);
		}
	}
	
	// Cas ou il existe une section de symboles non dynamiques
	if (nb >= 1)
	{
		// Calcul du nombre de symbole et allocation memoire de la liste des symboles
		liste_symb.nb_symb = symtab.header.sh_size / symtab.header.sh_entsize;
		liste_symb.symbTab = malloc(sizeof(Symbole) * liste_symb.nb_symb);
		
		// Parcours des symboles
		for (int i = 0; i < liste_symb.nb_symb; i++)
		{
			liste_symb.symbTab[i] = lire_symbole(symtab, strtab, i);
		}
	}

	return liste_symb;
}


/*	afficher_symboles(char *nom, Symbole *symboles, int nb_symb)
		Affiche les informations d'une liste de symboles
*/
void afficher_symboles(char *nom, Symbole *symboles, int nb_symb)
{
	printf("\nSymbol table '%s' contains %d entries:\n", nom, nb_symb);
	printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");
	
	// Parcours des symboles	
	for (int i = 0; i < nb_symb; i++)
	{
		Symbole s = symboles[i];
		printf("   %3d: %08x %5d %-7s %-6s %-8s %3s %s\n",
			   i, s.symb.st_value, s.symb.st_size,
			   s.type, s.bind, s.vis, s.ndx, s.name);
	}
}


/*	afficher_symboles_table(SymbolesList liste)
		Affiche les informations de la table des symboles
*/
void afficher_symboles_table(SymbolesList liste)
{
	// Cas des symboles dynamiques
	if (liste.nb_symbDyn > 0)
	{
		afficher_symboles(".dynsym", liste.symbDynTab, liste.nb_symbDyn);
	}
	
	// Cas des autres symboles
	if (liste.nb_symb > 0)
	{
		afficher_symboles(".symtab", liste.symbTab, liste.nb_symb);
	}	
}


/*	ecrire_symbole(Section symSect, Symbole symbole, int i)
		Ecrit dans la section dediee les informations du symbole i
*/
void ecrire_symbole(Section symSect, Symbole symbole, int i)
{
	*((uint32_t *) &(symSect.dataTab[i*16   ])) = symbole.symb.st_name;
	*((uint32_t *) &(symSect.dataTab[i*16+4 ])) = symbole.symb.st_value;
	*((uint32_t *) &(symSect.dataTab[i*16+8 ])) = symbole.symb.st_size;
	*((uint8_t *)  &(symSect.dataTab[i*16+12])) = symbole.symb.st_info;
	*((uint8_t *)  &(symSect.dataTab[i*16+13])) = symbole.symb.st_other;
	*((uint16_t *) &(symSect.dataTab[i*16+14])) = symbole.symb.st_shndx;
}


/*	ecrire_symboles_table(SectionsList liste_sect, SymbolesList liste_symb)
		Ecrit la table des symboles dans les contenus des sections dediees
*/
void ecrire_symboles_table(SectionsList liste_sect, SymbolesList liste_symb)
{
	Section symtab, dynsym;
	
	// Recherche des sections ou ecrire les tables de symboles
	for (int i = 0; i < liste_sect.nb_sect; i++)
	{
		Section sect = liste_sect.sectTab[i];
		if (sect.header.sh_type == 11)
		{
			dynsym = sect;
		}
		else if (sect.header.sh_type == 2)
		{
			symtab = sect;
		}
	}
	
	// Cas des symboles dynamiques
	if (liste_symb.nb_symbDyn > 0)
	{
		// Parcours des symboles
		for (int i = 0; i < liste_symb.nb_symbDyn; i++)
		{
			ecrire_symbole(dynsym, liste_symb.symbDynTab[i], i);
		}
	}

	// Cas des autres symboles
	if (liste_symb.nb_symb > 0)
	{
		// Parcours des symboles
		for (int i = 0; i < liste_symb.nb_symb; i++)
		{
			ecrire_symbole(symtab, liste_symb.symbTab[i], i);
		}
	}
}


/*	supprimer_symboles_table(SymbolesList liste)
		Libere l'espace memoire de la table des symboles donnee
*/
void supprimer_symboles_table(SymbolesList liste)
{
	free(liste.symbTab);
	free(liste.symbDynTab);
}
