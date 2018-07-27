//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
// Copyright 2004 Quentin Quadrat <lecrapouille@gmail.com>,
//                Minh-Long Nguyen <>,
//                Benoit Marcot <>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "camera.h"

void init_val_camera()
{
  t_pos vZero = {0.0, 0.0, 0.0};        // Init a vector to 0 0 0 for our position
  t_pos vView = {0.0, 1.0, 0.5};        // Init a starting view vector (looking up and out the screen)
  t_pos vUp   = {0.0, 0.0, 1.0};        // Init a standard up vector (Rarely ever changes)

  camera.position   = vZero;                  // Init the position to zero
  camera.cible        = vView;                  // Init the view to a std starting view
  camera.orientation    = vUp;
}

void set_val_camera( float positionX, float positionY, float positionZ,
                                 float viewX,     float viewY,     float viewZ,
                               float upVectorX, float upVectorY, float upVectorZ)
{
  t_pos vPosition    = {positionX, positionY, positionZ};
  t_pos vView        = {viewX, viewY, viewZ};
  t_pos vUpVector    = {upVectorX, upVectorY, upVectorZ};

  camera.position = vPosition;                    // Assign the position
  camera.cible = vView;                            // Assign the view
  camera.orientation = vUpVector;                    // Assign the up vector

}

void MoveCamera_(float val_speed)
{
  t_pos vVector;
  vVector.x=0.0;
  vVector.y=0.0;
  vVector.z=0.0;
// Init a vector for our view

    // Below we are getting the "View Vector".  To get a vector from
    // 2 points, we subtract the first point from the second point.  Basically,
    // what this does is give us a direction that we are looking.  That is,
    // How we know which direction we are looking at in 3D. Later we will create
    // functions that will do this stuff for us instead of doing it like this. Read
    // "Quick Notes" for some more detail on vectors and the camera's view vector.

    printf ("vVector: %f %f %f\n",vVector.x,vVector.y,vVector.z);
    // Get our view vector (The direciton we are facing)
    vVector.x = camera.cible.x - camera.position.x;        // This gets the direction of the X
    vVector.y = camera.cible.y - camera.position.y;        // This gets the direction of the Y
    vVector.z = camera.cible.z - camera.position.z;        // This gets the direction of the Z
    printf ("vVector2: %f %f %f\n",vVector.x,vVector.y,vVector.z);
    // Below is the code that will move our view and camera position forward or backward.
    // What we are doing is adding adding our (view vector times the speed) plus
    // our current postion and view.  You might be thinking that we should just
    // add/subtract the speed to our view and position, and forget about the vector.
    // Well, the reason why we do this is because we want to go in the direction
    // that we are looking at.  It won't work if we just add the constant speed
    // to the X and Z values of our position and view.  Try it and you will see.
    // Sure, if we are looking DIRECTLY down the X or Z axis it would work, but
    // when we start to rotate out view at a unique angle, it won't work.  Trust me.
    // So basically, if we are looking at a 45 degree angle, we move in that direction.
    // If we want to move forward we pass in a positive speed, and if we want to move
    // backwards, just pass in a negative speed.
    printf ("dans move camera2\n");
    printf ("cible %f %f %f\n",camera.cible.x,camera.cible.y,camera.cible.z);
    printf ("position %f %f %f\n",camera.position.x,camera.position.y,camera.position.z);
    printf ("vector.x * speed= %f\n", vVector.x * val_speed);
    camera.position.x += vVector.x * val_speed;        // Add our acceleration to our position's X
    camera.position.z += vVector.z * val_speed;        // Add our acceleration to our position's Z
    camera.cible.x += vVector.x * val_speed;            // Add our acceleration to our view's X
    camera.cible.z += vVector.z * val_speed;            // Add our acceleration to our view's Z
    printf ("dans move camera3\n");
    printf ("cible %f %f %f\n",camera.cible.x,camera.cible.y,camera.cible.z);
    printf ("position %f %f %f\n",camera.position.x,camera.position.y,camera.position.z);
}

void RotationView_(float angle, float x, float y, float z)
{
  printf ("Rotation\n");
  printf ("angle:%f x:%f y:%f z:%f\n",angle,x,y,z);
  t_pos vNewView;
  t_pos vView;

  vView.x = camera.cible.x - camera.position.x;        // This gets the direction of the X
  vView.y = camera.cible.y - camera.position.y;        // This gets the direction of the Y
  vView.z = camera.cible.z - camera.position.z;

  float cosTheta = (float)cos(angle);
  float sinTheta = (float)sin(angle);

  // Find the new x position for the new rotated point
  vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)        * vView.x;
  vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)    * vView.y;
  vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)    * vView.z;

  // Find the new y position for the new rotated point
  vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)    * vView.x;
  vNewView.y += (cosTheta + (1 - cosTheta) * y * y)        * vView.y;
  vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)    * vView.z;

  // Find the new z position for the new rotated point
  vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)    * vView.x;
  vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)    * vView.y;
  vNewView.z += (cosTheta + (1 - cosTheta) * z * z)        * vView.z;

  // Now we just add the newly rotated vector to our position to set
  // our new rotated view of our camera.
  camera.cible.x = camera.position.x + vNewView.x;
  camera.cible.y = camera.position.y + vNewView.y;
  camera.cible.z = camera.position.z + vNewView.z;
}
