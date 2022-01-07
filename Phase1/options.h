#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct options{
	int h, S, s, r;
	int nb_sec, nb_file;
	char *secList[1000];
	char *fileList[1000];
} options;

void help();

options read_options(int argc, char *argv[]);

void affiche_options(options Opt);

#endif