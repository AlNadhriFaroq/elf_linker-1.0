#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../Phase1/readelfHeader.h"
#include "../Phase1/readelfSectTable.h"
#include "../Phase1/readelfSectLect.h"
#include "../Phase1/readelfSymbTable.h"


/*******************************************************************************
 * corriger_symboles
 * parametres : SectionsList liste
 * resultat : aucun
 * description : Modifie la structure donnee selon l'etape 6.
 * effet de bord : Modification de liste
 *******************************************************************************/
void corriger_symbole(SectionsList liste);
