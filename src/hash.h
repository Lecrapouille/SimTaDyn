#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <string.h>
#include "memoire.h"
#include "mot.h"
#include "header.h"
#include "store.h"

void print_etat(t_etat etat);
t_hash nouveau_hash();
unsigned int fct_hash(char * key);
bool etat_est(t_hash *hash, int i, t_etat etat);
bool ajouter_donnee_hash(t_hash *hash, void* donnee, char* cle);
int chercher_donnee_hash(t_hash *hash, char *elt);
t_cfa chercher_mot_hash(t_hash *hash, char * cle);
void supprimer_mot_hash(t_hash *hash, char * cle);
void destroy_hash(t_hash * hash);
bool ajouter_mot_hash_et_gtk(t_hash *hash, void* donnee, char* cle);
bool ajouter_mot_hash(t_hash *hash, void* donnee, char* cle);
void forcer_ajout_hash(t_hash *hash, void* donnee, char* cle);
void afficher_hash(t_hash *hash);
void reinit_hash(t_hash *hash);
void ieme_reinit_hash(t_hash *hash, int i);

#endif /* HASH_H */
