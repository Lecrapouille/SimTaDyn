#ifndef DESSIN_H
#define DESSIN_H

#include "header.h"
#include "opgl.h"
#include "font.h"
#include "memoire.h"

void creer_poignet_3D();
void creer_poignet_2D();
void dessiner_poignet(float x, float y, float z, GLuint l);
void dessine_tous_sommets();
void dessine_tous_arcs();
void dessine_toutes_zones();
void dessine_tous_arcs2();
void dessine_toutes_zones2();
void dessine_toutes_normal();
void dessiner_la_carte();
t_pos point_commun(int ,int );
void dessine_tous_les_triangles();
#endif
