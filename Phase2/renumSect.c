
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../Phase1/readelfOptions.h"
#include "../Phase1/readelfHeader.h"
#include "../Phase1/readelfSectTable.h"
#include "../Phase1/readelfSectLect.h"

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

	//int nbSecSupp = 0;
	long idx = 0;
	Elf32_Ehdr hdr;
	Elf32_Phdr progHdr;
	Elf32_Shdr sectHdr;

	fread(&hdr, 1, sizeof(hdr), elfFile);
	fwrite(&hdr, 1, sizeof(hdr), outFile);

	//lecture et ecriture de program header if exists
	//fread(&progHdr, 1, sizeof(progHdr), elfFile);
	//fwrite(&progHdr, 1, sizeof(progHdr), outFile);

	fseek(elfFile, sizeof(hdr) + sizeof(progHdr), SEEK_SET);

	fseek(elfFile, hdr.e_shoff, SEEK_SET);
	fread(&sectHdr, 1, sizeof(sectHdr), elfFile);

	char c;
	int jdx = sizeof(hdr);
	while (jdx < sectHdr.sh_offset+0x154){
		fseek(elfFile,jdx, SEEK_SET);
		fread(&c, 1, sizeof(c), elfFile);
		fwrite(&c,1,sizeof(c),outFile);
		jdx++;
	}

	SectionsList sList = lire_sections_table(elfFile, hdr);
	lire_sections(elfFile, sList);
	char *list[] = {"", "-S", "-x", "2", "renumSect"};
	Options op = lire_options(5, list);
	afficher_sections_table(sList, hdr.e_shoff);
	afficher_sections(sList, op) ;

	while (idx < sList.nb_sect)
	{
		// ecriture section content
		Section sect = sList.sectTab[idx];
		int diff = sList.sectTab[idx + 1].header.sh_offset - (sect.header.sh_offset + sect.header.sh_size);

		fwrite(sect.dataTab, 1, sect.header.sh_size, outFile);
		printf("%p\n",&(sect.dataTab));
		
		if ((idx+1 <= sList.nb_sect) && (diff != 0))
		{
			c=0x0;
			fwrite(&c, 1, sizeof(c), outFile);
		}

		idx++;
	}
	// Lecture et ecriture section header
	for (int i = 0; i < hdr.e_shnum ; i++)
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

	supprimer_sections_table(sList);
	//hdr.e_shnum -= nbSecSupp;
	//fseek(outFile, 0, SEEK_SET);
	//fwrite(&hdr, 1, sizeof(hdr), outFile);
	fclose(elfFile);
	fclose(outFile);
	return 0;
}