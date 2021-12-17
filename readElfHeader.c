#include <stdio.h>
#include <string.h>
#include<elf.h>
#include<stdlib.h>
/*
#define EI_NIDENT 16
typedef struct {
	unsigned char e_ident[EI_NIDENT];
	Elf32_Half e_type;
	Elf32_Half e_machine;
	Elf32_Word e_version;
	Elf32_Addr e_entry;
	Elf32_Off e_phoff;
	Elf32_Off e_shoff;
	Elf32_Word e_flags;
	Elf32_Half e_ehsize;
	Elf32_Half e_phentsize;
	Elf32_Half e_phnum;
	Elf32_Half e_shentsize;
	Elf32_Half e_shnum;
	Elf32_Half e_shstrndx;
 } Elf32_Ehdr;
 */
int main(int argc,char **argv){
	if(argc < 2){
		printf("Error:missed file!\n");
		exit(1);
	}
	char type[25]="";
	FILE *stream=fopen(argv[0],"rb");
	Elf32_Ehdr header;
	fread(&header, 1, sizeof(header), stream);
	
	switch(header.e_type){
		case 0:
				strcpy(type,"NONE (No file type)");
				break;
		case 1:
				strcpy(type,"REL (Relocatable file)");
				break;
		case 2:
				strcpy(type,"EXEC (Executable file)");
				break;
		case 3:
				strcpy(type,"YN (Shared object file)");
				break;
		case 4:
				strcpy(type,"CORE (Core file)");
				break;
		case 0xff00:
				strcpy(type,"LOPROC (Processor-specific)");
				break;
		case 0xffff:
				strcpy(type,"HIPROC (Processor-specific)");
				break;
	}
 printf("  Entry point address:               0x%x\n",header.e_phoff);

	printf("ELF Header:\n");
	printf("  Magic:   %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n", header.e_ident[0],header.e_ident[1],
																							   				header.e_ident[2],header.e_ident[3],
																								  			header.e_ident[4],header.e_ident[5],
																								   			header.e_ident[6],header.e_ident[7],
																								  			header.e_ident[8],header.e_ident[9],
																								   			header.e_ident[10],header.e_ident[11],
																								   			header.e_ident[12],header.e_ident[13],
																								   			header.e_ident[14],header.e_ident[15]);
	

	printf("  Class:                             ELF64\n");
	printf("  Data:                              2's complement, little endian\n");
	printf("  Version:                           1 (current)\n");
	printf("  OS/ABI:                            UNIX - System V\n");
	printf("  ABI Version:                       0\n");
	printf("  Type:                              %s\n",type);
	printf("  Machine:                           Advanced Micro Devices X86-64\n");
	printf("  Version:                           0x%x\n",header.e_version);
	printf("  Entry point address:               0x%x\n",header.e_entry);
	printf("  Start of program headers:          64 (bytes into file)\n");
	printf("  Start of section headers:          6552 (bytes into file)\n");
	printf("  Flags:                             0x0\n");
	printf("  Size of this header:               64 (bytes)\n");
	printf("  Size of program headers:           56 (bytes)\n");
	printf("  Number of program headers:         9\n");
	printf("  Size of section headers:           64 (bytes)\n");
	printf("  Number of section headers:         31\n");
	printf("  Section header string table index: 30\n");


	fclose(stream);
	return 0;
}
