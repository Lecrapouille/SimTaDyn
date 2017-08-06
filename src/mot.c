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

#include "mot.h"

inline void change_masque(octet * info, octet masque)
{
     *info = *info ^ masque;
}

/*
** retourne le caractere immediat d'un mot
*/
inline bool est_immediat(octet info)
{
     info = info >> 7;
     return (info == 1);
}

inline bool est_primitif(octet info)
{
     info = info << 1;
     info = info >> 7;
     return (info == 1);
}

/*
** donne la longueur du nom d'un mot FORTH
*/
inline octet longueur(octet info)
{
     octet temp = info;

     info = info << 2;
     info = info >> 2;

     return temp;
}

inline void change_longueur(octet nb, octet * info)
{
     *info = *info >> 7;
     *info = *info << 7;
     *info = *info | nb;
}

inline t_nfa nouveau_nfa(gchar * nom, octet masque, FONCTION_C)
{
     t_nfa nfa;

     nfa = (t_nfa) xmalloc(sizeof(t_nf));
     nfa->nom = nom;//g_strdup(nom);
     nfa->info = masque;
     change_longueur(strlen(nom), &nfa->info);
     nfa->fp = fp;

     return nfa;
}

t_cfa nouveau_mot(gchar * nom, octet masque, FONCTION_C)
{
     t_cfa mot;
     t_nfa nfa;

     nfa = nouveau_nfa(nom, masque, fp);
     nfa->taille_utilisee = 1;

     if (fp == NULL)
          nfa->taille_allouee = TAILLE_VECTEUR_MOT;
     else
     {
          change_masque(&nfa->info, PRIMITIVE_BIT);
          nfa->taille_allouee = 2;
     }

     mot = (t_cfa) xmalloc(nfa->taille_allouee*sizeof(t_cf));
     mot[0].adresse = nfa;
     mot[0].type = NFA;
     mot[1].adresse = NULL;
     mot[1].type = AD_NULL;

     return mot;
}

bool nouveau_mot_courant()
{
     t_cfa mot;

     guts.mytoken = my_cut(&guts.tampon, guts.mytoken, &guts.iter);
     if (chercher_donnee_hash(&guts.dictionnaire, guts.mytoken) != -1)
     {
          erreur_parser(ERREUR_MOT_DEJA_CONNU, bf_nouv, bf_res);
          return false;
     }

     mot = nouveau_mot(guts.mytoken, EMPTY_WORD, NULL);
     guts.dernier_mot_cree = mot;

     return true;
}

t_cfa adresse_fin_mot(t_cfa mot)
{
     t_nfa nfa;

     nfa = (t_nfa) mot[0].adresse;
     return mot + nfa->taille_utilisee;
}

void ajouter_elt_dans_mot(t_cfa mot, t_cf elt)
{
     t_nfa nfa;

     nfa = (t_nfa) mot[0].adresse;
     mot[nfa->taille_utilisee] = elt;

     ++nfa->taille_utilisee;
     if (nfa->taille_utilisee >= nfa->taille_allouee)
     {
          nfa->taille_allouee += TAILLE_VECTEUR_MOT;
          mot = xrealloc(mot, nfa->taille_allouee*sizeof(t_cf));
     }
     mot[nfa->taille_utilisee].adresse = NULL;
     mot[nfa->taille_utilisee].type = AD_NULL;
}

void ajouter_adresse(t_cfa mot, void * ad, t_type type)
{
     t_nfa nfa;

     nfa = (t_nfa) mot[0].adresse;

     mot[nfa->taille_utilisee].adresse = ad;
     mot[nfa->taille_utilisee].type = type;

     ++nfa->taille_utilisee;
     if (nfa->taille_utilisee >= nfa->taille_allouee)
     {
          nfa->taille_allouee += TAILLE_VECTEUR_MOT;
          mot = xrealloc(mot, nfa->taille_allouee*sizeof(t_cf));
     }
     mot[nfa->taille_utilisee].adresse = NULL;
     mot[nfa->taille_utilisee].type = AD_NULL;
}

void detruire_mot(t_cfa mot)
{
     t_nfa nfa;

     nfa = (t_nfa) mot[0].adresse;
     //g_free(nfa->nom);
     free(nfa);
     free(mot);
}

void afficher_mot(t_cfa mot)
{
     t_nfa nfa;
     unsigned int i;

     nfa = (t_nfa) mot[0].adresse;
     printf(" %10p | %4x %9s |  (%i)", mot, nfa->info, nfa->nom, nfa->taille_utilisee);
            if (nfa->fp != NULL)
                 printf("%10p | ", nfa->fp);
            else
                 printf("%10s | ", " ");
//     afficher_type(mot[0].type);

     for (i=1; i < nfa->taille_utilisee; ++i)
     {
          afficher_elt(mot[i]);
//        afficher_type(mot[i].type);
//        printf("\t");

     }
}
