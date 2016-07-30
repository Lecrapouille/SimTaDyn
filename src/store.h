#ifndef STORE_H
#define STORE_H

#include "support.h"
#include "header.h"
#include "pile.h"

void ajouter_colonne_visualdico(GtkTreeView *vue);
GtkTreeModel *creer_visualdico(void);
void ajouter_mot_dans_visualdico(GtkTreeModel * store, t_cfa mot);

void ajouter_nomdeschamps_magazin_champ(GtkTreeStore * store, const gchar* nom);
void ajouter_col_visual_champ(GtkTreeView *vue);

void ajouter_col_visual_pile_donnees(GtkTreeView *vue);
void ajouter_col_visual_pile_retour(GtkTreeView *vue);
void ajouter_id_cell_visual_pile(GtkListStore * store, int i);

void create_popup_menu(GtkWidget *widget, GtkWidget ** menu);
void nouveau_item_popup(gchar * chaine, GtkWidget ** menu);

#endif /* STORE_H */
