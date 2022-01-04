#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readElfHeader.h"

void affiche_header(Elf64_Ehdr header) {
  char class[15] = "";
  char type[30] = "";
  char version[20] = "";
  char data[40] = "";

  switch (header.e_ident[4]) {
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

  switch (header.e_ident[5]) {
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

  switch (header.e_ident[6]) {
  case 0:
    strcpy(version, "0 (none)");
    break;
  case 1:
    strcpy(version, "1 (current)");
    break;
  }

  switch (header.e_type) {
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

  /************** Affichage de l'en-tete *******************/
  printf("ELF Header:\n");
  printf("  Magic:   %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n",
         header.e_ident[0], header.e_ident[1], header.e_ident[2],
         header.e_ident[3], header.e_ident[4], header.e_ident[5],
         header.e_ident[6], header.e_ident[7], header.e_ident[8],
         header.e_ident[9], header.e_ident[10], header.e_ident[11],
         header.e_ident[12], header.e_ident[13], header.e_ident[14],
         header.e_ident[15]);
  printf("  Class:                             %s\n", class);
  printf("  Data:                              %s\n", data);
  printf("  Version:                           %s\n", version);
  printf("  OS/ABI:                            UNIX - System V\n");
  printf("  ABI Version:                       %d\n", header.e_ident[7]);
  printf("  Type:                              %s\n", type);
  printf("  Machine:                           Advanced Micro Devices X86-64\n");
  printf("  Version:                           0x%x\n", header.e_version);
  printf("  Entry point address:               0x%lx\n", header.e_entry);
  printf("  Start of program headers:          %lu (bytes into file)\n", header.e_phoff);
  printf("  Start of section headers:          %lu (bytes into file)\n", header.e_shoff);
  printf("  Flags:                             0x%x\n", header.e_flags);
  printf("  Size of this header:               %hu (bytes)\n", header.e_ehsize);
  printf("  Size of program headers:           %hu (bytes)\n", header.e_phentsize);
  printf("  Number of program headers:         %hu\n", header.e_phnum);
  printf("  Size of section headers:           %hu (bytes)\n", header.e_shentsize);
  printf("  Number of section headers:         %hu\n", header.e_shnum);
  printf("  Section header string table index: %hu\n", header.e_shstrndx);
}
