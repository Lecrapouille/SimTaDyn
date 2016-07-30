#ifndef CELLULE_H
#define CELLULE_H

#include "header.h"
#include "memoire.h"

void afficher_type(t_type type);
void afficher_elt(t_cf elt);
void afficher_elt_buffer(t_cf elt, GtkTextBuffer *bf);
t_cf int_to_elt(int val);
t_cf float_to_elt(float val);
t_cf ad_to_elt(void * val, t_type type);
gchar* elt_to_string(t_cf elt);
#if 0
t_cf zone_to_elt(int val);
t_cf sommet_to_elt(int val);
t_cf arc_to_elt(int val);
bool est_cellule(t_cf elt);
#endif

#endif /* CELLULE_H */
