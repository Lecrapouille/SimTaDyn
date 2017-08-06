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

#include "prompt.h"

void action_prompt(GtkEntry *entry, const gchar * texte)
{
  t_cfa mot;
  char * ch;
  char buffer[100];
  float reel;
  int i;

  switch (guts.cellule)
    {
    case SOMMET :
      switch (guts.champ)
        {
          case ID :
          gtk_entry_set_text(entry, "<< Champ non modifiable !! >>");
          return;
        case NOM :
          gtk_entry_set_text(entry, "<< Champ non modifiable !! >>");
          return ;
        case POSX :
          if (chaine_to_float(texte, &reel))
            gtk_entry_set_text(entry, "Reel");
          else
            gtk_entry_set_text(entry, "Pas reel");
          gtk_entry_set_text(entry, "<< Champ non modifiable !! >>");
          return ;
        case POSY:
          if (chaine_to_float(texte, &reel))
            gtk_entry_set_text(entry, "Reel");
          else
            gtk_entry_set_text(entry, "Pas reel");
            return;
          case POSZ:
          if (chaine_to_float(texte, &reel))
            {
              buffer[0]='\0';
              ch = nouvelle_chaine();
              ch = g_ascii_strup(texte, -1);
              for (i=0;i<nbr_selec_som;i++) //pour chaque sommet de la pile
                {
                  sprintf (buffer,"UPDATE som set coord_z = %s WHERE id = %d",ch,guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier);
                  mysql_execreq(buffer);
                  buffer[0] = '\0';
                }
              TsommetInit();
              mysql_db_som();
              deselection_som();
              gtk_entry_set_text(entry, "");
              g_free(ch);
            }
          else
            {
              gtk_entry_set_text(entry, "Pas reel");
              return;
            }
            break;
            case FORTH :
          ch = nouvelle_chaine();
          ch = g_ascii_strup(texte, -1);
          mot = chercher_mot_hash(&guts.dictionnaire, ch);
          if ((mot == NULL) && (ch[0] != '\0'))
            {
              gtk_entry_set_text(entry, "<< Le mot n'existe pas dans le dictionnaire !! >>");
              return ;
            }
          buffer[0] = '\0';
          for (i=0;i<nbr_selec_som;i++) //pour chaque sommet de la pile
            {
              sprintf (buffer,"\"%s\"",ch);
              mysql_modif_mot_forth(buffer, guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier, "som");
              buffer[0] = '\0';
            }
          TsommetInit();
          mysql_db_som();
          deselection_som();
          gtk_entry_set_text(entry, "");
          g_free(ch);
          break;
        default: printf("\007Erreur case 'guts.champ' dans 'action_prompt' dans callbacks.c\n");
          return;
        }
      break;
      case ARC:
      switch (guts.champ)
        {
          case ID :
          gtk_entry_set_text(entry, "<< Champ non modifiable !! >>");
          return;
        case NOM :
          gtk_entry_set_text(entry, "<< Champ non modifiable !! >>");
          return ;
        case POSX :
          if (chaine_to_float(texte, &reel))
            gtk_entry_set_text(entry, "Reel");
          else
            gtk_entry_set_text(entry, "Pas reel");
          return ;
        case POSY:
          if (chaine_to_float(texte, &reel))
            gtk_entry_set_text(entry, "Reel");
          else
            gtk_entry_set_text(entry, "Pas reel");
          return;
        case POSZ:
          if (chaine_to_float(texte, &reel))
            gtk_entry_set_text(entry, "Reel");
          else
            gtk_entry_set_text(entry, "Pas reel");
            return;
        case COUT :
          if (chaine_to_float(texte, &reel))
            {
              buffer[0]='\0';
              ch = nouvelle_chaine();
              ch = g_ascii_strup(texte, -1);
              for (i=0;i<nbr_selec_arete;i++) //pour chaque arete de la pile
                {
                  sprintf (buffer,"UPDATE arc set cout = %s WHERE id = %d",ch,guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier);
                  mysql_execreq(buffer);
                  buffer[0] = '\0';
                }
              TareteInit();
              mysql_db_arc();
              deselection_arete();
              gtk_entry_set_text(entry, "");
              g_free(ch);
            }
          else
            {
              gtk_entry_set_text(entry, "Pas reel");
              return;
            }
            break;
        case FORTH :
          ch = nouvelle_chaine();
          ch = g_ascii_strup(texte, -1);
          mot = chercher_mot_hash(&guts.dictionnaire, ch);
          if ((mot == NULL) && (ch[0] != '\0'))
            {
              gtk_entry_set_text(entry, "<< Le mot n'existe pas dans le dictionnaire !! >>");
              return ;
            }
          buffer[0] = '\0';
          for (i=0;i<nbr_selec_arete;i++) //pour chaque arete de la pile
            {
              sprintf (buffer,"\"%s\"",ch);
              mysql_modif_mot_forth(buffer, guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier, "arc");
              buffer[0] = '\0';
            }
          TareteInit();
          mysql_db_arc();
          deselection_arete();
          gtk_entry_set_text(entry, "");
          g_free(ch);
          break;
        default: printf("\007Erreur case 'guts.champ' dans 'action_prompt' dans callbacks.c\n");
          return;
        }
      break;
      case ZONE:
      switch (guts.champ)
        {
          case ID :
          gtk_entry_set_text(entry, "<< Champ non modifiable !! >>");
          return ;
        case NOM :
          gtk_entry_set_text(entry, "<< Champ non modifiable !! >>");
          return ;
        case POSX :
          if (chaine_to_float(texte, &reel))
            gtk_entry_set_text(entry, "Reel");
          else
            gtk_entry_set_text(entry, "Pas reel");
          gtk_entry_set_text(entry, "<< Champ non modifiable !! >>");
          return ;
        case FORTH :
          ch = nouvelle_chaine();
          ch = g_ascii_strup(texte, -1);
          mot = chercher_mot_hash(&guts.dictionnaire, ch);
          if ((mot == NULL) && (ch[0] != '\0'))
            {
              gtk_entry_set_text(entry, "<< Le mot n'existe pas dans le dictionnaire !! >>");
              return ;
            }
          buffer[0] = '\0';
          for (i=0;i<nbr_selec_zone;i++) //pour chaque zone de la pile
            {
              sprintf (buffer,"\"%s\"",ch);
              mysql_modif_mot_forth(buffer, guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier, "zone");
              buffer[0] = '\0';
            }
          TzoneInit();
          mysql_db_zone();
          deselection_zone();
          gtk_entry_set_text(entry, "");
          g_free(ch);
          break;
        default: printf("\007Erreur case 'guts.champ' dans 'action_prompt' dans callbacks.c\n");
          return;
        }
        break;
    default : printf("\007Erreur 'guts.cellule' dans 'action_prompt' dans callbacks.c\n");
      return;
    }

  gdk_window_invalidate_rect (drawingarea->window, &drawingarea->allocation, FALSE);
}
