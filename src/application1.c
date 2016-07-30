#include "affichage.h"
#include "main.h"

void vide_som_sql()
{
  char *buffer;
  buffer = (char *)malloc(250 * sizeof(char));
  sprintf(buffer,
	  "DELETE FROM som WHERE name = %s","\"node test\"");
  mysql_execreq(buffer);
  free(buffer);
  buffer = (char *)malloc(250 * sizeof(char));
  sprintf(buffer,
	  "DELETE FROM arc WHERE name = %s","\"arc test\"");
  mysql_execreq(buffer);
  free(buffer);
  
}


void application1 (float x,float y)
{
  x=y;
#if 0  
  float i,j;
  float backx,backy;
  int k;
  vide_som_sql();
  /*CREATION DES SOMMETS*/
  for (i=0;i<= (float) BORDURE_X;i+=x)
    {
      for (j=0;j<= (float) BORDURE_Y;j+=y)
	{
	  mysql_insert_som(i,j,(float) (random()%10),1.0,0.0,0.0);
	}
    }
  TsommetInit();
  mysql_db_som();
  for (k=0;k<nbr_sommet;k++)
    {
      if (som[k].pos.x==BORDURE_X||som[k].pos.y==BORDURE_Y)
	strcpy(som[k].mot_forth,"NILL");
      else
	strcpy(som[k].mot_forth,"LAPLACIEN");
    }

  /*CREATION DES ARCS*/
  for (k=0;k<nbr_sommet;k++)
    {
      coordx=(int) som[k].pos.x;
      coordy=(int) som[k].pos.y;
      backx=coordx;
      backy=coordy;
      selec_sommet();
      coordx=coordx+GRILLAGE_X;
      if (coordx<=BORDURE_X)
	{
	  selec_sommet();
	  mysql_insert_arc(1.0,1.0,1.0,sommet_selec1,sommet_selec2);
	}
      sommet_selec2=-1;
      coordx=backx;
      coordy=backy+GRILLAGE_Y;
      if (coordy<=BORDURE_Y)
	{
	  selec_sommet();
	  mysql_insert_arc(1.0,1.0,1.0,sommet_selec1,sommet_selec2);
	}
      sommet_selec1=-1;
      sommet_selec2=-1;
    }
  TareteInit();
  mysql_db_arc();
  GtkWidget *entry_nom;
  entry_nom = lookup_widget (GTK_WIDGET (win_main),"entry_nom");
  gtk_entry_set_text (GTK_ENTRY (entry_nom),"\0");
  GtkWidget *entry_val;
  entry_val = lookup_widget (GTK_WIDGET (win_main),"entry_val");
  gtk_entry_set_text (GTK_ENTRY (entry_val),"\0");
  GtkWidget *entry_ID;
  entry_ID = lookup_widget (GTK_WIDGET (win_main),"entry_ID");
  gtk_entry_set_text (GTK_ENTRY (entry_ID),"\0");
  mode = VU3D;
#endif
}


