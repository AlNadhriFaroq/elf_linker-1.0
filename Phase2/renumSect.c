
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../Phase1/readelfHeader.h"
#include "../Phase1/readelfSectTable.h"

#define SH_RELA 4
#define SH_REL 9

typedef struct
{
	uint32_t p_type;
	uint32_t p_offset;
	uint32_t p_vaddr;
	uint32_t p_paddr;
	uint32_t p_filesz;
	uint32_t p_memsz;
	uint32_t p_flags;
	uint32_t p_align;
} Elf32_Phdr;

/*
  A faire:
  - supprimer sh_rela et sh_rel section
  - supprimer sh_rela et sh_rel entete de la section header 
  - renumeroter les section et modfier les offset des sections
*/
int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("Error:input file outputfile \n ");
		exit(1);
	}

	FILE *elfFile = fopen(argv[1], "rb");
	if (elfFile == NULL)
	{
		printf("File open error!\n");
		exit(1);
	}
	FILE *outFile = fopen(argv[2], "wr");
	if (outFile == NULL)
	{
		printf("File open error!\n");
		exit(1);
	}

	int nbSecSupp = 0;
	long idx = 0x0;
	Elf32_Ehdr hdr;
	Elf32_Phdr progHdr;
	Elf32_Shdr sectHdr;

	fread(&hdr, 1, sizeof(hdr), elfFile);

	//lecture et ecriture de program header if exists
	fread(&progHdr, 1, sizeof(progHdr), elfFile);
	fwrite(&progHdr, 1, sizeof(progHdr), outFile);

	fseek(elfFile, sizeof(hdr) + sizeof(progHdr), SEEK_SET);

	uint8_t *sec = NULL;
	fseek(elfFile, hdr.e_shoff, SEEK_SET);
	fread(&sectHdr, 1, sizeof(sectHdr), elfFile);

	int err=0;
	while (idx != hdr.e_shnum)
	{
		idx++;
		err = fseek(elfFile, sectHdr.sh_offset, SEEK_SET); // seg fault in fseek
		if (err != 0)
		{
			printf("error\n");
		}

		// Lecture et ecriture section content
		fread(sec, 1, sectHdr.sh_size, elfFile);
		fwrite(sec, 1, sectHdr.sh_offset, outFile);

		//Lecture section header
		fseek(elfFile, hdr.e_shoff + idx * sizeof(sectHdr), SEEK_SET);
		fread(&sectHdr, 1, sizeof(sectHdr), elfFile);
	}
	for (int i = 0; i < hdr.e_shnum; i++)
	{
		fseek(elfFile, hdr.e_shoff + i * sizeof(sectHdr), SEEK_SET);
		fread(&sectHdr, 1, sizeof(sectHdr), elfFile);
		// if (sectHdr.sh_type != SH_RELA&& sectHdr.sh_type != SH_REL)
		//{
		fwrite(&sectHdr, 1, sizeof(sectHdr), outFile);
		/*}
		else
		{
		  nbSecSupp++;
		}*/
	}

	hdr.e_shnum -= nbSecSupp;
	fseek(outFile, 0, SEEK_SET);
	fwrite(&hdr, 1, sizeof(hdr), outFile);
	fclose(elfFile);
	fclose(outFile);
	return 0;
}