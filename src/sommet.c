#include "main.h"

void selec_sommet()
{
  int i;
  for (i=0;i<nbr_sommet;i++)
    {
      if (sqrt((float)((coordx-(int)som[i].pos.x)*(coordx-(int)som[i].pos.x)+(coordy-(int)som[i].pos.y)*(coordy-(int)som[i].pos.y)))<10)
	{
	  som[i].selec=1;
	  if(sommet_selec1==-1)
	    sommet_selec1=som[i].id;
	  else
	      sommet_selec2=som[i].id;
	}
    } 
}

void ajout_som()
{
  mysql_insert_som((float)coordx,(float)coordy,(float) (random()%10),1000.0,1000.0,1000.0,1000.0);
  
  TsommetInit();
  mysql_db_som();
}

void suppr_som()
{
  int i;
  for (i=0;i<nbr_arete;i++)
    {
      arete_selec=-1;
      if ((sommet_selec1==arete[i].pos1.s)||(sommet_selec1==arete[i].pos2.s))
	{
	  arete_selec=arete[i].id;
	  suppr_arete();
	}
    }
  mysql_delete_som();
  TzoneInit();
  TsommetInit();
  TareteInit();
  sommet_selec1=-1;
  mysql_db_som();
  mysql_db_arc();
  mysql_db_zone();
}

void deplace_sommet(int sommet_selec_temp)
{
  int i;
  modif_sql_sommet(sommet_selec_temp );
  calcul_poignet_arete_et_zone_sommet();
  sommet_selec1=-1;
  sommet_selec2=-1;
  sommet_selec_temp=-1;
  for (i=0;i<nbr_sommet;i++)
    som[i].selec=0;
  
}

void deplace_sommet_zone()
{
  int i;
  int nbr_moy=0;
  float xmoy=0.0;
  float ymoy=0.0;
  t_liste templist;
  int temp=0;
  for (i=0;i<nbr_zone;i++)
    {  
      templist=zone[i].liste_arete;
      while(templist!=NULL)
	{
	  temp=templist->valeur;
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
	  templist=templist->suivant;
	} 
      if ( zone[i].poignet.x==(xmoy)/(2*nbr_moy)||
	   zone[i].poignet.y==(ymoy)/(2*nbr_moy))
	{
	  char *buffer;
	  buffer = malloc (sizeof(char)*100);
	  sprintf (buffer,
		   "UPDATE zone set x_poig = %s and y_poig = %s WHERE id = %i",
		   floattostring((xmoy)/(2*nbr_moy)),
		   floattostring((ymoy)/(2*nbr_moy)),
		   zone[i].id);
	  mysql_execreq(buffer);
	  free(buffer);
	}
    }
}

void calcul_poignet_arete_et_zone_sommet()
{
  char *buffer;
  int i;
  buffer = malloc (sizeof (char) *100);
  for (i=0;i<nbr_arete;i++)
    {
      sprintf (buffer,"UPDATE arc set x_poig = %s WHERE id = %i",
	       floattostring(arete[i].poignet.x),
	       arete[i].id);
      mysql_execreq(buffer);
      strcpy(buffer,"\0");
      sprintf (buffer,"UPDATE arc set y_poig = %s WHERE id = %i",  
	       floattostring(arete[i].poignet.y),
	       arete[i].id);
      mysql_execreq(buffer);
      strcpy(buffer,"\0");
    }
  for (i=0;i<nbr_zone;i++)
    {
      sprintf (buffer,"UPDATE zone set x_poig = %s WHERE id = %i",  
	       floattostring(zone[i].poignet.x),
	       zone[i].id);
      mysql_execreq(buffer);
      strcpy(buffer,"\0");
      sprintf (buffer,"UPDATE zone set y_poig = %s WHERE id = %i",  
	       floattostring(zone[i].poignet.y),
	       zone[i].id);
      mysql_execreq(buffer);
      strcpy(buffer,"\0");
    }
  free(buffer);
}

void calcul_poignet_zone_sommet()
{
  int count;
  /*char * buffer;
    buffer = malloc (sizeof (char) * 200);*/
  for (count=0;count<nbr_zone;count++)
    {
      int temp=0;
      float xmoy=0.0,ymoy=0.0;
      int nbr_moy=0;
      
      int i;
      t_liste l;
      l=zone[count].liste_arete;
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
      zone[count].poignet.x=(xmoy)/(2*nbr_moy);
      zone[count].poignet.y=(ymoy)/(2*nbr_moy);
    }
}

void tab_to_mysql_som()
{
  printf (" debut sommet update\n");
  int i;
  char *buffer;
  buffer = malloc (sizeof (char) * 200);
  for (i=0;i<nbr_sommet;i++)
    {
      sprintf (buffer,"UPDATE som set coord_x = %s, coord_y = %s, coord_z = %s, R = %s, G = %s, B = %s, A = %s  WHERE id = %i",
	       floattostring(som[i].pos.x),
	       floattostring(som[i].pos.y),
	       floattostring(som[i].pos.z),
	       floattostring(som[i].densite.R),
	       floattostring(som[i].densite.G),
	       floattostring(som[i].densite.B),
	       floattostring(som[i].densite.A),
	       som[i].id);
      printf ("%s\n",buffer);
      mysql_execreq(buffer);
      strcpy(buffer,"\0");
    }
  free(buffer);
  printf ("fin update sommet\n");
}

