#include "readElfHeader.h"
#include "readElfSecTable.h"
#include "readElfRel.h"
#include "readElfLecSect.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

  FILE *elfFile;

  if (argc == 3 && argv[1][1] != 'x')
  {
    elfFile = fopen(argv[2], "rb");
  }

  else if (argc == 4 && argv[1][1] == 'x')
  {
    elfFile = fopen(argv[3], "rb");
  }
  else
  {

    printf("Error: readelf <option> elf-file\n");
    exit(1);
  }

  if (elfFile == NULL)
  {
    printf("File open error!\n");
    exit(1);
  }

  Elf64_Ehdr header;
  fread(&header, 1, sizeof(header), elfFile); // Lecture de l'entete du fichier
  // test if elf file
  if (!(header.e_ident[0] == 0x7f && header.e_ident[1] == 'E' &&
        header.e_ident[2] == 'L' && header.e_ident[3] == 'F'))
  {
    printf("readelf: Error: Not an ELF file - it has the wrong magic bytes at the start\n");
    exit(1);
  }

  char flag = argv[1][1];
  switch (flag)
  {
  case 'h':
    affiche_header(header);
    break;
  case 'S':
    affiche_section_table(elfFile, header);
    break;
  case 'x':
    affichage_section(elfFile, header, argv[2]);
    break;
  case 's':
    printf("afficher la table des symboles d’un fichier ELF\n");
    break;
  case 'r':
    affiche_tables_entree_reimplantation(elfFile, header);
    break;
  default:
    printf("unkown falg:try one of the following falgs -h,-S,-r,-x,-s\n");
    exit(1);
  }
  fclose(elfFile);
  return 0;
}