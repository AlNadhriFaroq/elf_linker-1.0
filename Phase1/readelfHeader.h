#ifndef __HEADER_H__
#define __HEADER_H__

#include <stdint.h>
#include <stdio.h>


// Documentation chapitre 1-4 Figure 1-3
#define EI_NIDENT 16
typedef struct
{
	unsigned char e_ident[EI_NIDENT];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint32_t e_entry;
	uint32_t e_phoff;
	uint32_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
} Elf32_Ehdr;


/*******************************************************************************
 * lire_entete
 * parametres : FILE *elfFile
 * resultat : Elf32_Ehdr
 * description : Lit dans le fichier elfFile les donnees de l'en-tete et les
 *               renvoie sous la forme d'une structure dediee.
 * effet de bord : aucun
 *******************************************************************************/
Elf32_Ehdr lire_entete(FILE *elfFile);


/*******************************************************************************
 * est_fichier_elf
 * parametres : struct Elf64_Ehdr header
 * resultat : int
 * description : Verifie si l'en-tete donne correspond a celui d'un fichier elf.
 *               Renvoie 1 si c'est le cas, 0 sinon.
 * effet de bord : aucun
 *******************************************************************************/
int est_fichier_elf(Elf32_Ehdr header);


/*******************************************************************************
 * afficher_entete
 * parametres : struct Elf64_Ehdr header
 * resultat : aucun
 * description : Affiche de facon lisible les informations contenu dans la
 *               structures header donnee.
 * effet de bord : aucun
 *******************************************************************************/
void afficher_entete(Elf32_Ehdr header);


/*******************************************************************************
 * ecrire_entete
 * parametres : FILE *outFile, struct Elf64_Ehdr header
 * resultat : aucun
 * description : Ecrit dans le fichier outFile le contenu de l'en-tete defini
 *               dans la structure header.
 * effet de bord : ecriture dans le fichier
 *******************************************************************************/
void ecrire_entete(FILE *outFile, Elf32_Ehdr header);

#endif
