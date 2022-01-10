#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../Phase1/readelfHeader.h"
#include "../Phase1/readelfSectTable.h"
/*
    a Modifier
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

    char c;
    int nbSecSupp = 0;
    long idx = 0x0;
    Elf32_Ehdr hdr;
    Elf32_Shdr sectHdr;

    fread(&hdr, 1, sizeof(hdr), elfFile);
    fseek(elfFile, sizeof(hdr), SEEK_SET);
    fread(&c, 1, sizeof(c), elfFile);
    while (idx != hdr.e_shoff)
    {
        idx++;
        fwrite(&c, 1, sizeof(c), outFile);
        fseek(elfFile, idx * sizeof(c), SEEK_SET);
        fread(&c, 1, sizeof(c), elfFile);
    }

    for (int i = 0; i < hdr.e_shnum; i++)
    {
        fseek(elfFile, hdr.e_shoff + i * sizeof(sectHdr), SEEK_SET);
        fread(&sectHdr, 1, sizeof(sectHdr), elfFile);
        if (sectHdr.sh_type != 4 && sectHdr.sh_type != 9) // 4=SH_RELA et 9= SH_REL
        {
            fwrite(&sectHdr, 1, sizeof(sectHdr), outFile);
        }
        else
        {
            nbSecSupp++;
        }
    }

    hdr.e_shnum -= nbSecSupp;
    fseek(outFile, 0, SEEK_SET);
    fwrite(&hdr, 1, sizeof(hdr), outFile);
    fclose(elfFile);
    fclose(outFile);
    return 0;
}