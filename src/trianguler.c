#include "trianguler.h"

t_pos calcul_poignet(t_liste l)
{
  t_liste temp;
  t_pos res;
  int temp3;
  temp=l;
  res.z=0.0;
  while (temp!=NULL)
    {
      temp3=id_to_tab2(temp->valeur);
      printf("temp3:%i\n",temp3);
      res.x=res.x+arete[temp3].poignet.x;
      res.y=res.y+arete[temp3].poignet.y;
      temp=temp->suivant;
    }
  res.x=(res.x)/3;
  res.y=(res.y)/3;
  printf ("res.x:%f\n",res.x);
  printf ("res.y:%f\n",res.y);
  return res;
};

void trianguler(t_liste tempzone)
{
  t_pos poignet_temp;
  t_pos normal_temp;
  char buffer[250];
  if (nbr_elt_liste(tempzone)==3)
    {
	 //Pourquoi pas appeller creer_zone ???

      poignet_temp=calcul_poignet(tempzone);
      normal_temp=Normal(tempzone->valeur,tempzone->suivant->valeur);
      sprintf(buffer,
	      "INSERT INTO triangle (arete1,arete2,arete3,x_poig,y_poig,x_normal,y_normal,z_normal) values(%i, %i,%i,%s, %s,%s,%s,%s)",
	      tempzone->valeur,
	      tempzone->suivant->valeur,
	      tempzone->suivant->suivant->valeur,
	      floattostring(poignet_temp.x),
	      floattostring(poignet_temp.y),
	      floattostring(normal_temp.x),
	      floattostring(normal_temp.y),
	      floattostring(normal_temp.z));
      printf ("%s\n",buffer);
      mysql_execreq(buffer);
      printf ("fin trianguler\n");
    }
  else
    {
      int i;
      int j=0;
      int maxid=0;;
      gboolean rep=TRUE;; 
      t_liste triangle;
      triangle=init_liste();
      int arete1,arete2;
      int arete1_temp,arete2_temp,arete3_temp,arete4_temp;

// il faut prendre 2 aretes consecutives c'est pas le cas ici
      arete1=suppr_elt(&tempzone);
      arete2=suppr_elt(&tempzone);
      insert_liste(arete1,&triangle);
      insert_liste(arete2,&triangle);
      arete1_temp=arete[id_to_tab2(arete1)].pos1.s;
      arete2_temp=arete[id_to_tab2(arete1)].pos2.s;
      arete3_temp=arete[id_to_tab2(arete2)].pos1.s;
      arete4_temp=arete[id_to_tab2(arete2)].pos2.s;
      if (arete1_temp==arete3_temp)
	arete1_temp=arete4_temp;
      else
	if (arete1_temp==arete4_temp)
	  arete1_temp=arete3_temp;
	else
	  if (arete2_temp==arete3_temp)
	    arete2_temp=arete4_temp;
	  else
	    arete2_temp=arete3_temp;
      for (i=0;i<nbr_arete&&rep;i++)
	{
	  if ((arete[i].pos1.s==arete1_temp&&arete[i].pos2.s==arete2_temp)||
	      (arete[i].pos1.s==arete2_temp&&arete[i].pos2.s==arete1_temp))
	    rep=FALSE;
	}
      if (rep)
	{
	  ajout_arete(arete1_temp,arete2_temp,1);
	  for (j=0;j<nbr_arete;j++)
	    {
	      if(maxid<arete[j].id)
		maxid=arete[j].id;
	    }
	  insert_liste(maxid,&triangle);
	}
      else
      {
	   printf (" relance calcul triangle\n");
	   insert_liste(arete[i-1].id,&triangle);
      }
	   poignet_temp=calcul_poignet(triangle);
      normal_temp=Normal(triangle->valeur,triangle->suivant->valeur); 
      sprintf(buffer,
	      "INSERT INTO triangle (arete1,arete2,arete3,x_poig,y_poig,x_normal,y_normal,z_normal) values(%i, %i,%i,%s, %s,%s,%s,%s)",
	      triangle->valeur,
	      triangle->suivant->valeur,
	      triangle->suivant->suivant->valeur,
	      floattostring(poignet_temp.x),
	      floattostring(poignet_temp.y),
	      floattostring(normal_temp.x),
	      floattostring(normal_temp.y),
	      floattostring(normal_temp.z));
      printf ("%s\n",buffer);
      mysql_execreq(buffer);
      if (rep)
	  insert_liste(maxid,&tempzone);
      else
	  insert_liste(arete[i-1].id,&tempzone);
      trianguler (tempzone);
    }
}

void trianguler_carte()
{
  printf (" debut trianguler carte\n");
  int i;
  mysql_execreq("DROP TABLE triangle");
  mysql_execreq("CREATE TABLE triangle (id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY, arete1 INTEGER, arete2 INTEGER, arete3 INTEGER, x_poig FLOAT, y_poig FLOAT, x_normal FLOAT , y_normal FLOAT, z_normal FLOAT)");
 
  for (i=0;i<nbr_arete;i++)
    {
      if (arete[i].triangle==1)
	{
	  arete_selec=arete[i].id;
	  suppr_arete();
	}
    }
  
  for (i=0;i<nbr_zone;i++)
    {
      t_liste temp;
      temp=zone[i].liste_arete;
      trianguler(temp); 
    }
  TtriangleInit();
  mysql_db_triangle();
  mysql_execreq("DROP TABLE zone");
  mysql_execreq("CREATE TABLE zone (id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY, x_poig FLOAT, y_poig FLOAT, z_poig FLOAT, name TEXT, R FLOAT, G FLOAT, B FLOAT, A FLOAT, nbr_arete INTEGER, liste_arete TEXT,mforth TEXT)");

  nbr_zone=0;
  for (i=0;i<nbr_triangle;i++)
    {
       listetemp=init_liste();
       listetemp=triangl[i].liste_arete;
       ajout_zone();
       }

  arete_selec=-1;
  mysql_execreq("UPDATE arc set triangle = 0 WHERE triangle = 1");
  TzoneInit();
  mysql_db_zone();
  TareteInit();
  mysql_db_arc();
  printf ("fin trianguler carte\n");
}
