#include "string_util.h"

bool chaine_to_int(gchar * ch, int *nb)
{
     bool res = true;
     gchar * cp_ch;

     if (ch[0] == '\0')
	  return false;

     /*
     ** verifie que que des nombres
     */
     cp_ch = ch;
     if ((ch[0] == '-') || (ch[0] == '+')) ++ch;
     while ((*ch != '\0') && (res))
     {
	  res = g_ascii_isdigit(*ch);
	  ++ch;
     }
     
     if ((!res) || ((ch[1] == '\0') && ((ch[0] == '-') || (ch[0] == '+'))))
	  return false;

     *nb = g_ascii_strtoull(cp_ch, NULL, 10);
     return true;
}

bool chaine_to_float(gchar * ch, float *nb)
{
     bool res = true;
     int nb_point = 0;
     gchar * cp_ch;

     if (ch[0] == '\0')
	  return false;

     /*
     ** verifie que que des nombres
     */
     cp_ch = ch;
     if ((ch[0] == '-') || (ch[0] == '+')) ++ch;
     while ((*ch != '\0') && (res) && (nb_point <= 1))
     {
	  if ((*ch == '.') || (*ch == ','))
	       ++nb_point;
	  else
	       res = g_ascii_isdigit(*ch);
	  ++ch;
     }

     if ((!res) || ((ch[1] == '\0') && 
		    ((ch[0] == '.') || 
		     (ch[0] == ',') || 
		     (ch[0] == '-') || 
		     (ch[0] == '+'))))
	  return false;

     *nb = strtof(cp_ch, NULL); 
     return true;
}

void my_printf(const gchar * texte, GtkTextTag * balise, GtkTextBuffer * txtbf)
{
     GtkTextIter debut, fin;
     
     gtk_text_buffer_get_end_iter(txtbf, &debut);  
     gtk_text_buffer_insert(txtbf, &debut, texte, -1);
     gtk_text_buffer_get_end_iter(txtbf, &fin);  
     gtk_text_buffer_apply_tag(txtbf, balise, &debut, &fin);
}

unsigned int maxi(unsigned int a, unsigned int b)
{
     if (a > b)
	  return a;
     else
	  return b;
}

t_buffer nouveau_buffer(unsigned int taille, char * chaine)
{
     t_buffer buf;

     buf.taille_allouee = MAX(taille, strlen(chaine));
     buf.str = (char*) malloc(buf.taille_allouee*sizeof(char));
     buf.str = strcat(buf.str, chaine);

     return buf;
}
     
void ajouter_string(t_buffer *buf, char* chaine)
{
     unsigned int t = (*buf).taille_allouee;
     
     (*buf).taille_allouee += strlen(chaine);
     if ((*buf).taille_allouee > t)
	  (*buf).str = (char*) realloc((*buf).str, (*buf).taille_allouee*sizeof(char));
     (*buf).str = strcat((*buf).str, chaine);
}

inline void destroy_buffer(t_buffer buf)
{
     free(buf.str);
}

inline void reinit_buffer(t_buffer *buf)
{
     free((*buf).str);
     (*buf).str = (char*) malloc(sizeof(char));
     (*buf).str[0] = '\0';
     (*buf).taille_allouee = 0;
}

t_iterateur nouveau_iterateur()
{
     t_iterateur iter;

     iter.x = 0;
     iter.y = 0;
     iter.oldx = 0;
     iter.oldy = 0;

     return iter;
}

gchar* nouvelle_chaine(void)
{
     gchar * buffer;
 
     buffer = (gchar *) xmalloc(sizeof(char));
     buffer[0] = '\0';

     return buffer;
}

inline bool car_separation(gchar * ch)
{
     return ((*ch == ' ') || (*ch == '\n') || (*ch == '\t'));
}

gchar *my_cut(gchar ** ch, gchar * mytoken, t_iterateur * iter)
{
     int lg = -1; 
     mytoken = nouvelle_chaine();

     while ((**ch != '\0') && (car_separation(*ch)))
     {
	  if (**ch == '\n')
	  {
	       ++(*iter).y;
	       (*iter).x = 0;
	  }
	  else
	       ++(*iter).x;
	  ++(*ch);
     }

     (*iter).oldx = (*iter).x; (*iter).oldy = (*iter).y;
     while ((**ch != '\0') && (!car_separation(*ch)))
     {
	  ++lg;
	  mytoken = g_strconcat(mytoken, " ", NULL);
	  mytoken[lg] = MAJUSCULE(**ch);
	  ++(*iter).x; ++(*ch);
     }
     return mytoken;
}

