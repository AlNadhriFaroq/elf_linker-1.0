#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../Phase1/readelfHeader.h"
#include "../Phase1/readelfSectTable.h"
#include "../Phase1/readelfSectLect.h"
#include "../Phase1/readelfSymbTable.h"


/*******************************************************************************
 * corriger_symboles
 * parametres : SectionsList *liste_sect, SymbolesList *liste_symb
 * resultat : aucun
 * description : Modifie la table des sections et la tables des symboles en
 *               changeant les adresses.
 * effet de bord : Modification de structures donnees
 *******************************************************************************/
void corriger_symboles(SectionsList *liste_sect, SymbolesList *liste_symb);

