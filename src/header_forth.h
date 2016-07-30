#ifndef HEADER_FORTH_H
#define HEADER_FORTH_H

#define MODE_COMPILATION 0
#define MODE_EXECUTION 1
#define LIRE_TERMINAL 0
#define LIRE_FICHIER 1

#define TAILLE_PILE_INIT 10
#define TAILLE_VECTEUR_MOT 100
#define TAILLE_BUFFER_INIT 1
#define MAX_ELT_HASH 10000
#define LONGUEUR_NOM_MOT 64

#define FONCTION_C void (*fp)(void)

#define MAJUSCULE(c) ( ((c >= 'a') && (c <= 'z')) ? (c - ('a' - 'A')) : (c) )
#define MINUSCULE(c) ( ((c >= 'A') && (c <= 'Z')) ? (c + ('a' - 'A')) : (c) )

#include <gtk/gtk.h>

#define bool char
#define octet unsigned char
#define true 1
#define false 0

/*
** ===============================================
**
**                    ENUMERATEUR
**
** ===============================================
*/

typedef enum t_erreur {
     CORRECT,
     ERREUR_CAR_TROP_GRAND,
     ERREUR_PAREN_FERMANTE,
     ERREUR_PAREN_OUVRANTE,
     ERREUR_MOT_INCONNU,
     ERREUR_MOT_DEJA_CONNU,
} t_erreur;

typedef enum t_outil {
     AUCUN, SELECTION, AJOUT, SUPPRESSION, DEPLACEMENT,
} t_outil;

typedef enum t_cell {
  TOUTES, AUCUNE, SOMMET, ARC, ZONE,TRIANGLE,
} t_cell;

typedef enum t_type {
     ENTIER, REEL, ADRESSE, HANDLE, CFA, NFA, AD_NULL,
     AD_ENTIER, AD_REEL, AD_STRING,
} t_type;

typedef enum t_nom_champ {
     ID, NOM, FORTH, POSITION, POSX, POSY, POSZ, COUT,
     LONGUEUR, AIRE, PERIMETRE, DENSITE, POIGNET, 
     POIGNETX, POIGNETY, COULEUR, ROUGE, VERT, BLEU, ALPHA,
     NB_ARC, 
} t_nom_champ;

typedef enum t_vue {
     VUE_2D, VUE_3D,
} t_vue;

typedef struct t_lassot {
     int x1,y1,x2,y2;
     bool show;
} t_lassot;

/*
** ===============================================
**
**                    ITERATEUR
**
** ===============================================
*/

typedef struct t_iterateur {
     int x,y,oldx,oldy;
} t_iterateur;


/*
** ======================================================
**
**    CODE FIELD ADDRESSE --- CODE FIELD
**
**    un mot forth est un tab dynamique de CF.
**  La case zero du tab sert a stocker un NFA.
**  >> tab[0].type = TYPE_NFA
**  >> tab[0].adresse = (t_nfa) XXX.
**
**  Les autres cases contiennent l'adresse d'un token
**  cad l'adresse de la case zero du tab (debut du mot)
**  >> tab[i].type = TYPE_CFA
**  >> tab[i].adresse = (t_cfa) XXX.
**
**  Elles peuvent aussi contenir des entiers, des 
**          flottants et des adresses.
**
**  ATTENTION : lors de l'execution d'un mot (cad lors 
**      du parcours de l'arbre) a ne pas utiliser
** la case zero du tab mais celle qui suit (la case une).
**
**  ATTENTION : la pile (de retour) contient des 
**  cellules qui sont des ad sur des cfa (TYPE_HANDLE).
** ========================================================
*/

typedef struct t_cf * t_cfa;  // Code Field Addresse
typedef struct t_cf {         // Code Field
     t_type type;
     union {
	  int    entier;
	  float  reel;
	  void * adresse;
     };
} t_cf;

/* 
** ===============================================
**
**                         PILE
**
** ===============================================
*/

typedef struct t_pile {
     int taille_allouee;
     int position;
     t_cfa donnees; //tab dynamique de cfa
} t_pile;

/*
** ===============================================
**
**                  NAME FIELD ADDRESSE
**
** ===============================================
*/

typedef struct t_nf * t_nfa;
typedef struct t_nf {     
     octet     info;   
     char*     nom;
     FONCTION_C;
     GtkTreeIter  ad_gtk;
     unsigned int taille_allouee;
     unsigned int taille_utilisee;
} t_nf;

/*
** ===============================================
**
**                  TABLE DE HACHAGE
**
** ===============================================
*/
typedef enum t_etat {
     LIBRE, VIDE, OCCUPE,
} t_etat;

typedef struct t_cellule_hash {
     void *        donnee;
     void *        cle;
     t_etat        etat;
     int           lien;
} t_cellule_hash;

typedef struct t_hash {
     t_cellule_hash tab[MAX_ELT_HASH];
     int   reserve;
     int   nb_elt;
} t_hash;

/*
** ===============================================
**
**                  BUFFER    
**
** ===============================================
*/

typedef struct t_buffer {
     char * str;
     unsigned int taille_allouee;
} t_buffer;

/*
** ===============================================
**
**                   GARBAGE COLLECTOR
**
** ===============================================
*/

typedef struct t_trash {
     unsigned int     i;
     void*            tab[100];
} t_trash;

/*
** ===============================================
**
**             BOYAUX DE L'INTERPRETEUR    
**
** ===============================================
*/
typedef struct t_guts {
     GTimer           *chrono;
     bool             erreur;
     t_hash           dictionnaire;
     t_trash          main_trash;
     t_pile           pile_donnees;
     t_pile           pile_retour;
     gchar*           tampon;
     gchar*           mytoken;       // 
     t_iterateur      iter;          // 
     octet            mode;          // Compilation --- Execution
     bool	      debugger;
     t_cfa            dernier_mot_cree;
     t_cfa            IP;               // Pointeur d'Interpretation
     t_cfa            PC;               // pointeur en cours
     t_nfa            nfa;              //
     t_cell           cellule;
     t_outil          outil;
     t_vue            vue;
     float            width,height;
     t_nom_champ      champ;
} t_guts;


/*
** ===============================================
**
**                PARAMETRES GLOBAUX    
**
** ===============================================
*/
t_guts guts;

GtkWidget *menu_popup;

GtkWidget *drawingarea;
GtkTreeModel *cl_dico;
GtkTreeStore *store_champ;
GtkTreeSelection *select_champ;
GtkListStore *store_pile;
GtkTreeIter iter_pile;
GtkTextTag *balise_rouge, *balise_soulignee,  
     *balise_bonne, *balise_erreur, *balise_normale;
GtkTextBuffer *bf_nouv, *bf_res, *bf_svgd_code, *bf_debug;
GtkWidget * win_main, *win_champ;
t_lassot lassot;

#endif /* HEADER_FORTH_H */
