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

/*****************************************************************************
CREATION GRAPHE EN LISTE D'ADJACENCE
*****************************************************************************/
/*l_graphe creation_graphe()
  {
  l_graphe res;
  int min_som;
  int i;
  min_som=som[nbr_sommet-1].id;
  for (i=0;i<nbr_sommet;i++)
    res=(ajout_sommet(res,i));
    for (i=0;i<nbr_arete;i++)
    res=(ajout_arete_la(res,(arete[i].a1-min_som),(arete[i].a2-min_som)));
    return res;
    }

    l_graphe ajout_sommet (l_graphe G, t_sommet s)
    {
    l_graphe new, p=G;
    while (p)
    {
    if (p->som == s)
    return G;
    p=p->suiv;
    }
    if (!(new= (l_graphe) malloc (sizeof (s_sommet))))
    exit (1);

    new->som = s;
    new->suiv = NULL;
    new->succ = NULL;
    new->pred = NULL;

    new->suiv = G;
    return new;
    }

    l_graphe recherche (l_graphe g, int s)
    {
    l_graphe p;
    p=g;
    while (p && (p->som != s))
    p = p->suiv;
    return p;
    }

    int ajout_arete_la (l_graphe G,int s ,int i)
    {
    som_adj su,pr;
    l_graphe ps,pi;

    ps=rechercher(G,i);
    if(!pi)
    return 0;

    su=ps->suiv;
    while(su)
    {
    if(su->vsom->somm==s)
    return 1;
    su=su->suiv;
    }

    if(!(su=(som_adj)malloc(sizeof(s_adj))))
    exit(1);
    if(!(pr=(som_adj)malloc(sizeof(s_adj))))
    exit(1);

    su->suiv=ps->succ;
    ps->succ=su;
    su->vsom=pi;

    return 1;
    }


    int ordre(l_graphe G)
    {
    int o=0;
    l_graphe p;

    p=G;
    while(p)
    {
    o++;
    p=p->suiv;
    }
    return o;
    }

    int * classif_NO(t_graphe g,int *pere)
    {
    int *maT_classf;
    int i,j,k,n;
    l_graphe p;
    som_adj ps;

    n=ordre(g);

    if (!(maT_classf=calloc((n*(n+1))/2,sizeof(int))))
    exit();

    p=g;
    while(p)
    {
    i=p->som-1;
    ps=p->succ;
    while(ps)
    {
    j=ps->vsom->som-1;
    k=(i*(i+1))/2+j;
    if (i==pere[j]||j==pere[i])
    maT_classf[k]=1;
    else
    maT_classf[k]=2;
    ps=ps->suiv;
    }
    p=p->suiv;
    }
   return maT_classf;
   }

   int *parcours_profondeur_init(l_graphe g,int s)
   {
   int *pere, *marque, *mat_classf;
   int i;
   l_graphe p,q;
   som_adj ps;

   n=ordre(g);

   if (!(pere=malloc(n*sizeof(int))))
   exit(1);

   for (i=0;i<n;i++)
   pere[i]=-1;

   rechercher(G,s);
   p_prof_recNO(q,marque,pere);
   q=q->suiv;
   while(q)
   {
   if (!marque[q->som-1])
   p_prof_recNO(q,marque,pere);
   q=q->suiv;
   }

   free(marque);
   free(pere);
   return (mat_classf);
   }


   void p_porf_recNO(l_graphe g,int *m,int *pere)
   {
   int i,j;
   som_adj p;

   p=g->succ;
   j=g->som-1;
   marque[j]=1;

  while(p)
  {
  i=p->vsom->som-1;
  if (!m[i])
  {
  pere[i]=j;
  p_prof_recN0(p,m,pere);
  }
  p=p->suiv;
  }
  }

  int test_chaine_rec(l_graphe pi, l_graphe pj, int *marque)
  {
  som_adj lsucc;
  marque[pi->som-1] = 1;
  lsucc=pi->succ;

  while(lsucc)
  {
  if (!marque[lsucc->vsom->som-1])
  {
  if (lsucc->vsom==pj)
  return 1;
  if (test_chaine_rec(lsucc->vsom,pj,marque))
  return 1;
  }
  lsucc=lsucc->suiv;
  }
  return 0;
  }

  int existence_chaine(l_graphe g,int i,int j)
  {
  l_graphe pi,pj;
  int *marque;
  int test;

   if(!(marque=(int*) calloc (ordre(g),sizeof(int))))
   exit();
   if(!(pi=recherche(i,g)||!(pj=recherche(j,G))))
    return (-1);
    test=test_chaine_rec(pi,pj,marque);
    free (marque);
    return test;
    }

    int recherche_cycle_rec(l_graphe pi, l_graphe pere, int *marque)
    {
    som_adj lsucc;
    marque[pi->som-1]=1;
    lsucc=pi->succ;

    while(lsucc)
    {
    if(marque[lsucc->vsom->som-1]&&(lsucc->vsom != pere))
    return 1;
    else
    if (cherche_circuit_rec(lsucc->vsom,pi,marque))
    return 1;
    lsucc=lsucc->suiv;
    }
    return 0;
    }

    void parcours_prof_rec_comp(l_graphe g,int *comp, int no)
    {
    som_adj ps;

    comp[g->som-1]=no;
    ps=g->succ;

    while(ps)
    {
    if(!comp[ps->vsom->som-1])
    parcours_prof_rec_comp(ps->vsom,comp,no);
    ps=ps->suiv;
    }
    }

    int composantes_connexes(l_graphe g, int *comp)
    {
    l_graphe p;
    int no;

    if (!(comp = (int*) calloc (ordre(G),sizeof(int))))
    exit();

    p=g;
    no=1;
    while(p)
    {
    if (!marque[p->som-1])
    parcours_prof_rec_comp(p,comp,no++);
    p=p->suiv;
    }

    free(marque);
    return no;
    }

    void points_art_rec (l_graphe g, l_graphe pere, int *op,int *cpt,int *plushaut, int *points)
    {
    som_adj ps;
    int x,y;

    x=g->som-1;
    op[x]=(*cpt)++;
    plushaut[x]=op[x];
    ps=g->succ;

    while(ps)
    {
    y=ps->vsom->som-1;
    if(!op[y])
    {
    points_arc_rec(ps->vsom,g,op,cpt,plushaut,points);
    if(plushaut[y]<plushaut[x])
    plushaut[x]=plushaut[y];
    if(!points[x]&&(plushaut[y]>=op[x]))
    points[x]=1;
    }
    else
    if((ps->vsom!pere)&&(op[y]<plushaut[x]))
    plushaut[x]=op[y];
    ps=ps->suiv;
    }
    }


    int *points_art_init(l_graphe g)
    {
    int *op, *plushaut, *points;
    int cpt,n,appel;
    som_adj ps;
  int r,x;

  n=ordre(G);
  if(!(op=(int*) calloc(n,sizeof(int))))
  exit();
  if(!(plushaut=(int*)calloc(n,sizeof(int))))
  exit();
  appel=0;
  r=G->som-1;
  op[r]=1;
  cpt=2;
  plushaut[r]=1;

  ps=G->succ;
  while(ps)
  {
  x=ps->vsom->som-1;
  if(!op[x])
  {
  points_art_rec(ps->vsom,G,op,&cpt,plushaut,points);
  appel++;
  }
  ps=ps->suiv;
  }

  if(appel>1)
  points[r]=1;

  free(op);
  free(plushaut);

  return points;
  }

  t_poids *plus_court_chamin(l_graphe g,int s)
  {
  t_poids res;
  res=(t_poids) malloc (ordre(g)*sizeof(t_poids));
  }

  t_poids *dijkstra(l_graphe g,int s,int **pere)
  {
  l_graphe* marque,p;
  som_adj ps;
  int i, imin,n;
  t_poids pmin, *d;

  n=ordre(G);
  marque=malloc(n*sizeof(l_graphe));
  d=malloc(n*sizeof(t_poids));
  (*pere)=malloc(n*sizeof(int));
  memset(*pere,-1,n*sizeof(int));
   for(i=0;i<n;i++)
   d[i]=MAX_POIDS;
   p=g;
   while(p)
   {
   marque[p->som-1]=p;
   p=p->suiv;
   }
   d[s-1]=0;
   imin=s-1;
   pmin=0;

   while(imin!=-1)
   {
   ps=marque[imin]->succ;
   while(ps)
   {
   i=ps->vsom->som-1;
   if (pmin+ps->cout<d[i])
   {
   d[i]=pmin+ps->cout;
   (*pere)[i]=imin;
   }
   ps=ps->suiv;
   }
   marque[imin)=NULL;
   imin=-1;
   pmin=MAX_POIDS;
   for(i=0;i<n;i++)
   if(!marque[i]&&d[i]<pmin)
   {
   imin=i;
   pmin=d[i];
   }
   }
   free(marque);
   return d;
   }
*/

int ordresom ()
{
  int res=0;
  int i;
  for (i=0;i<nbr_arete;i++)
    {
      if ((sommet_selec1==arete[i].pos1.s)||(sommet_selec1==arete[i].pos2.s))
        res++;
    }
  return res;
}

int degre(int s)
{
  int res=0;
  int i;
  for (i=0;i<nbr_arete;i++)
    {
      if ((s==arete[i].pos1.s)||(s==arete[i].pos2.s))
        res++;
    }
  return res;
}


int ieme_succ(int s, int ieme)
{
  int res=0,res2=0;
  int i;
  for (i=0;i<nbr_arete;i++)
    {
      if ((s==arete[i].pos1.s)||(s==arete[i].pos2.s))
        {
          res2++;
          if (s==arete[i].pos1.s)
            res=arete[i].pos2.s;
          else
            res=arete[i].pos1.s;
          if (res2==ieme)
            return res;
          }
    }
  return res;
}

int iemesucc(int ieme)
{
  int res=0,res2=1;
  int i;
  for (i=0;i<nbr_arete&&res2!=ieme;i++)
    {
      if ((sommet_selec1==arete[i].pos1.s)||(sommet_selec1==arete[i].pos2.s))
        {
          res2++;
          if (sommet_selec1==arete[i].pos1.s)
            res=arete[i].pos2.s;
          else
            res=arete[i].pos1.s;
        }
    }
  return res;
}

int ieme_succ_arete (int ordre, int val_id_som)
{
     int count=0,i;
     for (i=0;i<nbr_arete;i++)
     {
          if ((arete[i].pos1.s==val_id_som)||(arete[i].pos2.s==val_id_som))
               if ((count++)==ordre)
                    return arete[i].id;
     }
     return 0;
}

int extremite(int s, int a)
{
     int i;

     if (arete[id_to_tab2(a)].pos1.s==s)
          return arete[id_to_tab2(a)].pos2.s;
     else
          return arete[id_to_tab2(a)].pos1.s;
}
