#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../Phase1/readelfHeader.h"
#include "../Phase1/readelfSectTable.h"


/*******************************************************************************
 * supprimer_section
 * parametres : SectionsList * liste, int i
 * resultat : uint32_t
 * description : Modifie les sections et la table des sections en supprimant la
 *               la section i.
 * effet de bord : Modifie le contenu de la structure donnee
 *******************************************************************************/
uint32_t supprimer_section(SectionsList *liste, int i);


/*******************************************************************************
 * renumeroter_sections
 * parametres : SectionsList * liste, Elf32_Ehdr *header
 * resultat : void
 * description : Modifie les sections, la table des sections et le header en
 *               suppriment les sections de type SH_REL et SH_RELA.
 * effet de bord : Modifie le contenu des structures donnees
 *******************************************************************************/
void renumeroter_sections(SectionsList * liste, Elf32_Ehdr *header);

