#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "options.h"

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

options read_options(int argc, char *argv[])
{
	options Opt = {};
	
	for (int i = 1 ; i < argc ; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] == '-')
		{
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
				i++;
				if (argv[i] != NULL)
				{
					Opt.secList[Opt.nb_sec] = argv[i];
					Opt.nb_sec++;
				}
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
			else
			{
				printf("readelf: unrecognized option -- '%s'\n", argv[i]);
				help();
			}
		}
		else if (argv[i][0] == '-')
		{
			for (int j = 1 ; j < strlen(argv[i]) ; j++)
			{
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
				case 'x':
					if (j != strlen(argv[i])-1)
					{
						Opt.secList[Opt.nb_sec] = &(argv[i][j+1]);
						j = strlen(argv[i]);
					}
					else if (argv[i+1] != NULL)
					{
						i++;
						Opt.secList[Opt.nb_sec] = argv[i];
					}
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
				default:
					printf("readelf: invalid option -- '%c'\n", argv[i][j]);
					help();
					break;
				}
			}
		}
		else
		{
			Opt.fileList[Opt.nb_file] = argv[i];
			Opt.nb_file++;
		}
	}
	
	return Opt;
}

void affiche_options(options Opt)
{
	printf("h = %d\nS = %d\ns = %d\nr = %d\n", Opt.h, Opt.S, Opt.s, Opt.r);
	printf("nb_sec = %d\n", Opt.nb_sec);
	for (int i = 0 ; i < Opt.nb_sec ; i++)
	{
		printf("   %s\n", Opt.secList[i]);
	}
	printf("nb_file = %d\n", Opt.nb_file);
	for (int i = 0 ; i < Opt.nb_file ; i++)
	{
		printf("   %s\n", Opt.fileList[i]);
	}
}
