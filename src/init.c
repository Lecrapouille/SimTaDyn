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
