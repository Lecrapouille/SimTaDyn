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

#include "elt_pile.h"

/* ************** t_cfa************* */
void afficher_type(t_type type)
{
     switch(type)
     {
     case ENTIER :
          printf("TYPE_ENTIER");
          break;
     case REEL :
          printf("TYPE_REEL");
          break;
     case ADRESSE :
          printf("TYPE_ADRESSE");
          break;
     case NFA :
          printf("TYPE_NFA");
          break;
     case CFA :
          printf("TYPE_CFA");
          break;
     case AD_NULL :
          printf("TYPE_FIN_DE_MOT");
          break;
     case AD_ENTIER :
          printf("TYPE_ADRESSE_ENTIER");
          break;
     case AD_REEL :
          printf("TYPE_ADRESSE_REEL");
          break;
     case AD_STRING :
          printf("TYPE_ADRESSE_STRING");
          break;
     case HANDLE :
          printf("TYPE_HANDLE");
          break;
     default :
          guts.erreur = true;
          printf("\007Erreur 'afficher_type' dans elt_pile.c !!\n");
          break;
     }
}

void afficher_elt(t_cf elt)
{
     t_cfa ad; t_nfa nfa;

     switch(elt.type)
     {
     case ENTIER :
          printf("%d ", elt.entier);
          break;
     case REEL :
          printf("%f ", elt.reel);
          break;
     case AD_ENTIER :
          printf("%p (ad entier) ", elt.adresse);
          break;
     case AD_REEL :
          printf("%p (ad reel) ", elt.adresse);
          break;
     case ADRESSE :
          printf("%p ", elt.adresse);
          break;
     case NFA :
          printf("\007Erreur type NFA dans elt_pile !!\n");
          break;
     case CFA :
          nfa = (t_nfa) (*((t_cfa) elt.adresse)).adresse;
          printf("%s ", nfa->nom);
          break;
     case AD_NULL :
          printf("(fin de mot) ");
          break;
     case AD_STRING :
          printf("\"%s\" ", (char*) elt.adresse);
          break;
     case HANDLE :
          ad = (t_cfa) (*((t_cfa) elt.adresse)).adresse;
          if (ad == NULL)
               printf("(fin de mot) ");
          else
          {
               nfa = (t_nfa) (*ad).adresse;
               printf("%s ", nfa->nom);
          }
          break;
     default:
          guts.erreur = true;
          printf("\007Erreur afficher_elt dans elt_pile.c !!\n");
          break;
     }
}
void afficher_elt_buffer(t_cf elt, GtkTextBuffer *bf)
{
     t_cfa ad; t_nfa nfa;
     gchar* buf;
     GtkTextIter fin;

     switch(elt.type)
     {
     case ENTIER :
          buf = (gchar *) xmalloc(50*sizeof(gchar));
          buf[0] = '\0';
          sprintf(buf, "%d ", elt.entier);
          gtk_text_buffer_get_end_iter(bf, &fin);
          gtk_text_buffer_insert(bf, &fin, buf, -1);
          g_free(buf);
          break;
     case REEL :
          buf = (gchar *) xmalloc(50*sizeof(gchar));
          buf[0] = '\0';
          sprintf(buf, "%f ", elt.reel);
          gtk_text_buffer_get_end_iter(bf, &fin);
          gtk_text_buffer_insert(bf, &fin, buf, -1);
          g_free(buf);
          break;
     case AD_ENTIER :
          buf = (gchar *) xmalloc(50*sizeof(gchar));
          buf[0] = '\0';
          sprintf(buf, "%p (ad entier) ", elt.adresse);
          gtk_text_buffer_get_end_iter(bf, &fin);
          gtk_text_buffer_insert(bf, &fin, buf, -1);
          g_free(buf);
          break;
     case AD_REEL :
          buf = (gchar *) xmalloc(50*sizeof(gchar));
          buf[0] = '\0';
          sprintf(buf, "%p (ad reel) ", elt.adresse);
          gtk_text_buffer_get_end_iter(bf, &fin);
          gtk_text_buffer_insert(bf, &fin, buf, -1);
          g_free(buf);
          break;
     case ADRESSE :
          buf = (gchar *) xmalloc(50*sizeof(gchar));
          buf[0] = '\0';
          sprintf(buf, "%p ", elt.adresse);
          gtk_text_buffer_get_end_iter(bf, &fin);
          gtk_text_buffer_insert(bf, &fin, buf, -1);
          g_free(buf);
          break;
     case NFA :
          gtk_text_buffer_get_end_iter(bf, &fin);
          gtk_text_buffer_insert(
               bf, &fin,
               "Erreur NFA dans 'afficher_elt_buffer' dans elt_pile.c !!\n",
               -1);
          break;
     case CFA ://MOT_FORTH :
          nfa = (t_nfa) (*((t_cfa) elt.adresse)).adresse;
          gtk_text_buffer_get_end_iter(bf, &fin);
          gtk_text_buffer_insert(bf, &fin, nfa->nom, -1);
          gtk_text_buffer_get_end_iter(bf, &fin);
          gtk_text_buffer_insert(bf, &fin, " ", -1);
          break;
     case AD_NULL :
          gtk_text_buffer_get_end_iter(bf, &fin);
          gtk_text_buffer_insert(bf, &fin, "(fin) ", -1);
          break;
     case AD_STRING :
          buf = (gchar *) xmalloc(50*sizeof(gchar));
          buf[0] = '\0';
          sprintf(buf, "\"%s\" ", (char*) elt.adresse);
          gtk_text_buffer_get_end_iter(bf, &fin);
          gtk_text_buffer_insert(bf, &fin, buf, -1);
          g_free(buf);
          break;
     case HANDLE :
          ad = (t_cfa) (*((t_cfa) elt.adresse)).adresse;
          if (ad == NULL)
          {
               gtk_text_buffer_get_end_iter(bf, &fin);
               gtk_text_buffer_insert(bf, &fin, "(fin de mot) ", -1);
          }
          else
          {
               nfa = (t_nfa) (*ad).adresse;
               gtk_text_buffer_get_end_iter(bf, &fin);
               gtk_text_buffer_insert(bf, &fin, nfa->nom, -1);
          }
          break;
     default:
          guts.erreur = true;
          gtk_text_buffer_get_end_iter(bf, &fin);
          gtk_text_buffer_insert(bf, &fin, "Erreur afficher_elt dans elt_pile.c !!\n",-1);
          break;
     }
}

gchar* elt_to_string(t_cf elt)
{
     t_cfa ad; t_nfa nfa;
     gchar* buf;

     switch(elt.type)
     {
     case ENTIER :
          buf = (gchar *) xmalloc(50*sizeof(gchar));
          buf[0] = '\0';
          sprintf(buf, "%d ", elt.entier);
          return buf;
          break;
     case REEL :
          buf = (gchar *) xmalloc(50*sizeof(gchar));
          buf[0] = '\0';
          sprintf(buf, "%f ", elt.reel);
          return buf;
          break;
     case AD_ENTIER :
          buf = (gchar *) xmalloc(50*sizeof(gchar));
          buf[0] = '\0';
          sprintf(buf, "%p (ad entier) ", elt.adresse);
          return buf;
          break;
     case AD_REEL :
          buf = (gchar *) xmalloc(50*sizeof(gchar));
          buf[0] = '\0';
          sprintf(buf, "%p (ad reel) ", elt.adresse);
          return buf;
          break;
     case AD_STRING :
          buf = (gchar *) xmalloc(50*sizeof(gchar));
          buf[0] = '\0';
          sprintf(buf, "%p (ad string) ", elt.adresse);
          return buf;
          break;
     case ADRESSE :
          buf = (gchar *) xmalloc(50*sizeof(gchar));
          buf[0] = '\0';
          sprintf(buf, "%p ", elt.adresse);
          return buf;
          break;
     case NFA :
          printf("\007 Erreur NFA dans 'afficher_elt' dans elt_pile !!\n");
          return NULL;
          break;
     case CFA ://MOT_FORTH :
          nfa = (t_nfa) (*((t_cfa) elt.adresse)).adresse;
          return nfa->nom;
          break;
     case AD_NULL :
          buf = (gchar *) xmalloc(50*sizeof(gchar));
          buf[0] = '\0';
          strcpy(buf, "(fin de mot) ");
          return buf;
          break;
     case HANDLE :
          buf = (gchar *) xmalloc(50*sizeof(gchar));
          buf[0] = '\0';
          ad = (t_cfa) (*((t_cfa) elt.adresse)).adresse;
          if (ad == NULL)
          {
               strcpy(buf, "(fin de mot)");
               return buf;
          }
          else
          {
               nfa = (t_nfa) (*ad).adresse;
               return nfa->nom;
          }
          break;
     default :
          guts.erreur = true;
          printf("\007Erreur 'elt_to_string' dans elt_pile.c !!\n");
          return NULL;
          break;
     }
}


inline t_cf int_to_elt(int val)
{
     t_cf elt;

     elt.entier = val;
     elt.type = ENTIER;

     return elt;
}

inline t_cf float_to_elt(float val)
{
     t_cf elt;

     elt.reel = val;
     elt.type = REEL;

     return elt;
}

inline t_cf ad_to_elt(void * val, t_type type)
{
     t_cf elt;

     elt.adresse = val;
     elt.type = type;

     if ((type == ENTIER) || (type == REEL))
     {
          printf("\007Erreur : le type ");
          afficher_type(type);
          printf("n'est pas une adresse !!\n");
          guts.erreur = true;
     }

     return elt;
}

#if 0
inline t_cf arc_to_elt(int val)
{
     t_cf elt;

     elt.entier = val;
     elt.type = ARC;

     return elt;
}

inline t_cf sommet_to_elt(int val)
{
     t_cf elt;

     elt.entier = val;
     elt.type = SOMMET;

     return elt;
}

inline t_cf zone_to_elt(int val)
{
     t_cf elt;

     elt.entier = val;
     elt.type = ZONE;

     return elt;
}

inline bool est_cellule(t_cf elt)
{
     return ((elt.type == SOMMET) ||
             (elt.type == ARC) ||
             (elt.type == ZONE));
}
#endif
