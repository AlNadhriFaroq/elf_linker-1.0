#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <stdio.h>
#include <stdlib.h>

// Structure destinee a stocke les informations entrees en parametre du programme
typedef struct options{
	int h, S, s, r;
	int nb_sec, nb_file;
	char *secList[1000];
	char *fileList[1000];
} options;

/**********************************************************************************
 * help
 * parametres : aucun
 * resultat : aucun
 * description : Affiche les differentes options disponibles en entree du programme
 * effet de bord : aucun
 **********************************************************************************/
void help();

/******************************************************************************
 * read_options
 * parametres : int argc, char* argv[]
 * resultat : struct options
 * description : Lit et interprete les options entrees en donnee et les renvoie
				 sous forme d'une structure dediee davantage exploitable
 * effet de bord : aucun
 ******************************************************************************/
options read_options(int argc, char *argv[]);

/***************************************************************
 * affiche_options
 * parametres : struct options Opt
 * resultat : aucun
 * description : Affiche les informations de la structure donnee
 * effet de bord : aucun
 ***************************************************************/
void affiche_options(options Opt);

#endif