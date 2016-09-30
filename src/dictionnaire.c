#include "dictionnaire.h"


static void extremite_core()
{
     int s,a;

     s = sommet(&guts.pile_donnees).entier;
     depiler(&guts.pile_donnees);
     a = sommet(&guts.pile_donnees).entier;
     depiler(&guts.pile_donnees);
     empiler(&guts.pile_donnees, int_to_elt(extremite(s,a)));
}

static void screen_core()
{
     screenshot();
}

static void sqrt_core()
{
       t_cf e;

       e = sommet(&guts.pile_donnees);
       guts.pile_donnees.donnees[guts.pile_donnees.position].type = REEL;
       if (e.type == ENTIER)
            guts.pile_donnees.donnees[guts.pile_donnees.position].reel = sqrt(e.entier);
       else
            guts.pile_donnees.donnees[guts.pile_donnees.position].reel = sqrt(e.reel);
}

/* mot Forth : >STRING */
static void to_string_core()
{
     t_cf e;
     char* ch;

     ch = (char*) xmalloc(10*sizeof(char));
     e = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     if (e.type == ENTIER)
          sprintf(ch,"%d",e.entier);
     else if (e.type == REEL)
          sprintf(ch,"%f",e.reel);
     else guts.erreur = true;
     empiler(&guts.pile_donnees, ad_to_elt(ch, AD_STRING));
//     ajout_trash(&guts.main_trash, ch);
}

/* Mot FORTH : CARD */
static void nb_elt_pile_core()
{
     empiler(&guts.pile_donnees, int_to_elt(guts.pile_donnees.position));
}

/* Mot FORTH : CONCAT */
static void concat_core()
{
     t_cf e1,e2;
     char * ch;

     ch = nouvelle_chaine();
     e1 = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     e2 = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);

     ch = g_strconcat((char*)e2.adresse,(char*)e1.adresse,NULL);
     empiler(&guts.pile_donnees, ad_to_elt(ch, AD_STRING));
     g_free(e1.adresse); g_free(e2.adresse);
     //   ajout_trash(&guts.main_trash, ch);
}

/*
** ============================================
**
**  KERNEL : ne pas toucher a ces definitions.
**           Sans eux plus de definition de
**           nouveaux mots FORTH !!!!
**
** ============================================
*/

/* mot FORTH : SAVE */
static void save_core()
{
     t_nfa nfa;

     nfa = (t_nfa) guts.dernier_mot_cree[0].adresse;
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, guts.dernier_mot_cree, nfa->nom);
}

/* mot FORTH : SVGDE */
static void sauvegarde_core()
{
     t_nfa nfa;

     nfa = (t_nfa) guts.dernier_mot_cree[0].adresse;
     ajouter_mot_hash(&guts.dictionnaire, guts.dernier_mot_cree, nfa->nom);
}

/* mot FORTH : EXEC */
static void exec_core()
{
     guts.mode = MODE_EXECUTION;
}

/* mot FORTH : COMP */
static void comp_core()
{
     guts.mode = MODE_COMPILATION;
}

/* mot FORTH : BUILDS */
static void builds_core()
{
     nouveau_mot_courant();
}

/* mot FORTH : : */
void deux_points_core()
{
     t_cfa mot;

     mot = nouveau_mot(":", EMPTY_WORD, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "COMP"), CFA);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "BUILDS"), CFA);
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, ":");
}

/* mot FORTH : ; */
void point_virgule_core()
{
     t_cfa mot;

     mot = nouveau_mot(";", PRECEDENCE_BIT, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "EXEC"), CFA);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "SAVE"), CFA);
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, ";");
}

/* mot FORTH : :REC */
void deux_points_rec_core()
{
     t_cfa mot;

     mot = nouveau_mot(":REC", PRECEDENCE_BIT, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "COMP"), CFA);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "BUILDS"), CFA);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "SVGDE"), CFA);
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, ":REC");
}

/* mot FORTH : ;REC */
void point_virgule_rec_core()
{
     guts.mode = MODE_EXECUTION;
     ajouter_mot_dans_visualdico(cl_dico, guts.dernier_mot_cree);
#if 0
     t_cfa mot;

     mot = nouveau_mot(";REC", PRECEDENCE_BIT, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "EXEC"), CFA);
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, ";REC");
#endif
}

void forget_core()
{
     guts.mytoken = my_cut(&guts.tampon, guts.mytoken, &guts.iter);
     supprimer_mot_hash(&guts.dictionnaire, guts.mytoken);
}

/*
** ============================================
**
** MOTS LIES A LA COMPILATION
**
** ============================================
*/

/* mot FORTH : COMPILE */
static void compile_core()
{
     ++guts.IP;
     ajouter_elt_dans_mot(guts.dernier_mot_cree, *(guts.IP));
}

/* mot FORTH : }COMPILE */
static void end_compile_core()
{
     //NE FAIT RIEN
}

/* mot FORTH : COMPILE{ */
static void begin_compile_core()
{
     t_cfa ad;
     ad = guts.dictionnaire.tab[chercher_donnee_hash(&guts.dictionnaire, "}COMPILE")].donnee;
     while ((*(guts.IP+1)).adresse != ad)
     {
          ++guts.IP;
          ajouter_elt_dans_mot(guts.dernier_mot_cree, *(guts.IP));
     }
}

/*
** ============================================
**
** AUTRES MOTS IMPORTANTS
**
** ============================================
*/

/* mot FORTH : LITERAL */
static void literal_core()
{
     ++guts.IP; ++guts.PC;
     empiler(&guts.pile_donnees, *(guts.PC));
}

/* mot FORTH : ALLOT */
static void allot_core()
{
     int i, nb;

     /* nb = guts.dernier_mot_cree.taille_utilisee */
     nb = sommet(&guts.pile_donnees).entier;
     depiler(&guts.pile_donnees);
     for (i = 0; i < nb; i++)
          ajouter_adresse(guts.dernier_mot_cree, NULL, AD_NULL);
}

/* mot FORTH : , */
static void comma_core()
{
     //non le vrai mot est : ajouter_elt_dans_mot(@ contenue dans HERE, sommet(&guts.pile_donnees));
     ajouter_elt_dans_mot(guts.dernier_mot_cree, sommet(&guts.pile_donnees));
     depiler(&guts.pile_donnees);
}

/* mot FORTH : IMMEDIATE */
static void immediat_core()
{
     t_nfa nfa;

     nfa = (t_nfa) guts.dernier_mot_cree[0].adresse;
     change_masque(&nfa->info, PRECEDENCE_BIT);
}

/* mot FORTH : FIND */
static void find_core()
{
     guts.mytoken = my_cut(&guts.tampon, guts.mytoken, &guts.iter);
     t_cfa cfa = chercher_mot_hash(&guts.dictionnaire, guts.mytoken);
     if (cfa == NULL)
          empiler(&guts.pile_donnees, int_to_elt(0));
     else
          empiler(&guts.pile_donnees, ad_to_elt(cfa,ADRESSE));
}


/* mot FORTH ." */
static void create_string_core()
{
     char * ch;
     t_cfa mot;
     char s[2];

     s[1] = '\0';
     ch = nouvelle_chaine();
     ++guts.tampon;// ++guts.iter;
     while ((*guts.tampon != '\0') && ('\"' != *guts.tampon))
     {
          s[0] = *guts.tampon;
          ch = g_strconcat(ch, s, NULL);
          ++guts.tampon;
          //++guts.iter;
     }

     if (guts.mode == MODE_EXECUTION)
          empiler(&guts.pile_donnees, ad_to_elt(ch, AD_STRING));
     else
     {
          mot = chercher_mot_hash(&guts.dictionnaire, "LITERAL");
          ajouter_adresse(guts.dernier_mot_cree, mot, CFA);
          ajouter_elt_dans_mot(guts.dernier_mot_cree, ad_to_elt(ch, AD_STRING));
     }

#if 0
     guts.mytoken = my_cut(&guts.tampon, guts.mytoken, &guts.iter);
     while ((guts.mytoken != "") && strcmp("\"",guts.mytoken) != 0)
     {
          if (prem) ch = g_strconcat(ch, guts.mytoken, NULL);
          else ch = g_strconcat(ch, " ", guts.mytoken, NULL);
          prem = false;
          guts.mytoken = my_cut(&guts.tampon, guts.mytoken, &guts.iter);
     }
     empiler(&guts.pile_donnees, ad_to_elt(ch, AD_STRING));
#endif
}

/* mot FORTH " */
static void guillemets_core()
{
/* ne fait rien */
}

/* mot FORTH : MYSQL */
static void mysql_core()
{
     t_cf e;

     e = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     selection_forth(ret_mysql((char*) e.adresse));
     g_free(e.adresse);
}

/* mot FORTH : @END-! */
static void end_store_core()
{
     t_cf e1, e2;
     t_cfa mot;

     e1 = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     e2 = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);

     mot = adresse_fin_mot(guts.dernier_mot_cree) - e1.entier / sizeof(t_cf);
     *mot = e2;
}

/* mot FORTH : ! */
static void store_core()
{
     t_cf e1, e2;
     GtkTextIter fin;

     e1 = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     e2 = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);

     switch (e1.type)
     {
     case ADRESSE :
          *((t_cfa) e1.adresse) = e2;
          break;
     case AD_ENTIER :
          *((int*) e1.adresse) = (int) e2.entier;
          break;
     case AD_REEL :
          *((float*) e1.adresse) = (float) e2.reel;
          break;
//     case AD_STRING :
//        *((float*) e1.adresse) = (char*) e2.adresse;
//        break;
     default :
          guts.erreur = true;
          printf("\007Erreur : le sommet de la pile doit etre une adresse pour que le token @ puisse fonctionner correctement.\nOr ce n'est pas le cas ici !!\n");
          gtk_text_buffer_get_end_iter(bf_res, &fin);
          gtk_text_buffer_insert(bf_res, &fin, "Erreur : le sommet de la pile doit etre une adresse pour que le token @ puisse fonctionner correctement.\nOr ce n'est pas le cas ici !!\n", -1);
          break;
     }
}

/* mot FORTH : @ */
static void fetch_core()
{
     GtkTextIter fin;
     t_cf elt;

     elt = sommet(&guts.pile_donnees);
     switch (elt.type)
     {
     case ADRESSE :
          guts.pile_donnees.donnees[guts.pile_donnees.position] = *((t_cfa) elt.adresse);
          break;
     case AD_ENTIER :
          guts.pile_donnees.donnees[guts.pile_donnees.position].entier = *((int*) elt.adresse);
          guts.pile_donnees.donnees[guts.pile_donnees.position].type = ENTIER;
          break;
     case AD_REEL :
          guts.pile_donnees.donnees[guts.pile_donnees.position].reel = *((float*) elt.adresse);
          guts.pile_donnees.donnees[guts.pile_donnees.position].type = REEL;
          break;

     default :
          guts.erreur = true;
          printf("\007Erreur : le sommet de la pile doit etre une adresse pour que le token @ puisse fonctionner correctement.\nOr ce n'est pas le cas ici !!\n");
          gtk_text_buffer_get_end_iter(bf_res, &fin);
          gtk_text_buffer_insert(bf_res, &fin, "Erreur : le sommet de la pile doit etre une adresse pour que le token @ puisse fonctionner correctement.\nOr ce n'est pas le cas ici !!\n", -1);
          break;
     }
}

/*
** ============================================
**
** MOT DE DEFINITION
**
** ============================================
*/

/* mot FORTH : CREATE creer un nouveau mot et alloue 2 emplacements */
void create_core()
{
     t_cfa mot;

     mot = nouveau_mot("CREATE", EMPTY_WORD, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "BUILDS"), CFA);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "LITERAL"), CFA);
     ajouter_elt_dans_mot(mot, int_to_elt(2));
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "ALLOT"), CFA);

     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, "CREATE");
}

/* mot FORTH : DOES il doit toujours etre employe avec CREATE */
void does_core()
{
     t_cfa mot;

     mot = nouveau_mot("DOES", EMPTY_WORD, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "DOES1"), CFA);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "DOES2"), CFA);

     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, "DOES");
}

static void does1_core()
{
     /* preparation du deroutement */
     guts.dernier_mot_cree[1].adresse = chercher_mot_hash(&guts.dictionnaire, "DOES2");
     guts.dernier_mot_cree[1].type    = CFA;

     guts.dernier_mot_cree[2].adresse = (t_cfa) sommet(&guts.pile_retour).adresse;
     guts.dernier_mot_cree[2].type    = HANDLE;
     depiler(&guts.pile_retour);

     save_core();
     /* arret de l'execution */
     while ((*(guts.IP+1)).adresse != NULL)
          ++guts.IP;
     guts.PC = guts.IP;


//*((t_cfa) sommet(&guts.pile_retour).adresse);
}

static void does2_core()
{
     empiler(&guts.pile_donnees, ad_to_elt(guts.IP+2, ADRESSE));
     //empiler(&guts.pile_donnees, *(guts.IP+2));

     /*
     ** deroutement sur le mot DOES et non pas le mot
     ** qui suit DOES comme le suggere [SALMAN83]
     ** car dans execute_mot (interpreteur.c)
     ** apres avoir executer la fct C guts.IP est
     ** incremente de 1.
     */
     guts.IP = (*(guts.IP+1)).adresse;
     --guts.IP;
}

/* mot FORTH : CONSTANT */
void constant_core()
{
     t_cfa mot;

     mot = nouveau_mot("CONSTANT", PRECEDENCE_BIT, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "CREATE"), CFA);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, ","), CFA);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "DOES"), CFA);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "@"), CFA);
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, "CONSTANT");
}

/* mot FORTH : VARIABLE */
void variable_core()
{
     t_cfa mot;

     mot = nouveau_mot("VARIABLE", PRECEDENCE_BIT, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "CREATE"), CFA);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, ","), CFA);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "DOES"), CFA);
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, "VARIABLE");
}

/*
** ============================================
**
** BRANCHEMENT
**
** ============================================
*/

/* mot FORTH : HERE */
static void here_core()
{
     t_nfa nfa;
     t_cfa cfa;

     cfa = guts.dernier_mot_cree;
     nfa = (t_nfa) (*cfa).adresse;
     empiler(&guts.pile_donnees, ad_to_elt(cfa+nfa->taille_utilisee, ADRESSE));
     //normalement c'est ad_to_elt(adresse_fin_mot(cfa, ADRESSE)
}

/* mot FORTH : BRANCH  il est tjs suivit d'une @ ou un nombre */
static void branch_core()
{
     guts.PC = guts.IP + 1;
     if ((*guts.PC).type == ADRESSE)
          guts.IP += ((*guts.PC).entier) / sizeof(t_cf);
     else
          guts.IP += (*guts.PC).entier;
}

/* mot FORTH : 0BRANCH */
static void zero_branch_core()
{
     t_cf elt = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);

     if ((elt.adresse == NULL) || (elt.entier == 0))
          branch_core();
     else
     {
          ++guts.IP; //il faut sauter le nombre qui suit 0BRANCH
          ++guts.PC;
     }
}

/* mot FORTH : 1BRANCH */
static void un_branch_core()
{
     if (pile_vide(&guts.pile_donnees))
          return ;

     t_cf elt = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);

     if (elt.entier == 1)
          branch_core();
     else
     {
          ++guts.IP; //il faut sauter le nombre qui suit 1BRANCH
          ++guts.PC;
     }
}

/*
** ============================================
**
** MANIPULATION DES PILES
**
** ============================================
*/

/* mot FORTH : ABORT */
static void abort_core()
{
     reinit_pile(&guts.pile_donnees);
     reinit_pile(&guts.pile_retour);
}

/* mot FORTH : >R */
static void data_to_return_core()
{
     empiler(&guts.pile_retour, sommet(&guts.pile_donnees));
     depiler(&guts.pile_donnees);
}

/* mot FORTH : R> */
static void return_to_data_core()
{
     empiler(&guts.pile_donnees, sommet(&guts.pile_retour));
     depiler(&guts.pile_retour);
}

/* mot FORTH : DUP */
static void dup_core()
{
     empiler(&guts.pile_donnees, sommet(&guts.pile_donnees));
}

/* mot FORTH : DROP */
static void drop_core()
{
     depiler(&guts.pile_donnees);
}

/* mot FORTH : SWAP */
static void swap_core()
{
     t_cf elt = guts.pile_donnees.donnees[guts.pile_donnees.position-1];
     guts.pile_donnees.donnees[guts.pile_donnees.position-1] =
          guts.pile_donnees.donnees[guts.pile_donnees.position];
     guts.pile_donnees.donnees[guts.pile_donnees.position] = elt;
}

/* mot FORTH : OVER */
static void over_core()
{
     t_cf elt = guts.pile_donnees.donnees[guts.pile_donnees.position-1];
     empiler(&guts.pile_donnees, elt);
}

/* mot FORTH : ROT */
static void rot_core()
{
     t_cf e1, e2, e3;

     e1 = guts.pile_donnees.donnees[guts.pile_donnees.position];
     e2 = guts.pile_donnees.donnees[guts.pile_donnees.position-1];
     e3 = guts.pile_donnees.donnees[guts.pile_donnees.position-2];

     guts.pile_donnees.donnees[guts.pile_donnees.position]   = e3;
     guts.pile_donnees.donnees[guts.pile_donnees.position-1] = e1;
     guts.pile_donnees.donnees[guts.pile_donnees.position-2] = e2;
}

/* mot FORTH : 2DUP */
static void deux_dup_core()
{
     over_core();
     over_core();
}

/* mot FORTH : 2DROP */
static void deux_drop_core()
{
     depiler(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
}

/* mot FORTH : 2SWAP */
static void deux_swap_core()
{
     rot_core(); data_to_return_core();
     rot_core(); return_to_data_core();
}

/* mot FORTH : 2OVER */
static void deux_over_core()
{
     data_to_return_core(); data_to_return_core();
     deux_dup_core();
     return_to_data_core(); return_to_data_core();
     deux_swap_core();
}

/* mot FORTH : 2ROT */
static void deux_rot_core()
{
     data_to_return_core(); data_to_return_core(); deux_swap_core();
     return_to_data_core(); return_to_data_core(); deux_swap_core();
}

/*
** ============================================
**
**  AFFICHAGE
**
** ============================================
*/

/* mot FORTH : CR */
static void retour_chariot_core()
{
     printf("\n");
}

/* mot FORTH : TAB */
static void tabulation_core()
{
     printf("\t");
}

/* mot FORTH : WORDS */
static void words_core()
{
     afficher_hash(&guts.dictionnaire);
}

/* FORTH : . */
static void afficher_core()
{
//     GtkTextIter fin;

//     printf("---> ");
//     gtk_text_buffer_get_end_iter(bf_res, &fin);
//     gtk_text_buffer_insert(bf_res, &fin, "---> ", -1);
     afficher_elt(sommet(&guts.pile_donnees));
     afficher_elt_buffer(sommet(&guts.pile_donnees), bf_res);
     depiler(&guts.pile_donnees);
//     gtk_text_buffer_get_end_iter(bf_res, &fin);
//     gtk_text_buffer_insert(bf_res, &fin, "\n", -1);
     printf("\n");
}

/* FORTH : PRETURN */
static void return_stack_core()
{
     GtkTextIter fin;

     printf("pile retour ");
     afficher_pile(guts.pile_retour);
     printf("\n");
     gtk_text_buffer_get_end_iter(bf_res, &fin);
     gtk_text_buffer_insert(bf_res, &fin, "pile retour : ", -1);
     afficher_pile_buffer(guts.pile_retour, bf_debug);
     gtk_text_buffer_get_end_iter(bf_res, &fin);
     gtk_text_buffer_insert(bf_res, &fin, "\n", -1);
}

/* FORTH : PDATA */
static void data_stack_core()
{
     GtkTextIter fin;

     printf("pile donnees "); afficher_pile(guts.pile_donnees); printf("\n");
     gtk_text_buffer_get_end_iter(bf_res, &fin);
     gtk_text_buffer_insert(bf_res, &fin, "pile donnees : ", -1);
     afficher_pile_buffer(guts.pile_donnees, bf_debug);
     gtk_text_buffer_get_end_iter(bf_res, &fin);
     gtk_text_buffer_insert(bf_res, &fin, "\n", -1);
}

/*
** ============================================
**
**  OPERATIONS ARTHIMETIQUE ET DE LOGIQUE
**
** ============================================
*/

void addition_core()
{
     t_cf a,b;

     a = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     b = sommet(&guts.pile_donnees);
     if ((a.type == REEL) && (b.type == REEL))
          guts.pile_donnees.donnees[guts.pile_donnees.position].reel = a.reel+b.reel;
     else if ((a.type == ENTIER) && (b.type == ENTIER))
          guts.pile_donnees.donnees[guts.pile_donnees.position].entier = a.entier+b.entier;
     else if ((a.type == ADRESSE) && (a.type == ADRESSE))
          guts.pile_donnees.donnees[guts.pile_donnees.position].entier = b.adresse - a.adresse;
     else if (a.type == ENTIER)
     {
          guts.pile_donnees.donnees[guts.pile_donnees.position].reel = a.entier+b.reel;
          guts.pile_donnees.donnees[guts.pile_donnees.position].type = REEL;
     }
     else
     {
          guts.pile_donnees.donnees[guts.pile_donnees.position].reel = a.reel+b.entier;
          guts.pile_donnees.donnees[guts.pile_donnees.position].type = REEL;
     }
}

void multiplication_core()
{
     t_cf a,b;

     a = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     b = sommet(&guts.pile_donnees);
     if ((a.type == REEL) && (b.type == REEL))
          guts.pile_donnees.donnees[guts.pile_donnees.position].reel = a.reel*b.reel;
     else if ((a.type == ENTIER) && (b.type == ENTIER))
          guts.pile_donnees.donnees[guts.pile_donnees.position].entier = a.entier*b.entier;
     else if (a.type == ENTIER)
     {
          guts.pile_donnees.donnees[guts.pile_donnees.position].reel = a.entier*b.reel;
          guts.pile_donnees.donnees[guts.pile_donnees.position].type = REEL;
     }
     else
     {
          guts.pile_donnees.donnees[guts.pile_donnees.position].reel = a.reel*b.entier;
          guts.pile_donnees.donnees[guts.pile_donnees.position].type = REEL;
     }
}

void soustraction_core()
{
     t_cf a,b;

     a = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     b = sommet(&guts.pile_donnees);
     if ((a.type == REEL) && (b.type == REEL))
          guts.pile_donnees.donnees[guts.pile_donnees.position].reel = b.reel - a.reel;
     else if ((a.type == ENTIER) && (b.type == ENTIER))
          guts.pile_donnees.donnees[guts.pile_donnees.position].entier = b.entier - a.entier;
     else if ((a.type == ADRESSE) && (a.type == ADRESSE))
          guts.pile_donnees.donnees[guts.pile_donnees.position].entier = b.adresse - a.adresse;
     else if (a.type == ENTIER)
     {
          guts.pile_donnees.donnees[guts.pile_donnees.position].reel = a.entier*b.reel;
          guts.pile_donnees.donnees[guts.pile_donnees.position].type = REEL;
     }
     else
     {
          guts.pile_donnees.donnees[guts.pile_donnees.position].reel = a.reel*b.entier;
          guts.pile_donnees.donnees[guts.pile_donnees.position].type = REEL;
     }
}

void division_core()
{
     t_cf a,b;

     a = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     b = sommet(&guts.pile_donnees);
     if ((a.type == REEL) && (b.type == REEL))
          guts.pile_donnees.donnees[guts.pile_donnees.position].reel = b.reel/a.reel;
     else if ((a.type == ENTIER) && (b.type == ENTIER))
          guts.pile_donnees.donnees[guts.pile_donnees.position].entier = b.entier/a.entier;
     else if (a.type == ENTIER)
     {
          guts.pile_donnees.donnees[guts.pile_donnees.position].reel = b.reel/a.entier;
          guts.pile_donnees.donnees[guts.pile_donnees.position].type = REEL;
     }
     else
     {
          guts.pile_donnees.donnees[guts.pile_donnees.position].reel = b.entier/a.reel;
          guts.pile_donnees.donnees[guts.pile_donnees.position].type = REEL;
     }
}

static void egalite_core()
{
     t_cf e1, e2;

     e1 = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     e2 = sommet(&guts.pile_donnees);
     guts.pile_donnees.donnees[guts.pile_donnees.position].entier = (e1.entier == e2.entier);
     guts.pile_donnees.donnees[guts.pile_donnees.position].type = ENTIER;
}

/* mot FORTH : 0= */
static void egalite_zero_core()
{
     guts.pile_donnees.donnees[guts.pile_donnees.position].entier = (sommet(&guts.pile_donnees).entier == 0);
     guts.pile_donnees.donnees[guts.pile_donnees.position].type = ENTIER;
}

/* mot FORTH : 0!= */
static void non_zero_core()
{
     guts.pile_donnees.donnees[guts.pile_donnees.position].entier = (sommet(&guts.pile_donnees).entier != 0);
     guts.pile_donnees.donnees[guts.pile_donnees.position].type = ENTIER;
}

static void inf_core()
{
     t_cf e1, e2;

     e1 = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     e2 = sommet(&guts.pile_donnees);
     guts.pile_donnees.donnees[guts.pile_donnees.position].entier = (e1.entier > e2.entier);
     guts.pile_donnees.donnees[guts.pile_donnees.position].type = ENTIER;
}

static void inf_egal_core()
{
     t_cf e1, e2;

     e1 = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     e2 = sommet(&guts.pile_donnees);
     guts.pile_donnees.donnees[guts.pile_donnees.position].entier = (e1.entier >= e2.entier);
     guts.pile_donnees.donnees[guts.pile_donnees.position].type = ENTIER;
}

static void sup_core()
{
     t_cf e1, e2;

     e1 = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     e2 = sommet(&guts.pile_donnees);
     guts.pile_donnees.donnees[guts.pile_donnees.position].entier = (e1.entier < e2.entier);
     guts.pile_donnees.donnees[guts.pile_donnees.position].type = ENTIER;
}

static void sup_egal_core()
{
     t_cf e1, e2;

     e1 = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     e2 = sommet(&guts.pile_donnees);
     guts.pile_donnees.donnees[guts.pile_donnees.position].entier = (e1.entier <= e2.entier);
     guts.pile_donnees.donnees[guts.pile_donnees.position].type = ENTIER;
}

/* mot FORTH : 1+ */
static void plus_un_entier_core()
{
     guts.pile_donnees.donnees[guts.pile_donnees.position].entier += 1;
}

/* mot FORTH : 1- */
static void moins_un_entier_core()
{
     guts.pile_donnees.donnees[guts.pile_donnees.position].entier -= 1;
}

/* mot FORTH : 1.+ */
static void plus_un_reel_core()
{
     guts.pile_donnees.donnees[guts.pile_donnees.position].reel += 1;
}

/* mot FORTH : 1.- */
static void moins_un_reel_core()
{
     guts.pile_donnees.donnees[guts.pile_donnees.position].reel -= 1;
}

/* mot FORTH : >INT */
static void to_int_core()
{
     float x = guts.pile_donnees.donnees[guts.pile_donnees.position].reel;
     guts.pile_donnees.donnees[guts.pile_donnees.position].type = ENTIER;
     guts.pile_donnees.donnees[guts.pile_donnees.position].entier = (int) x;
}

/* mot FORTH : >FLOAT */
static void to_float_core()
{
     int x = guts.pile_donnees.donnees[guts.pile_donnees.position].entier;
     guts.pile_donnees.donnees[guts.pile_donnees.position].type = REEL;
     guts.pile_donnees.donnees[guts.pile_donnees.position].reel = (float) x;
}

/*
** ============================================
**
** SIMTADYN : tt les fcts qui manipulent la carte
**
** ============================================
*/

/* mot FORTH : ARC */
static void arc_core()
{
     t_cf e1, e2;
     int s;
     GtkTextIter fin;
     char * ch;

     e1 = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     e2 = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     s = id_to_tab2(e2.entier);

     if (s == -1)
     {
          ch = (char*) xmalloc(30 * sizeof(char));
          printf("\007Le sommet %d n'existe pas !!\n", e2.entier);
          sprintf(ch, "Le sommet %d n'existe pas !!\n", e2.entier);
          gtk_text_buffer_get_end_iter(bf_res, &fin);
          gtk_text_buffer_insert(bf_res, &fin, ch, -1);
          free(ch);
          guts.erreur = true;
          return ;
     }
     switch (e1.entier)
     {
     case ID :
          empiler(&guts.pile_donnees, ad_to_elt(&arete[s].id, AD_ENTIER));
          break;
     case COUT :
          empiler(&guts.pile_donnees, ad_to_elt(&arete[s].cout, AD_REEL));
          break;
     default :
          ch = (char*) xmalloc(30 * sizeof(char));
          sprintf(ch, "Champ %d non recevable pour la structure ARC !!\n",e1.entier);
          printf("\007 Champ %d non recevable pour la structure ARC !!\n",e1.entier);
          gtk_text_buffer_get_end_iter(bf_res, &fin);
          gtk_text_buffer_insert(bf_res, &fin, ch, -1);
          free(ch);
          guts.erreur = true;
     }
}

/* A>S */
static void arc_to_som_core()
{
     int e;
     e = id_to_tab2(sommet(&guts.pile_donnees).entier);
     depiler(&guts.pile_donnees);
     empiler(&guts.pile_donnees, int_to_elt(arete[e].pos1.s));
     empiler(&guts.pile_donnees, int_to_elt(arete[e].pos2.s));
}

/* mot FORTH : SOMMET */
static void sommet_core()
{
     t_cf e1, e2;
     int s;
     GtkTextIter fin;
     char * ch;

     e1 = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     e2 = sommet(&guts.pile_donnees);
     depiler(&guts.pile_donnees);
     s = id_to_tab(e2.entier);

     if (s == -1)
     {
          ch = (char*) xmalloc(30 * sizeof(char));
          printf("\007Le sommet %d n'existe pas !!\n", e2.entier);
          sprintf(ch, "Le sommet %d n'existe pas !!\n", e2.entier);
          gtk_text_buffer_get_end_iter(bf_res, &fin);
          gtk_text_buffer_insert(bf_res, &fin, ch, -1);
          free(ch);
          guts.erreur = true;
          return ;
     }
     switch (e1.entier)
     {
     case ID :
          empiler(&guts.pile_donnees, ad_to_elt(&som[s].id, AD_ENTIER));
          break;
     case POIGNETX :
     case POSX :
          empiler(&guts.pile_donnees, ad_to_elt(&som[s].pos.x, AD_REEL));
          break;
     case POIGNETY :
     case POSY :
          empiler(&guts.pile_donnees, ad_to_elt(&som[s].pos.y, AD_REEL));
          break;
     case POSZ :
          empiler(&guts.pile_donnees, ad_to_elt(&som[s].pos.z, AD_REEL));
          break;
     case POIGNET :
     case POSITION :
          empiler(&guts.pile_donnees, ad_to_elt(&som[s].pos.z, AD_REEL));
          empiler(&guts.pile_donnees, ad_to_elt(&som[s].pos.y, AD_REEL));
          empiler(&guts.pile_donnees, ad_to_elt(&som[s].pos.x, AD_REEL));
          break;
     default :
          ch = (char*) xmalloc(30 * sizeof(char));
          sprintf(ch, "Champ %d non recevable pour la structure SOMMET !!\n",e1.entier);
          printf("\007 Champ %d non recevable pour la structure SOMMET !!\n",e1.entier);
          gtk_text_buffer_get_end_iter(bf_res, &fin);
          gtk_text_buffer_insert(bf_res, &fin, ch, -1);
          free(ch);
          guts.erreur = true;
     }
}

/* mot FORTH : COUT */
static void type_cout_core()
{
     t_cfa mot;

     mot = nouveau_mot("COUT", EMPTY_WORD, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "LITERAL"), CFA);
     ajouter_elt_dans_mot(mot, int_to_elt(COUT));
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, "COUT");
}

/* mot FORTH : ID */
static void type_ID_core()
{
     t_cfa mot;

     mot = nouveau_mot("ID", EMPTY_WORD, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "LITERAL"), CFA);
     ajouter_elt_dans_mot(mot, int_to_elt(ID));
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, "ID");
}

/* mot FORTH : POSX */
static void type_positionx_core()
{
     t_cfa mot;

     mot = nouveau_mot("POSX", EMPTY_WORD, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "LITERAL"), CFA);
     ajouter_elt_dans_mot(mot, int_to_elt(POSX));
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, "POSX");
}

/* mot FORTH : POSY */
static void type_positiony_core()
{
     t_cfa mot;

     mot = nouveau_mot("POSY", EMPTY_WORD, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "LITERAL"), CFA);
     ajouter_elt_dans_mot(mot, int_to_elt(POSY));
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, "POSY");
}

/* mot FORTH : POSZ */
static void type_positionz_core()
{
     t_cfa mot;

     mot = nouveau_mot("POSZ", EMPTY_WORD, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "LITERAL"), CFA);
     ajouter_elt_dans_mot(mot, int_to_elt(POSZ));
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, "POSZ");
}

/* mot FORTH : POSY */
static void type_position_core()
{
     t_cfa mot;

     mot = nouveau_mot("POSITION", EMPTY_WORD, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "LITERAL"), CFA);
     ajouter_elt_dans_mot(mot, int_to_elt(POSITION));
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, "POSITION");
}

/* mot FORTH : POIGX */
static void type_poignetx_core()
{
     t_cfa mot;

     mot = nouveau_mot("POIGX", EMPTY_WORD, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "LITERAL"), CFA);
     ajouter_elt_dans_mot(mot, int_to_elt(POIGNETX));
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, "POIGX");
}

/* mot FORTH : POIGY */
static void type_poignety_core()
{
     t_cfa mot;

     mot = nouveau_mot("POIGY", EMPTY_WORD, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "LITERAL"), CFA);
     ajouter_elt_dans_mot(mot, int_to_elt(POIGNETY));
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, "POIGY");
}

/* mot FORTH : POIGNET */
static void type_poignet_core()
{
     t_cfa mot;

     mot = nouveau_mot("POIGNET", EMPTY_WORD, NULL);
     ajouter_adresse(mot, chercher_mot_hash(&guts.dictionnaire, "LITERAL"), CFA);
     ajouter_elt_dans_mot(mot, int_to_elt(POIGNET));
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, mot, "POIGNET");
}

/* mot FORTH : NSOM */
static void nb_sommets_core()
{
     empiler(&guts.pile_donnees, int_to_elt(nbr_sommet));
}

/* mot FORTH : NARC */
static void nb_arcs_core()
{
     empiler(&guts.pile_donnees, int_to_elt(nbr_arete));
}

/* mot FORTH : NZONE */
static void nb_zones_core()
{
     empiler(&guts.pile_donnees, int_to_elt(nbr_zone));
}

/* mot FORTH : NCEL */
static void nb_cellules_core()
{
     empiler(&guts.pile_donnees, int_to_elt(nbr_zone+nbr_arete+nbr_sommet));
}

/* mot FORTH : FORALL */
static void forall_core()
{
     empiler(&guts.pile_donnees, int_to_elt(nbr_selec_som));
}

/* mot FORTH : >IDS */
static void to_id_sommet_core()
{
     t_cf e;

     e = sommet(&guts.pile_donnees);
     guts.pile_donnees.donnees[guts.pile_donnees.position].entier = som[e.entier].id;
}

/* mot FORTH : >IDA */
static void to_id_arc_core()
{
     t_cf e;

     e = sommet(&guts.pile_donnees);
     guts.pile_donnees.donnees[guts.pile_donnees.position].entier = arete[e.entier].id;
}

/* mot FORTH : DEG */
static void degre_core()
{
     int i;
     i = sommet(&guts.pile_donnees).entier;
     guts.pile_donnees.donnees[guts.pile_donnees.position].entier = degre(i);
}

/* mot FORTH : IVOISIN */
static void ieme_voisin_core()
{
     int s, i;
     s = sommet(&guts.pile_donnees).entier;
     depiler(&guts.pile_donnees);
     i = sommet(&guts.pile_donnees).entier;
     guts.pile_donnees.donnees[guts.pile_donnees.position].entier = ieme_succ(s, i);
}

static void ieme_arete_core()
{
     int s,i;
     s = sommet(&guts.pile_donnees).entier;
     depiler(&guts.pile_donnees);
     i = sommet(&guts.pile_donnees).entier;
     guts.pile_donnees.donnees[guts.pile_donnees.position].entier = ieme_succ_arete(s,i);
}

/* MOT FORTH : S+ */
static void new_sommet_core()
{
     coordx = sommet(&guts.pile_donnees).entier;
     depiler(&guts.pile_donnees);
     coordy = sommet(&guts.pile_donnees).entier;
     depiler(&guts.pile_donnees);
     ajout_som();
}

/* MOT FORTH : S- */
static void destroy_sommet_core()
{
     sommet_selec1 = sommet(&guts.pile_donnees).entier;
     depiler(&guts.pile_donnees);
     suppr_som();
}

/* MOT FORTH : A+ */
static void new_arete_core()
{
     sommet_selec1 = sommet(&guts.pile_donnees).entier;
     depiler(&guts.pile_donnees);
     sommet_selec2 = sommet(&guts.pile_donnees).entier;
     depiler(&guts.pile_donnees);
     ajout_arete(sommet_selec1,sommet_selec2,0);
}

/* MOT FORTH : A- */
static void destroy_arete_core()
{
     arete_selec = sommet(&guts.pile_donnees).entier;
     depiler(&guts.pile_donnees);
     suppr_arete();
}

/* mot Forth : EXISTS */
static void exists_core()
{
     int s;

     s = sommet(&guts.pile_donnees).entier;
     if (id_to_tab(s) == -1)
          guts.pile_donnees.donnees[guts.pile_donnees.position].entier = 0;
     else
          guts.pile_donnees.donnees[guts.pile_donnees.position].entier = 1;
}

/* mot Forth : TOK2POP */
static void token_to_popup_core()
{
     guts.mytoken = my_cut(&guts.tampon, guts.mytoken, &guts.iter);
     nouveau_item_popup(guts.mytoken, &menu_popup);
}

/* mot Forth : >TABLE */
static void to_table_core()
{
     tab_to_mysql_som();
     tab_to_mysql_arete();
     tab_to_mysql_zone();
}

void creer_dico_min()
{
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("SAVE", EMPTY_WORD, save_core), "SAVE");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("SVGDE", EMPTY_WORD, sauvegarde_core), "SVGDE");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("EXEC", EMPTY_WORD, exec_core), "EXEC");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("COMP", EMPTY_WORD, comp_core), "COMP");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("BUILDS", EMPTY_WORD, builds_core), "BUILDS");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("FORGET", EMPTY_WORD, forget_core), "FORGET");
     deux_points_core(); point_virgule_core();
     deux_points_rec_core();
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot(";REC", PRECEDENCE_BIT, point_virgule_rec_core), ";REC");

     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("COMPILE", EMPTY_WORD, compile_core), "COMPILE");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("}COMPILE", EMPTY_WORD, end_compile_core), "}COMPILE");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("COMPILE{", EMPTY_WORD, begin_compile_core), "COMPILE{");

     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("MYSQL", EMPTY_WORD, mysql_core), "MYSQL");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot(".\"", PRECEDENCE_BIT, create_string_core), ".\"");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("\"", PRECEDENCE_BIT, guillemets_core), "\"");

     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("LITERAL", EMPTY_WORD, literal_core), "LITERAL");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("FIND", EMPTY_WORD, find_core), "FIND");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("ABORT", EMPTY_WORD, abort_core), "ABORT");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("IMMEDIATE", EMPTY_WORD, immediat_core), "IMMEDIATE");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("!", EMPTY_WORD, store_core), "!");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("@END-!", EMPTY_WORD, end_store_core), "@END-!");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("@", EMPTY_WORD, fetch_core), "@");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot(",", EMPTY_WORD, comma_core), ",");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("ALLOT", EMPTY_WORD, allot_core), "ALLOT");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("DOES1", EMPTY_WORD, does1_core), "DOES1");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("DOES2", EMPTY_WORD, does2_core), "DOES2");
     create_core();   does_core();
     constant_core(); variable_core();

     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("HERE", EMPTY_WORD, here_core), "HERE");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("BRANCH", EMPTY_WORD, branch_core), "BRANCH");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("0BRANCH", EMPTY_WORD, zero_branch_core), "0BRANCH");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("1BRANCH", EMPTY_WORD, un_branch_core), "1BRANCH");

     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("*", EMPTY_WORD, multiplication_core), "*");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("+", EMPTY_WORD, addition_core), "+");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("1+", EMPTY_WORD, plus_un_entier_core), "1+");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("1-", EMPTY_WORD, moins_un_entier_core), "1-");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("1.+", EMPTY_WORD, plus_un_reel_core), "1.+");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("1.-", EMPTY_WORD, moins_un_reel_core), "1.-");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot(">INT", EMPTY_WORD, to_int_core), ">INT");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot(">FLOAT", EMPTY_WORD, to_float_core), ">FLOAT");

     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("SQRT", EMPTY_WORD, sqrt_core), "SQRT");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("-", EMPTY_WORD, soustraction_core), "-");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("/", EMPTY_WORD, division_core), "/");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("=", EMPTY_WORD, egalite_core), "=");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("0=", EMPTY_WORD, egalite_zero_core), "0=");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("0!=", EMPTY_WORD, non_zero_core), "0!=");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("<", EMPTY_WORD, inf_core), "<");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("<=", EMPTY_WORD, inf_egal_core), "<=");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot(">", EMPTY_WORD, sup_core), ">");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot(">=", EMPTY_WORD, sup_egal_core), ">=");

     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot(">R", EMPTY_WORD, data_to_return_core), ">R");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("R>", EMPTY_WORD, return_to_data_core), "R>");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("DUP", EMPTY_WORD, dup_core), "DUP");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("DROP", EMPTY_WORD, drop_core), "DROP");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("SWAP", EMPTY_WORD, swap_core), "SWAP");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("OVER", EMPTY_WORD, over_core), "OVER");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("ROT", EMPTY_WORD, rot_core), "ROT");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("2DUP", EMPTY_WORD, deux_dup_core), "2DUP");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("2DROP", EMPTY_WORD, deux_drop_core), "2DROP");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("2SWAP", EMPTY_WORD, deux_swap_core), "2SWAP");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("2OVER", EMPTY_WORD, deux_over_core), "2OVER");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("2ROT", EMPTY_WORD, deux_rot_core), "2ROT");

     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot(".", EMPTY_WORD, afficher_core), ".");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("CR", EMPTY_WORD, retour_chariot_core),"CR");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("TAB", EMPTY_WORD, tabulation_core), "TAB");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("WORDS", EMPTY_WORD, words_core), "WORDS");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("PDATA", EMPTY_WORD, data_stack_core), "PDATA");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("PRETURN", EMPTY_WORD, return_stack_core), "PRETURN");

     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot(">STRING", EMPTY_WORD, to_string_core), ">STRING");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("CARD", EMPTY_WORD, nb_elt_pile_core), "CARD");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("CONCAT", EMPTY_WORD, concat_core), "CONCAT");

     /* gestion carte */
     type_ID_core(); type_positionx_core(); type_positiony_core(); type_positionz_core();
     type_poignetx_core(); type_poignety_core(); type_cout_core();
     type_position_core(); type_poignet_core();
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot(">TABLE", EMPTY_WORD, to_table_core), ">TABLE");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("NSOM", EMPTY_WORD, nb_sommets_core), "NSOM");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("NARC", EMPTY_WORD, nb_arcs_core), "NARC");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("NZONE", EMPTY_WORD, nb_zones_core), "NZONE");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("NCEL", EMPTY_WORD, nb_cellules_core), "NCEL");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot(">IDS", EMPTY_WORD, to_id_sommet_core), ">IDS");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot(">IDA", EMPTY_WORD, to_id_arc_core), ">IDA");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("S+", EMPTY_WORD, new_sommet_core), "S+");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("S-", EMPTY_WORD, destroy_sommet_core), "S-");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("A+", EMPTY_WORD, new_arete_core), "A+");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("A-", EMPTY_WORD, destroy_arete_core), "A-");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("FORALL", EMPTY_WORD, forall_core), "FORALL");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("SOMMET", EMPTY_WORD, sommet_core), "SOMMET");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("ARC", EMPTY_WORD, arc_core), "ARC");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("DEG", EMPTY_WORD, degre_core), "DEG");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("A>S", EMPTY_WORD, arc_to_som_core), "A>S");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("IVOISIN", EMPTY_WORD, ieme_voisin_core), "IVOISIN");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("IARC", EMPTY_WORD, ieme_arete_core), "IARC");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("EXISTS", EMPTY_WORD, exists_core), "EXISTS");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("SCREEN", EMPTY_WORD, screen_core), "SCREEN");
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("EXTREMITE", EMPTY_WORD, extremite_core), "EXTREMITE");

     /* gestion de l'interface */
     ajouter_mot_hash_et_gtk(&guts.dictionnaire, nouveau_mot("TOK2POP", EMPTY_WORD, token_to_popup_core), "TOK2POP");
}
