#ifndef MOT_H
#define MOT_H

#include <stdio.h>
#include <string.h>
#include "memoire.h"
#include "erreur.h" 
#include "header.h"
#include "hash.h"
#include "elt_pile.h"

/*
** PRECEDENCE BIT = mot immediat (8eme bit)
** LONGUEUR MOT (du 1er bit --> 7eme bit)
*/

#define PRIMITIVE_BIT 0x40
#define PRECEDENCE_BIT 0x80
#define EMPTY_WORD 0x00

/*
** change le caractere immediat de 'entete'
*/
void change_masque(octet * info, octet masque);
bool est_immediat(octet info);
bool est_primitif(octet info);
octet longueur(octet info);
void change_longueur(octet nb, octet * info);

void detruire_mot(t_cfa mot);
t_cfa adresse_fin_mot(t_cfa mot);
bool nouveau_mot_courant(void);    
t_nfa nouveau_nfa(gchar * nom, octet masque, FONCTION_C);
t_cfa nouveau_mot(gchar * nom, octet masque, FONCTION_C);
void ajouter_adresse(t_cfa mot, void * ad, t_type type); //A SUPP
void ajouter_elt_dans_mot(t_cfa mot, t_cf elt);
void afficher_mot(t_cfa mot);

#endif /* MOT_H */
