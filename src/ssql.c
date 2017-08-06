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

/*#include "affichage.h"*/
#include "header.h"

char* floattostring (float f)
{
     int i=0;
     char* s;
     s=malloc (sizeof(char)*50);
     sprintf (s,"%f",f);
     while (s[i]!='\0')
     {
          if (s[i]==',')
               s[i]='.';
          i++;
     }
     return s;
}

void myinitsql()
{
     char *buffer;
     buffer = (char *)malloc(250 * sizeof(char));
     sprintf(buffer,
             "DELETE FROM zone WHERE id = %i",
             11);
     mysql_execreq(buffer);
     free(buffer);
}

int db_connect(const char *mysql_host, const char *mysql_user, const char *mysql_password, const char *mysql_db)
{
    /* init database */
     printf("MySQL init...");
     if(mysql_init(&mysql))
          printf("\t\t[OK]\n");
     else
          printf("\t\t[FAILED]\n");

     /* connect to database */
     printf("Connection to MySQL...");
     connection = mysql_real_connect(&mysql,mysql_host,mysql_user,mysql_password,mysql_db, 0, 0,0);

     /* check for connection error */
     if (connection == NULL)
     {
          /* print error message */
          printf("\t[FAILED]\n");
          printf(mysql_error(&mysql));
          return 1;
     }

     printf("\t[OK]\n");

     return 0;
}

void exit_mysql()
{
     printf("Flushing memory...");
     mysql_free_result(result);
     printf("\t[OK]\n");

     /* close the connection */
     printf("Closing MySQL db...");
     mysql_close(connection);
     printf("\t[OK]\n");
     printf("MySQL db closed.\n");
}

void mysql_execreq(char *req)
{
     int state;
     state = mysql_query(connection, req);
     printf("Executing request...");
     if (state != 0)
     {
          printf("\t[FAILED]\n");
          printf(mysql_error(connection));
     }
     else
          printf("\t[OK]\n");
}

void mysql_db_som()
{
     unsigned int i;
     struct st_field
     {
          int   id;
          float coord_x,
               coord_y,
               coord_z;
          char *name;
          float R,
            G,
            B,
            A;
          char *mot_forth;
     };
     typedef struct st_field node;

     char *req_select = "SELECT id, coord_x, coord_y, coord_z,name, R, G, B, A,mforth FROM som";
     unsigned int       num_rows,
          fc,
          token;

     token = 0;

     node *g;

     mysql_execreq(req_select);
     /* must call mysql_store_result() before we can issue any other
        query calls */
     result = mysql_store_result(connection);

     /* Info sur le nombre de ligne dans la base de données */
     num_rows = (unsigned)mysql_num_rows(result);

     /* Allocation mémoire du tableau stockant le graphe */
     g = malloc(num_rows * sizeof(node));

     /* process each row in result set */
     while ((row = mysql_fetch_row(result)) /*!= NULL*/)
     {
          /* Fonction de l API MySQL qui renvoie le nombre de champs
             dans la table mysql_field_count(connection)); */

          fc = 0;
          node tmp;
          while(fc < mysql_field_count(connection))
          {

               /* On remplie une case du tableau de structure */
               switch(fc)
               {
               case 0: tmp.id = atoi(row[fc]); break;
               case 1: tmp.coord_x = atof(row[fc]);break;
               case 2: tmp.coord_y = atof(row[fc]);break;
               case 3: tmp.coord_z = atof(row[fc]);break;
               case 4: tmp.name = row[fc];break;
               case 5: tmp.R = atof(row[fc]);break;
               case 6: tmp.G = atof(row[fc]);break;
               case 7: tmp.B = atof(row[fc]);break;
               case 8: tmp.A = atof(row[fc]);break;
               case 9: tmp.mot_forth = row[fc];break;
               default: break;
               }

               fc++;
          }
          g[token] = tmp;
          token++;
     }
     char buffer[50];
     for (i=0;i<num_rows;i++)
     {
          buffer[0]='\0';
          som[i].id=g[i].id;
          som[i].pos.x=g[i].coord_x;
          som[i].pos.y=g[i].coord_y;
          som[i].pos.z=g[i].coord_z;
          som[i].nom  = (char*) malloc  (sizeof (char)*10);
          sprintf (som[i].nom,"S%i",g[i].id);
          sprintf (buffer,"UPDATE som set name = \"%s\" WHERE id = %i",
                   som[i].nom,
                   som[i].id);
          mysql_execreq(buffer);
          //printf ("g %f %f %f %f\n",g[i].R,g[i].G,g[i].B,g[i].A);
          som[i].densite.R=g[i].R;
          som[i].densite.G=g[i].G;
          som[i].densite.B=g[i].B;
          som[i].densite.A=g[i].A;
          printf ("som densite %f %f %f %f\n",som[i].densite.R,som[i].densite.G,som[i].densite.B,som[i].densite.A);
          if (g[i].mot_forth!=NULL)
          {
               som[i].mot_forth = (char*) malloc  (sizeof (char)*20);
               strcpy (som[i].mot_forth,g[i].mot_forth);
          }
          //som[i].name = (char*) malloc  (sizeof (char)*20);
          //strcpy (som[i].name,g[i].name);

     }

     nbr_sommet=num_rows;
}

void mysql_db_arc()
{
     unsigned int i;
     struct st_field
     {
          int   id;
          float coord_x,
               coord_y;
          float R,
               G,
               B,
            A;
       float cout;
          char *name;
          int id_som_1,
               id_som_2;
          char *mot_forth;
          int triangle;
     };
     typedef struct st_field arc;
     char *req_select = "SELECT id, x_poig, y_poig, R, G, B, A,cout,name, id_som_1, id_som_2, mforth, triangle FROM arc";
     unsigned int       num_rows,
          fc,
          token;
     arc *g;
     token = 0;

     mysql_execreq(req_select);
     /* must call mysql_store_result() before we can issue any other
        query calls */
     result = mysql_store_result(connection);

     /* Info sur le nombre de ligne dans la base de données */
     num_rows = (unsigned)mysql_num_rows(result);

     /* Allocation mémoire du tableau stockant le graphe */
     g = malloc(num_rows * sizeof(arc));

     /* process each row in result set */
     while ((row = mysql_fetch_row(result)) /*!= NULL*/)
     {
          /* Fonction de l API MySQL qui renvoie le nombre de champs
             dans la table mysql_field_count(connection)); */

          fc = 0;
          arc tmp;

          while(fc < mysql_field_count(connection))
          {

               /* On remplie une case du tableau de structure */
               switch(fc)
               {
               case 0: tmp.id = atoi(row[fc]); break;
               case 1: tmp.coord_x = atof(row[fc]);break;
               case 2: tmp.coord_y = atof(row[fc]);break;
               case 3: tmp.R = atof(row[fc]);break;
               case 4: tmp.G =  atof(row[fc]);break;
               case 5: tmp.B =  atof(row[fc]);break;
               case 6: tmp.A = atof(row[fc]);break;
               case 7: tmp.cout = atof(row[fc]);break;
               case 8: tmp.name = row[fc];break;
               case 9: tmp.id_som_1 = atoi(row[fc]);break;
               case 10: tmp.id_som_2 = atoi(row[fc]);break;
               case 11: tmp.mot_forth = row[fc];break;
               case 12: tmp.triangle = atoi(row[fc]);break;
               default: break;
               }
               fc++;
          }
          g[token] = tmp;
          token++;
     }

     int j;
     char buffer[50];
     for (i=0;i<num_rows;i++)
     {
          buffer[0]='\0';
          arete[i].id=g[i].id;
          arete[i].poignet.x=g[i].coord_x;
          arete[i].poignet.y=g[i].coord_y;
          arete[i].densite.R=g[i].R;
          arete[i].densite.G=g[i].G;
          arete[i].densite.B=g[i].B;
          arete[i].densite.A=g[i].A;
          arete[i].cout=g[i].cout;
          arete[i].pos1.s=g[i].id_som_1;
          arete[i].pos2.s=g[i].id_som_2;
          arete[i].nom  = (char*) malloc  (sizeof (char)*10);
          sprintf (arete[i].nom,"A%i",g[i].id);
          sprintf (buffer,"UPDATE arc set name = \"%s\" WHERE id = %i",
                   arete[i].nom,
                   arete[i].id);
          mysql_execreq(buffer);

          arete[i].triangle=g[i].triangle;
          if (g[i].mot_forth!=NULL)
          {
               arete[i].mot_forth = (char*) malloc  (sizeof (char)*20);
               strcpy (arete[i].mot_forth,g[i].mot_forth);
          }
          else
               arete[i].mot_forth=NULL;
          for (j=0;j<nbr_sommet;j++)
          {
               if (arete[i].pos1.s==som[j].id)
               {
                    arete[i].pos1.x=som[j].pos.x;
                    arete[i].pos1.y=som[j].pos.y;
                    arete[i].pos1.z=som[j].pos.z;
               }
               if (arete[i].pos2.s==som[j].id)
               {
                    arete[i].pos2.x=som[j].pos.x;
                    arete[i].pos2.y=som[j].pos.y;
                    arete[i].pos2.z=som[j].pos.z;
               }
          }
     }
     nbr_arete=num_rows;

}

void mysql_db_zone()
{
      unsigned int i;
     struct st_field
     {
          int   id;
          float x_poig,
               y_poig;
          float R,
               G,
               B,
            A;
          char *name;
          int nbr_arete;
          char *liste_arete;
          char *mot_forth;
     };
     typedef struct st_field zones;

     char *req_select = "SELECT id, x_poig, y_poig, R, G, B, A,name, nbr_arete, liste_arete, mforth FROM zone";
     unsigned int       num_rows,
          fc,
          token;
     zones *g;
     token = 0;

     mysql_execreq(req_select);
     /* must call mysql_store_result() before we can issue any other
        query calls */
     result = mysql_store_result(connection);

     /* Info sur le nombre de ligne dans la base de données */
     num_rows = (unsigned)mysql_num_rows(result);

     /* Allocation mémoire du tableau stockant le graphe */
     g = malloc(num_rows * sizeof(zones));

     /* process each row in result set */
     while ((row = mysql_fetch_row(result)) /*!= NULL*/)
     {
          /* Fonction de l API MySQL qui renvoie le nombre de champs
             dans la table mysql_field_count(connection)); */

          fc = 0;
          zones tmp;

          while(fc < mysql_field_count(connection))
          {

               /* On remplie une case du tableau de structure */
               switch(fc)
               {
               case 0: tmp.id = atoi(row[fc]); break;
               case 1: tmp.x_poig = atof(row[fc]);break;
               case 2: tmp.y_poig = atof(row[fc]);break;
               case 3: tmp.R = atof(row[fc]);break;
               case 4: tmp.G = atof(row[fc]);break;
               case 5: tmp.B = atof(row[fc]);break;
               case 6: tmp.A = atof(row[fc]);break;
               case 7: tmp.name = row[fc];break;
               case 8: tmp.nbr_arete = atof (row[fc]);break;
               case 9: tmp.liste_arete = row[fc];break;
               case 10: tmp.mot_forth = row[fc];break;
               default: break;
               }

               fc++;
          }
          g[token] = tmp;
          token++;
     }
     char buffer[50];
     for (i=0;i<num_rows;i++){
          buffer[0]='\0';
          zone[i].densite.R=g[i].R;
          zone[i].densite.G=g[i].G;
          zone[i].densite.B=g[i].B;
          zone[i].densite.A=g[i].A;
          zone[i].id = g[i].id;
          zone[i].poignet.x=g[i].x_poig;
          zone[i].poignet.y=g[i].y_poig;
          zone[i].poignet.z=0;
          zone[i].nom  = (char*) malloc  (sizeof (char)*10);
          sprintf (zone[i].nom,"Z%i",g[i].id);
          sprintf (buffer,"UPDATE zone set name = \"%s\" WHERE id = %i",
                   zone[i].nom,
                   zone[i].id);
          mysql_execreq(buffer);

          zone[i].nbr_arete=g[i].nbr_arete;
          zone[i].liste_arete=string_to_liste(g[i].liste_arete);

          if (g[i].mot_forth!=NULL)
          {
               zone[i].mot_forth  = (char*) malloc  (sizeof (char)*100);
               strcpy (zone[i].mot_forth,g[i].mot_forth);
          }
     }
     nbr_zone=num_rows;

}

void mysql_db_triangle()
{
     unsigned int i;
     struct st_field
     {
          int   id;
          float x_poig,
               y_poig;
          int  arete1,
               arete2,
               arete3;
          float x_normal,
               y_normal,
               z_normal;
     };
     typedef struct st_field triangles;

     char *req_select = "SELECT id, x_poig, y_poig, arete1, arete2, arete3, x_normal, y_normal, z_normal FROM triangle";
     unsigned int       num_rows,
          fc,
          token;
     triangles *g;
     token = 0;

     mysql_execreq(req_select);
     /* must call mysql_store_result() before we can issue any other
        query calls */
     result = mysql_store_result(connection);

     /* Info sur le nombre de ligne dans la base de données */
     num_rows = (unsigned)mysql_num_rows(result);

     /* Allocation mémoire du tableau stockant le graphe */
     g = malloc(num_rows * sizeof(triangles));

     /* process each row in result set */
     while ((row = mysql_fetch_row(result)) /*!= NULL*/)
     {
          /* Fonction de l API MySQL qui renvoie le nombre de champs
             dans la table mysql_field_count(connection)); */

          fc = 0;
          triangles tmp;

          while(fc < mysql_field_count(connection))
          {

               /* On remplie une case du tableau de structure */
               switch(fc)
               {
               case 0: tmp.id = atoi(row[fc]); break;
               case 1: tmp.x_poig = atof(row[fc]);break;
               case 2: tmp.y_poig = atof(row[fc]);break;
               case 3: tmp.arete1 = atoi(row[fc]);break;
               case 4: tmp.arete2 = atoi(row[fc]);break;
               case 5: tmp.arete3 = atoi(row[fc]);break;
               case 6: tmp.x_normal = atof (row[fc]);break;
               case 7: tmp.y_normal = atof (row[fc]);break;
               case 8: tmp.z_normal = atof (row[fc]);break;
               default: break;
               }

               fc++;
          }
          g[token] = tmp;
          token++;
     }

     for (i=0;i<num_rows;i++){
          triangl[i].id = g[i].id;
          triangl[i].poignet.x=g[i].x_poig;
          triangl[i].poignet.y=g[i].y_poig;
          insert_liste(g[i].arete1,&triangl[i].liste_arete);
          insert_liste(g[i].arete2,&triangl[i].liste_arete);
          insert_liste(g[i].arete3,&triangl[i].liste_arete);
          triangl[i].normal.x=g[i].x_normal;
          triangl[i].normal.y=g[i].y_normal;
          triangl[i].normal.z=g[i].z_normal;
          triangl[i].selec=0;
     }
     nbr_triangle=num_rows;
}


char * mysql_db_forth()
{
     struct st_field
     {
          int id;
          char *code;
     };
     typedef struct st_field t_forth2;

     char *req_select = "SELECT id, prog FROM forth";
     unsigned int       num_rows,
          fc,
          token;
    // t_forth2 *g;
     token = 0;

     mysql_execreq(req_select);
     /* must call mysql_store_result() before we can issue any other
        query calls */
     result = mysql_store_result(connection);

     /* Info sur le nombre de ligne dans la base de données */
     num_rows = (unsigned)mysql_num_rows(result);

     /* Allocation mémoire du tableau stockant le graphe */
    // g = malloc(num_rows * sizeof(t_forth2));
     t_forth2 tmp;
     /* process each row in result set */
     while ((row = mysql_fetch_row(result)) /*!= NULL*/)
     {
          /* Fonction de l API MySQL qui renvoie le nombre de champs
             dans la table mysql_field_count(connection)); */

          fc = 0;

          while(fc < mysql_field_count(connection))
          {

               /* On remplie une case du tableau de structure */
               switch(fc)
               {
               case 0: tmp.id = atoi(row[fc]); break;
               case 1: tmp.code = row[fc];break;
               default: break;
               }

               fc++;
          }
          //g[token] = tmp;
          token++;
     }

        if (token  == 0) return NULL;
        else
     return tmp.code;
}

void mysql_insert_som(float x, float y, float z,float R, float G, float B, float A)
{
     char *buffer = (char *)malloc(250 * sizeof(char));
     sprintf(buffer,
             "INSERT INTO som(coord_x, coord_y, coord_z, R, G, B, A) values(%s, %s,%s,%s, %s, %s,%s)",
             floattostring (x),floattostring (y) ,floattostring (z),
             floattostring (R),floattostring (G),floattostring (B),floattostring (A));
     mysql_execreq(buffer);
     free (buffer);
}

int erreur_ajout_arete(int poignetx,int poignety)
{
  int i;
  for (i=0;i<nbr_sommet;i++)
    {
      if (sqrt((float)((poignetx-(int)som[i].pos.x)*(poignetx-(int)som[i].pos.x)+(poignety-(int) som[i].pos.y)*(poignety-(int)som[i].pos.y)))<10)
        {
          return 0;
        }
    }
  for (i=0;i<nbr_arete;i++)
    if (poignetx==arete[i].poignet.x && poignety==arete[i].poignet.y)
      return 0;
  return 1;
}

void mysql_insert_arc(float R, float G, float B, float A, float cout, int sommet_selec1,int sommet_selec2,int triangle)
{
  int rep=0;
  int i=0;
  char *buffer = (char *)malloc(250 * sizeof(char));
  float poignetx;
  float poignety;
  float a1x=0,a1y=0,a2x=0,a2y=0;
  for (i=0;i<nbr_sommet;i++)
    {
      if (sommet_selec1==som[i].id)
        {
          a1x=som[i].pos.x;
          a1y=som[i].pos.y;
        }
      if (sommet_selec2==som[i].id)
        {
          a2x=som[i].pos.x;
          a2y=som[i].pos.y;
        }
    }
  poignetx=(a1x+a2x)/2;
  poignety=(a1y+a2y)/2;

  rep=erreur_ajout_arete(poignetx,poignety);
  if (rep==1)
    {
      sprintf(buffer,
              "INSERT INTO arc(x_poig, y_poig, z_poig,R, G, B, A, cout, id_som_1, id_som_2,triangle) values(%s, %s, 0,%s, %s, %s, %s, %s, %i, %i, %i)",
              floattostring (poignetx),floattostring (poignety),
              floattostring (R),floattostring (G),floattostring (B),floattostring (A),
              floattostring(cout),
              sommet_selec1,
              sommet_selec2,
              triangle);
         mysql_execreq(buffer);
       }
     free (buffer);
     sommet_selec1=-1;
     sommet_selec2=-1;
}

void mysql_insert_zone(float R, float G, float B, float A,float x_poig, float y_poig, int nbr_arete_temp,t_liste liste)
{
     char *buffer = (char *)malloc(500 * sizeof(char));
     sprintf(buffer,
             "INSERT INTO zone(x_poig, y_poig, z_poig, R, G, B, A,nbr_arete,liste_arete) values(%s, %s,0, %s, %s, %s, %s,%i,%s)",
             floattostring (x_poig),floattostring (y_poig),
             floattostring (R),floattostring (G),floattostring (B),floattostring(A),
             nbr_arete_temp,
             liste_to_string(liste)
          );
     mysql_execreq(buffer);
     free (buffer);
}

void mysql_delete_som()
{
     char *buffer;
     buffer = (char *)malloc(250 * sizeof(char));

     sprintf(buffer,
             "DELETE FROM arc WHERE id_som_1 = %d OR id_som_2 = %d",
             sommet_selec1,
             sommet_selec1);
     mysql_execreq(buffer);
     strcpy(buffer, "\0");

     sprintf(buffer,
             "DELETE FROM som WHERE id = %d",
             sommet_selec1);
     mysql_execreq(buffer);
     free(buffer);

}

void mysql_delete_arc()
{
     int i;
     t_liste temp;
     char *buffer;
     buffer = (char *)malloc(250 * sizeof(char));
     for (i=0;i<nbr_zone;i++)
     {
          temp=zone[i].liste_arete;
          if (suppr_zone_arete(temp)==1)
          {
               sprintf(buffer,
                       "DELETE FROM zone WHERE id = %i",
                       zone[i].id);
               mysql_execreq(buffer);
               strcpy(buffer, "\0");
          }
     }

     sprintf(buffer,
             "DELETE FROM arc WHERE id = %i",
             arete_selec);
     mysql_execreq(buffer);
     free(buffer);
}

void mysql_delete_zone()
{
     char *buffer;
     buffer = (char *)malloc(250 * sizeof(char));
     sprintf(buffer,
             "DELETE FROM zone WHERE id = %i",
             zone_selec);
     mysql_execreq(buffer);
     free(buffer);
}

void modif_sql_sommet(int val_id)
{
     char *buffer;
     buffer = (char *)malloc(250 * sizeof(char));
     sprintf(buffer,
             "UPDATE som set coord_x = %s WHERE id = %i",
             floattostring (som[id_to_tab(val_id)].pos.x),
             val_id);
     mysql_execreq(buffer);
     strcpy(buffer,"\0");

     sprintf(buffer,
             "UPDATE som set coord_y = %s WHERE id = %i",
             floattostring (som[id_to_tab(val_id)].pos.y),
             val_id);
     mysql_execreq(buffer);
     free(buffer);
}


void mysql_modif_mot_forth (char *mot,int val_id,char *cellule)
{
     char *buffer;
     buffer = malloc (sizeof(char)*100);
     sprintf (buffer,
              "UPDATE %s set mforth = %s WHERE id = %i",
              cellule,
              mot,
              val_id);
     mysql_execreq(buffer);
     free(buffer);
}

void mysql_create_database(const char* dbname)
{
     char *buffer, *buffer2;

     char* table_som = "CREATE TABLE som (id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY, coord_x FLOAT, coord_y FLOAT, coord_z FLOAT, name TEXT, R FLOAT, G FLOAT, B FLOAT, A FLOAT, mforth TEXT)";

     char* table_arc = "CREATE TABLE arc (id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY, x_poig FLOAT, y_poig FLOAT, z_poig FLOAT, name TEXT, R FLOAT, G FLOAT, B FLOAT, A FLOAT, cout FLOAT, id_som_1 INTEGER, id_som_2 INTEGER, mforth TEXT, triangle INTEGER)";

     char* table_zone = "CREATE TABLE zone (id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY, x_poig FLOAT, y_poig FLOAT, z_poig FLOAT, name TEXT, R FLOAT, G FLOAT, B FLOAT, A FLOAT, nbr_arete INTEGER, liste_arete TEXT,mforth TEXT)";

     char* table_prog_forth = "CREATE TABLE  forth (id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY, prog TEXT)";

     char* table_triangle = "CREATE TABLE triangle (id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY, arete1 INTEGER, arete2 INTEGER, arete3 INTEGER, x_poig FLOAT, y_poig FLOAT, x_normal FLOAT , y_normal FLOAT, z_normal FLOAT)";

     buffer = (char *)malloc(250 * sizeof(char));
     buffer2 = (char *)malloc(250 * sizeof(char));
     sprintf(buffer,
             "CREATE DATABASE %s",
             dbname);
     sprintf(buffer2,
             "USE %s",
             dbname);
     mysql_execreq(buffer);
     mysql_execreq(buffer2);
     mysql_execreq(table_som);
     mysql_execreq(table_arc);
     mysql_execreq(table_zone);
     mysql_execreq(table_prog_forth);
     mysql_execreq(table_triangle);
     free(buffer);
     free(buffer2);
}


/*
************************************************************************************
***
***            FONCTIONS SUR LA FENETRE DE CONNEXION A MYSQL
***
************************************************************************************
*/

void mysql_delete_db(const char* dbname)
{
     char *buffer;
     buffer = (char *)malloc(250 * sizeof(char));
     sprintf(buffer,
             "DROP DATABASE %s",
             dbname);
     mysql_execreq(buffer);
     //mysql_nom_base_donnee[0]='\0';
     free(buffer);
}

void mysql_chgt_db(const char* dbname)
{
     char *buffer;
     buffer = (char *)malloc(250 * sizeof(char));
     sprintf(buffer,
             "USE %s",
             dbname);
     mysql_execreq(buffer);
     free(buffer);
}

void prog_forth (const char *prog)
{
     mysql_execreq("DROP TABLE forth");
     mysql_execreq("CREATE TABLE  forth (id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY, prog TEXT)");
     char *buffer = (char *)malloc(500 * sizeof(char));
     sprintf(buffer,
             "INSERT INTO forth ( prog ) values (\"%s\")",
             prog
          );
     mysql_execreq(buffer);
     free (buffer);
}

char *mysql_succ(int sommet)
{
     MYSQL_RES *result;
     MYSQL_ROW row;

     int state;
     char *str = (char *)malloc(sizeof(char));
     char *buffer = (char *)malloc(20 * sizeof(char));
     char *req = (char *)malloc(200 * sizeof(char));

     str = "\0";

     sprintf(buffer, "%i", sommet);

     /* 1er passage */
     sprintf(req, "SELECT id_som_1 FROM arc WHERE id_som_2 = %s AND triangle = 0", buffer);

     state = mysql_query(connection, req);

     printf("Executing request...");
     if (state != 0)
     {
          printf("\t[FAILED]\n");
          printf(mysql_error(connection));
     }
     else
     {
          printf("\t[OK]\n");
     }

     result = mysql_store_result(connection);
     while ((row = mysql_fetch_row(result)))
     {
          if(strlen(str) == 0)
          {
               str = g_strconcat(str, row[0], NULL);
          }
          else
          {
               str = g_strconcat(str, " ", row[0], NULL);
          }
     }

     /* 2eme passage */
     sprintf(req, "SELECT id_som_2 FROM arc WHERE id_som_1 = %s  AND triangle = 0", buffer);

     state = mysql_query(connection, req);

     printf("Executing request...");
     if (state != 0)
     {
          printf("\t[FAILED]\n");
          printf(mysql_error(connection));
     }
     else
     {
          printf("\t[OK]\n");
     }

     result = mysql_store_result(connection);
     while ((row = mysql_fetch_row(result)))
     {
          if(strlen(str) == 0)
          {
               str = g_strconcat(str, row[0], NULL);
          }
          else
          {
               str = g_strconcat(str, " ", row[0], NULL);
          }
     }

     /* Libere la memeoire */
     mysql_free_result(result);
     free(buffer);
     free(req);

     return str;
}


char *ret_mysql(char *requete)
{
     MYSQL_RES *result;
     MYSQL_ROW row;
     int i;
     int state;
     char *str = (char *)malloc(sizeof(char));
     char temp[7];

     temp[6] = '\0';
     str[0] = '\0';

     /* 1er passage */

     state = mysql_query(connection, requete);


     printf("Executing request...");
     if (state != 0)
     {
          printf("\t[FAILED]\n");
          printf(mysql_error(connection));
          return NULL;
     }
     else
     {
          printf("\t[OK]\n");
     }
     for (i=0;i<6;i++)
     {
          temp[i]=requete[i];
     }

     if (strcmp(temp,"SELECT") == 0)
     {
          result = mysql_store_result(connection);
          while ((row = mysql_fetch_row(result)))
          {
               if(strlen(str) == 0)
               {
                    str = g_strconcat(str, row[0], NULL);
               }
               else
               {
                    str = g_strconcat(str, " ", row[0], NULL);
               }
          }

     }
     else
          return NULL;

     /* Libere la memeoire */
     mysql_free_result(result);
     return str;
}

int MySQL2XML(char *filename)
{
  int state;

  FILE* pFile = fopen(filename, "w");

  fprintf(pFile, "%s", "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
  fprintf(pFile, "%s", "<?xml-stylesheet type=\"text/xsl\" href=\"graph.xsl\"?>\n");
  fprintf(pFile, "%s", "<root>\n");

  /* (1) - Table 'som' */
  /* =============================================================== */

  state = mysql_query(connection, "SELECT * FROM som");
  if (state != 0)
    {
      printf(mysql_error(connection));
      return 1;
    }

  result = mysql_store_result(connection);

  fprintf(pFile, "%s", "<graphe-sommet>\n");

  /* process each row in result set */
  while ((row = mysql_fetch_row(result)))
    {
      fprintf(pFile, "%s", "<sommet>\n");

      fprintf(pFile, "%s", "<id>");
      fprintf(pFile, "%s", row[0]);
      fprintf(pFile, "%s", "</id>\n");

      fprintf(pFile, "%s", "<x>");
      fprintf(pFile, "%s", row[1]);
      fprintf(pFile, "%s", "</x>\n");

      fprintf(pFile, "%s", "<y>");
      fprintf(pFile, "%s", row[2]);
      fprintf(pFile, "%s", "</y>\n");

      fprintf(pFile, "%s", "<z>");
      fprintf(pFile, "%s", row[3]);
      fprintf(pFile, "%s", "</z>\n");

      fprintf(pFile, "%s", "<name>");
      fprintf(pFile, "%s", row[4]);
      fprintf(pFile, "%s", "</name>\n");

      fprintf(pFile, "%s", "<R>");
      fprintf(pFile, "%s", row[5]);
      fprintf(pFile, "%s", "</R>\n");

      fprintf(pFile, "%s", "<G>");
      fprintf(pFile, "%s", row[6]);
      fprintf(pFile, "%s", "</G>\n");

      fprintf(pFile, "%s", "<B>");
      fprintf(pFile, "%s", row[7]);
      fprintf(pFile, "%s", "</B>\n");

      fprintf(pFile, "%s", "<A>");
      fprintf(pFile, "%s", row[8]);
      fprintf(pFile, "%s", "</A>\n");

      fprintf(pFile, "%s", "<mforth>");
      fprintf(pFile, "%s", row[9]);
      fprintf(pFile, "%s", "</mforth>\n");

      fprintf(pFile, "%s", "</sommet>\n");
    }

  fprintf(pFile, "%s", "</graphe-sommet>\n");

  /* (2) - Table 'arc' */
  /* =============================================================== */

  state = mysql_query(connection, "SELECT * FROM arc");
  if (state != 0)
    {
      printf(mysql_error(connection));
      return 1;
    }

  result = mysql_store_result(connection);

  fprintf(pFile, "%s", "<graphe-arc>\n");

  /* process each row in result set */
  while ((row = mysql_fetch_row(result)))
    {
      fprintf(pFile, "%s", "<arc>\n");

      fprintf(pFile, "%s", "<id>");
      fprintf(pFile, "%s", row[0]);
      fprintf(pFile, "%s", "</id>\n");

      fprintf(pFile, "%s", "<x>");
      fprintf(pFile, "%s", row[1]);
      fprintf(pFile, "%s", "</x>\n");

      fprintf(pFile, "%s", "<y>");
      fprintf(pFile, "%s", row[2]);
      fprintf(pFile, "%s", "</y>\n");

      fprintf(pFile, "%s", "<z>");
      fprintf(pFile, "%s", row[3]);
      fprintf(pFile, "%s", "</z>\n");

      fprintf(pFile, "%s", "<name>");
      fprintf(pFile, "%s", row[4]);
      fprintf(pFile, "%s", "</name>\n");

      fprintf(pFile, "%s", "<R>");
      fprintf(pFile, "%s", row[5]);
      fprintf(pFile, "%s", "</R>\n");

      fprintf(pFile, "%s", "<G>");
      fprintf(pFile, "%s", row[6]);
      fprintf(pFile, "%s", "</G>\n");

      fprintf(pFile, "%s", "<B>");
      fprintf(pFile, "%s", row[7]);
      fprintf(pFile, "%s", "</B>\n");

      fprintf(pFile, "%s", "<A>");
      fprintf(pFile, "%s", row[8]);
      fprintf(pFile, "%s", "</A>\n");

      fprintf(pFile, "%s", "<cout>");
      fprintf(pFile, "%s", row[9]);
      fprintf(pFile, "%s", "</cout>\n");

      fprintf(pFile, "%s", "<id_som_1>");
      fprintf(pFile, "%s", row[10]);
      fprintf(pFile, "%s", "</id_som_1>\n");

      fprintf(pFile, "%s", "<id_som_2>");
      fprintf(pFile, "%s", row[11]);
      fprintf(pFile, "%s", "</id_som_2>\n");

      fprintf(pFile, "%s", "<mforth>");
      fprintf(pFile, "%s", row[12]);
      fprintf(pFile, "%s", "</mforth>\n");

      fprintf(pFile, "%s", "<triangle>");
      fprintf(pFile, "%s", row[13]);
      fprintf(pFile, "%s", "</triangle>\n");

      fprintf(pFile, "%s", "</arc>\n");
    }

  fprintf(pFile, "%s", "</graphe-arc>\n");

  /* (3) - Table 'zone' */
  /* =============================================================== */

  state = mysql_query(connection, "SELECT * FROM zone");
  if (state != 0)
    {
      printf(mysql_error(connection));
      return 1;
    }

  result = mysql_store_result(connection);

  fprintf(pFile, "%s", "<graphe-zone>\n");

  /* process each row in result set */
  while ((row = mysql_fetch_row(result)))
    {
      fprintf(pFile, "%s", "<zone>\n");

      fprintf(pFile, "%s", "<id>");
      fprintf(pFile, "%s", row[0]);
      fprintf(pFile, "%s", "</id>\n");

      fprintf(pFile, "%s", "<x>");
      fprintf(pFile, "%s", row[1]);
      fprintf(pFile, "%s", "</x>\n");

      fprintf(pFile, "%s", "<y>");
      fprintf(pFile, "%s", row[2]);
      fprintf(pFile, "%s", "</y>\n");

      fprintf(pFile, "%s", "<z>");
      fprintf(pFile, "%s", row[3]);
      fprintf(pFile, "%s", "</z>\n");

      fprintf(pFile, "%s", "<name>");
      fprintf(pFile, "%s", row[4]);
      fprintf(pFile, "%s", "</name>\n");

      fprintf(pFile, "%s", "<R>");
      fprintf(pFile, "%s", row[5]);
      fprintf(pFile, "%s", "</R>\n");

      fprintf(pFile, "%s", "<G>");
      fprintf(pFile, "%s", row[6]);
      fprintf(pFile, "%s", "</G>\n");

      fprintf(pFile, "%s", "<B>");
      fprintf(pFile, "%s", row[7]);
      fprintf(pFile, "%s", "</B>\n");

      fprintf(pFile, "%s", "<A>");
      fprintf(pFile, "%s", row[8]);
      fprintf(pFile, "%s", "</A>\n");

      fprintf(pFile, "%s", "<nbar>");
      fprintf(pFile, "%s", row[9]);
      fprintf(pFile, "%s", "</nbar>\n");

      fprintf(pFile, "%s", "<listar>");
      fprintf(pFile, "%s", row[10]);
      fprintf(pFile, "%s", "</listar>\n");

      fprintf(pFile, "%s", "<mforth>");
      fprintf(pFile, "%s", row[11]);
      fprintf(pFile, "%s", "</mforth>\n");

      fprintf(pFile, "%s", "</zone>\n");
    }

  fprintf(pFile, "%s", "</graphe-zone>\n");

  /* =============================================================== */

  fprintf(pFile, "%s", "</root>\n");

  fclose(pFile);

  /* free the result set */
  mysql_free_result(result);

  return 0;
}

int XML2MySQL(char *filename)
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
                  "INSERT INTO som(id, coord_x, coord_y, coord_z, name, R, G, B, A, mforth) VALUES(%s, %s, %s, %s, \"%s\", %s, %s, %s, %s, \"%s\")",
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
          mysql_execreq(mysql_req_som);
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
          mysql_execreq(mysql_req_arc);
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
          mysql_execreq(mysql_req_zone);
        }
      t++;
    }

  fclose(file);
  return 0;
}
