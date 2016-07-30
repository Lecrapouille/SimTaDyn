#include "affichage.h"

void affich_struct()
{
  int i;
  printf ("***************************************************************\n");
  for (i=0;i<nbr_sommet;i++)
    {
      printf ("sommet %s %f| %f| %f| %f| %f %i %s %i\n",som[i].nom,som[i].pos.x,som[i].pos.y,som[i].densite.R,som[i].densite.G,som[i].densite.B,som[i].id,som[i].mot_forth,som[i].selec);
    }
  for (i=0;i<nbr_arete;i++)
    {
      printf ("arete %s %f %f %f %f %f %i %i %i %s %i %i %f\n",arete[i].nom,arete[i].poignet.x,arete[i].poignet.y,arete[i].densite.R,arete[i].densite.G,arete[i].densite.B,arete[i].pos1.s,arete[i].pos2.s,arete[i].id,arete[i].mot_forth,arete[i].selec,arete[i].triangle,arete[i].cout);
    }
  for (i=0;i<nbr_zone;i++)
    {
      printf ("zone %s ",zone[i].nom);
      printf ("%f %f %f %f %f %i %i %s %i\n",zone[i].poignet.x,zone[i].poignet.y,zone[i].densite.R,zone[i].densite.G,zone[i].densite.B,zone[i].id,zone[i].nbr_arete,zone[i].mot_forth,zone[i].selec);
      affich_liste(zone[i].liste_arete);
    }
  for (i=0;i<nbr_triangle;i++)
    {
      printf ("triangle %f %f ",triangl[i].poignet.x,triangl[i].poignet.y);
      affich_liste(triangl[i].liste_arete);
    }
  printf ("**************************************************************\n");
}

