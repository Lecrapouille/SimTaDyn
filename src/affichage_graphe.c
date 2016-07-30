#include "affichage.h"

void calcul_coord()
{
  if (xold>=250)
    coordx= (xold-250)/250;
  else
    coordx=(xold-250)/250;
  if (yold>=250)
    coordy=(250-yold)/250;
  else
    coordy=(250-yold)/250;
}

void mode_normal()
{
  mysql_execreq("UPDATE arc SET R = 1 WHERE R = 0");
  mysql_execreq("UPDATE arc SET G = 1 WHERE G = 0");
  TareteInit();
  mysql_db_arc();
  arete_selec=-1;
  sommet_selec1=-1;
  sommet_selec2=-1;
  zone_selec=-1;
}

/*
**************************************************************************
****
***                  CREATION DE LISTE CHAINEE SIMPLE POUR LES ZONES
***
**************************************************************************
*/

t_liste init_liste ()
{
  return NULL;
}

void insert_liste (int x,t_liste *l)
{
  t_liste laux ;
  laux=(t_liste)malloc(sizeof(element)) ;
  laux->valeur=x ;
  laux->suivant=(*l);
  (*l)=laux;
}

int suppr_elt (t_liste *l)
{
  int res;
  res=(*l)->valeur;
  (*l)=(*l)->suivant;
  return res;
}

void suppr_liste (t_liste *l)
{
  t_liste temp;
  while ((*l)!=NULL)
    {
      temp=(*l);
      (*l)=(*l)->suivant;
      free (temp);
    }
  (*l)=NULL;
}

int fin_liste (t_liste* l)
{
  t_liste temp;
  int val=0;
  temp=(*l);
  while (temp->suivant!=NULL)
    temp=temp->suivant;
  val=temp->valeur;
  return val;
}

void affich_liste (t_liste l)
{
  t_liste temp;
  temp=l;
  while (temp!=NULL)
    {
	 /*printf ("%i,",temp->valeur);*/
      temp=temp->suivant;
    }
  printf ("\n");
}

int nbr_elt_liste (t_liste l)
{
  t_liste temp;
  int res=0;
  temp=l;
  while (temp!=NULL)
    {
      res++;
      temp=temp->suivant;
    }
  return res;
}

char* liste_to_string (t_liste l)
{
  char *res;
  char *buffer;
  t_liste temp;
  res=malloc (sizeof(char)*100);
  strcpy(res,"\"");
  buffer=malloc (sizeof( char)*100);
  temp=l;
  while (temp->suivant!=NULL)
    {
      sprintf (buffer,"%i,",temp->valeur);
      temp=temp->suivant;
      strcat (res,buffer);
      strcpy(buffer,"\0");
    }
  sprintf (buffer,"%i\"",temp->valeur);
  strcat (res,buffer);
  free (buffer);
  return res;
}

t_liste string_to_liste (char *chaine)
{
  char s[50];
  int i=0;
  int k=0;
  int val;
  t_liste liste;
  liste=init_liste();
  while (chaine[i]!='\0')
    {
     
      if (chaine[i]!=',')
	{	
	  s[k]=chaine[i];
	  s[k+1]='\0';
	  k++;
	}
      else
	{
	  val=atoi(s);
	  //printf ("%i\n",val);
	  insert_liste(val,&liste);
	  strcpy(s,"\0"); 
	  k=0;
	}
      i++;
    }
  val=atoi(s);
  insert_liste(val,&liste); 
  return liste;
}

char* init_chaine(char *chaine)
{
  unsigned int i;
  for (i=0;i<50;i++)
    chaine[i]='\0';
  return (char *) chaine; 
}


/*
*************************************************************************************
***
***                       FONCTIONS DE RECHERCHE DANS TABLEAU
***
*************************************************************************************
*/

int recherche_pos_tab_arete (int x)
{
  int i;
  for (i=0;(i<nbr_arete)&&(arete[i].id!=x);i++);
  return i;  
} 

/*
*****************************************************************************
***
***               INIT TAB SELEC
***
******************************************************************************
*/

void init_tab_selec()
{
  int i;
  for (i=0;i<nbr_sommet;i++)
    som[i].selec=0;
  for (i=0;i<nbr_arete;i++)
    arete[i].selec=0;
  for (i=0;i<nbr_zone;i++)
    zone[i].selec=0;
}


int id_to_tab (int val_id)
{
     int i,res;
     res = -1;
     for (i=0;i<nbr_sommet;i++)
     {
	  if (som[i].id == val_id)
	  {
	    res = i;
	  }
     }
     return res;
}

int id_to_tab2 (int val_id)
{
     int i,res;
     res = -1;
     for (i=0;i<nbr_arete;i++)
     {
	  if (arete[i].id == val_id)
	  {
	    res = i;
	  }
     }
     return res;
}

