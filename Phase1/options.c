#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "options.h"

/*	help()
		Affiche l'aide et les differentes options disponibles
*/
void help()
{
	printf("Usage: readelf <option(s)> elf-file(s)\n");
	printf(" Display information about the contents of ELF format files\n");
	printf(" Options are:\n");
	printf("  -a --all               Equivalent to: -h -S -s -r\n");
	printf("  -h --file-header       Display the ELF file header\n");
	printf("  -S --section-headers   Display the sections' header\n");
	printf("     --sections          An alias for --section-headers\n");
	printf("  -s --syms              Display the symbol table\n");
	printf("     --symbols           An alias for --syms\n");
	printf("  -r --relocs            Display the relocations (if present)\n");
	printf("  -x --hex-dump=<number|name>\n");
	printf("                         Dump the contents of section <number|name> as bytes\n");
	printf("  -H --help              Display this information\n");
	exit(1);
}

/*	read_options(int argc, char *argv[])
		Lit et interprete les options donnees en entree du programme
*/
options read_options(int argc, char *argv[])
{
	// Cas sans argument, affiche l'aide
	if (argc == 1)
	{
		help();
	}
	
	options Opt = {};
	
	// Parcours de chaque argument entree
	for (int i = 1 ; i < argc ; i++)
	{
		// Cas ou l'argument commence par deux tiret '--'
		if (argv[i][0] == '-' && argv[i][1] == '-')
		{
			// comparaison direct entre l'argument courant et les options possibles
			if (strcmp(argv[i], "--all") == 0)
			{
				Opt.h = 1;
				Opt.S = 1;
				Opt.s = 1;
				Opt.r = 1;
			}
			else if (strcmp(argv[i], "--file-header") == 0)
			{
				Opt.h = 1;
			}
			else if (strcmp(argv[i], "--section-headers") == 0 || strcmp(argv[i], "--sections") == 0)
			{
				Opt.S = 1;
			}
			else if (strcmp(argv[i], "--syms") == 0 || strcmp(argv[i], "--symbols") == 0)
			{
				Opt.s = 1;
			}
			else if (strcmp(argv[i], "--relocs") == 0)
			{
				Opt.r = 1;
			}
			else if (strcmp(argv[i], "--hex-dump") == 0)
			{
				// Cette option necessite de lire aussi l'argument suivant
				i++;
				if (argv[i] != NULL)
				{
					Opt.secList[Opt.nb_sec] = argv[i];
					Opt.nb_sec++;
				}
				// Cas ou il n'y a pas d'argument suivant
				else
				{
					printf("readelf: '--hex-dump' option need argument\n");
					help();
				}
			}
			else if (strncmp(argv[i], "--hex-dump=", 11) == 0)
			{
				Opt.secList[Opt.nb_sec] = &(argv[i][11]);
				Opt.nb_sec++;
			}
			else if (strcmp(argv[i], "--help") == 0)
			{
				help();
			}
			// Cas ou l'argument n'est pas reconnu
			else
			{
				printf("readelf: unrecognized option -- '%s'\n", argv[i]);
				help();
			}
		}
		
		// Cas ou l'argument commence par un seul tiret '-'
		else if (argv[i][0] == '-')
		{
			// Parcours de chaque caractere
			for (int j = 1 ; j < strlen(argv[i]) ; j++)
			{
				// comparaison direct entre le caractere courant et les options possibles
				switch (argv[i][j])
				{
				case 'a':
					Opt.h = 1;
					Opt.S = 1;
					Opt.s = 1;
					Opt.r = 1;
					break;
				case 'h':
					Opt.h = 1;
					break;
				case 'S':
					Opt.S = 1;
					break;
				case 's':
					Opt.s = 1;
					break;
				case 'r':
					Opt.r = 1;
					break;
				// Cette option necessite de lire les caracteres suivant ou l'argument suivant
				case 'x':
					// Cas de la lecture des caracteres suivants de l'argument courant
					if (j != strlen(argv[i])-1)
					{
						Opt.secList[Opt.nb_sec] = &(argv[i][j+1]);
						j = strlen(argv[i]);
					}
					// Cas de la lecture de l'argument suivant
					else if (argv[i+1] != NULL)
					{
						i++;
						Opt.secList[Opt.nb_sec] = argv[i];
					}
					// Cas sans caracteres suivants, ni argument suivant
					else
					{
						printf("readelf: option need argument -- 'x'\n");
						help();
					}
					Opt.nb_sec++;
					break;
				case 'H':
					help();
					break;
				// Cas ou le caractere ne correspond a aucune option
				default:
					printf("readelf: invalid option -- '%c'\n", argv[i][j]);
					help();
					break;
				}
			}
		}
		
		// Cas ou l'argument ne commence pas par un tiret
		else
		{
			// L'argument est considere comme un nom de fichier
			Opt.fileList[Opt.nb_file] = argv[i];
			Opt.nb_file++;
		}
	}
	
	// Cas sans aucun fichier entree, rien à faire, affiche l'aide
	if (Opt.nb_file == 0)
	{
		printf("readelf: WARNING: Nothing to do.\n");
		help();
	}
	
	return Opt;
}

/*	affiche_options(options Opt)
		Affiche les options donnees en entree du programme
*/
void affiche_options(options Opt)
{
	printf("Afficher les headers : %d\n", Opt.h);
	printf("Afficher la table des sections : %d\n", Opt.S);
	printf("Afficher la table des symboles : %d\n", Opt.s);
	printf("Afficher la table de reimplantation : %d\n", Opt.r);
	
	printf("Sections a afficher : %d\n", Opt.nb_sec);
	for (int i = 0 ; i < Opt.nb_sec ; i++)
	{
		printf("    %s\n", Opt.secList[i]);
	}
	
	printf("Fichiers a afficher %d\n", Opt.nb_file);
	for (int i = 0 ; i < Opt.nb_file ; i++)
	{
		printf("    %s\n", Opt.fileList[i]);
	}
}
