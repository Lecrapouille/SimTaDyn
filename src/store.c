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

#include "store.h"

#define NB_TITRES_VISUAL_DICO 5
static char* Titre_visualdico[] =
{
     {"Token"},
     {"Ad"},
     {"Info"},
     {"Fct C"},
     {"Definition mot"},
};

#define TITRE_VISUAL_CHAMP "Nom des champs :"
#define TITRE_VISUAL_PILE_DONNEES "Pile de donnees"

void ajouter_colonne_visualdico(GtkTreeView *vue)
{
     GtkTreeViewColumn *colonne;
     GtkCellRenderer *renderer;
     int i;

     renderer = gtk_cell_renderer_text_new();
     for (i=0; i<NB_TITRES_VISUAL_DICO; ++i)
     {
          colonne = gtk_tree_view_column_new_with_attributes(Titre_visualdico[i],
                                                             renderer,
                                                             "text",
                                                             i, NULL);
          gtk_tree_view_column_set_sort_column_id(colonne, i);
          gtk_tree_view_append_column(vue, colonne);
     }
}

GtkTreeModel *creer_visualdico(void)
{
     GtkListStore *store;

     store =
          gtk_list_store_new(NB_TITRES_VISUAL_DICO,
                             G_TYPE_STRING,
                             G_TYPE_STRING,
                             G_TYPE_STRING,
                             G_TYPE_STRING,
                             G_TYPE_STRING);
     return GTK_TREE_MODEL(store);
}

void ajouter_mot_dans_visualdico(GtkTreeModel * store, t_cfa mot)
{
     GtkTreeIter iter;
     t_nfa nfa;
     t_cfa cfa;
     char *ch;
     unsigned int i;
     char s_cfa[50];
     char s_info[50];
     char s_fp[50];

     ch = nouvelle_chaine();
     cfa = mot;
     nfa = (t_nfa) mot[0].adresse;

     for (i=1; i < nfa->taille_utilisee; ++i)
          ch = g_strconcat(ch, " ", elt_to_string(mot[i]), NULL);

     sprintf(s_cfa, "%p", cfa);
     sprintf(s_fp,  "%p", nfa->fp);
     sprintf(s_info,"%x", nfa->info);
     gtk_list_store_append(GTK_LIST_STORE(store), &iter);
     nfa->ad_gtk = iter;
     gtk_list_store_set(GTK_LIST_STORE(store), &iter,
                        0, nfa->nom,
                        1, s_cfa,
                        2, s_info,
                        3, s_fp,
                        4, ch, -1);
     g_free(ch);
}


/*
** Arbre contenant le nom des champs
*/
void ajouter_nomdeschamps_magazin_champ(GtkTreeStore * store, const gchar* nom)
{
     GtkTreeIter iter1, iter2, iter3, iter4;
     char *carte;

     carte = nouvelle_chaine();
     carte = g_strconcat("Carte ", nom, NULL);
     gtk_tree_store_append(store, &iter1, NULL);
     gtk_tree_store_set(store, &iter1, 0, carte, 1, TOUTES, 2, ID, -1);
     gtk_tree_store_append(store, &iter2, &iter1);
     g_free(carte);

     /* Sommet */
     gtk_tree_store_set(store, &iter2, 0, "Sommet", 1, SOMMET, 2, ID, -1);
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "ID", 1, SOMMET, 2, ID, -1);
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "Nom", 1, SOMMET, 2, NOM, -1);
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "Forth", 1, SOMMET, 2, FORTH, -1);
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "Position", 1, SOMMET, 2, POSITION, -1);
     gtk_tree_store_append(store, &iter4, &iter3);
     gtk_tree_store_set(store, &iter4, 0, "x", 1, SOMMET, 2, POSX, -1);
     gtk_tree_store_append(store, &iter4, &iter3);
     gtk_tree_store_set(store, &iter4, 0, "y", 1, SOMMET, 2, POSY, -1);
     gtk_tree_store_append(store, &iter4, &iter3);
     gtk_tree_store_set(store, &iter4, 0, "z", 1, SOMMET, 2, POSZ, -1);
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "Poignet", 1, SOMMET, 2, POIGNET, -1);
     gtk_tree_store_append(store, &iter4, &iter3);
     gtk_tree_store_set(store, &iter4, 0, "x", 1, SOMMET, 2, POIGNETX, -1);
     gtk_tree_store_append(store, &iter4, &iter3);
     gtk_tree_store_set(store, &iter4, 0, "y", 1, SOMMET, 2, POIGNETY, -1);

     /* Arcs */
     gtk_tree_store_append(store, &iter2, &iter1);
     gtk_tree_store_set(store, &iter2, 0, "Arcs", 1, ARC, 2, ID, -1);
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "ID", 1, ARC, 2, ID, -1);
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "Nom", 1, ARC, 2, NOM, -1);
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "Forth", 1, ARC, 2, FORTH, -1);
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "Cout", 1, ARC, 2, COUT, -1);
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "Poignet", 1, ARC, 2, POIGNET, -1);
     gtk_tree_store_append(store, &iter4, &iter3);
     gtk_tree_store_set(store, &iter4, 0, "x", 1, ARC, 2, POIGNETX, -1);
     gtk_tree_store_append(store, &iter4, &iter3);
     gtk_tree_store_set(store, &iter4, 0, "y", 1, ARC, 2, POIGNETY, -1);


     /* zones */
     gtk_tree_store_append(store, &iter2, &iter1);
     gtk_tree_store_set(store, &iter2, 0, "Zones", 1, ZONE, 2, ID, -1);
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "ID", 1, ZONE, 2, ID, -1);
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "Nom", 1, ZONE, 2, NOM, -1);
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "Forth", 1, ZONE, 2, FORTH, -1);
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "Poignet", 1, ZONE, 2, POIGNET, -1);
     gtk_tree_store_append(store, &iter4, &iter3);
     gtk_tree_store_set(store, &iter4, 0, "x", 1, ZONE, 2, POIGNETX, -1);
     gtk_tree_store_append(store, &iter4, &iter3);
     gtk_tree_store_set(store, &iter4, 0, "y", 1, ZONE, 2, POIGNETY, -1);

     //Triangles
     gtk_tree_store_append(store, &iter2, &iter1);
     gtk_tree_store_set(store, &iter2, 0, "Triangles", 1, TRIANGLE, 2, ID, -1);
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "ID", 1, TRIANGLE, 2, ID, -1);
     /* gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "Nom", 1, TRIANGLE, 2, NOM, -1);*/
     /*gtk_tree_store_append(store, &iter3, &iter2);
       gtk_tree_store_set(store, &iter3, 0, "Forth", 1, TRIANGLE, 2, FORTH, -1);*/
     gtk_tree_store_append(store, &iter3, &iter2);
     gtk_tree_store_set(store, &iter3, 0, "Poignet", 1, TRIANGLE, 2, POIGNET, -1);
     gtk_tree_store_append(store, &iter4, &iter3);
     gtk_tree_store_set(store, &iter4, 0, "x", 1, TRIANGLE, 2, POIGNETX, -1);
     gtk_tree_store_append(store, &iter4, &iter3);
     gtk_tree_store_set(store, &iter4, 0, "y", 1, TRIANGLE, 2, POIGNETY, -1);

}

void ajouter_col_visual_champ(GtkTreeView *vue)
{
     GtkTreeViewColumn *colonne;
     GtkCellRenderer *renderer;

     renderer = gtk_cell_renderer_text_new();
     colonne = gtk_tree_view_column_new_with_attributes(TITRE_VISUAL_CHAMP,
                                                        renderer,
                                                        "text",
                                                        0, NULL);
     gtk_tree_view_column_set_sort_column_id(colonne, 0);
     gtk_tree_view_append_column(vue, colonne);
}


/*****
 **
 ** CELLULES
 **
 */
void ajouter_id_cell_visual_pile(GtkListStore * store, int i)
{
     GtkTreeIter iter;

     gtk_list_store_append(GTK_LIST_STORE(store), &iter);
     gtk_list_store_set(GTK_LIST_STORE(store), &iter,
                        0, som[i].nom,
                        -1);
}

void ajouter_col_visual_pile_donnees(GtkTreeView *vue)
{
     GtkTreeViewColumn *colonne;
     GtkCellRenderer *renderer;

     renderer = gtk_cell_renderer_text_new();
     colonne = gtk_tree_view_column_new_with_attributes(TITRE_VISUAL_PILE_DONNEES,
                                                        renderer,
                                                        "text",
                                                        0, NULL);
     gtk_tree_view_column_set_sort_column_id(colonne, 0);
     gtk_tree_view_append_column(vue, colonne);
}


/*
** Popup menu
*/
static gboolean
popup_menu_handler(GtkWidget      *widget,
                   GdkEventButton *event)
{
     if (event->button == 3)
     {
          gtk_menu_popup(GTK_MENU(widget), NULL, NULL, NULL, NULL, event->button, event->time);
          return TRUE;
     }

     return FALSE;
}

static void
popup_menu_detacher(GtkWidget *attach_widget,
                    GtkMenu   *menu)
{
/* ne fait rien */
}


void create_popup_menu(GtkWidget *widget, GtkWidget ** menu)
{
     *menu = gtk_menu_new();
     gtk_menu_attach_to_widget(GTK_MENU(*menu),GTK_WIDGET(widget),popup_menu_detacher);

     g_signal_connect_swapped(G_OBJECT(widget), "button_press_event",
                              G_CALLBACK(popup_menu_handler), *menu);
}

static void item_popup_active(GtkWidget *attach_widget,
                              gchar * chaine)
{
     printf("%s\n", chaine);
}

void nouveau_item_popup(gchar * chaine, GtkWidget ** menu)
{
  GtkWidget *item     = gtk_menu_item_new_with_label(chaine);

  gtk_menu_shell_append(GTK_MENU_SHELL(*menu), item);
  gtk_widget_show(item);

  g_signal_connect(G_OBJECT(item), "activate",
                   G_CALLBACK(item_popup_active), chaine);
}


#if 0
void user_function(GtkCellRendererText *renderer,
                   gchar *path_string, gchar *new_text,
                   gpointer user_data)
{
     GtkWidget *vue = lookup_widget(GTK_WIDGET(win_main), "tree_som");
     GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(vue));
     GtkTreeModel *model = cl_som;
     GtkTreeIter iter;

     if (gtk_tree_selection_get_selected(selection, &model, &iter))
     {
          //OKKKK ::  gtk_list_store_remove(GTK_LIST_STORE(cl_som), &iter);
          printf("%s\n", path_string);
          gtk_list_store_set(GTK_LIST_STORE(cl_som), &iter, atoi(path_string),
                             new_text, -1);
     }

     //GtkTreeViewColumn *col = gtk_tree_view_get_column(GTK_TREE_VIEW(vue), g_ascii_strtoull(path_string, NULL, 10));
}
#endif
