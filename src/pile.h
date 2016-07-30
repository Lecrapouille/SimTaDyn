#ifndef PILE_H
#define PILE_H

#include "memoire.h"
#include "header.h"
#include "string_util.h"
#include "elt_pile.h"

t_pile nouvelle_pile(void);
bool pile_vide(t_pile * pile);
void reinit_pile(t_pile * pile);
void reallouer_pile(t_pile * pile, unsigned int taille);
t_cf sommet(t_pile * pile);
void depiler(t_pile * pile);
void empiler(t_pile * pile, t_cf elt);
int nb_elt_pile(t_pile * pile);
void afficher_pile(t_pile pile);
void afficher_pile_buffer(t_pile pile, GtkTextBuffer * bf);
void reinit_pile_gtk(GtkListStore * store);
void depiler_gtk(GtkListStore * store);
void empiler_gtk(GtkListStore * store, t_cf elt, GtkTreeIter * iter);
void recopier_pile_gtk(t_pile * pile, GtkListStore * store, GtkTreeIter * iter);

#endif /* PILE_H */
 
