#ifndef NORMAL_H
#define NORMAL_H

#include "header.h"
#include "opgl.h"
#include "font.h"
#include "memoire.h"
#include "dessine_carte.h"

t_pos Cross(t_pos vVector1, t_pos vVector2);
t_pos Vector(t_pos vPoint1, t_pos vPoint2);
float Magnitude(t_pos vNormal);
t_pos Normalize(t_pos vNormal);
t_pos Normal(int p1,int p2);
t_pos pos2_to_pos (t_pos2);
#endif
