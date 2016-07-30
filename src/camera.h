#ifndef CAMERA_H
#define CAMERA_H

#include "header.h"
#include "opgl.h"
#include "memoire.h"

/*void init_val_camera();
void set_val_camera( float positionX, float positionY, float positionZ,
                                 float viewX,     float viewY,     float viewZ,
                     float upVectorX, float upVectorY, float upVector);*/
void MoveCamera_(float val_speed);
void RotationView_(float angle, float x, float y, float z);
#endif
