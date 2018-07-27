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

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

int parsexml(char *filename)
{
  FILE *file = fopen(filename, "r");;

  char *balise = (char *)malloc(100 * sizeof(char));
  char *read = (char *)malloc(100 * sizeof(char));
  char *end = (char *)malloc(2 * sizeof(char));

  char *tmp_id = (char *)malloc(20 * sizeof(char));
  char *tmp_x = (char *)malloc(20 * sizeof(char));
  char *tmp_y = (char *)malloc(20 * sizeof(char));
  char *tmp_z = (char *)malloc(20 * sizeof(char));
  char *tmp_name = (char *)malloc(40 * sizeof(char));
  char *tmp_R = (char *)malloc(20 * sizeof(char));
  char *tmp_G = (char *)malloc(20 * sizeof(char));
  char *tmp_B = (char *)malloc(20 * sizeof(char));
  char *tmp_A = (char *)malloc(20 * sizeof(char));
  char *tmp_mforth = (char *)malloc(100 * sizeof(char));

  char *tmp_cout = (char *)malloc(20 * sizeof(char));
  char *tmp_idsom1 = (char *)malloc(20 * sizeof(char));
  char *tmp_idsom2 = (char *)malloc(20 * sizeof(char));
  char *tmp_triangle = (char *)malloc(20 * sizeof(char));

  char *tmp_nbar = (char *)malloc(20 * sizeof(char));
  char *tmp_listar = (char *)malloc(100 * sizeof(char));

  char *mysql_req_som = (char *)malloc(300 * sizeof(char));
  char *mysql_req_arc = (char *)malloc(300 * sizeof(char));
  char *mysql_req_zone = (char *)malloc(300 * sizeof(char));


  int l_balise,
    t,
    indice = 0,
    token = 0;

 /* On commence par traiter les sommets */
  t = 0;
  while(!feof(file))
    {
      fseek(file, t, 0);
      balise = "<sommet>";
      l_balise = strlen(balise);
      fgets(read, l_balise + 1, file);

      if(!(strcmp(read, balise)))
        {
          /* On sort l'id */
          indice = t + 13;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_id, token + 1, file);
          //printf("id : %s\n", tmp_id);
          indice = indice + token;

          /* On sort le x */
          indice = indice + 9;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_x, token + 1, file);
          //printf("x : %s\n", tmp_x);
          indice = indice + token;

          /* On sort le y */
          indice = indice + 8;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_y, token + 1, file);
          //printf("y : %s\n", tmp_y);
          indice = indice + token;

          /* On sort le z */
          indice = indice + 8;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_z, token + 1, file);
          //printf("z : %s\n", tmp_z);
          indice = indice + token;

          /* On sort le name */
          indice = indice + 11;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_name, token + 1, file);
          //printf("name : %s\n", tmp_name);
          indice = indice + token;

          /* On sort le R */
          indice = indice + 11;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_R, token + 1, file);
          //printf("R : %s\n", tmp_R);
          indice = indice + token;

          /* On sort le G */
          indice = indice + 8;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_G, token + 1, file);
          //printf("G : %s\n", tmp_G);
          indice = indice + token;

          /* On sort le B */
          indice = indice + 8;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_B, token + 1, file);
          //printf("B : %s\n", tmp_B);
          indice = indice + token;

          /* On sort le A */
          indice = indice + 8;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_A, token + 1, file);
          //printf("B : %s\n", tmp_B);
          indice = indice + token;

          /* On sort le mforth */
          indice = indice + 13;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_mforth, token + 1, file);
          //printf("mforth : %s\n", tmp_mforth);

          sprintf(mysql_req_som,
                  "INSERT INTO som(id, x, y, z, name, R, G, B, A, mforth) VALUES(%s, %s, %s, %s, \"%s\", %s, %s, %s, %s, \"%s\")",
                  tmp_id,
                  tmp_x,
                  tmp_y,
                  tmp_z,
                  tmp_name,
                  tmp_R,
                  tmp_G,
                  tmp_B,
                  tmp_A,
                  tmp_mforth);
          printf("mysql_request_som : %s\n", mysql_req_som);
        }
      t++;
    }

  /* Puis on traite les arcs */
  t = 0;
  rewind(file);
  while(!feof(file))
    {
      fseek(file, t, 0);
      balise = "<arc>";
      l_balise = strlen(balise);
      fgets(read, l_balise + 1, file);

      if(!(strcmp(read, balise)))
        {
          /* On sort l'id */
          indice = t + 10;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_id, token + 1, file);
          //printf("id : %s\n", tmp_id);
          indice = indice + token;

          /* On sort le x */
          indice = indice + 9;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_x, token + 1, file);
          //printf("x : %s\n", tmp_x);
          indice = indice + token;

          /* On sort le y */
          indice = indice + 8;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_y, token + 1, file);
          //printf("y : %s\n", tmp_y);
          indice = indice + token;

          /* On sort le z */
          indice = indice + 8;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_z, token + 1, file);
          //printf("z : %s\n", tmp_z);
          indice = indice + token;

          /* On sort le name */
          indice = indice + 11;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_name, token + 1, file);
          //printf("name : %s\n", tmp_name);
          indice = indice + token;

          /* On sort le R */
          indice = indice + 11;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_R, token + 1, file);
          //printf("R : %s\n", tmp_R);
          indice = indice + token;

          /* On sort le G */
          indice = indice + 8;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_G, token + 1, file);
          //printf("G : %s\n", tmp_G);
          indice = indice + token;

          /* On sort le B */
          indice = indice + 8;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_B, token + 1, file);
          //printf("B : %s\n", tmp_B);
          indice = indice + token;

          /* On sort le A */
          indice = indice + 8;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_A, token + 1, file);
          //printf("B : %s\n", tmp_B);
          indice = indice + token;

          /* On sort le cout */
          indice = indice + 11;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_cout, token + 1, file);
          indice = indice + token;

          /* On sort le id_som_1 */
          indice = indice + 18;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_idsom1, token + 1, file);
          //printf("idsom1 : %s\n", tmp_idsom1);
          indice = indice + token;

          /* On sort le id_som_2 */
          indice = indice + 22;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_idsom2, token + 1, file);
          //printf("idsom1 : %s\n", tmp_idsom2);
          indice = indice + token;

          /* On sort le mforth */
          indice = indice + 20;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_mforth, token + 1, file);
          //printf("mforth : %s\n", tmp_mforth);
          indice = indice + token;

          /* On sort le triangle */
          indice = indice + 20;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_triangle, token + 1, file);

          sprintf(mysql_req_arc,
                  "INSERT INTO arc(id, x_poig, y_poig, z_poig, name, R, G, B, A, cout, id_som_1, id_som_2, mforth, triangle) VALUES(%s, %s, %s, %s, \"%s\", %s, %s, %s, %s, %s, %s, %s, \"%s\", %s)",
                  tmp_id,
                  tmp_x,
                  tmp_y,
                  tmp_z,
                  tmp_name,
                  tmp_R,
                  tmp_G,
                  tmp_B,
                  tmp_A,
                  tmp_cout,
                  tmp_idsom1,
                  tmp_idsom2,
                  tmp_mforth,
                  tmp_triangle);
          printf("mysql_request_arc : %s\n", mysql_req_arc);
        }
      t++;
    }

  /* Et on s'occupe enfin des zones */
  t = 0;
  rewind(file);
  while(!feof(file))
    {
      fseek(file, t, 0);
      balise = "<zone>";
      l_balise = strlen(balise);
      fgets(read, l_balise + 1, file);

      if(!(strcmp(read, balise)))
        {
          /* On sort l'id */
          indice = t + 11;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_id, token + 1, file);
          //printf("id : %s\n", tmp_id);
          indice = indice + token;

          /* On sort le x */
          indice = indice + 9;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_x, token + 1, file);
          //printf("x : %s\n", tmp_x);
          indice = indice + token;

          /* On sort le y */
          indice = indice + 8;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_y, token + 1, file);
          //printf("y : %s\n", tmp_y);
          indice = indice + token;

          /* On sort le z */
          indice = indice + 8;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_z, token + 1, file);
          //printf("z : %s\n", tmp_z);
          indice = indice + token;

          /* On sort le name */
          indice = indice + 11;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_name, token + 1, file);
          //printf("name : %s\n", tmp_name);
          indice = indice + token;

          /* On sort le R */
          indice = indice + 11;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_R, token + 1, file);
          //printf("R : %s\n", tmp_R);
          indice = indice + token;

          /* On sort le G */
          indice = indice + 8;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_G, token + 1, file);
          //printf("G : %s\n", tmp_G);
          indice = indice + token;

          /* On sort le B */
          indice = indice + 8;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_B, token + 1, file);
          //printf("B : %s\n", tmp_B);
          indice = indice + token;

          /* On sort le A */
          indice = indice + 8;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_A, token + 1, file);
          //printf("B : %s\n", tmp_B);
          indice = indice + token;

          /* On sort le nbar */
          indice = indice + 11;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_nbar, token + 1, file);
          //printf("idsom1 : %s\n", tmp_idsom1);
          indice = indice + token;

          /* On sort le listar */
          indice = indice + 16;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_listar, token + 1, file);
          //printf("idsom1 : %s\n", tmp_idsom2);
          indice = indice + token;

          /* On sort le mforth */
          indice = indice + 18;
          fseek(file, indice, 0);
          fgets(end, 2, file);
          token = 0;
          while(strcmp(end, "<"))
            {
              token++;
              fseek(file, indice + token, 0);
              fgets(end, 2, file);
            }
          fseek(file, indice, 0);
          fgets(tmp_mforth, token + 1, file);
          //printf("mforth : %s\n", tmp_mforth);

          sprintf(mysql_req_zone,
                  "INSERT INTO zone(id, x_poig, y_poig, z_poig, name, R, G, B, A, nbr_arete, liste_arete, mforth) VALUES(%s, %s, %s, %s, \"%s\", %s, %s, %s, %s, %s, \"%s\", \"%s\")",
                  tmp_id,
                  tmp_x,
                  tmp_y,
                  tmp_z,
                  tmp_name,
                  tmp_R,
                  tmp_G,
                  tmp_B,
                  tmp_A,
                  tmp_nbar,
                  tmp_listar,
                  tmp_mforth);
          printf("mysql_request_zone : %s\n", mysql_req_zone);
        }
      t++;
    }

  fclose(file);
  return 0;
}

int main()
{
  printf("XML2MySQL Parser - Juzam\n");
  parsexml("graph.xml");
  return 0;
}
