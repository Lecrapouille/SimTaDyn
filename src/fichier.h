#ifndef FICHIER_H
#define FICHIER_H

#include <stdio.h>
#include <string.h>
#include "header.h"
#include "memoire.h"
#include "string_util.h"
#include "interpreteur.h"

#define TAILLE_TAMPON 1024

gchar* fichier_to_tampon(const gchar * nomfichier);
bool tampon_to_fichier(gchar * tampon, const gchar * nomfichier);
bool charger_fichier_forth(const gchar * nomfichier);

#endif /* FICHIER_H */
