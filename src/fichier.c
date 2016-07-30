#include "fichier.h"

#define FICHIER_SVGDE_DATABASE "database"

gchar* fichier_to_tampon(const gchar * nomfichier)
{
     FILE *F;
     
     F = fopen(nomfichier, "r");
     if (!F)
     {
	  printf("fichier %s inexistant\n", nomfichier);
	  return NULL;
     }

     gchar * tampon;
     gint taille_lu, taille_tampon, ret;
     
     taille_lu = 0;
     taille_tampon = TAILLE_TAMPON;
     tampon = (gchar *) xmalloc(taille_tampon*sizeof(gchar));
     tampon[0] = '\0';
     do {
	  if (taille_tampon < taille_lu + TAILLE_TAMPON)
	  {
	       taille_tampon *= 2;
	       tampon = xrealloc(tampon, taille_tampon);
	  }
	  ret = fread(tampon + taille_lu, 1, TAILLE_TAMPON, F);
	  taille_lu += ret;
	  if (ret < TAILLE_TAMPON)
	       break;
     } while (ret >= TAILLE_TAMPON);
     fclose(F);
     return tampon; // tampon_to_fichier libere tampon
}

bool tampon_to_fichier(gchar * tampon, const gchar * nomfichier)
{
     FILE * F;
     gint bytes_written, len;

     
     F = fopen (nomfichier, "w");
     if (F == NULL)
     {
	  free(tampon);
	  printf("Le fichier %s n'existe pas\n", nomfichier);
	  return false;
     }
     
     len = strlen(tampon);
     bytes_written = fwrite(tampon, sizeof(gchar), len, F);
     fclose (F);
     g_free(tampon);
     return true;
}

bool charger_fichier_forth(const gchar * nomfichier)
{
     guts.iter = nouveau_iterateur();
     guts.tampon = fichier_to_tampon(nomfichier);
     if (guts.tampon == NULL)
	  return false;
     if (parser_tampon(&guts.tampon) == CORRECT)
     {
	  printf("Interpretation du fichier %s termine avec succes\n", nomfichier);
	  return true;
     }
     else
     {
	  printf("\007Echec de lors de l'interpretation du fichier %s\n", nomfichier);
	  return false;
     }
}
