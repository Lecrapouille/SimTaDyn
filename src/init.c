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

void TsommetInit()
{
  int i,j;
  char temp[10];
  for(i=0;i<NBR_SOMMET_MAX;i++)
    {
      som[i].pos.x=0.0;
      som[i].pos.y=0.0;
      som[i].id=0;
      som[i].nom=NULL;
      som[i].selec=0;
      som[i].mot_forth=NULL;
      som[i].densite.R=0.0;
      som[i].densite.G=0.0;
      som[i].densite.B=0.0;
      som[i].densite.A=0.0;
    }
    nbr_sommet=0;
}

void TareteInit()
{
  int i,j;
  char temp[10];
  nbr_arete=0;
  for(i=0;i<NBR_ARETE_MAX;i++)
    {
      arete[i].pos1.s=0;
      arete[i].pos2.s=0;
      arete[i].pos1.x=0.0;
      arete[i].pos2.x=0.0;
      arete[i].pos1.y=0.0;
      arete[i].pos2.y=0.0;
      arete[i].pos1.z=0.0;
      arete[i].pos2.z=0.0;
      arete[i].id=0;
      arete[i].poignet.x=2.0;
      arete[i].poignet.y=2.0;
      arete[i].nom=NULL;
      arete[i].selec=0;
      arete[i].mot_forth=NULL;
      arete[i].triangle=0;
      arete[i].cout=0;
    }
}

void TzoneInit()
{
  int i,j;
  char temp[10];
  for(i=0;i<NBR_ZONE_MAX;i++)
    {
      zone[i].densite.R=1.0;
      zone[i].densite.G=0.0;
      zone[i].densite.B=0.0;
      zone[i].poignet.x=0.0;
      zone[i].poignet.y=0.0;
      zone[i].nom=NULL;
      zone[i].id=0;
      zone[i].liste_arete=init_liste();
      zone[i].selec=0;
      zone[i].mot_forth=NULL;
    }
  nbr_zone=0;
  nbr_arete_zone=0;
  for (i=0;i<nbr_zone;i++)
    suppr_liste(&(zone[i].liste_arete));
}

void TtriangleInit()
{
  int i;
  for (i=0;i<NBR_TRIANGLE_MAX;i++)
    {
      triangl[i].poignet.x=0.0;
      triangl[i].poignet.y=0.0;
      triangl[i].liste_arete=init_liste();
      triangl[i].selec=0;
    }
}
