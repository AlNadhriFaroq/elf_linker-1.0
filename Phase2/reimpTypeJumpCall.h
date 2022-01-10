#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../Phase1/readelfHeader.h"
#include "../Phase1/readelfSectTable.h"
#include "../Phase1/readelfSectLect.h"
#include "../Phase1/readelfReimpTable.h"


/*******************************************************************************
 * reimplanter_type_jump_call
 * parametres : SectionsList liste
 * resultat : aucun
 * description : Modifie la structure donnee selon l'etape 9.
 * effet de bord : Modification de liste
 *******************************************************************************/
void reimplanter_type_jump_call(SectionsList liste);
