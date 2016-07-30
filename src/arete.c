#include "affichage.h"

void selec_arete()
{
  int i;
  for (i=0;i<nbr_arete;i++)
    {
      if (sqrt((float)((coordx-(int)arete[i].poignet.x)*(coordx-(int)arete[i].poignet.x)+(coordy-(int) arete[i].poignet.y)*(coordy-(int)arete[i].poignet.y)))<10)
	{
	  arete_selec=arete[i].id;
	  arete[i].selec=1;
	  break;
	}
    } 
}



void ajout_arete(int sommet1,int sommet2,int triangle)
{
  int i;
  //printf ("helo1\n");
  mysql_insert_arc(0.0,1000.0,0.0,1000.0,0.0,sommet1,sommet2,triangle);
  TareteInit();
  //printf ("helo2\n");
  mysql_db_arc();
  //printf ("helo3\n");
  sommet_selec1=-1;
  sommet_selec2=-1;
  for (i=0;i<nbr_sommet;i++)
    som[i].selec=0;
}

void suppr_arete()
{
  mysql_delete_arc();
  TareteInit();
  TzoneInit();
  mysql_db_arc();
  mysql_db_zone();
  arete_selec=-1;
}

int suppr_zone_arete(t_liste temp)
{
  while (temp!=NULL)
    {
      if (temp->valeur==arete_selec)
	return 1;
      else
	temp=temp->suivant;
    }
  return 0;
}

void tab_to_mysql_arete()
{
  //printf ("printf tab_to_ my arete\n");
  int i;
  char *buffer;
  buffer = malloc (sizeof (char) * 200);
  for (i=0;i<nbr_arete;i++)
    {
      sprintf (buffer,"UPDATE arc set x_poig = %s, y_poig = %s, z_poig = %s, R = %s, G = %s, B = %s, A = %s, cout = %s, id_som_1 = %i, id_som_2 = %i, triangle = %i WHERE id = %i",
	       floattostring(arete[i].poignet.x),
	       floattostring(arete[i].poignet.y),
	       floattostring(arete[i].poignet.z),
	       floattostring(arete[i].densite.R),
	       floattostring(arete[i].densite.G),
	       floattostring(arete[i].densite.B),
	       floattostring(arete[i].densite.A),
	       floattostring(arete[i].cout),
	       arete[i].pos1.s,
	       arete[i].pos2.s,
	       arete[i].triangle,
	       arete[i].id);
      //printf ("%s\n",buffer);
      mysql_execreq(buffer);
      buffer[0]='\0';
    }
  free(buffer);
  //printf (" fin update tab mysql arete\n");
}
