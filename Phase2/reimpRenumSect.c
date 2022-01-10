#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../Phase1/readelfHeader.h"
#include "../Phase1/readelfSectTable.h"
#include "../Phase1/readelfSectLect.h"
#include "reimpRenumSect.h"


/*
  A faire:
  - supprimer sh_rela et sh_rel section
  - supprimer sh_rela et sh_rel entete de la section header 
  - renumeroter les section et modfier les offset des sections
*/
void renommer_sections(Elf32_Ehdr header, SectionsList liste)
{
	//Modification de header et liste selon etape 6
}