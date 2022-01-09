#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <stdio.h>
#include <stdlib.h>


// Structure destinee a stocke les informations entrees en parametre du programme
typedef struct options{
	int h, S, s, r;
	int nb_sect, nb_file;
	char *sectList[1000];
	char *fileList[1000];
} Options;


/*******************************************************************************
 * afficher_aide
 * parametres : aucun
 * resultat : aucun
 * description : Affiche les diverses options disponibles en entree du programme
 * effet de bord : aucun
 *******************************************************************************/
void afficher_aide();


/*******************************************************************************
 * lire_options
 * parametres : int argc, char* argv[]
 * resultat : struct Options
 * description : Lit et interprete les options entrees en donnee et les renvoie
				 sous forme d'une structure dediee davantage exploitable
 * effet de bord : aucun
 *******************************************************************************/
Options lire_options(int argc, char *argv[]);


/*******************************************************************************
 * afficher_options
 * parametres : struct Options Opt
 * resultat : aucun
 * description : Affiche de facon lisible les informations contenu dans la
				 structure Opt donnee
 * effet de bord : aucun
 *******************************************************************************/
void afficher_options(Options Opt);

#endif