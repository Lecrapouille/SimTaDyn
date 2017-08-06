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

#include "normal.h"

t_pos Cross(t_pos vVector1, t_pos vVector2)
{
  t_pos vNormal;
  vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
  vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
  vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));
  return vNormal;
}


t_pos Vector(t_pos vPoint1, t_pos vPoint2)
{
  t_pos vVector;
  vVector.x = vPoint1.x - vPoint2.x;
  vVector.y = vPoint1.y - vPoint2.y;
  vVector.z = vPoint1.z - vPoint2.z;
  return vVector;
}

float Magnitude(t_pos vNormal)
{
  return (float)sqrt( (vNormal.x * vNormal.x) +
                      (vNormal.y * vNormal.y) +
                      (vNormal.z * vNormal.z) );
}

t_pos Normalize(t_pos vNormal)
{
  float magnitude = Magnitude(vNormal);
  vNormal.x /= magnitude;
  vNormal.y /= magnitude;
  vNormal.z /= magnitude;
  return vNormal;
}

t_pos Normal(int a1,int a2)
{

  t_pos vVector1;
  t_pos vVector2;
  t_pos vNormal;
  t_pos postemp1,postemp2,postemp3,postemp1b,postemp2b;

  postemp1=pos2_to_pos(arete[id_to_tab2(a1)].pos1);
  postemp2=pos2_to_pos(arete[id_to_tab2(a1)].pos2);
  postemp1b=pos2_to_pos(arete[id_to_tab2(a2)].pos1);
  postemp2b=pos2_to_pos(arete[id_to_tab2(a2)].pos2);
  if (postemp1.x==postemp1b.x && postemp1.y==postemp1b.y && postemp1.z==postemp1b.z)
    {
      postemp3= postemp2b;
      vVector1=Vector(postemp2,postemp1);
      vVector2=Vector(postemp3,postemp1);
    }
  else
    if (postemp1.x==postemp2b.x && postemp1.y==postemp2b.y && postemp1.z==postemp2b.z)
      {
        postemp3= postemp1b;
        vVector1=Vector(postemp2,postemp1);
        vVector2=Vector(postemp3,postemp1);
      }
    else
      if (postemp2.x==postemp1b.x && postemp2.y==postemp1b.y && postemp2.z==postemp1b.z)
        {
          postemp3=postemp2b;
          vVector1=Vector(postemp1,postemp2);
          vVector2=Vector(postemp3,postemp2);
        }
      else
        if (postemp2.x==postemp2b.x && postemp2.y==postemp2b.y && postemp2.z==postemp2b.z)
          {
            postemp3=postemp1b;
            vVector1=Vector(postemp1,postemp2);
            vVector2=Vector(postemp3,postemp2);
          }
        else
          printf ("probleme\n");

  vNormal = Cross(vVector1, vVector2);
  vNormal = Normalize(vNormal);
  return vNormal;
}

t_pos pos2_to_pos (t_pos2 val)
{
  t_pos res;
  res.x=val.x;
  res.y=val.y;
  res.z=val.z;
  return res;
}
