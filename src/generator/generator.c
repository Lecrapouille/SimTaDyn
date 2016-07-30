
#include <stdio.h>
#include <stdlib.h>

#include <mysql.h>


MYSQL_RES *result;
MYSQL_ROW row;
MYSQL *connection;
MYSQL mysql;

int db_connect(char *mysql_host,
               char *mysql_user,
               char *mysql_password,
               char *mysql_db)
{
  /* connect to database */
  mysql_init(&mysql);
  connection = mysql_real_connect(&mysql,
                                  mysql_host,
                                  mysql_user,
                                  mysql_password,
                                  mysql_db,
                                  0,
                                  0,
                                  0);

  /* check for connection error */
  if (connection == NULL)
    {
      /* print error message */
      printf(mysql_error(&mysql));
      return 1;
    }

  return 0;
}

int MySQL2XML()
{
  int state;

  FILE* pFile = fopen("graph.xml", "w");

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

int main()
{
  char *hostname;
  char *user;
  char *password;
  char *database;

  hostname = (char *)malloc(250 * sizeof(char));
  user = (char *)malloc(250 * sizeof(char));
  password = (char *)malloc(250 * sizeof(char));
  database = (char *)malloc(250 * sizeof(char));

  printf("###############################################\n");
  printf("###          UltraSimtadyn Project          ###\n");
  printf("###           MySQL2XML Generator           ###\n");
  printf("###         Code by Juzam - Apr 2004        ###\n");
  printf("###############################################\n\n");

  printf("Hostname : ");
  scanf("%s", hostname);
  printf("User : ");
  scanf("%s", user);
  printf("Password : ");
  scanf("%s", password);
  printf("Database : ");
  scanf("%s", database);

  if(db_connect(hostname, user, password, database) != 0)
    {
      printf("Connexion au serveur MySQL impossible\n");
      return 1;
    }


  if(MySQL2XML() != 0)
    {
      printf("fatal error ...\n");
      return 1;
    }
  else
    {
      printf("output > graph.xml\n");
    }

  /* close the connection */
  mysql_close(connection);
  return EXIT_SUCCESS;
}
