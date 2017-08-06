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

#include "interpreteur.h"

/*
** gerer guts.nfa avec une pile
*/
void execute_mot_avec_debuggeur(int nb)
{
     t_cfa mot, ad;
     t_nfa nfa;
     gchar buf[200];
     GtkTextIter fin;


     buf[0] = '\0';
     gtk_text_buffer_set_text(bf_debug,"", -1);
     mot = guts.dictionnaire.tab[nb].donnee;
     nfa = (t_nfa) mot[0].adresse;
     if (est_primitif(nfa->info))
     {

          printf(">>> J\'execute la fct de %s (%p)\n", nfa->nom, nfa->fp);
          sprintf(buf, ">>> J\'execute la fct de %s (%p)\n", nfa->nom, nfa->fp);
          gtk_text_buffer_get_end_iter(bf_debug, &fin);
          gtk_text_buffer_insert(bf_debug, &fin, buf, -1);
          buf[0] = '\0';
          nfa->fp();
          return ;
     }

     reinit_pile(&guts.pile_retour);
     guts.nfa = nfa;
     gtk_text_buffer_get_end_iter(bf_debug, &fin);
     gtk_text_buffer_insert(bf_debug, &fin,
                            "================= Etat initial ===================", -1);
     gtk_text_buffer_get_end_iter(bf_debug, &fin);
     gtk_text_buffer_insert(bf_debug, &fin, "\npile donnees ", -1);
     printf("\n=========================== Etat initial ==============================");
     printf("\npile donnees "); afficher_pile(guts.pile_donnees); afficher_pile_buffer(guts.pile_donnees, bf_debug);
     gtk_text_buffer_get_end_iter(bf_debug, &fin);
     gtk_text_buffer_insert(bf_debug, &fin, "\npile retour ", -1);
     printf("\npile retour  "); afficher_pile(guts.pile_retour); afficher_pile_buffer(guts.pile_retour, bf_debug);
     gtk_text_buffer_get_end_iter(bf_debug, &fin);
     gtk_text_buffer_insert(bf_debug, &fin, "\n", -1);
     printf("\n");

     empiler(&guts.pile_retour, ad_to_elt(mot+1, CFA));
     do {
          guts.IP = (t_cfa) sommet(&guts.pile_retour).adresse;
          depiler(&guts.pile_retour);
          while ((!guts.erreur) && ((*guts.IP).adresse != NULL))
          {
               ad = (t_cfa) (*guts.IP).adresse;
               nfa = (t_nfa) ad[0].adresse;
               if (est_primitif(nfa->info))
               {
                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin,
                                           "#1 =========================================", -1);
                    printf("#1 =====================================================================");
                    guts.PC = guts.IP;
                    printf("\n>>> J\'execute la fct de %s (%p)\n", nfa->nom, nfa->fp);
                    sprintf(buf, "\n>>> J\'execute la fct de %s (%p)\n", nfa->nom, nfa->fp);
                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, buf, -1);
                    nfa->fp();
                    ++guts.IP;
                    buf[0] = '\0';

                    /* affichage */
                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, "Mot courant : ", -1);
                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, guts.nfa->nom, -1);
                    printf("Mot courant : %s", guts.nfa->nom);

                    printf("\t\tPC : %s", nfa->nom);
                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, "\t\tPC : ", -1);

                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, nfa->nom, -1);

                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, "\t\tIP : ", -1);
                    printf("\t\tIP : "); afficher_elt(*guts.IP); afficher_elt_buffer(*guts.IP, bf_debug);

                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, "\npile donnees : ", -1);
                    printf("\npile donnees ");
                    afficher_pile(guts.pile_donnees); afficher_pile_buffer(guts.pile_donnees, bf_debug);
                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, "\npile retour : ", -1);
                    printf("\npile retour  ");
                    afficher_pile(guts.pile_retour); afficher_pile_buffer(guts.pile_retour, bf_debug);
                    printf("\n");
                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, "\n", -1);
               }
               else
               {
                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin,
                                           "#2 =========================================\n", -1);
                    printf("#2 =====================================================================\n");
                    guts.PC = guts.IP;
                    empiler(&guts.pile_retour, ad_to_elt(guts.IP+1, HANDLE));
                    guts.IP = (t_cfa) (*guts.IP).adresse;
                    guts.nfa = (*guts.IP).adresse;
                    ++guts.IP; /* mot[1] */

                    /* affichage */
                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, "Mot courant : ", -1);
                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, guts.nfa->nom, -1);
                    printf("Mot courant : %s", guts.nfa->nom);

                    printf("\t\tPC : %s", nfa->nom);
                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, "\t\tPC : ", -1);

                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, nfa->nom, -1);

                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, "\t\tIP : ", -1);
                    printf("\t\tIP : "); afficher_elt(*guts.IP); afficher_elt_buffer(*guts.IP, bf_debug);

                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, "\npile donnees : ", -1);
                    printf("\npile donnees ");   afficher_pile(guts.pile_donnees);
                    afficher_pile_buffer(guts.pile_donnees, bf_debug);
                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, "\npile retour : ", -1);
                    printf("\npile retour  "); afficher_pile(guts.pile_retour);
                    afficher_pile_buffer(guts.pile_retour, bf_debug);
                    printf("\n");
                    gtk_text_buffer_get_end_iter(bf_debug, &fin);
                    gtk_text_buffer_insert(bf_debug, &fin, "\n", -1);
               }
          }
     } while ((!guts.erreur) && (!pile_vide(&guts.pile_retour)));
     gtk_text_buffer_get_end_iter(bf_debug, &fin);
     gtk_text_buffer_insert(bf_debug, &fin,
                            "\n================ Etat final ====================", -1);
     gtk_text_buffer_get_end_iter(bf_debug, &fin);
     gtk_text_buffer_insert(bf_debug, &fin, "\npile donnees ", -1);
     printf("\n=========================== Etat final ==============================");
     printf("\npile donnees "); afficher_pile(guts.pile_donnees);
     afficher_pile_buffer(guts.pile_donnees, bf_debug);
     gtk_text_buffer_get_end_iter(bf_debug, &fin);
     gtk_text_buffer_insert(bf_debug, &fin, "\npile retour ", -1);
     printf("\npile retour  "); afficher_pile(guts.pile_retour);
     afficher_pile_buffer(guts.pile_retour, bf_debug);
     printf("\n");
     gtk_text_buffer_get_end_iter(bf_debug, &fin);
     gtk_text_buffer_insert(bf_debug, &fin, "\n", -1);

     reinit_pile(&guts.pile_retour);
}

/*
** gerer guts.nfa avec une pile
*/
void execute_mot_sans_debuggeur(int nb)
{
     t_cfa mot, tt_mot, ad;
     t_nfa nfa;

     mot = guts.dictionnaire.tab[nb].donnee;
     reinit_pile(&guts.pile_retour);

     nfa = (t_nfa) mot[0].adresse;
     guts.nfa = nfa;
     if (est_primitif(nfa->info))
     {
          nfa->fp();
          return ;
     }

     empiler(&guts.pile_retour, ad_to_elt(mot+1, CFA));
     do {
          guts.IP = (t_cfa) sommet(&guts.pile_retour).adresse;
          depiler(&guts.pile_retour);
          while ((!guts.erreur) && ((*guts.IP).adresse != NULL))
          {
               ad = (t_cfa) (*guts.IP).adresse;
               nfa = (t_nfa) ad[0].adresse;
               if (est_primitif(nfa->info))
               {
                    guts.PC = guts.IP;
                    nfa->fp();
                    ++guts.IP;
               }
               else
               {
                    guts.PC = guts.IP;
                    tt_mot = (t_cfa) (*guts.IP).adresse;
                    guts.nfa = (t_nfa) tt_mot[0].adresse;

                    empiler(&guts.pile_retour, ad_to_elt(guts.IP+1, CFA));
                    guts.IP = (t_cfa) (*guts.IP).adresse;
                    ++guts.IP; /* mot[1] */
               }
          }
     } while ((!guts.erreur) && (!pile_vide(&guts.pile_retour)));
     reinit_pile(&guts.pile_retour);
}

t_erreur parser_tampon(gchar ** tampon)
{
//     t_cfa mot;
     int nbpar = 0;   // nombre de parentheses
     int lg = -1;     // longueur du token
     int nb, entier;
     t_cfa mot; t_nfa nfa;
     float reel;
     t_cf elt;

     while ((!guts.erreur) && (*tampon != '\0'))
     {
          nb = -1;
          guts.mytoken = my_cut(tampon, guts.mytoken, &guts.iter);
          lg = strlen(guts.mytoken);

          if (guts.mytoken[0] == '\0') /* si token == "" */
               return CORRECT;
          else if (lg > LONGUEUR_NOM_MOT) /* longueur du nom d'un mot FORTH <= 64 */
               return ERREUR_CAR_TROP_GRAND;
          else if ((lg == 1) && (guts.mytoken[0] == '(')) /* debut d'1 commentaire FORTH */
               ++nbpar;
          else if ((lg == 1) && (guts.mytoken[0] == ')')) /* fin d'1 commentaire FORTH */
          {
               --nbpar;
               if (nbpar < 0)
                    return ERREUR_PAREN_FERMANTE;
          }
          /*
          ** LE PLUS IMPORTANT : on n'est pas dans un commentaire
          ** on interprete alors le token.
          */

          else if (nbpar == 0)
          {
               nb = chercher_donnee_hash(&guts.dictionnaire, guts.mytoken);
               if (nb == -1)
               {
                    if (chaine_to_int(guts.mytoken, &entier)) /* est-ce un entier ? */
                         elt = int_to_elt(entier);
                    else if (chaine_to_float(guts.mytoken, &reel)) /* est-ce un reel ? */
                         elt = float_to_elt(reel);
                    /* est-ce une cellule ? */
#if 0
                    else if (((guts.mytoken[0] == 'A') || (guts.mytoken[0] == 'S') || (guts.mytoken[0] == 'Z'))
                             && (chaine_to_int(guts.mytoken+1,&entier)))
                    {
                         switch (guts.mytoken[0])
                         {
                         case 'A' :
                              elt = arc_to_elt(entier);
                              break;
                         case 'S' :
                              elt = sommet_to_elt(entier);
                              break;
                         case 'Z' :
                              elt = zone_to_elt(entier);
                              break;
                         }
                    }
#endif
                    else return ERREUR_MOT_INCONNU;

                    switch (guts.mode)
                    {
                    case MODE_EXECUTION:
                         empiler(&guts.pile_donnees, elt);
                         empiler_gtk(store_pile, elt, &iter_pile);
                         break;
                    case MODE_COMPILATION:
                         // if ((elt.type == SOM) || (elt.type == ARC) || (elt.type == ZONE))
                         //    mot = chercher_mot_hash(&guts.dictionnaire, "CELLULE");
                         //else
                         mot = chercher_mot_hash(&guts.dictionnaire, "LITERAL");
                         ajouter_adresse(guts.dernier_mot_cree, mot, CFA);
                         ajouter_elt_dans_mot(guts.dernier_mot_cree, elt);
                         break;
                    }
               }

               /*
               ** execution ou compilation du
               ** token selon les etats du token
               ** ou de l'inter
               */
               else
               {
                    switch (guts.mode)
                    {
                    case MODE_COMPILATION:
                         mot = guts.dictionnaire.tab[nb].donnee;
                         nfa = (t_nfa) mot[0].adresse;
                         if (! est_immediat(nfa->info))
                              ajouter_adresse(guts.dernier_mot_cree, mot, CFA);
                         else
                              execute_mot(nb);
                         break;
                    case MODE_EXECUTION:
                         execute_mot(nb);
                         break;
                    }
               }
          }
     }

     if (nbpar > 0) /* trop de parentheses ouvrantes */
          return ERREUR_PAREN_OUVRANTE;
     return CORRECT;
}
