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

#include "affichage.h"

void selec_zone()
{
  int i;
  for (i=0;i<nbr_zone;i++)
    {
      if (sqrt((float)((coordx-(int)zone[i].poignet.x)*(coordx-(int)zone[i].poignet.x)+(coordy-(int) zone[i].poignet.y)*(coordy-(int)zone[i].poignet.y)))<10)
        {
          zone_selec=zone[i].id;
          break;
        }
    }
}

void recherche_poignet_zone()
{
  int temp=0;
  float xmoy=0.0,ymoy=0.0;
  int nbr_moy=0;

  int i;
  t_liste l;
  l=listetemp;

  while(l!=NULL)
    {
      temp=l->valeur;
      for (i=0;i<nbr_arete;i++)
        {
          if (arete[i].id==temp)
            {
              nbr_moy++;
              xmoy=xmoy+arete[i].pos1.x;
              ymoy=ymoy+arete[i].pos1.y;
              xmoy=xmoy+arete[i].pos2.x;
              ymoy=ymoy+arete[i].pos2.y;
            }
        }
      l=l->suivant;
    }
  zone[nbr_zone-1].poignet.x=(xmoy)/(2*nbr_moy);
  zone[nbr_zone-1].poignet.y=(ymoy)/(2*nbr_moy);
}

void ajout_zone()
{
  int nbr_arete_temp;
  arete_selec=-1;
  nbr_zone++;
  recherche_poignet_zone();
  nbr_arete_temp=nbr_elt_liste (listetemp);
  mysql_insert_zone(0.0,0.0,1000.0,1000.0, zone[nbr_zone-1].poignet.x,zone[nbr_zone-1].poignet.y,nbr_arete_temp,listetemp);
  TzoneInit();
  mysql_db_zone();
  suppr_liste(&listetemp);
  zone_selec=-1;
  init_tab_selec();
  listetemp=NULL;
}

void suppr_zone()
{
  mysql_delete_zone();
  TzoneInit();
  mysql_db_zone();
  zone_selec=-1;
}


void tab_to_mysql_zone()
{
  printf ("debut tab_ti_mysql_zone\n");
  int i;
  char *buffer;
  buffer = malloc (sizeof (char)*200);
  for (i=0;i<nbr_zone;i++)
  {
    sprintf (buffer,"UPDATE zone set x_poig = %s, y_poig = %s, z_poig = %s, R = %s, G = %s, B = %s, A= %s, nbr_arete = %i, liste_arete = %s",
             floattostring(zone[i].poignet.x),
             floattostring(zone[i].poignet.y),
             floattostring(zone[i].poignet.z),
             floattostring(zone[i].densite.R),
             floattostring(zone[i].densite.G),
             floattostring(zone[i].densite.B),
             floattostring(zone[i].densite.A),
             zone[i].nbr_arete,
             liste_to_string(zone[i].liste_arete));
    printf ("%s\n",buffer);
    mysql_execreq(buffer);
    strcpy(buffer,"\0");
  }
  free (buffer);
  printf ("fin tab_to_mysql_zone\n");
}
