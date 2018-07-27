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

#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#   include <GLUT/glut.h> /* pour Mac OS X */
#   include <mysql/mysql.h>
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#else
#   include <GL/glut.h> /* pour les autres systemes */
#   include "mysql/mysql.h"
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif
#include <string.h>
#include <math.h>
#include <sys/time.h>


/*** Use OpenGL extensions. ***/
/* #include <gdk/gdkglglext.h> */

#ifdef G_OS_WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#define NBR_SOMMET_MAX 200
#define NBR_ARETE_MAX 200
#define NBR_ZONE_MAX 200
#define NBR_TRIANGLE_MAX 200
#define NBR_FORTH_MAX 10
#define BORDURE_X 500
#define BORDURE_Y 500
#define DEFAULT_TITLE  "GRAPHE"
#define THICKNESS 5
#define GRILLAGE_X 100.0
#define GRILLAGE_Y 100.0
#define MAX_POIDS 5000

//QQ
#define DISTANCE_CLIPPING 100000
#define ALTITUDE_MIN_CAMERA 5
#define ALTITUDE_MAX_CAMERA (DISTANCE_CLIPPING-1)
#define VITESSE_DEROULEMENT_PLAN 120
#define VITESSE_DEROULEMENT_VERT 120

typedef struct t_souris {
     float x,y,dx,dy;
} t_souris;

typedef struct
{
     float R;
     float G;
     float B;
     float A; //alpha
} t_couleur;

#define t_densite t_couleur

typedef struct elem {
     int valeur ;
     struct elem * suivant ;
} element ;
typedef element * t_liste;

typedef struct
{
     float x;
     float y;
     float z;
}t_pos;

typedef struct
{
     float x;
     float y;
     float z;
     int s;
}t_pos2;

typedef struct
{
     t_pos position ;
     t_pos cible ;
     t_pos orientation ;
     float zoom;
}t_camera;

typedef struct
{
     char *nom;
     t_pos pos;
     int id;
     t_couleur densite;
     char *mot_forth;
     int selec;
}t_sommet;

typedef struct
{
  char *nom;
  char *mot_forth;
  t_pos2 pos1;
  t_pos2 pos2;
  t_couleur densite;
  int id;
  float cout;
  t_pos poignet;
  int selec;
  int triangle;
}t_arete;

typedef struct
{
     char *nom;
     char *mot_forth;
     int id;
     t_couleur densite;
     t_pos poignet;
     t_liste liste_arete;
     int nbr_arete;
     int selec;
}t_zone;

typedef struct
{
  int id;
  t_pos poignet;
  t_liste liste_arete;
  int selec;
  t_pos normal;
}t_triangle;

typedef struct
{
     int id;
     char *code;
}t_forth;

typedef float t_poids;

typedef struct sommets * l_graphe;
typedef struct adj * som_adj;

typedef struct adj
{
     l_graphe vsom;
     t_poids cout;
     som_adj suiv;
}s_adj;

typedef struct
{
     t_sommet som;
     som_adj succ;
     l_graphe suiv;
}s_sommet;

t_camera camera;
t_souris souris;
t_couleur Couleur;
int presse;
int sommet_selec1;
int sommet_selec2;
int arete_selec;
int zone_selec;
int x;
int y;
int selecx;
int selecy;
int xold;
int yold;
t_sommet som[NBR_SOMMET_MAX];
t_arete arete[NBR_ARETE_MAX];
t_zone zone[NBR_ZONE_MAX];
t_triangle triangl[NBR_TRIANGLE_MAX];
t_forth forth[NBR_FORTH_MAX];
int nbr_sommet;
int nbr_arete;
int nom_arete;
int nom_zone;
int nbr_zone;
int nbr_triangle;
int nbr_forth;
int nbr_arete_zone;
int nbr_selec_som;
int nbr_selec_arete;
int nbr_selec_zone;
int trianguler_mode;
int coordx;
int coordy;
//char mysql_nom_base_donnee[100]="";
MYSQL_RES *result;
MYSQL_ROW row;
MYSQL *connection, mysql;
l_graphe liste_adjacence;
t_liste listetemp;
int g_id;
/*GdkGLConfig *configure_gl      (void);*
  GtkWidget   *create_window     (GdkGLConfig *glconfig);*/

void TsommetInit();
void TareteInit();
void TzoneInit();
void TtriangleInit();
/*void openglInit();*/
void mode_normal();
void calcul_coord();
void affich_struct();
void clavier(unsigned char touche,int x,int y);
void mouse(int button,int state,int x, int y);

void selec_sommet();
void selec_arete();
void selec_zone();
void deplace_sommet();
void deplace_sommet_zone();

void ajout_som();
void suppr_som();

void ajout_arete();
void suppr_arete();
int suppr_zone_arete(t_liste temp);

void ajout_zone();
void mysql_create_tab_arete_pour_zone();
void mysql_remplissage_tab_arete_pour_zone (t_liste *);
void suppr_zone();

int db_connect(const char *,const char *,const char *,const char *);
void mysql_execreq(char *);
void exit_mysql();
void myinitsql();
void mysql_modif_mot_forth (char *mot,int val_id,char *cellule);
void mysql_insert_som(float, float,float, float, float ,float, float);
int erreur_ajout_arete(int poingetx,int poignety);
void mysql_insert_arc(float, float, float,float, float,int, int,int);
void mysql_insert_zone(float, float, float, float,float, float,int,t_liste);

void mysql_delete_som();
void mysql_delete_arc();
void mysql_delete_zone();

void mysql_db_som();
void mysql_db_arc();
void mysql_db_zone();
void mysql_db_triangle();
char* mysql_db_forth();

void mysql_create_database(const char* dbname);
void mysql_delete_db(const char* dbname);
void mysql_chgt_db(const char* dbname);

void modif_sql_sommet(int);
void modif_mot_forth(char *,int,char*);
void vide_som_sql();
void sphere();
void application1(float x,float y);

/*void modif_mot_forth (char mot[64]);*/
int ordresom();
int iemesucc(int ieme);
void sphere();

char * init_chaine (char* chaine);
l_graphe creation_graphe();

l_graphe ajout_sommet (l_graphe G, int s);

l_graphe recherche (l_graphe g, int s);

int ajout_arete_la (l_graphe g,int s ,int i);

int ordre(l_graphe G);

int *classif_O(l_graphe G,int *pere,int *op,int *os);

int * classif_NO(l_graphe g,int *pere);

int *parcours_profondeur_init(l_graphe g,int s);

void p_prof_recO(l_graphe g,int *m,int *op,int *os,int *t,int *pere);

void p_porf_recNO(l_graphe g,int *m,int *pere);

int test_chaine_rec(l_graphe pi, l_graphe pj, int *marque);

int existence_chaine(l_graphe g,int i,int j);

int recherche_cycle_rec(l_graphe pi, l_graphe pere, int *marque);

void parcours_prof_rec_comp(l_graphe g,int *comp, int no);


int composantes_connexes(l_graphe g, int *comp);

void points_art_rec (l_graphe g, l_graphe pere, int *op,int *cpt,int *plushaut, int *points);

int *points_art_init(l_graphe g);

t_poids *dijkstra(l_graphe g,int s,int **pere);
char*  floattostring (float f);

t_liste init_liste ();
void insert_liste (int x,t_liste* l);
void suppr_liste (t_liste* l);
int suppr_elt (t_liste *l);
int fin_liste (t_liste* l);
void affich_liste (t_liste l);
int nbr_elt_liste (t_liste l);
t_liste string_to_liste (char *chaine);
char* liste_to_string (t_liste l);

int recherche_pos_tab_arete (int x);

void init_tab_selec();

int id_to_tab (int val_id);
int id_to_tab2 (int val_id);
int ieme_succ(int s, int ieme);
void calcul_poignet_arete_et_zone_sommet();
void calcul_poignet_zone_sommet();
int degre(int s);
void prog_forth (const char *prog);
char *mysql_succ(int sommet);
void tab_to_mysql_arete();
void tab_to_mysql_som();
void tab_to_mysql_zone();
void rotation(int,int,int,int);
char *ret_mysql(char *requete);
int ieme_succ_arete (int ordre, int val_id_som);

int extremite(int s, int a);
#endif /*AFFICHAGE_F*/
