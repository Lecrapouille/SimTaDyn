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

#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>

#include "main.h"
#include "interface.h"
#include "support.h"

#include "header.h"
#include "hash.h"
#include "interpreteur.h"
#include "fichier.h"
#include "memoire.h"
#include "opgl.h"
#include "dessine_carte.h"
#include "font.h"
#include "normal.h"

void
on_bt_RAZ_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_RAZ_non_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_RAZ_oui_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_nouveau1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_ouvrir1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_enregistrer1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_enregistrer_sous1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_quitter1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_couper1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_copier1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_coller1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_supprimer1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);


void
on_a_propos1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_bt_win_quit_annule_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_win_about_fermer_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_win_open_ok_button_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_win_open_cancel_button_clicked   (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_win_save_ok_button_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_win_save_cancel_button_clicked   (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_execute_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_debug_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_tg_mode_excel_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_tg_mode_som_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_tg_mode_arc_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_tg_mode_zone_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_tg_mode_ajout_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_tg_mode_supp_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_tg_mode_select_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_tg_mode_deplace_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_tg_mode_vue_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_abort_clicked                    (GtkButton       *button,
                                        gpointer         user_data);
#endif

void
on_bt_gconex_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_gcreate_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_gchgt_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_gdel_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_valider_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_entry_cellule_editing_done          (GtkCellEditable *celleditable,
                                        gpointer         user_data);

void
on_exporter_carte_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_mb_connexion_graphe_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_tg_mode_som_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_tg_mode_arc_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_tg_mode_zone_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_win_connexion_realize               (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_radio_ouvrir_carte_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radio_nouvelle_carte_toggled        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_cancel_win_gconnex_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_nouvelle_carte_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_supp_carte_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_charger_carte_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
conexion_serveur_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_win_gconex_realize                  (GtkWidget       *widget,
                                        gpointer         user_data);

gboolean
on_win_champ_delete_event              (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_win_champ_destroy_event             (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_bt_connexion_mysql_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_tg_show_forth_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_tg_show_champ_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_en_prompt_activate                  (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_slct_couleur_ok_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_slct_couleur_cancel_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_show_color_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_execute_carte_clicked            (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_win_champ_destroy_event             (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_win_champ_delete_event              (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void enfonce_bt_outil(bool aj, bool sup, bool ins, bool dep);

void enfonce_bt_cell(bool som, bool arc, bool zone);

void
on_exporter_code_forth_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void MoveCamera(float speed);
void RotationView(float angle, float x, float y, float z);
void RotateAroundPoint(t_pos vCenter, float angle, float x, float y, float z);

void
on_bt_exporter_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_importer_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_importer_ok_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_bt_exporter_ok_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_cancel_button1_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_cancel_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data);
