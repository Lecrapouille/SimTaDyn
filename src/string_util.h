#ifndef STRING_UTIL_H
#define STRING_UTIL__H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "header.h"
#include "memoire.h"

#define PRINTF_ERREUR(texte) (my_printf(texte, balise_erreur, bf_res))
#define PRINTF_BON(texte) (my_printf(texte, balise_bonne, bf_res))
#define PRINTF_SOULIGNE(texte) (my_printf(texte, balise_soulignee, bf_res))
#define PRINTF_NORMALE(texte) (my_printf(texte, balise_normale, bf_res))

void my_printf(const gchar * texte, GtkTextTag * balise, GtkTextBuffer *txtbf);

bool chaine_to_int(gchar * ch, int *nb);
bool chaine_to_float(gchar * ch, float *nb);

t_buffer nouveau_buffer(unsigned int taille, char * chaine);
void ajouter_string(t_buffer *buf, char* chaine);
void destroy_buffer(t_buffer buf);
void reinit_buffer(t_buffer *buf);

t_iterateur nouveau_iterateur(void);
gchar* nouvelle_chaine(void);
bool car_separation(gchar * ch);
gchar * my_cut(gchar ** ch, gchar * mytoken, t_iterateur * iter);

#endif /* STRING_UTIL_H */
