#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "readElfSecTable.h"
#include "readElfHeader.h"

/* Fichier principal de l'etape 2 : affichage de la table des sections */

/*
	find_type(long num, char *sh_type)
		Lit la valeur donnee dans le premier parametre afin de definir la valeur du second
*/
void find_type(long num, char *sh_type)
{
	// Documentation chapitre 1-11 Figure 1-9
	switch (num)
	{
	case 0:
		strcpy(sh_type, "NULL");
		break;
	case 1:
		strcpy(sh_type, "PROGBITS");
		break;
	case 2:
		strcpy(sh_type, "SYMTAB");
		break;
	case 3:
		strcpy(sh_type, "STRTAB");
		break;
	case 4:
		strcpy(sh_type, "RELA");
		break;
	case 5:
		strcpy(sh_type, "HASH");
		break;
	case 6:
		strcpy(sh_type, "DYNAMIC");
		break;
	case 7:
		strcpy(sh_type, "NOTE");
		break;
	case 8:
		strcpy(sh_type, "NOBITS");
		break;
	case 9:
		strcpy(sh_type, "REL");
		break;
	case 10:
		strcpy(sh_type, "SHLIB");
		break;
	case 11:
		strcpy(sh_type, "DYNSYM");
		break;
	case 0xe:
		strcpy(sh_type, "INIT_ARRAY");
		break;
	case 0xf:
		strcpy(sh_type, "FINI_ARRAY");
		break;
	case 0x6ffffff6:
		strcpy(sh_type, "GNU_HASH");
		break;
	case 0x6ffffffd:
		strcpy(sh_type, "VERDEF");
		break;
	case 0x6ffffffe:
		strcpy(sh_type, "VERNEED");
		break;
	case 0x6fffffff:
		strcpy(sh_type, "VERSYM");
		break;
	case 0x70000000:
		strcpy(sh_type, "LOPROC");
		break;
	case 0x7fffffff:
		strcpy(sh_type, "HIPROC");
		break;
	case 0x80000000:
		strcpy(sh_type, "LOUSER");
		break;
	case 0xffffffff:
		strcpy(sh_type, "HIUSER");
		break;
	default:
		strcpy(sh_type, "UNKOWN");
		break;
	}
}

/*
	revstr(char *str)
		Prend une chaine de caracteres et modifie sa valeur en l'inversant
		utilisee pour faire la transition big Endian / little Endian
*/
void revstr(char *str)
{
	int i, len, temp;
	len = strlen(str);
	for (i = 0; i < len / 2; i++)
	{
		temp = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = temp;
	}
}

/*
	find_flags(char *tab, int n)
		Traduction des actions effectuees en flags pour une section
*/
void find_flags(char *tab, int n)
{
	int i, j = 0;
	strcpy(tab, "");
	// les valeurs de tabVal proviennent du site https://refspecs.linuxbase.org/elf/gabi4+/ch4.sheader.html (Figure 4.11)
	int tabVal[7] = {0x40, 0x20, 0x10, 0x4, 0x2, 0x1, 0x0};
	char tabChar[7] = {'I', 'S', 'M', 'X', 'A', 'W', ' '};
	for (i = 0; n > 0x0; i++)
	{
		if (n >= tabVal[i])
		{
			tab[j] = tabChar[i];
			n -= tabVal[i];
			j++;
		}
	}
	tab[j] = '\0';
}

/*
	affiche_section_table(FILE *elfFile, Elf64_Ehdr header)
		Affichage de la table des sections et des informations pour chaque section
*/
void affiche_section_table(FILE *elfFile, Elf64_Ehdr header)
{
	char flags[6] = "";
	Elf64_Shdr sectHdr;
	char *sectNames = NULL;
	char sh_type[25] = "";

	// Placement du curseur au debut de la premiere section
	fseek(elfFile, header.e_shoff + header.e_shstrndx * header.e_shentsize, SEEK_SET);

	// Lecture de l'en-tete de la premiere section
	fread(&sectHdr, 1, sizeof(sectHdr), elfFile);

	// Allocation dans sectNames du nom de la premiere section
	sectNames = malloc(sectHdr.sh_size);
	fseek(elfFile, sectHdr.sh_offset, SEEK_SET);
	fread(sectNames, 1, sectHdr.sh_size, elfFile);

	printf("There are %d section headers, starting at offset 0x%lx:\n\n",
		   header.e_shnum, header.e_shoff);
	printf("Section Headers:\n  [Nr] Name              Type             Address "
		   "          Offset\n       Size              EntSize          Flags  "
		   "Link  Info  Align\n");

	// Parcourt de toutes les sections
	for (int i = 0; i < header.e_shnum; i++)
	{
		char *sh_name = "";

		// Lecture de l'en-tete de la section courante
		fseek(elfFile, header.e_shoff + i * sizeof(sectHdr), SEEK_SET);
		fread(&sectHdr, 1, sizeof(sectHdr), elfFile);

		// Lecture des flags la section courante
		find_flags(flags, sectHdr.sh_flags);
		revstr(flags);

		// Lecture du type de la section courante
		find_type(sectHdr.sh_type, sh_type);
		sh_name = sectNames + sectHdr.sh_name;
		if(strlen(sh_name) > 16){
			sh_name[17] = '\0';
		}
		

		// Affichage des informations
		printf("  [%2d] %-17s %-17s%016lx  %08lx\n", i, sh_name, sh_type,
			   sectHdr.sh_addr, sectHdr.sh_offset);
		printf("       %016lx  %016lx  %2s      %2d    %2d     %ld\n", sectHdr.sh_size,
			   sectHdr.sh_entsize, flags, sectHdr.sh_link,
			   sectHdr.sh_info, sectHdr.sh_addralign);
	}

	/* En termes de flags, certains n'étaient pas présents dans la documentation.
	   W, A, X sont dans la documentation chapitre 1-13 et 1-14 Figure 1-11.
	   Les autres flags sont dans la fonction reference readelf -S <nomFichier> */
	printf("Key to Flags:\n");
	printf("  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n");
	printf("  L (link order), O (extra OS processing required), G (group), T (TLS),\n");
	printf("  C (compressed), x (unknown), o (OS specific), E (exclude),\n");
	printf("  l (large), p (processor specific)\n");

	free(sectNames);
}
