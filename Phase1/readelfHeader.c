#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readelfHeader.h"

/* Fichier principal de l'etape 1 : affichage de l'en-tete */


/*	lire_entete(FILE *elfFile)
		Lit les informations du header du fichier donne
*/
Elf32_Ehdr lire_entete(FILE *elfFile)
{
	Elf32_Ehdr header;
	fread(&header, 1, sizeof(header), elfFile); 
	return header;
}


/*	est_fichier_elf(Elf64_Ehdr header)
		Renvoie 1 si l'en-tete donnee correspond à celui d'un fichier elf, 0 sinon
*/
int est_fichier_elf(Elf32_Ehdr header)
{
	return (header.e_ident[0] == 0x7f &&
			header.e_ident[1] == 'E' &&
			header.e_ident[2] == 'L' &&
			header.e_ident[3] == 'F');
}


/*	affiche_entete(Elf64_Ehdr header)
		Affichage des informations de la structure entree en parametre
*/
void afficher_entete(Elf32_Ehdr header)
{
	// variables en texte pur, non recuperees dans le header
	char class[15] = "";
	char type[30] = "";
	char version[20] = "";
	char data[40] = "";

	// Instanciation de la variable class en fonction de la valeur du champ EI_CLASS (DOC chapitre 1-7)
	switch (header.e_ident[4])
	{
	case 0:
		strcpy(class, "Invalid class");
		break;
	case 1:
		strcpy(class, "ELF32");
		break;
	case 2:
		strcpy(class, "ELF64");
		break;
	}

	// Instanciation de la variable data en fonction de la valeur du champ EI_DATA (DOC chapitre 1-7)
	switch (header.e_ident[5])
	{
	case 0:
		strcpy(data, "0 Invalid data encoding");
		break;
	case 1:
		strcpy(data, "2's complement, little endian");
		break;
	case 2:
		strcpy(data, "1's complement, big endian");
		break;
	}

	// Instanciation de la variable version en fonction de la valeur du champ EI_VERSION (DOC chapitre 1-7)
	switch (header.e_ident[6])
	{
	case 0:
		strcpy(version, "0 (none)");
		break;
	case 1:
		strcpy(version, "1 (current)");
		break;
	}

	// Instanciation de la variable type en fonction de la valeur du champ e_type (DOC chapitre 1-4)
	switch (header.e_type)
	{
	case 0:
		strcpy(type, "NONE (No file type)");
		break;
	case 1:
		strcpy(type, "REL (Relocatable file)");
		break;
	case 2:
		strcpy(type, "EXEC (Executable file)");
		break;
	case 3:
		strcpy(type, "DYN (Shared object file)");
		break;
	case 4:
		strcpy(type, "CORE (Core file)");
		break;
	case 0xff00:
		strcpy(type, "LOPROC (Processor-specific)");
		break;
	case 0xffff:
		strcpy(type, "HIPROC (Processor-specific)");
		break;
	}

	// Affichage de l'en-tete
	printf("ELF Header:\n");
	printf("  Magic:   %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n",
		   header.e_ident[0], header.e_ident[1], header.e_ident[2], header.e_ident[3],
		   header.e_ident[4], header.e_ident[5], header.e_ident[6], header.e_ident[7],
		   header.e_ident[8], header.e_ident[9], header.e_ident[10], header.e_ident[11],
		   header.e_ident[12], header.e_ident[13], header.e_ident[14], header.e_ident[15]);
	printf("  Class:                             %s\n", class);
	printf("  Data:                              %s\n", data);
	printf("  Version:                           %s\n", version);
	printf("  OS/ABI:                            UNIX - System V\n");
	printf("  ABI Version:                       %d\n", header.e_ident[7]);
	printf("  Type:                              %s\n", type);
	printf("  Machine:                           Advanced Micro Devices X86-64\n");
	printf("  Version:                           0x%x\n", header.e_version);
	printf("  Entry point address:               0x%x\n", header.e_entry);
	printf("  Start of program headers:          %d (bytes into file)\n", header.e_phoff);
	printf("  Start of section headers:          %d (bytes into file)\n", header.e_shoff);
	printf("  Flags:                             0x%x\n", header.e_flags);
	printf("  Size of this header:               %d (bytes)\n", header.e_ehsize);
	printf("  Size of program headers:           %d (bytes)\n", header.e_phentsize);
	printf("  Number of program headers:         %d\n", header.e_phnum);
	printf("  Size of section headers:           %d (bytes)\n", header.e_shentsize);
	printf("  Number of section headers:         %d\n", header.e_shnum);
	printf("  Section header string table index: %d\n", header.e_shstrndx);
}
