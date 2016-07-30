#ifndef INTERPRETEUR_H
#define INTERPRETEUR_H

#include "erreur.h"
#include "string_util.h"
#include "pile.h"
#include "mot.h"
#include "header.h"
#include "pile.h"
#include "elt_pile.h"

void (*execute_mot)(int); //pointeur sur fct
void execute_mot_avec_debuggeur(int nb);
void execute_mot_sans_debuggeur(int nb);
t_erreur parser_tampon(gchar ** tampon);

#endif /* INTERPRETEUR_H */
