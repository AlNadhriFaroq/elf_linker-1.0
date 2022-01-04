#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "readElfSecTable.h"
#include "readElfHeader.h"

/*
  find_type: trouve la chaine de caracteres associee a un entier.
*/
void find_type(long num, char *sh_type)
{
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
  find_flags:
*/
void find_flags(char *tab, int n)
{
  int i, j = 0;
  strcpy(tab, "");
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

void affiche_section_table(FILE *elfFile, Elf64_Ehdr header)
{
  char flags[6] = "";
  Elf64_Shdr sectHdr;
  char *sectNames = NULL;
  char sh_type[25] = "";

  fseek(elfFile, header.e_shoff + header.e_shstrndx * header.e_shentsize, SEEK_SET);
  fread(&sectHdr, 1, sizeof(sectHdr), elfFile);
  sectNames = malloc(sectHdr.sh_size);
  fseek(elfFile, sectHdr.sh_offset, SEEK_SET);
  fread(sectNames, 1, sectHdr.sh_size, elfFile);

  printf("There are %d sectHdr headers, starting at offset 0x%lx:\n\n",
         header.e_shnum, header.e_shoff);
  printf("sectHdr Headers:\n  [Nr] Name               Type             Address "
         "          Offset\n       Size               EntSize          Flags  "
         "Link  Info  Align\n");

  // main loop
  for (int i = 0; i < header.e_shnum; i++)
  {
    char *sh_name = "";

    // Lecture de la sectHdr de la table du fichier
    fseek(elfFile, header.e_shoff + i * sizeof(sectHdr), SEEK_SET);
    fread(&sectHdr, 1, sizeof(sectHdr), elfFile);
	
    find_flags(flags, sectHdr.sh_flags);
    revstr(flags);

    find_type(sectHdr.sh_type, sh_type);
    sh_name = sectNames + sectHdr.sh_name;

    printf("  [%2d] %-18s %-17s %016lx  %08lx\n", i, sh_name, sh_type,
           sectHdr.sh_addr, sectHdr.sh_offset);

    printf("       %016lx   %016lx   %-5s %-5d %-5d  %ld\n", sectHdr.sh_size,
           sectHdr.sh_entsize, flags, sectHdr.sh_link,
           sectHdr.sh_info, sectHdr.sh_addralign);
  }

  printf("Key to Flags:\n");
  printf("  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n");
  printf("  L (link order), O (extra OS processing required), G (group), T (TLS),\n");
  printf("  C (compressed), x (unknown), o (OS specific), E (exclude),\n");
  printf("  l (large), p (processor specific)\n");

  free(sectNames);
}
