#ifndef ERREUR_H
#define ERREUR_H

#include <string.h>
#include "header.h"
#include "memoire.h"

void erreur_parser(t_erreur erreur, 
		   GtkTextBuffer *bf_nouv, 
		   GtkTextBuffer *bf_res);

#endif /* ERREUR_H */
