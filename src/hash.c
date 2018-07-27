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

#include "hash.h"

void print_etat(t_etat etat)
{
     switch (etat)
     {
     case LIBRE :
          printf("LIBRE");
          break;
     case OCCUPE :
          printf("OCCUPE");
          break;
     case VIDE :
          printf("VIDE");
          break;
     default :
          printf("ERRERRR");
     }
}

/*
**  This hash function has been taken from an Article in Dr Dobbs Journal.
*/
unsigned fct_hash(char * key)
{
        int       len;
        unsigned  hash;
        int       i;

        len = strlen(key);
        for (hash=0, i=0 ; i<len ; i++) {
                hash += (unsigned)key[i] ;
                hash += (hash<<10);
                hash ^= (hash>>6) ;
        }
        hash += (hash <<3);
        hash ^= (hash >>11);
        hash += (hash <<15);
        return (hash %  MAX_ELT_HASH);
}

inline bool etat_est(t_hash *hash, int i, t_etat etat)
{
     return ((*hash).tab[i].etat == etat);
}


t_hash nouveau_hash()
{
     int i;
     t_hash hash;

     hash.reserve = MAX_ELT_HASH;
     hash.nb_elt = 0;
     for (i=0; i<MAX_ELT_HASH; ++i)
       {
         hash.tab[i].lien = -1;
         hash.tab[i].etat = VIDE;
         hash.tab[i].cle = NULL;
         hash.tab[i].donnee = NULL;
       }
     return hash;
}

bool ajouter_donnee_hash(t_hash *hash, void* donnee, char* cle)
{
     int i, lib;

     i = fct_hash(cle);
     lib = 0;

     ++(*hash).nb_elt;
     if ((*hash).nb_elt >= MAX_ELT_HASH)
     {
          GtkTextIter fin;

          printf("***\007ERREUR : la table de hachage est pleine !!\n");
          gtk_text_buffer_get_end_iter(bf_res, &fin);
          gtk_text_buffer_insert(bf_res, &fin, "ERREUR : la table de hachage est pleine ***\n",-1);
          return false;
     }

     if (etat_est(hash, i, VIDE))
     {
          (*hash).tab[i].etat   = OCCUPE;
          (*hash).tab[i].donnee = donnee;
          (*hash).tab[i].cle    = cle;
          (*hash).tab[i].lien   = MAX_ELT_HASH;
          return true;
     }
     while ((etat_est(hash,i,LIBRE) || (etat_est(hash,i,OCCUPE) && (strcmp((*hash).tab[i].cle,cle) != 0)))
            && ((*hash).tab[i].lien != MAX_ELT_HASH))
     {
          if (etat_est(hash,i,LIBRE) && (lib==0))
          {
               lib = i;
               (*hash).tab[lib].etat   = OCCUPE;
               (*hash).tab[lib].donnee = donnee;
               (*hash).tab[lib].cle    = cle;
          }
          i = (*hash).tab[i].lien;
     }

     if (etat_est(hash,i,OCCUPE) && (strcmp((*hash).tab[i].cle,cle) == 0))
     {
          if (lib > 0)
               (*hash).tab[i].etat = LIBRE;
          return true;
     }
     else
     {
          if (etat_est(hash,i,LIBRE))
          {
               if (lib == 0)
               {
                    (*hash).tab[i].etat   = OCCUPE;
                    (*hash).tab[i].donnee = donnee;
                    (*hash).tab[i].cle    = cle;
               }
               return true;
          }
          else
          {
               --(*hash).reserve;
               while (((*hash).reserve >= 0) && (!etat_est(hash,(*hash).reserve,VIDE)))
                    --(*hash).reserve;
               if ((*hash).reserve >= 0)
               {
                    (*hash).tab[i].lien = (*hash).reserve;
                    (*hash).tab[(*hash).reserve].etat   = OCCUPE;
                    (*hash).tab[(*hash).reserve].donnee = donnee;
                    (*hash).tab[(*hash).reserve].cle    = cle;
                    (*hash).tab[(*hash).reserve].lien   = MAX_ELT_HASH;
                    return true;
               }
               else
                    return false;
          }
     }
}

int chercher_donnee_hash(t_hash *hash, char * cle)
{
     int i;

     i = fct_hash(cle);
     while ((etat_est(hash,i,LIBRE) || (etat_est(hash,i,OCCUPE) && (strcmp((*hash).tab[i].cle,cle) != 0)))
            && ((*hash).tab[i].lien != MAX_ELT_HASH))
          i = (*hash).tab[i].lien;
     if (etat_est(hash,i,VIDE) || etat_est(hash,i,LIBRE) || (strcmp((*hash).tab[i].cle,cle) != 0))
          return -1;
     else
          return i;
}

void supprimer_mot_hash(t_hash *hash, char * cle)
{
     int i;
     t_cfa cfa;
     t_nfa nfa;

     i = chercher_donnee_hash(hash, cle);
     if (i != -1)
     {
          cfa = (t_cfa) (*hash).tab[i].donnee;
          nfa = (t_nfa) cfa[0].adresse;
          gtk_list_store_remove(GTK_LIST_STORE(cl_dico), &nfa->ad_gtk);
          detruire_mot((*hash).tab[i].donnee);
          (*hash).tab[i].donnee = NULL;
          (*hash).tab[i].cle = NULL;
          (*hash).tab[i].etat = LIBRE;
     }
}

void reinit_hash(t_hash *hash)
{
     int i;

     gtk_list_store_clear(GTK_LIST_STORE(cl_dico));
     for(i=0; i<MAX_ELT_HASH; ++i)
     {
          if ((*hash).tab[i].etat == OCCUPE)
               detruire_mot((*hash).tab[i].donnee);
          (*hash).tab[i].donnee = NULL;
          (*hash).tab[i].cle = NULL;
          (*hash).tab[i].etat = VIDE;
          (*hash).tab[i].lien = -1;
     }
     (*hash).reserve = MAX_ELT_HASH;
     (*hash).nb_elt = 0;
}

void ieme_reinit_hash(t_hash *hash, int i)
{
     t_cfa cfa;
     t_nfa nfa;

     if (i<MAX_ELT_HASH)
     {
          cfa = (t_cfa) (*hash).tab[i].donnee;
          nfa = (t_nfa) cfa[0].adresse;
          gtk_list_store_remove(GTK_LIST_STORE(cl_dico), &nfa->ad_gtk);

          if ((*hash).tab[i].etat == OCCUPE)
               detruire_mot((*hash).tab[i].donnee);
          (*hash).tab[i].donnee = NULL;
          (*hash).tab[i].cle = NULL;
          (*hash).tab[i].etat = VIDE;
          (*hash).tab[i].lien = -1;
     }
     (*hash).reserve = MAX_ELT_HASH;
     (*hash).nb_elt = 0;
}

/* ******************************************************
**
**   SPECIFIQUE AU MOT FORTH
**
** *******************************************************
**/

void forcer_ajout_hash(t_hash *hash, void* donnee, char* cle)
{
     supprimer_mot_hash(hash, cle);
     ajouter_mot_hash_et_gtk(hash, donnee, cle);
}

bool ajouter_mot_hash_et_gtk(t_hash *hash, void* donnee, char* cle)
{
     int i = chercher_donnee_hash(hash, cle);

     if (i != -1)
          return false;

     i = ajouter_donnee_hash(hash, donnee, cle);
     if  (!i)
          return false;

     ajouter_mot_dans_visualdico(cl_dico, donnee);
     return true;
}

bool ajouter_mot_hash(t_hash *hash, void* donnee, char* cle)
{
     int i = chercher_donnee_hash(hash, cle);

     if (i != -1)
          return false;

     i = ajouter_donnee_hash(hash, donnee, cle);
     if  (!i)
          return false;
     else
          return true;
}

t_cfa chercher_mot_hash(t_hash *hash, char * cle)
{
     int nb;

     nb = chercher_donnee_hash(hash, cle);
     if (nb == -1)
          return NULL;
     else
          return ((t_cfa) (*hash).tab[nb].donnee);
}

void afficher_hash(t_hash *hash)
{
     int i, j;

     printf("\n+====+=======+============+================+=============+==============================================================================+\n");
       printf("| N° | Token |   Adresse  | Infos      Nom | Fonction C  | Liste de CFA \n");
       printf("+====+=======+============+================+=============+==============================================================================+\n");

     j = 0;
     for (i=0; i<MAX_ELT_HASH; ++i)
     {
          if ((*hash).tab[i].donnee != NULL)
          {
               ++j;
               printf("|%3d| %6d %6d |", j,  i, (*hash).tab[i].lien);
               afficher_mot((*hash).tab[i].donnee);
               printf("\n+====+=======+==============+==============+=============+==============================================================================+\n");
          }
     }
     printf("Il y a %d mots dans le dico\n", j);

}

void destroy_hash(t_hash * hash)
{
     int i;

     for(i=0; i<MAX_ELT_HASH; ++i)
     {
          if ((*hash).tab[i].donnee != NULL)
          {
               detruire_mot((*hash).tab[i].donnee);
               (*hash).tab[i].donnee = NULL;
          }
     }
}
