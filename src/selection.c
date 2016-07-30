//MINH
#include "selection.h"

void selec_et_empile_sommet()
{
     int i;
     for (i=0;i<nbr_sommet;i++)
     {
	  if (sqrt((float)((coordx-(int)som[i].pos.x)*(coordx-(int)som[i].pos.x)+(coordy-(int)som[i].pos.y)*(coordy-(int)som[i].pos.y)))<10)
	  {	       
	       if(sommet_selec1==-1)
		    sommet_selec1=som[i].id;
	       else
		    sommet_selec2=som[i].id;
	       som[i].selec=1;	       
	       empiler(&guts.pile_donnees, int_to_elt(som[i].id));
	       empiler_gtk(store_pile, int_to_elt(som[i].id), &iter_pile);	
	  }
     }
}

void selec_et_empile_arete()
{
     int i;
     for (i=0;i<nbr_arete;i++)
     {
	  if (sqrt((float)((coordx-(int)arete[i].poignet.x)*(coordx-(int)arete[i].poignet.x)+(coordy-(int)arete[i].poignet.y)*(coordy-(int)arete[i].poignet.y)))<10 && arete[i].triangle==0)
	    {	
	      arete_selec=arete[i].id;
	      arete[i].selec=1;	       
	      empiler(&guts.pile_donnees, int_to_elt(arete[i].id));
	      empiler_gtk(store_pile, int_to_elt(arete[i].id), &iter_pile);
	  }
     }
}


void selec_et_empile_zone()
{
     int i;
     for (i=0;i<nbr_zone;i++)
     {
	  if (sqrt((float)((coordx-(int)zone[i].poignet.x)*(coordx-(int)zone[i].poignet.x)+(coordy-(int)zone[i].poignet.y)*(coordy-(int)zone[i].poignet.y)))<10)
	  {	       
	    zone_selec=zone[i].id;
	    zone[i].selec=1;	       
	    empiler(&guts.pile_donnees, int_to_elt(zone[i].id));
	    empiler_gtk(store_pile, int_to_elt(zone[i].id), &iter_pile);       
	  }
     }
}

int selection_som() //selection au lassot
{
     float xmin,xmax,ymin,ymax;
     int j,i;

     xmin = MIN(lassot.x1,lassot.x2);
     ymin = MIN(lassot.y1,lassot.y2);
     xmax = MAX(lassot.x1,lassot.x2);
     ymax = MAX(lassot.y1,lassot.y2);
     
     if (nbr_sommet>0&&xmin>0&&ymin>0)
     {
	  //nbr_selec_som=0;
	  for (i=0;i<nbr_sommet;i++)
	  {
	       if ((som[i].pos.x<=xmax&&som[i].pos.y<=ymax) &&
		   (som[i].pos.x>=xmin&&som[i].pos.y>=ymin))
		 {
		   if (som[i].selec != 1) /* empile un sommet */
		     {
			 nbr_selec_som++;
			 empiler(&guts.pile_donnees, int_to_elt(som[i].id));
			 empiler_gtk(store_pile, int_to_elt(som[i].id), &iter_pile);
			 som[i].selec=1;
		    }
		   else /* deselectionne le sommet */		    {
		     --nbr_selec_som;
		     som[i].selec = 0;
		     j=1;
		     while ((j <= guts.pile_donnees.position) && 
			    (guts.pile_donnees.donnees[j].entier != som[i].id))
		       ++j;
		     while (j <= guts.pile_donnees.position-1)
		       {
			 guts.pile_donnees.donnees[j] = guts.pile_donnees.donnees[j+1];
			 ++j;
		       }
		     --guts.pile_donnees.position;
		     recopier_pile_gtk(&guts.pile_donnees, store_pile, &iter_pile);
		    }
	       }
	  }
     }
     return nbr_selec_som;
}

void deselection_som()
{
     int i;
     for (i=0;i<nbr_sommet;i++)
	  som[i].selec=0;
     guts.pile_donnees.position = MAX(0,guts.pile_donnees.position-nbr_selec_som);
     nbr_selec_som=0;
     recopier_pile_gtk(&guts.pile_donnees,store_pile, &iter_pile);
}

int selection_arete()
{
  float xmin,xmax,ymin,ymax;
  int j,i;

  xmin = MIN(lassot.x1,lassot.x2);
  ymin = MIN(lassot.y1,lassot.y2);
  xmax = MAX(lassot.x1,lassot.x2);
  ymax = MAX(lassot.y1,lassot.y2);

  if (nbr_arete>0&&xmin>0&&ymin>0)
    {
      for (i=0;i<nbr_arete;i++)
	  {
	    if ((arete[i].poignet.x<=xmax&&arete[i].poignet.y<=ymax) &&
		(arete[i].poignet.x>=xmin&&arete[i].poignet.y>=ymin))
	      {
		if (arete[i].selec != 1 && arete[i].triangle==0) /* empile une arete */
		  {
		    nbr_selec_arete++;
		    empiler(&guts.pile_donnees, int_to_elt(arete[i].id));
		    empiler_gtk(store_pile, int_to_elt(arete[i].id), &iter_pile);
		    arete[i].selec=1;
		  }
		else /* deselectionne l'arete */
		  {
		    --nbr_selec_arete;
		    arete[i].selec = 0;
		    j=1;
		    while ((j <= guts.pile_donnees.position) &&
			   (guts.pile_donnees.donnees[j].entier != arete[i].id))
			  ++j;
			while (j <= guts.pile_donnees.position-1)
			  {
			    guts.pile_donnees.donnees[j] = guts.pile_donnees.donnees[j+1];
			    ++j;
			  }
			--guts.pile_donnees.position;
			recopier_pile_gtk(&guts.pile_donnees, store_pile, &iter_pile);
		      }
	      }
	  }
    }
  return nbr_selec_arete;
}


void deselection_arete()
{
   int i;
  for (i=0;i<nbr_arete;i++)
    arete[i].selec=0;
  guts.pile_donnees.position = MAX(0,guts.pile_donnees.position-nbr_selec_arete);
  nbr_selec_arete=0;
  recopier_pile_gtk(&guts.pile_donnees,store_pile, &iter_pile);
}

int selection_zone()
{

  float xmin,xmax,ymin,ymax;
  int j,i;

  xmin = MIN(lassot.x1,lassot.x2);
  ymin = MIN(lassot.y1,lassot.y2);
  xmax = MAX(lassot.x1,lassot.x2);
  ymax = MAX(lassot.y1,lassot.y2);

  if (nbr_zone>0&&xmin>0&&ymin>0)
    {
      for (i=0;i<nbr_zone;i++)
	 {
	   if ((zone[i].poignet.x<=xmax&&zone[i].poignet.y<=ymax) &&
	       (zone[i].poignet.x>=xmin&&zone[i].poignet.y>=ymin))
	     {
	       if (zone[i].selec != 1) /* empile une arete */
		 {
		   nbr_selec_zone++;
		   empiler(&guts.pile_donnees, int_to_elt(zone[i].id));
		   empiler_gtk(store_pile, int_to_elt(zone[i].id), &iter_pile);
		   zone[i].selec=1;
		 }
	       else /* deselectionne l'arete */		    {
		 --nbr_selec_zone;
		 zone[i].selec = 0;
		 j=1;
		 while ((j <= guts.pile_donnees.position) &&
			(guts.pile_donnees.donnees[j].entier != zone[i].id))
		   ++j;
		 while (j <= guts.pile_donnees.position-1)
		   {
		     guts.pile_donnees.donnees[j] = guts.pile_donnees.donnees[j+1];
		     ++j;
		   }
		 --guts.pile_donnees.position;
		 recopier_pile_gtk(&guts.pile_donnees, store_pile, &iter_pile);
	       }
	     }
	 }
    }
  return nbr_selec_zone;
 }

void deselection_zone()
{
   int i;
  for (i=0;i<nbr_zone;i++)
    zone[i].selec=0;
  guts.pile_donnees.position = MAX(0,guts.pile_donnees.position-nbr_selec_zone);
  nbr_selec_zone=0;
  recopier_pile_gtk(&guts.pile_donnees,store_pile, &iter_pile);

}


//REQUETE POUR FORTH :: il faut liberer la meme-oire ici
void selection_forth (char *chaine_selec)
{
     unsigned int i,k;
     int val;
     char temp[10]="\0";
     for (i=0;i<strlen(chaine_selec);i++)
     {
	  k=0;
	  while (chaine_selec[i]!=' '&&chaine_selec[i]!='\0')
	  {
	       temp[k]=chaine_selec[i];
	       i++;
	       k++;
	  }
	  val=atoi(temp);
	  ++nbr_selec_som;
	  empiler(&guts.pile_donnees,int_to_elt(val));
	  som[id_to_tab(val)].selec=1;
	  temp[0]='\0';
     }
     enfonce_bt_cell(true,false,false);
     guts.cellule = SOMMET;
     enfonce_bt_outil(false,false,true,false);
     guts.outil = SELECTION;
     recopier_pile_gtk(&guts.pile_donnees, store_pile, &iter_pile);

     gdk_window_invalidate_rect (drawingarea->window, &drawingarea->allocation, FALSE);
}
