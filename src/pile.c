//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
// Copyright 2004 Quentin Quadrat <lecrapouille@gmail.com>,
//                Minh-Long Nguyen <>,
//                Benoit Marcot <>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "pile.h"

t_pile nouvelle_pile(void)
{
     t_pile pile;

     pile.position = 0;
     pile.taille_allouee = TAILLE_PILE_INIT;
     pile.donnees = (t_cfa) xmalloc(TAILLE_PILE_INIT*(sizeof(t_cf)));
     pile.donnees[0].entier = 0;
     pile.donnees[0].type = ENTIER;
     return pile;
}

inline bool pile_vide(t_pile * pile)
{
     return ((*pile).position == 0);
}

inline void reinit_pile(t_pile * pile)
{
     (*pile).position = 0;
}

inline void reinit_pile_gtk(GtkListStore * store)
{
     gtk_list_store_clear(GTK_LIST_STORE(store));
}

inline void reallouer_pile(t_pile * pile, unsigned int taille)
{
     (*pile).taille_allouee += taille;
     (*pile).donnees = xrealloc((*pile).donnees, (*pile).taille_allouee*sizeof(t_cf));
}

inline t_cf sommet(t_pile * pile)
{
     return (*pile).donnees[(*pile).position];
}

inline void depiler(t_pile * pile)
{
     GtkTextIter fin;

     if ((*pile).position <= 0)
     {
          printf("\007Erreur : Pile vide !!\n");
          gtk_text_buffer_get_end_iter(bf_res, &fin);
          gtk_text_buffer_insert(bf_res, &fin, "Erreur : Pile vide !!", -1);
          guts.erreur = true;
     }
     else
          --(*pile).position;
}

inline void depiler_gtk(GtkListStore * store)
{
     GtkTreePath *path;
     GtkTreeIter iter;

     path = gtk_tree_path_new_from_indices(guts.pile_donnees.position,-1);
     gtk_tree_model_get_iter (GTK_TREE_MODEL (store),
                              &iter,
                              path);
     gtk_tree_path_free (path);
     gtk_list_store_remove(GTK_LIST_STORE(store), &iter);
}

inline void empiler(t_pile * pile, t_cf elt)
{
     ++(*pile).position;
     if ((*pile).position >= (*pile).taille_allouee)
     {
          (*pile).taille_allouee += TAILLE_PILE_INIT;
          (*pile).donnees = xrealloc((*pile).donnees, (*pile).taille_allouee*sizeof(t_cf));
     }
     (*pile).donnees[(*pile).position] = elt;
}

void empiler_gtk(GtkListStore * store, t_cf elt, GtkTreeIter * iter)
{
     char * ch = elt_to_string(elt);
     gtk_list_store_append(GTK_LIST_STORE(store), iter);
     gtk_list_store_set(GTK_LIST_STORE(store), iter, 0, ch, -1);
     free(ch);
}

void recopier_pile_gtk(t_pile * pile, GtkListStore * store, GtkTreeIter * iter)
{
     int i;
     reinit_pile_gtk(store);
     for (i = 1; i <= (*pile).position; i++)
          empiler_gtk(store, (*pile).donnees[i], iter);
}

inline int nb_elt_pile(t_pile * pile)
{
     return (*pile).position;
}

void afficher_pile_buffer(t_pile pile, GtkTextBuffer * bf)
{
     int i;
     t_cf elt;
     gchar * buf;
     GtkTextIter fin;

     buf = (gchar *) xmalloc(10*sizeof(gchar));
     buf[0] = '\0';
     sprintf(buf, "%d elt(s) : ", pile.position);
     gtk_text_buffer_get_end_iter(bf, &fin);
     gtk_text_buffer_insert(bf, &fin, buf, -1);
     g_free(buf);
     for (i = 1; i <= pile.position; i++)
     {
          elt = pile.donnees[i];
          afficher_elt_buffer(elt, bf);
     }
}


void afficher_pile(t_pile pile)
{
     int i;
     t_cf elt;

     printf("%d elt(s) : ", pile.position);
     for (i = 1; i <= pile.position; i++)
     {
          elt = pile.donnees[i];
          afficher_elt(elt);
     }
}
