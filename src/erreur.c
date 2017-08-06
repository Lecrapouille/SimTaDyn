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

#include "erreur.h"

void erreur_parser(t_erreur erreur,
                   GtkTextBuffer *bf_nouv,
                   GtkTextBuffer *bf_res)
{
     GtkTextIter debut, fin;
     gchar * buf;

     guts.erreur = true;
     printf("\007");
     buf = (gchar *) xmalloc(500*sizeof(char));
     sprintf(buf, "*** Erreur caractères (%u/%u) --> (%u/%u) : ", guts.iter.oldx, guts.iter.oldy, guts.iter.x, guts.iter.y);
     printf("%s",buf);
     gtk_text_buffer_set_text(bf_res,"", -1);
     gtk_text_buffer_get_end_iter(bf_res, &fin);
     gtk_text_buffer_insert(bf_res, &fin, buf, -1);

     switch(erreur)
     {
     case ERREUR_CAR_TROP_GRAND :
          sprintf(buf, "le token \'%s\' est trop grand ***\n", guts.mytoken);
          gtk_text_buffer_insert(bf_res, &fin, buf, -1);
          break;
     case ERREUR_PAREN_FERMANTE :
          gtk_text_buffer_insert(bf_res, &fin, "parenthèse fermante en trop ***\n", -1);
          break;
     case ERREUR_PAREN_OUVRANTE :
          gtk_text_buffer_insert(bf_res, &fin, "parenthèse ouvrante en trop ***\n", -1);
          break;
     case ERREUR_MOT_INCONNU :
          sprintf(buf, "le token \'%s\' n\'est pas connu du dictionnaire ***\n", guts.mytoken);
          gtk_text_buffer_insert(bf_res, &fin, buf, -1);
          break;
     case ERREUR_MOT_DEJA_CONNU :
          sprintf(buf, "le token \'%s\' existe deja dans le dictionnaire ***\n", guts.mytoken);
          gtk_text_buffer_insert(bf_res, &fin, buf, -1);
          break;
     case CORRECT :
     default: printf("\007mauvais switch dans erreur.c\n");
     }

     printf("%s",buf);
     gtk_text_buffer_get_bounds(bf_nouv, &debut, &fin);
     buf = gtk_text_buffer_get_text(bf_nouv, &debut, &fin, FALSE);
     gtk_text_buffer_get_end_iter(bf_res, &fin);
     gtk_text_buffer_insert(bf_res, &fin, buf, -1);

     gtk_text_buffer_get_iter_at_line_offset(bf_res, &debut, guts.iter.oldy+1, guts.iter.oldx);
     gtk_text_buffer_get_iter_at_line_offset(bf_res, &fin, guts.iter.y+1, guts.iter.x);
     gtk_text_buffer_apply_tag(bf_res, balise_erreur, &debut, &fin);

     printf("%s\n",buf);
     g_free(buf);
}