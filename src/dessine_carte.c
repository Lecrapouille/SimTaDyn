#include "dessine_carte.h"


static GLuint poignet_2D_list, poignet_3D_list;

void creer_poignet_3D()
{ 
  GLUquadricObj *qobj;
  qobj = gluNewQuadric ();
  gluQuadricDrawStyle (qobj, GLU_FILL);
  poignet_3D_list = glGenLists(1);
  glNewList(poignet_3D_list,GL_COMPILE);
  glScalef(10,10,10);
  gluSphere (qobj, 1.0, 40, 40);
  glEndList ();
}


void creer_poignet_2D()
{
  GLUquadricObj *qobj;
  qobj = gluNewQuadric ();
  gluQuadricDrawStyle (qobj, GLU_FILL);
  poignet_2D_list = glGenLists(1);
  glNewList(poignet_2D_list,GL_COMPILE);
  glRecti(THICKNESS, -THICKNESS, -THICKNESS, THICKNESS);
  glEndList ();
}

void dessiner_poignet(float x, float y, float z, GLuint l)
{
  glPushMatrix();
  glTranslated(x,y,z);
  glCallList(l);
  glPopMatrix();
}

void printf_sommet(int count, float s)
{
  char * ch;
  s=1;
  switch (guts.champ)
    {
    case ID :
      ch = (char*) xmalloc(10*sizeof(char));
      sprintf(ch, "%d", som[count].id);
      glPrint3D(som[count].pos.x,som[count].pos.y,0,s,ch);
      free(ch);
      break;
    case NOM :
      glPrint3D(som[count].pos.x,som[count].pos.y,0,s,som[count].nom);
      break;
    case FORTH :
      glPrint3D(som[count].pos.x,som[count].pos.y,0,s,som[count].mot_forth);
      break;
    case POIGNET :
    case POSITION :
      ch = (char*) xmalloc(50*sizeof(char));
      sprintf(ch, "(%g;%g;%g)", som[count].pos.x, som[count].pos.y, som[count].pos.z);
      glPrint3D(som[count].pos.x,som[count].pos.y,0,s, ch);
      free(ch);
      break;
    case POIGNETX :
    case POSX :
      ch = (char*) xmalloc(10*sizeof(char));
      sprintf(ch, "%3g", som[count].pos.x);
      glPrint3D(som[count].pos.x,som[count].pos.y,0,s, ch);
      free(ch);
      break;
    case POIGNETY :
    case POSY :
      ch = (char*) xmalloc(10*sizeof(char));
      sprintf(ch, "%3g", som[count].pos.y);
      glPrint3D(som[count].pos.x,som[count].pos.y,0,s, ch);
      free(ch);
      break;
    case POSZ :
      ch = (char*) xmalloc(10*sizeof(char));
      sprintf(ch, "%3g", som[count].pos.z);
      glPrint3D(som[count].pos.x,som[count].pos.y,0,s, ch);
      free(ch);
      break;
    default: break;
    }
}

void dessine_tous_sommets()
{
  int count;
  float s;

  for (count=0;count < nbr_sommet;count++)
    {

      if (som[count].selec==1)
	{ glColor3f(1.0, 0.0, 0.0); s = 1.2; }
      else
	{ glColor4f(som[count].densite.R/1000,
		    som[count].densite.G/1000,
		    som[count].densite.B/1000
		    ,som[count].densite.A/1000);}
       
      if (guts.cellule == SOMMET)
	printf_sommet(count, s);	 
       
      if (guts.vue == VUE_3D)
	dessiner_poignet(som[count].pos.x,
			 som[count].pos.y,
			 som[count].pos.z, 
			 poignet_3D_list);
      else
	dessiner_poignet(som[count].pos.x,
			 som[count].pos.y,
			 0, 
			 poignet_2D_list);
    }
}


void dessine_tous_arcs()
{     
 
  char * ch;     
  int i,j,count;
     
  glBegin(GL_LINES);
  for (i=0;i<nbr_arete;i++)
    {
      if (arete[i].triangle==0)
	{
	  for (j=0;j<nbr_sommet;j++)
	    {
	      if (som[j].id==arete[i].pos1.s)
		{
		  if (arete[i].selec==1)
		    glColor3f(0,0,1);
		  else
		    glColor4f(som[j].densite.R/1000,
			      som[j].densite.G/1000,
			      som[j].densite.B/1000,
			      som[j].densite.A/1000);
		  if (guts.vue==VUE_3D)
		    glVertex3f(som[j].pos.x,som[j].pos.y,som[j].pos.z); 
		  else
		    glVertex2f(som[j].pos.x,som[j].pos.y);
		}
	      if (som[j].id==arete[i].pos2.s)
		{
		  if (arete[i].selec==1)
		    glColor3f(0,0,1);
		  else
		    glColor4f(som[j].densite.R/1000,
			      som[j].densite.G/1000,
			      som[j].densite.B/1000,
			      som[j].densite.A/1000);
		  if (guts.vue==VUE_3D)
		    glVertex3f(som[j].pos.x,som[j].pos.y,som[j].pos.z);
		  else
		    glVertex2f(som[j].pos.x,som[j].pos.y);
		}
	    }
	}
    }
  glEnd();
  for (count=0;count < nbr_arete;count++)
    {
      if (arete[count].triangle==0)
	{
	  glColor4f(arete[count].densite.R,arete[count].densite.G,arete[count].densite.B,arete[count].densite.A);
	  glRecti(((int) arete[count].poignet.x) + THICKNESS,
		  ((int) arete[count].poignet.y) - THICKNESS,
		  ((int) arete[count].poignet.x) - THICKNESS,
		  ((int) arete[count].poignet.y) + THICKNESS);
	  if (guts.cellule == ARC)
	    {      
	      switch (guts.champ)
		{
		case ID :
		  ch = (char*) xmalloc(10*sizeof(char));
		  sprintf(ch, "%d", arete[count].id);
		  glPrint3D(arete[count].poignet.x,arete[count].poignet.y,0,1,ch);
		  free(ch);
		  break;
		case NOM :
		  glPrint3D(arete[count].poignet.x,arete[count].poignet.y,0,1,arete[count].nom);
		  break;
		case FORTH :
		  glPrint3D(arete[count].poignet.x,arete[count].poignet.y,0,1,arete[count].mot_forth);
		  break;
		case COUT :
		  ch = (char*) xmalloc(10*sizeof(char));
		  sprintf(ch, "%3g", arete[count].cout);
		  glPrint3D(arete[count].poignet.x,arete[count].poignet.y,0,1,ch); 
		  free(ch);
		default : break;
		}
	    }
	}
    }
}


void dessine_toutes_zones()
{
char * ch;
int k,l,count;
float r,g,b;
t_pos s1;
t_pos s2;
t_pos s3;
 if (guts.vue==VUE_3D)
   {
     for (count=0;count < nbr_zone;count++)
       {
	 glNormal3f(triangl[count].normal.x,triangl[count].normal.y,triangl[count].normal.z);
	 glBegin(GL_TRIANGLES);
	 glColor3f(0.0,0.0,1.0);
	 t_liste temp;
	 temp=zone[count].liste_arete;
	 s1=point_commun(id_to_tab2(temp->valeur),id_to_tab2(temp->suivant->valeur));
	 s2=point_commun(id_to_tab2(temp->valeur),id_to_tab2(temp->suivant->suivant->valeur));
	 s3=point_commun(id_to_tab2(temp->suivant->valeur),id_to_tab2(temp->suivant->suivant->valeur));
	 glVertex3f(s1.x,s1.y,s1.z);
	 glVertex3f(s2.x,s2.y,s2.z);
	 glVertex3f(s3.x,s3.y,s3.z); 
	 glEnd();
	 glColor3f(0,1,1);
	 glRecti(((int) zone[count].poignet.x) + THICKNESS,
		 ((int) zone[count].poignet.y) - THICKNESS,
		 ((int) zone[count].poignet.x) - THICKNESS,
		 ((int) zone[count].poignet.y) + THICKNESS);
       } 
   }
 else
   {
     for (count=0;count < nbr_zone;count++)
       {
	  if (zone[count].selec==1)
	    {
	      glBegin(GL_POLYGON);
	      glColor4f(zone[count].densite.R/1000,
			zone[count].densite.G/1000,
			zone[count].densite.B/1000,
			zone[count].densite.A/1000);
	      t_liste temp;
	      temp=zone[count].liste_arete;
	      while (temp!=NULL)
		{
		  k=0;
		  for (k=0;k<nbr_arete;k++)
		    if (temp->valeur==arete[k].id)
		      {
			for (l=0;l<nbr_sommet;l++)
			  {
			    if ((arete[k].pos1.s==som[l].id)||(arete[k].pos2.s==som[l].id)){
			      if (guts.vue==VUE_3D)
				glVertex3f(som[l].pos.x,som[l].pos.y,som[l].pos.z);
			      else
				glVertex2f(som[l].pos.x,som[l].pos.y);	
			    }
			  }
		      }
		  temp=temp->suivant;
		}
	      glEnd();
	    }
       if (zone[count].selec==1)
       { r= 1.0; g = 0.0; b = 0.0; }
       else
	 glColor4f(zone[count].densite.R/1000,
		   zone[count].densite.G/1000,
		   zone[count].densite.B/1000,
		   zone[count].densite.A/1000);
       glRecti(((int) zone[count].poignet.x) + THICKNESS,
	       ((int) zone[count].poignet.y) - THICKNESS,
	       ((int) zone[count].poignet.x) - THICKNESS,
	       ((int) zone[count].poignet.y) + THICKNESS);
       if (guts.cellule == ZONE)
	 {
	 switch (guts.champ)
	   {
	   case ID :
	     ch = (char*) xmalloc(10*sizeof(char));
	     sprintf(ch, "%d", zone[count].id);
	     glPrint3D(zone[count].poignet.x,zone[count].poignet.y,0,1,ch);
	     free(ch);
	     break;
	   case NOM :
	     glPrint3D(zone[count].poignet.x,zone[count].poignet.y,0,1,zone[count].nom);
		  break;
	   case FORTH :
	     glPrint3D(zone[count].poignet.x,zone[count].poignet.y,0,1,zone[count].mot_forth);
	     break;
	   default : break;
	   }
	 }
       }
   }
}

void dessine_tous_les_triangles()
{
  char * ch;
int k,l,count;
float r,g,b;
t_pos s1;
t_pos s2;
t_pos s3;
 if (guts.vue==VUE_3D)
   {
     for (count=0;count < nbr_triangle;count++)
       {
	 glNormal3f(triangl[count].normal.x,triangl[count].normal.y,triangl[count].normal.z);
	 glBegin(GL_TRIANGLES);
	 //glColor3f(zone[count].densite.R,zone[count].densite.G,zone[count].densite.B,zone[count].densite.A);
	 glColor3f(0,0,1);
	 t_liste temp;
	 temp=triangl[count].liste_arete;
	 s1=point_commun(id_to_tab2(temp->valeur),id_to_tab2(temp->suivant->valeur));
	 s2=point_commun(id_to_tab2(temp->valeur),id_to_tab2(temp->suivant->suivant->valeur));
	 s3=point_commun(id_to_tab2(temp->suivant->valeur),id_to_tab2(temp->suivant->suivant->valeur));
	 glVertex3f(s1.x,s1.y,s1.z);
	 glVertex3f(s2.x,s2.y,s2.z);
	 glVertex3f(s3.x,s3.y,s3.z); 
	 glEnd();
	 glColor3f(0,1,1);
       }
     glRecti(((int) triangl[count].poignet.x) + THICKNESS,
	     ((int) triangl[count].poignet.y) - THICKNESS,
	     ((int) triangl[count].poignet.x) - THICKNESS,
	     ((int) triangl[count].poignet.y) + THICKNESS);
   } 
 else
   {
	 glColor3f(0,0,1);
     for (count=0;count<nbr_triangle;count++)
       glRecti(((int) triangl[count].poignet.x) + THICKNESS,
	     ((int) triangl[count].poignet.y) - THICKNESS,
	     ((int) triangl[count].poignet.x) - THICKNESS,
	     ((int) triangl[count].poignet.y) + THICKNESS);
   }
 switch (guts.champ)
   {
   case ID :
     ch = (char*) xmalloc(10*sizeof(char));
     sprintf(ch, "%d", triangl[count].id);
     glPrint3D(triangl[count].poignet.x,triangl[count].poignet.y,0,1,ch);
     free(ch);
     break;
   default : break;
   }
}

void dessiner_la_carte()
{
  
  if ((guts.vue == VUE_2D) && (lassot.show))
    {
      glColor3f(1.0,0.0,0.0);
      glBegin(GL_LINE_LOOP);
      glVertex2f(lassot.x1,lassot.y1);
      glVertex2f(lassot.x2,lassot.y1);
      glVertex2f(lassot.x2,lassot.y2);
      glVertex2f(lassot.x1,lassot.y2);
      glEnd();
    }   
  //printf ("ici1\n"); 
  dessine_tous_sommets();
  //printf ("ici2\n"); 
  if (guts.cellule==TRIANGLE)
    {
      //printf ("ici3\n"); 
      dessine_tous_arcs2();
      //printf ("ici4\n"); 
      dessine_tous_les_triangles();
      //printf ("ici5\n"); 
    }
  else
    {
      //printf ("ici6\n"); 
      dessine_tous_arcs();
      //printf ("ici7\n"); 
      dessine_toutes_zones();
      //printf ("ici8\n"); 
    }
  //printf ("ici9\n"); 
  if (guts.vue==VUE_3D)
    dessine_toutes_normal();
  //printf ("ici10\n"); 
}

void dessine_tous_arcs2()
{ 
  char * ch;     
  int i,j,count;
  
  glBegin(GL_LINES);
  for (i=0;i<nbr_arete;i++)
    {
      for (j=0;j<nbr_sommet;j++)
	{
	  if (som[j].id==arete[i].pos1.s)
	    {
	      if (arete[i].selec==1)
		glColor3f(0,0,1);
	      else
		glColor4f(som[j].densite.R/1000,
			  som[j].densite.G/1000,
			  som[j].densite.B/1000,
			  som[j].densite.A/1000);
	      glVertex2f(som[j].pos.x,som[j].pos.y);
	    }
	  if (som[j].id==arete[i].pos2.s)
	    {
	      if (arete[i].selec==1)
		glColor3f(0,0,1);
	      else
		glColor4f(som[j].densite.R/1000,
			  som[j].densite.G/1000,
			  som[j].densite.B/1000,
			  som[j].densite.A/1000);
	      glVertex2f(som[j].pos.x,som[j].pos.y);
	    }
	}
    }
    
  glEnd();
  for (count=0;count < nbr_arete;count++)
    {
      glColor4f(arete[count].densite.R,arete[count].densite.G,arete[count].densite.B,arete[count].densite.A);
      glRecti(((int) arete[count].poignet.x) + THICKNESS,
	      ((int) arete[count].poignet.y) - THICKNESS,
	      ((int) arete[count].poignet.x) - THICKNESS,
	      ((int) arete[count].poignet.y) + THICKNESS);
      if (guts.cellule == ARC)
	{      
	  switch (guts.champ)
	    {
	    case ID :
		  
	      ch = (char*) xmalloc(10*sizeof(char));
	      sprintf(ch, "%d", arete[count].id);
	      glPrint3D(arete[count].poignet.x,arete[count].poignet.y,0,1,ch);
	      free(ch);
	      break;
	    case NOM :
	      glPrint3D(arete[count].poignet.x,arete[count].poignet.y,0,1,arete[count].nom);
	      break;
	    case FORTH :
	      glPrint3D(arete[count].poignet.x,arete[count].poignet.y,0,1,arete[count].mot_forth);
	      break;
	    case COUT:
	      ch = (char*) xmalloc(10*sizeof(char));
	      sprintf(ch, "%3g", arete[count].cout);
	      glPrint3D(arete[count].poignet.x,arete[count].poignet.y,0,1,ch);
	      free(ch);
	    default : break;
	    }
	}
    }
}


void dessine_toutes_zones2()
{
  int count;
  float r,g,b;
  
  for (count=0;count < nbr_triangle;count++)
    {
      { r= 0.0; g = 0.0; b = 1.0; }
      glColor3f(r,g,b);
      glRecti(((int) triangl[count].poignet.x) + THICKNESS,
	      ((int) triangl[count].poignet.y) - THICKNESS,
	      ((int) triangl[count].poignet.x) - THICKNESS,
	      ((int) triangl[count].poignet.y) + THICKNESS);
    }
}

void dessine_toutes_normal()
{
  int i;
  glColor3f(1,0,0);
  glBegin(GL_LINES);
  for (i=0;i<nbr_triangle;i++)
    {
      glVertex3f(triangl[i].poignet.x,triangl[i].poignet.y,triangl[i].poignet.z);
      glVertex3f((triangl[i].normal.z+triangl[i].poignet.x)*10000,(triangl[i].normal.y+triangl[i].poignet.y)*10000,(triangl[i].normal.x+0)*10000);
    }
  glEnd();
}

t_pos point_commun (int a1,int a2)
{
  int s1,s2,s3,s4;
  s1=arete[a1].pos1.s;
  s2=arete[a1].pos2.s;
  s3=arete[a2].pos1.s;
  s4=arete[a2].pos2.s;
  if (s1==s3 || s1==s4)
    return (som[id_to_tab(s1)].pos);
  else
    return (som[id_to_tab(s2)].pos);
}
 
