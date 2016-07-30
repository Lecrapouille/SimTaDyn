#ifndef DESSIN_H
#define DESSIN_H

#include "header.h"
#include "opgl.h"
#include "memoire.h"
#include "callbacks.h"
#include "pile.h"

void selec_et_empile_sommet();
void selec_et_empile_arete();
void selec_et_empile_zone();
int selection_som();
void deselection_som();
int selection_arete();
void deselection_arete();
int selection_zone();
void deselection_zone();
void selection_forth(char *);

#endif
