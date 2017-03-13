#include "callbacks.h"
#include "trackball.h"

#define PAGE_UP 65365
#define PAGE_DOWN 65366
#define ESC 65307
#define HAUT 65362
#define BAS 65364
#define GAUCHE 65361
#define DROITE 65363
#define DIG_2_RAD (G_PI / 180.0)
#define RAD_2_DIG (180.0 / G_PI)

#define ANIMATE_THRESHOLD 25.0

#define VIEW_SCALE_MAX 10.0
#define VIEW_SCALE_MIN -10

#define NUM_SHAPES 1



typedef struct _MaterialProp
{
     GLfloat ambient[4];
     GLfloat diffuse[4];
     GLfloat specular[4];
     GLfloat shininess;
} MaterialProp;

static MaterialProp mat_gold = {
     {0.24725, 0.1995, 0.0745, 1.0},
     {0.75164, 0.60648, 0.22648, 1.0},
     {0.628281, 0.555802, 0.366065, 1.0},
     0.4
};

static MaterialProp *mat_current = &mat_gold;

static float view_quat_diff[4] = { 0.0, 0.0, 0.0, 1.0 };
static float view_quat[4] = { 0.0, 0.0, 0.0, 1.0 };
static float view_scale = 1.0;

static gboolean animate = FALSE;

static void toggle_animation (GtkWidget *widget);

static gboolean bool_selec = TRUE;
static const GLuint shape_sphere       = 0;
static gboolean deselec=FALSE;
static GLuint shape_list_base = 0;
static GLuint shape_current = 0;
int xprec=0;
int yprec=0;
static int coordx2=0;
static float tourner_camera=0;
static int sommet_selec_temp;
static int arete_selec_temp;
static const gchar *MainWindowKey = "MainWindowKey";
static GtkWidget *Quel_text = NULL;
static GtkWidget *win_open = NULL;
static GtkWidget *win_save = NULL;
static gchar *current_filename = NULL;
static void open_file (GtkWidget *main_window);
static void save_as (GtkWidget *main_window);

static void real_open_file (GtkWidget *main_window, const gchar *filename);
static void real_save_file (GtkWidget *main_window, const gchar *filename);

void save_as (GtkWidget *main_window)
{
     if (win_save == NULL)
     {
          win_save = create_win_save();
          gtk_window_set_transient_for (GTK_WINDOW (win_save),
                                        GTK_WINDOW (main_window));
     }

     gtk_object_set_data (GTK_OBJECT (win_save), MainWindowKey, main_window);
     gtk_window_present (GTK_WINDOW (win_save));
}


static void
open_file (GtkWidget *main_window)
{
     if (win_open == NULL)
     {
          win_open = create_win_open();
          gtk_window_set_transient_for (GTK_WINDOW (win_open),
                                        GTK_WINDOW (main_window));
     }
     gtk_object_set_data (GTK_OBJECT (win_open), MainWindowKey, main_window);
     gtk_window_present (GTK_WINDOW (win_open));
}

static void
real_open_file (GtkWidget *main_window, const gchar *filename)
{
     GtkWidget *text, *notebook, *scrolled_bar, *label;
     GtkTextBuffer *text_buffer;
     gchar * tampon;
     GtkTextIter end;

     tampon = fichier_to_tampon(filename);
     if (tampon != NULL)
     {
          text = gtk_text_view_new();
          text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text));
          gtk_text_buffer_get_end_iter (text_buffer, &end);
          gtk_text_buffer_insert (text_buffer, &end, tampon, -1);

          scrolled_bar = gtk_scrolled_window_new(NULL, NULL);
          gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_bar),
                                         GTK_POLICY_AUTOMATIC,
                                         GTK_POLICY_AUTOMATIC);
          gtk_container_add(GTK_CONTAINER(scrolled_bar), text);


          label = gtk_label_new(strrchr(filename,'/')+1);
          notebook = lookup_widget (GTK_WIDGET (main_window), "ntbk_main");
          gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scrolled_bar, label);
          gtk_widget_show_all(notebook);
     }
}


static void
real_save_file (GtkWidget *main_window, const gchar *filename)
{
     GtkWidget *statusbar,       *tv_nouv;
     GtkTextBuffer *text_buffer;
     GtkTextIter start, end;
     gchar *data;
     FILE *fp;
     gint bytes_written, len;

     if (current_filename == NULL || strcmp (current_filename, filename))
          g_free (current_filename);


     tv_nouv = lookup_widget (GTK_WIDGET (main_window), "tv_nouv");
     text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv_nouv));

     gtk_text_buffer_get_bounds (text_buffer, &start, &end);
     data = gtk_text_buffer_get_text (text_buffer, &start, &end, TRUE);

     fp = fopen (filename, "w");
     if (fp == NULL)
     {
          g_free (data);
          return;
     }

     len = strlen (data);
     bytes_written = fwrite (data, sizeof (gchar), len, fp);
     fclose (fp);

     g_free (data);
}

/*
**********************************************************************
**
**                   ACTIONS SUR LA MENUBAR
**
**********************************************************************
*/

void
conexion_serveur_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
     static GtkWidget *win_gconex = NULL;

     if (win_gconex == NULL)
     {
          win_gconex = create_win_gconex();
          gtk_window_set_transient_for (GTK_WINDOW(win_gconex),
                                        GTK_WINDOW(win_main));
          gtk_window_set_resizable (GTK_WINDOW(win_gconex), FALSE);
     }
     gtk_window_present(GTK_WINDOW(win_gconex));
}

void
on_exporter_carte_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}

void
on_nouveau1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_ouvrir1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
     GtkWidget *main_window;

     main_window = lookup_widget (GTK_WIDGET (menuitem), "win_main");
     open_file (main_window);
}


void
on_enregistrer1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_enregistrer_sous1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
     save_as(win_main);
}


void
on_quitter1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
     static GtkWidget *win_quitte = NULL;

     if (win_quitte == NULL)
     {
          win_quitte = create_win_quitte();
          gtk_window_set_transient_for (GTK_WINDOW(win_quitte),
                                        GTK_WINDOW(win_main));
          gtk_window_set_resizable (GTK_WINDOW(win_quitte), FALSE);
     }
     gtk_window_present(GTK_WINDOW(win_quitte));
}


void
on_couper1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
     GtkWidget *text, *statusbar;

     text = lookup_widget (GTK_WIDGET (menuitem), "tv_nouv");
     g_signal_emit_by_name (G_OBJECT (text), "cut_clipboard", NULL);

     statusbar = lookup_widget (GTK_WIDGET (menuitem), "statusbar");
     gtk_statusbar_pop (GTK_STATUSBAR (statusbar), 1);
     gtk_statusbar_push (GTK_STATUSBAR (statusbar), 1, "Texte coupé");
}


void
on_copier1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
     GtkWidget *text, *statusbar;

     text = lookup_widget (GTK_WIDGET (menuitem), "tv_nouv");
     g_signal_emit_by_name (G_OBJECT (text), "copy_clipboard", NULL);
     statusbar = lookup_widget (GTK_WIDGET (menuitem), "statusbar");
     gtk_statusbar_pop (GTK_STATUSBAR (statusbar), 1);
     gtk_statusbar_push (GTK_STATUSBAR (statusbar), 1, "Texte copié");
}


void
on_coller1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
     GtkWidget *text, *statusbar;

     text = lookup_widget (GTK_WIDGET (menuitem), "tv_nouv");
     g_signal_emit_by_name (G_OBJECT (text), "paste_clipboard", NULL);

     statusbar = lookup_widget (GTK_WIDGET (menuitem), "statusbar");
     gtk_statusbar_pop (GTK_STATUSBAR (statusbar), 1);
     gtk_statusbar_push (GTK_STATUSBAR (statusbar), 1, "Texte collé");
}


void
on_supprimer1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}



void
on_a_propos1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
     static GtkWidget *win_about = NULL;

     if (win_about == NULL)
     {
          win_about = create_win_about();
          gtk_window_set_transient_for (GTK_WINDOW(win_about),
                                        GTK_WINDOW(win_main));
          gtk_window_set_resizable (GTK_WINDOW(win_about), FALSE);
     }
     gtk_window_present(GTK_WINDOW(win_about));
}

/*
**********************************************************************
**
**   FENETRE DE CONFIRMATION POUR QUITTER ET FENETRE APROPOS
**
**********************************************************************
*/

void
on_bt_win_about_fermer_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
     gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}

void
on_bt_win_quit_annule_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
     gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


/*
**********************************************************************
**
**         FENETRE DE SELECTION DE FICHIERS (OPEN & SAVE)
**
**********************************************************************
*/

void
on_bt_win_open_ok_button_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
     GtkWidget *filesel, *main_window;
     const gchar *filename;

     filesel = gtk_widget_get_toplevel (GTK_WIDGET (button));
     main_window = gtk_object_get_data (GTK_OBJECT (filesel), MainWindowKey);
     gtk_widget_hide (filesel);
     filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));
     real_open_file (main_window, filename);
}


void
on_bt_win_open_cancel_button_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
     gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_bt_win_save_ok_button_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
     GtkWidget *filesel, *main_window;
     const gchar *filename;

     filesel = gtk_widget_get_toplevel (GTK_WIDGET (button));
     main_window = gtk_object_get_data (GTK_OBJECT (filesel), MainWindowKey);
     gtk_widget_hide (filesel);
     filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filesel));
     real_save_file (main_window, filename);
}

void
on_bt_win_save_cancel_button_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
     gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}

/*
**********************************************************************
**
**                BOUTONS POUR L'EDITEUR DE FORTH
**
**********************************************************************
*/
void
on_bt_abort_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_bt_execute_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
     GtkWidget *tv_nouv, *tv_res, *statusbar;
     GtkTextBuffer *bf_nouv, *bf_res;
     GtkTextIter debut, fin;
     gdouble secondes = 0;
     gulong microsecondes = 0;
     char *buf, *tamp;
     bool select;
     t_erreur erreur;

     guts.tampon = nouvelle_chaine();
     guts.iter = nouveau_iterateur();

     statusbar = lookup_widget (GTK_WIDGET (button), "statusbar");
     tv_nouv = lookup_widget (GTK_WIDGET (button), "tv_nouv");
     tv_res  = lookup_widget (GTK_WIDGET (button), "tv_res");
     bf_nouv = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv_nouv));
     bf_res = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv_res));
     gtk_text_buffer_set_text(bf_res, "", -1);

     /*
     ** recupere le texte de tv_nouv
     */
     select = gtk_text_buffer_get_selection_bounds(bf_nouv, &debut, &fin);
     if (!select)
          gtk_text_buffer_get_bounds(bf_nouv, &debut, &fin);
     guts.tampon = gtk_text_buffer_get_text(bf_nouv, &debut, &fin, FALSE);
     tamp = guts.tampon;

     /*
     ** et on l'interpreteur la parse
     */
     g_timer_start(guts.chrono);
     erreur = parser_tampon(&guts.tampon);
     if ((!guts.erreur) && (erreur == CORRECT))
     {
          /* chrono */
          g_timer_stop(guts.chrono);

          secondes = g_timer_elapsed(guts.chrono, &microsecondes);
          buf = (gchar*) xmalloc(60*sizeof(gchar));
          sprintf(buf, "Terminé en %g", secondes);
          gtk_statusbar_pop (GTK_STATUSBAR (statusbar), 1);
          gtk_statusbar_push (GTK_STATUSBAR (statusbar), 1, buf);
          g_free(buf);

          /* svgd programme forth */
          buf = gtk_text_buffer_get_text(bf_nouv, &debut, &fin, FALSE);
          gtk_text_buffer_get_end_iter(bf_svgd_code, &fin);
          gtk_text_buffer_insert(bf_svgd_code, &fin, buf, -1);
          gtk_text_buffer_insert(bf_svgd_code, &fin, "\n", -1);
          if (!select)
               gtk_text_buffer_set_text(bf_nouv, "", -1);
          g_free(buf);
          gtk_text_buffer_insert_at_cursor(bf_res, "*** OK ***\n", -1);
     }
     else if (erreur != CORRECT)
     {
          erreur_parser(erreur, bf_nouv, bf_res);
          g_timer_stop(guts.chrono);
          gtk_statusbar_pop (GTK_STATUSBAR (statusbar), 1);
          gtk_statusbar_push (GTK_STATUSBAR (statusbar), 1, "Interpréteur FORTH : ECHEC");
     }
     else if (guts.erreur)
     {
          /* Peut etre afficher IP, PC et nfa */
     }

     recopier_pile_gtk(&guts.pile_donnees, store_pile, &iter_pile);
     guts.mode = MODE_EXECUTION;
     g_free(guts.mytoken);
     g_free(tamp);
     guts.tampon = NULL;
     guts.mytoken = NULL;
     guts.erreur = false;
}


void
on_bt_debug_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
     GtkWidget * statusbar;

     statusbar = lookup_widget (GTK_WIDGET (button), "statusbar");
     if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)))
     {
          gtk_statusbar_pop (GTK_STATUSBAR (statusbar), 1);
          gtk_statusbar_push (GTK_STATUSBAR (statusbar), 1, "Débugger activé");
          execute_mot = execute_mot_avec_debuggeur;
     }
     else
     {
          gtk_statusbar_pop (GTK_STATUSBAR (statusbar), 1);
          gtk_statusbar_push (GTK_STATUSBAR (statusbar), 1, "Débugger désactivé");
          execute_mot = execute_mot_sans_debuggeur;
     }
}


void
on_bt_RAZ_non_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
     gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_bt_RAZ_oui_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
     reinit_hash(&guts.dictionnaire);
     creer_dico_min();
     charger_fichier_forth("kernel.fth");
     gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
     gtk_text_buffer_set_text(bf_res, "Dictionnaire reinitialise !!", -1);
}


void
on_bt_RAZ_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{
     static GtkWidget *win_RAZ = NULL;

     if (win_RAZ == NULL)
     {
          win_RAZ = create_win_RAZ();
          gtk_window_set_transient_for (GTK_WINDOW(win_RAZ),
                                        GTK_WINDOW(win_main));
          gtk_window_set_resizable (GTK_WINDOW(win_RAZ), FALSE);
     }
     gtk_window_present(GTK_WINDOW(win_RAZ));
}

/*
**********************************************************************
**
**             FENETRE DE DESSIN POUR OPENGL
**
**********************************************************************
*/

void init_opgl()
{
     GLfloat aspect;
     GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
     GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
     GLfloat position[] = {0.0, 3.0, 3.0, 0.0};

     GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
     GLfloat local_view[] = {0.0};


     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();

     if (guts.vue == VUE_3D)
     {
       glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
       glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
       glLightfv (GL_LIGHT0, GL_POSITION, position);
       glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
       glLightModelfv (GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

          glFrontFace (GL_CW);
          /*glEnable (GL_LIGHTING);
          glEnable (GL_LIGHT0);
          glEnable (GL_AUTO_NORMAL);
          glEnable (GL_NORMALIZE);*/
          glEnable (GL_DEPTH_TEST);
          glDepthFunc (GL_LESS);

          if (guts.width > guts.height)
          {
               aspect = guts.width / guts.height;
               glFrustum (-aspect, aspect, -1.0, 1.0, 5.0, DISTANCE_CLIPPING);
          }
          else
          {
               aspect = guts.height / guts.width;
               glFrustum (-1.0, 1.0, -aspect, aspect, 5.0, DISTANCE_CLIPPING);
          }
          glMatrixMode(GL_MODELVIEW);
          glLoadIdentity();
          gluLookAt (camera.position.x, camera.position.y, camera.position.z,
                     camera.cible.x, camera.cible.y, camera.cible.z,
                     camera.orientation.x, camera.orientation.y, camera.orientation.z);
     }
     else
     {
          glDisable (GL_LIGHTING);
          glDisable (GL_LIGHT0);
          glDisable (GL_AUTO_NORMAL);
          glDisable (GL_NORMALIZE);
          glDisable (GL_DEPTH_TEST);

          glOrtho(0.0,guts.width, 0.0, guts.height, -1.0, DISTANCE_CLIPPING);
          glMatrixMode(GL_MODELVIEW);
          glLoadIdentity();
     }
}

void
on_drawingarea_realize                 (GtkWidget       *widget,
                                        gpointer         user_data)
{
     GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
     GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);

     /*** OpenGL BEGIN ***/
     if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
          return;

     guts.width = widget->allocation.width;
     guts.height = widget->allocation.height;
     glClearColor(0.0,0.0,0.0,0.0);
     glClearDepth(1.0);

glEnable (GL_LINE_SMOOTH);    glEnable (GL_BLEND);    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);    glLineWidth (1.5);

//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     //glEnable (GL_CULL_FACE);
     //glAlphaFunc(GL_GREATER,0.1f);
     glEnable(GL_ALPHA_TEST);

     if (!LoadTGA(&textures_font[0],"../data/Font.tga"))
     {
          printf("Textures manquantes\n");
          exit(1);
     }

     creer_poignet_3D();
     creer_poignet_2D();
     BuildFont();
     init_opgl();

     gdk_gl_drawable_gl_end (gldrawable);
     /*** OpenGL END ***/
}

gboolean
on_drawingarea_configure_event         (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data)
{
     GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
     GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);

     guts.width = widget->allocation.width;
     guts.height = widget->allocation.height;

     /*** OpenGL BEGIN ***/
     if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
          return FALSE;

     glViewport (0, 0, guts.width, guts.height);
     init_opgl();

     gdk_gl_drawable_gl_end (gldrawable);
     /*** OpenGL END ***/

     return TRUE;
}

gboolean
on_drawingarea_expose_event            (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
     GdkGLContext *glcontext = gtk_widget_get_gl_context (widget);
     GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (widget);

     if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext))
          return FALSE;

     /*** OpenGL BEGIN ***/

     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glLoadIdentity ();

     if (guts.vue == VUE_3D)
     {
          gluLookAt (camera.position.x, camera.position.y, camera.position.z,
                     camera.cible.x, camera.cible.y, camera.cible.z,
                     camera.orientation.x, camera.orientation.y, camera.orientation.z);
          glRotatef(tourner_camera,1.0,0.0,0.0);
     }

     dessiner_la_carte();

     /* Swap buffers */
     if (gdk_gl_drawable_is_double_buffered (gldrawable))
          gdk_gl_drawable_swap_buffers (gldrawable);
     else
          glFlush ();

     gdk_gl_drawable_gl_end (gldrawable);
     /*** OpenGL END ***/

     return TRUE;
}

/***
 *** The "motion_notify_event" signal handler. Any processing required when
 *** the OpenGL-capable drawing area is under drag motion should be done here.
 ***/
gboolean on_drawingarea_motion_notify (GtkWidget   *widget,
                                       GdkEventMotion *event,
                                       gpointer     user_data)
{
  gboolean redraw = FALSE;
  int valtemp;
  int i;

  switch (guts.outil)
    {
    case SELECTION :
      if ((event->state & GDK_BUTTON1_MASK ) || (event->state & GDK_BUTTON2_MASK))
        {
          if (guts.cellule==SOMMET||guts.cellule==ARC||guts.cellule==ZONE)
            {
              lassot.x2 = event->x;
              lassot.y2 = guts.height-event->y;
              lassot.show = true;
              gdk_window_invalidate_rect (widget->window, &widget->allocation, FALSE);
              return TRUE;
            }
          else
            return FALSE;
        }
      break;
    case DEPLACEMENT :
      if (event->state & GDK_BUTTON1_MASK)
        {
          switch (guts.cellule)
            {
            case SOMMET :
              sommet_selec_temp=sommet_selec1;
              valtemp=id_to_tab(sommet_selec1);
              som[valtemp].pos.x=event->x;
              som[valtemp].pos.y=guts.height-event->y;
              for (i=0;i<nbr_arete;i++)
                {
                  if (arete[i].pos1.s==sommet_selec_temp)
                    {
                      arete[i].pos1.x=event->x;
                      arete[i].pos1.y=guts.height-event->y;
                      arete[i].poignet.x=(arete[i].pos1.x+arete[i].pos2.x)/2;
                      arete[i].poignet.y=(arete[i].pos1.y+arete[i].pos2.y)/2;
                    }
                  if (arete[i].pos2.s==sommet_selec_temp)
                    {
                      arete[i].pos2.x=event->x;
                      arete[i].pos2.y=guts.height-event->y;
                      arete[i].poignet.x=(arete[i].pos1.x+arete[i].pos2.x)/2;
                      arete[i].poignet.y=(arete[i].pos1.y+arete[i].pos2.y)/2;
                    }
                }
              calcul_poignet_zone_sommet();
              gdk_window_invalidate_rect (widget->window, &widget->allocation, FALSE);
              return TRUE;
            default :
              return FALSE ;
            }
        }
      break;
    case AUCUN:
      if (guts.vue == VUE_3D)
        {
          if (event->state & GDK_BUTTON3_MASK)
            {
              if (coordx2-event->x<0)
                RotateAroundPoint(camera.cible, 0.1, 0, 1, 0);
              else
                RotateAroundPoint(camera.cible, -0.1, 0, 1, 0);
              redraw=TRUE;
            }
          if (event->state & GDK_BUTTON1_MASK)
            {

                int mouseX,mouseY;
               mouseX=event->x;
               mouseY=event->y;
               float angleY = 0.0;                           // This is the direction for looking up or down
               float angleZ = 0.0;                           // This will be the value we need to rotate around the Y axis (Left and Right)
               static float currentRotX = 0.0;

               angleY = (float)( (xprec - mouseX) ) / 1000.0;
               angleZ = (float)( (yprec - mouseY) ) / 1000.0;
               // Here we keep track of the current rotation (for up and down) so that
               // we can restrict the camera from doing a full 360 loop.
               currentRotX -= angleZ;

               // If the current rotation (in radians) is greater than 1.0, we want to cap it.
               if(currentRotX > 1.0)
                 currentRotX = 1.0;
               // Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
               else if(currentRotX < -1.0)
                 currentRotX = -1.0;
               // Otherwise, we can rotate the view around our position
               else
                 {
                   // To find the axis we need to rotate around for up and down
                   // movements, we need to get a perpendicular vector from the
                   // camera's view vector and up vector.  This will be the axis.
                   // Before using the axis, it's a good idea to normalize it first.
                   t_pos vAxis;
                   t_pos vAxistemp;
                   vAxistemp.x=camera.cible.x-camera.position.x;
                   vAxistemp.y=camera.cible.y-camera.position.y;
                   vAxistemp.z=camera.cible.z-camera.position.z;
                   vAxis.x = (Cross(vAxistemp, camera.orientation)).x;
                   vAxis.y = (Cross(vAxistemp, camera.orientation)).y;
                   vAxis.z = (Cross(vAxistemp, camera.orientation)).z;

                   vAxis = Normalize(vAxis);

        // Rotate around our perpendicular axis and along the y-axis
                   RotationView(angleZ, vAxis.x, vAxis.y, vAxis.z);
                   RotationView(angleY, 0, 1, 0);
                   redraw = TRUE;

                 }
            }
        }
      break;
    default :
      return FALSE;
    }
  xprec=event->x;
  yprec=event->y;
  if (redraw)
    {
      gdk_window_invalidate_rect (widget->window, &widget->allocation, FALSE);
      return TRUE;
    }
  return FALSE;
}


gboolean on_drawingarea_button_release (GtkWidget   *widget,
                                        GdkEventButton *event,
                                        gpointer     user_data)
{
     gboolean redraw = FALSE;

     switch (guts.outil)
     {
     case SELECTION :
       if ((event->state & GDK_BUTTON1_MASK) || (event->state & GDK_BUTTON2_MASK))
         {
           lassot.show = false;
           switch (guts.cellule)
             {
             case SOMMET :
               if ((lassot.x1 == lassot.x2) && (lassot.y1 == lassot.y2))
                 {
                   coordx = lassot.x1; coordy = lassot.y1;
                   ++nbr_selec_som;
                   if (event->state & GDK_BUTTON1_MASK)
                     deselection_som();
                   selec_et_empile_sommet();
                 }
               else //selection au lassot
                 selection_som();
               redraw = true;
               break;
             case ARC :
               if ((lassot.x1 == lassot.x2) && (lassot.y1 == lassot.y2))
                 {
                   coordx = lassot.x1; coordy = lassot.y1;
                   ++nbr_selec_arete;
                   if (event->state & GDK_BUTTON1_MASK)
                     deselection_arete();
                   selec_et_empile_arete();
                 }
               else //selection au lassot
                 selection_arete();
               redraw = true;
               break;
             case ZONE:
               if ((lassot.x1 == lassot.x2) && (lassot.y1 == lassot.y2))
                 {
                     coordx = lassot.x1; coordy = lassot.y1;
                     ++nbr_selec_zone;
                     if (event->state & GDK_BUTTON1_MASK)
                       deselection_zone();
                     selec_et_empile_zone();
                 }
               else //selection au lassot
                 selection_zone();
               redraw = true;
               break;
             default:
               return FALSE;
             }
           if (redraw)
             gdk_window_invalidate_rect (widget->window, &widget->allocation, FALSE);
           return TRUE;
         }
       break;
     case DEPLACEMENT :
       if (event->state & GDK_BUTTON1_MASK)
          {
               redraw = true;
               switch (guts.cellule)
               {
               case SOMMET :
                    deplace_sommet(sommet_selec_temp);
                    redraw=TRUE;
                    break;
               case ARC :

                    break;
               case ZONE :

                    break;
               default:
                 break;
               }
          }
          if (redraw)
            gdk_window_invalidate_rect (widget->window, &widget->allocation, FALSE);
          return TRUE;
     }
     return FALSE;
}


gboolean
on_drawingarea_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{

     int i;
     if (event->button == 3)
       {
         coordx2=event->x;
         return TRUE;
       }
     else
       if (event->button == 2)
         {
           coordx = event->x;
           coordy = (guts.height - event->y);
           lassot.x1 = coordx;
           lassot.y1 = coordy;
           lassot.x2 = lassot.x1;
           lassot.y2 = lassot.y1;
           lassot.show = false;

           souris.x = event->x;
           souris.y = event->y;
           return TRUE;
         }
       else if (event->button == 1)
         {
           coordx = event->x;
           coordy = (guts.height - event->y);
           xprec=coordx;
           yprec=coordy;
           switch (guts.outil)
            {
            case SELECTION :
              if (guts.cellule==SOMMET||guts.cellule==ARC||guts.cellule==ZONE)
                {
                  lassot.x1 = coordx;
                  lassot.y1 = coordy;
                  lassot.x2 = lassot.x1;
                  lassot.y2 = lassot.y1;
                  lassot.show = false;

                  if (nbr_selec_som>0)
                    {
                      deselection_som();
                      recopier_pile_gtk(&guts.pile_donnees,store_pile, &iter_pile);
                      deselec=FALSE;
                    }
                  else
                    if (nbr_selec_arete>0)
                      {
                        deselection_arete();
                        recopier_pile_gtk(&guts.pile_donnees,store_pile, &iter_pile);
                        deselec=FALSE;
                      }
                    else
                      if (nbr_selec_zone>0)
                        {
                          deselection_zone();
                          recopier_pile_gtk(&guts.pile_donnees,store_pile, &iter_pile);
                          deselec=FALSE;
                        }
                }
              break;
            case AJOUT :
              switch (guts.cellule)
                {
                case SOMMET :
                  ajout_som();
                  break;
               case ARC :
                 selec_sommet();
                 if (sommet_selec2!=-1&&sommet_selec1!=-1)
                   ajout_arete(sommet_selec1,sommet_selec2,0);
                 break;
                case ZONE:
                  arete_selec=-1;
                  selec_arete();
                  if (arete_selec!=-1)
                    {
                      if (nbr_elt_liste(listetemp)>1&&arete_selec==fin_liste(&listetemp))
                        {
                          arete_selec=-1;
                          zone[nbr_zone].liste_arete=listetemp;
                          ajout_zone();
                          suppr_liste(&listetemp);
                        }
                      else
                          insert_liste(arete_selec,&listetemp);
                    }
                  break;
                }
              break;
            case SUPPRESSION :
               switch (guts.cellule)
                 {
                 case SOMMET :
                   selec_sommet();
                   suppr_som();
                   break;
                 case ARC :
                    selec_arete();
                    if (arete_selec!=-1)
                      suppr_arete();
                    arete_selec=-1;
                    break;
                 case ZONE:
                    selec_zone();
                    if (zone_selec!=-1)
                      suppr_zone();
                    zone_selec=-1;
                    break;
                 }
               break;
            case DEPLACEMENT :
               switch (guts.cellule)
                 {
               case SOMMET :
                 sommet_selec1=-1;
                    selec_sommet();
                    break;
                 case ARC :
                   arete_selec=-1;
                    selec_arete();
                    break;
                 case ZONE:
                   break;
                 }
               break;
            case AUCUN :
              souris.x = event->x;
              souris.y = event->y;
              break;
            default :
              break;
            }
          gdk_window_invalidate_rect (widget->window, &widget->allocation, FALSE);
          return TRUE;
       }

     return FALSE;
}


gboolean on_drawingarea_key_press_event (GtkWidget   *widget,
                                         GdkEventKey *event,
                                         gpointer     user_data)
{
  gboolean redraw = FALSE;
  if (guts.vue!=VUE_3D)
    {
      if (event->keyval==PAGE_UP)
        affich_struct();
    }
  if (guts.outil == AUCUN && guts.vue == VUE_3D)
    {
      switch (event->keyval)
        {
        case BAS:
          MoveCamera(-0.05);
            redraw = TRUE;
          break;
        case HAUT:
          MoveCamera(0.05);
          redraw = TRUE;
          break;
        case GAUCHE:
          tourner_camera+=2;
          redraw = TRUE;
          break;
          case DROITE:
            tourner_camera-=2;
            redraw = TRUE;
            break;
          default :
            break;
        }
    }
  if (redraw)
    {
      gdk_window_invalidate_rect (widget->window, &widget->allocation, FALSE);
      return TRUE;
    }
  return FALSE;
}


/*
**********************************************************************
**
**       FENTERE DE CONEXION AU SERVEUR DE CARTES MYSQL
**
**********************************************************************
*/

void
on_bt_cancel_win_gconnex_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
     gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}

void
on_bt_nouvelle_carte_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
     GtkWidget *entry_server = lookup_widget(GTK_WIDGET(button), "en_host");
     const gchar *mysql_gtk_server = gtk_entry_get_text(GTK_ENTRY(entry_server));

     GtkWidget *entry_user = lookup_widget(GTK_WIDGET(button), "en_user");
     const gchar *mysql_gtk_user = gtk_entry_get_text(GTK_ENTRY(entry_user));

     GtkWidget *entry_pswd = lookup_widget(GTK_WIDGET(button), "en_pswd");
     const gchar *mysql_gtk_pswd = gtk_entry_get_text(GTK_ENTRY(entry_pswd));

     GtkWidget *entry_db = lookup_widget(GTK_WIDGET(button), "entry_database");
     const gchar *mysql_gtk_db = gtk_entry_get_text(GTK_ENTRY(entry_db));

     if (db_connect(mysql_gtk_server, mysql_gtk_user, mysql_gtk_pswd, NULL) == 0)
          mysql_create_database(mysql_gtk_db);
}


void
on_bt_supp_carte_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
     GtkWidget *entry_server = lookup_widget(GTK_WIDGET(button), "en_host");
     const gchar *mysql_gtk_server = gtk_entry_get_text(GTK_ENTRY(entry_server));

     GtkWidget *entry_user = lookup_widget(GTK_WIDGET(button), "en_user");
     const gchar *mysql_gtk_user = gtk_entry_get_text(GTK_ENTRY(entry_user));

     GtkWidget *entry_pswd = lookup_widget(GTK_WIDGET(button), "en_pswd");
     const gchar *mysql_gtk_pswd = gtk_entry_get_text(GTK_ENTRY(entry_pswd));

     GtkWidget *entry_db = lookup_widget(GTK_WIDGET(button), "combo_entry_supp");
     const gchar *mysql_gtk_db = gtk_entry_get_text(GTK_ENTRY(entry_db));

     if (db_connect(mysql_gtk_server, mysql_gtk_user, mysql_gtk_pswd, mysql_gtk_db) == 0)
          mysql_delete_db(mysql_gtk_db);
}


void
on_bt_charger_carte_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
     int i;
     t_erreur erreur;

     GtkWidget *entry_server = lookup_widget(GTK_WIDGET(button), "en_host");
     const gchar *mysql_gtk_server = gtk_entry_get_text(GTK_ENTRY(entry_server));

     GtkWidget *entry_user = lookup_widget(GTK_WIDGET(button), "en_user");
     const gchar *mysql_gtk_user = gtk_entry_get_text(GTK_ENTRY(entry_user));

     GtkWidget *entry_pswd = lookup_widget(GTK_WIDGET(button), "en_pswd");
     const gchar *mysql_gtk_pswd = gtk_entry_get_text(GTK_ENTRY(entry_pswd));

     GtkWidget *entry_db = lookup_widget(GTK_WIDGET(button), "combo_entry_charger");
     const gchar *mysql_gtk_db = gtk_entry_get_text(GTK_ENTRY(entry_db));

     //if (strcmp(mysql_gtk_db,mysql_nom_base_donnee)!=0)
     // {
         if (db_connect(mysql_gtk_server, mysql_gtk_user, mysql_gtk_pswd, mysql_gtk_db) == 0)
           {
             mysql_db_som();
             mysql_db_arc();
             mysql_db_zone();
             mysql_db_triangle();

             guts.tampon = nouvelle_chaine();
             guts.iter = nouveau_iterateur();

             guts.tampon = mysql_db_forth();
             if (guts.tampon != NULL)
               erreur = parser_tampon(&guts.tampon);
             printf("\007%d\n", erreur);

             //stocker tamp dans bf_nouv

             sommet_selec1=-1;
             sommet_selec2=-1;
             arete_selec=-1;
             zone_selec=-1;
             guts.vue = VUE_2D;
             ajouter_nomdeschamps_magazin_champ(GTK_TREE_STORE(store_champ), mysql_gtk_db);
             gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
             gdk_window_invalidate_rect (drawingarea->window, &drawingarea->allocation, FALSE);
             //strcpy(nom_base_donnee,mysql_gtk_db);
             //}
       }
         //else
         //printf (" erreur base de donne\n");
}

void
on_bt_connexion_mysql_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
     static GtkWidget *win_gconex = NULL;

     if (win_gconex == NULL)
     {
          win_gconex = create_win_gconex();
          gtk_window_set_transient_for (GTK_WINDOW(win_gconex),
                                        GTK_WINDOW(win_main));
          gtk_window_set_resizable (GTK_WINDOW(win_gconex), FALSE);
     }
     gtk_window_present(GTK_WINDOW(win_gconex));
}


void
on_tg_show_forth_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
     GtkWidget *hpaned;
     gint lg;

     lg = win_main->allocation.width;
     hpaned = lookup_widget(GTK_WIDGET (button), "hpaned1");

     if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)))
          gtk_paned_set_position(GTK_PANED(hpaned), 0);
     else
          gtk_paned_set_position(GTK_PANED(hpaned), lg - 40);
}


void
on_tg_show_champ_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
     if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)))
          gtk_widget_show_all(win_champ);
     else
          gtk_widget_hide(win_champ);
}


void
on_en_prompt_activate                  (GtkEntry        *entry,
                                        gpointer         user_data)
{
  const gchar *entry_text;

  entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  action_prompt(entry, entry_text);
}

void
selection_changed (GtkTreeSelection *selection)
{
     GtkTreeIter iter;
     t_nom_champ champ;
     t_cell cellule;
     GtkTreeModel *model;

     if (gtk_tree_selection_get_selected (selection, &model, &iter))
          gtk_tree_model_get(model, &iter, 1, &cellule, 2, &champ, -1);
     guts.champ = champ;
     guts.cellule = cellule;
     gdk_window_invalidate_rect (drawingarea->window, &drawingarea->allocation, FALSE);
}

void
on_slct_couleur_ok_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  int i;
     GtkWidget *cl_select = lookup_widget(GTK_WIDGET(button), "color_selection");
     GdkColor color;
     char buffer[256];
     gtk_color_selection_get_current_color(GTK_COLOR_SELECTION(cl_select), &color);
     Couleur.R = ((float) color.red) / 65535.0*1000;
     Couleur.G = ((float) color.green) / 65535.0*1000;
     Couleur.B = ((float) color.blue) / 65535.0*1000;
     Couleur.A = ((float) gtk_color_selection_get_current_alpha(GTK_COLOR_SELECTION(cl_select))) / 65535.0*1000;
     affich_struct();
     if (guts.cellule==SOMMET)
       {
         for (i=0;i<nbr_selec_som;i++) //pour chaque sommet de la pile
           {
             sprintf (buffer,"UPDATE som set R = %s , G = %s , B = %s, A = %s  WHERE id = %d",
                      floattostring (Couleur.R),
                      floattostring (Couleur.G),
                      floattostring (Couleur.B),
                      floattostring (Couleur.A),
                      guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier);
             mysql_execreq(buffer);
             /*som[id_to_tab(guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier)].densite.R=Couleur.R;
             som[id_to_tab(guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier)].densite.G=Couleur.G;
             som[id_to_tab(guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier)].densite.B=Couleur.B;
             som[id_to_tab(guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier)].densite.A=Couleur.A;*/
             buffer[0] = '\0';
           }
         TsommetInit();
         mysql_db_som();
         deselection_som();
         gdk_window_invalidate_rect (drawingarea->window, &drawingarea->allocation, FALSE);
       }
     affich_struct();
     if (guts.cellule==ARC)
       {
         for (i=0;i<nbr_selec_arete;i++) //pour chaque arete de la pile
           {
             sprintf (buffer,"UPDATE arc set R = %s , G = %s , B = %s, A = %s  WHERE id = %d",
                      floattostring (Couleur.R),
                      floattostring (Couleur.G),
                      floattostring (Couleur.B),
                      floattostring (Couleur.A),
                      guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier);
             mysql_execreq(buffer);
             /*arete[id_to_tab2(guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier)].densite.R=Couleur.R;
             arete[id_to_tab2(guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier)].densite.G=Couleur.G;
             arete[id_to_tab2(guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier)].densite.B=Couleur.B;
             arete[id_to_tab2(guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier)].densite.A=Couleur.A;*/
             buffer[0] = '\0';
           }
         TareteInit();
           mysql_db_arc();
         deselection_arete();
         gdk_window_invalidate_rect (drawingarea->window, &drawingarea->allocation, FALSE);
       }

     if (guts.cellule==ZONE)
       {
         for (i=0;i<nbr_selec_zone;i++) //pour chaque zone de la pile
           {
             sprintf (buffer,"UPDATE zone set R = %s , G = %s , B = %s, A = %s  WHERE id = %d",
                      floattostring (Couleur.R),
                      floattostring (Couleur.G),
                      floattostring (Couleur.B),
                      floattostring (Couleur.A),
                      guts.pile_donnees.donnees[guts.pile_donnees.position-i].entier);
             mysql_execreq(buffer);
             buffer[0] = '\0';
           }
         TzoneInit();
         mysql_db_zone();
         deselection_zone();
         gdk_window_invalidate_rect (drawingarea->window, &drawingarea->allocation, FALSE);
       }

     gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_slct_couleur_cancel_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
     gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_bt_show_color_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
     static GtkWidget *win_couleur = NULL;

     if (win_couleur == NULL)
     {
          win_couleur = create_win_couleur();
          gtk_window_set_transient_for (GTK_WINDOW(win_couleur),
                                        GTK_WINDOW(win_main));
          gtk_window_set_resizable (GTK_WINDOW(win_couleur), FALSE);
     }
     gtk_window_present(GTK_WINDOW(win_couleur));
}

/*
**********************************************************************
**                SELECTION DES ACTIONS
**********************************************************************
*/

void enfonce_bt_outil(bool aj, bool sup, bool ins, bool dep)
{
     GtkWidget *tg_mode_ajout;
     GtkWidget *tg_mode_supp;
     GtkWidget *tg_mode_select;
     GtkWidget *tg_mode_deplace;

     tg_mode_ajout = lookup_widget (GTK_WIDGET (win_main),"tg_mode_ajout");
     tg_mode_supp = lookup_widget (GTK_WIDGET (win_main),"tg_mode_supp");
     tg_mode_select = lookup_widget (GTK_WIDGET (win_main),"tg_mode_select");
     tg_mode_deplace = lookup_widget (GTK_WIDGET (win_main),"tg_mode_deplace");

     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tg_mode_ajout),aj);
     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tg_mode_supp),sup);
     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tg_mode_select),ins);
     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tg_mode_deplace),dep);
}

void
on_tg_mode_ajout_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
     sommet_selec1=-1;
     if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)))
     {
          enfonce_bt_outil(true,false,false,false);
          guts.outil = AJOUT;
     }
     else guts.outil = AUCUN;
}

void
on_tg_mode_supp_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
     coordx=coordy=selecx=selecy=0;
     if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)))
     {
          enfonce_bt_outil(false,true,false,false);
          guts.outil = SUPPRESSION;
     }
     else guts.outil = AUCUN;
}


void
on_tg_mode_select_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
     if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)))
     {
          enfonce_bt_outil(false,false,true,false);
          guts.outil = SELECTION;
     }
     else guts.outil = AUCUN;
}

void
on_tg_mode_deplace_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
     if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)))
     {
          enfonce_bt_outil(false,false,false,true);
          guts.outil = DEPLACEMENT;
     }
     else guts.outil = AUCUN;
}


void
on_tg_mode_vue_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
     if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button)))
       {
         guts.vue = VUE_3D;
         trianguler_carte();
         enfonce_bt_cell(FALSE,FALSE,FALSE);
         enfonce_bt_outil(FALSE,FALSE,FALSE,FALSE);
       }
     else
          guts.vue = VUE_2D;

     init_opgl();
     on_drawingarea_configure_event(drawingarea,NULL,user_data);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glLoadIdentity ();
     gdk_window_invalidate_rect (drawingarea->window, &drawingarea->allocation, FALSE);
}

/*
**********************************************************************
**                SELECTION DES OBJETS
**********************************************************************
*/

void enfonce_bt_cell(bool som, bool arc, bool zone)
{
     GtkWidget *tg_mode_som;
     GtkWidget *tg_mode_arc;
     GtkWidget *tg_mode_zone;

     guts.cellule = AUCUNE;

     tg_mode_som = lookup_widget (GTK_WIDGET (win_main),"tg_mode_som");
     tg_mode_arc = lookup_widget (GTK_WIDGET (win_main),"tg_mode_arc");
     tg_mode_zone = lookup_widget (GTK_WIDGET (win_main),"tg_mode_zone");

     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tg_mode_som),som);
     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tg_mode_arc),arc);
     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tg_mode_zone),zone);
}

void
on_tg_mode_som_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
     if (gtk_toggle_button_get_active(togglebutton))
     {
          enfonce_bt_cell(true,false,false);
          guts.cellule = SOMMET;
     }
     else
       guts.cellule = AUCUNE;
     gdk_window_invalidate_rect (drawingarea->window, &drawingarea->allocation, FALSE);
}

void
on_tg_mode_arc_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
     if (gtk_toggle_button_get_active(togglebutton))
     {
          enfonce_bt_cell(false,true,false);
          guts.cellule = ARC;
     }
     else
       guts.cellule = AUCUNE;
     gdk_window_invalidate_rect (drawingarea->window, &drawingarea->allocation, FALSE);
}

void
on_tg_mode_zone_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
     if (gtk_toggle_button_get_active(togglebutton))
     {
          enfonce_bt_cell(false,false,true);
          guts.cellule = ZONE;
     }
     else
       guts.cellule = AUCUNE;
     gdk_window_invalidate_rect (drawingarea->window, &drawingarea->allocation, FALSE);
}


void
on_bt_execute_carte_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
     int i,j;

     if (pile_vide(&guts.pile_donnees) || (sommet(&guts.pile_donnees).type != ENTIER))
          i = 1;
     else
     {
          i = sommet(&guts.pile_donnees).entier;
          depiler(&guts.pile_donnees);
     }

     switch (guts.cellule)
     {
     case SOMMET :
          for (j = 0; j < i; ++j)
          {

               for (sommet_selec1=0; sommet_selec1<nbr_sommet; sommet_selec1++)
               {
                    if ((som[sommet_selec1].mot_forth != NULL) && (som[sommet_selec1].mot_forth[0] != '\0'))
                    {
                         guts.iter = nouveau_iterateur();
                         guts.tampon = som[sommet_selec1].mot_forth;
                         empiler(&guts.pile_donnees, int_to_elt(som[sommet_selec1].id));
                         parser_tampon(&guts.tampon);
                    }
               }
          }
          break;
     case ARC :
          for (arete_selec=0; arete_selec<nbr_arete; arete_selec++)
          {
               if ((arete[arete_selec].mot_forth != NULL) && (arete[arete_selec].mot_forth[0] != '\0'))
               {
                    guts.iter = nouveau_iterateur();
                    guts.tampon = arete[arete_selec].mot_forth;
                    empiler(&guts.pile_donnees, int_to_elt(arete[arete_selec].id));
                    parser_tampon(&guts.tampon);
               }
          }
          break;
     default : printf("\007Erreur execute carte !!\n");
     }
     gdk_window_invalidate_rect (drawingarea->window, &drawingarea->allocation, FALSE);
}


gboolean
on_win_champ_destroy_event             (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
     GtkWidget *tg;

     tg = lookup_widget (GTK_WIDGET (win_main),"tg_show_champ");
     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tg), false);
     gtk_widget_hide(win_champ);

     return TRUE;
}


gboolean
on_win_champ_delete_event              (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
     GtkWidget *tg;

     tg = lookup_widget (GTK_WIDGET (win_main),"tg_show_champ");
     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(tg), false);
     gtk_widget_hide(win_champ);

     return TRUE;
}

void
on_exporter_code_forth_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
     GtkTextIter debut, fin;
     gchar * tampon;

     tampon = nouvelle_chaine();

     gtk_text_buffer_get_bounds(bf_nouv, &debut, &fin);
     tampon = gtk_text_buffer_get_text(bf_nouv, &debut, &fin, FALSE);

     prog_forth(tampon);
     g_free(tampon);
}


void MoveCamera(float speed)
{
  t_pos  vector={0,0,0};
  vector.x=camera.cible.x-camera.position.x;
  vector.y=camera.cible.y-camera.position.y;
  vector.z=camera.cible.z-camera.position.z;
  camera.position.x += vector.x * speed;
  camera.position.z += vector.z * speed;
  camera.cible.x += vector.x * speed;
  camera.cible.z += vector.z * speed;
  }

void RotationView(float angle, float x, float y, float z)
{
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

void RotateAroundPoint(t_pos vCenter, float angle, float x, float y, float z)
{
    t_pos vNewPosition;

    // To rotate our position around a point, what we need to do is find
    // a vector from our position to the center point we will be rotating around.
    // Once we get this vector, then we rotate it along the specified axis with
    // the specified degree.  Finally the new vector is added center point that we
    // rotated around (vCenter) to become our new position.  That's all it takes.

    // Get the vVector from our position to the center we are rotating around
    t_pos vPos;
    vPos.x= camera.position.x - vCenter.x;
    vPos.y= camera.position.y - vCenter.y;
    vPos.z= camera.position.z - vCenter.z;

    // Calculate the sine and cosine of the angle once
    float cosTheta = (float)cos(angle);
    float sinTheta = (float)sin(angle);

    // Find the new x position for the new rotated point
    vNewPosition.x  = (cosTheta + (1 - cosTheta) * x * x)        * vPos.x;
    vNewPosition.x += ((1 - cosTheta) * x * y - z * sinTheta)    * vPos.y;
    vNewPosition.x += ((1 - cosTheta) * x * z + y * sinTheta)    * vPos.z;

    // Find the new y position for the new rotated point
    vNewPosition.y  = ((1 - cosTheta) * x * y + z * sinTheta)    * vPos.x;
    vNewPosition.y += (cosTheta + (1 - cosTheta) * y * y)        * vPos.y;
    vNewPosition.y += ((1 - cosTheta) * y * z - x * sinTheta)    * vPos.z;

    // Find the new z position for the new rotated point
    vNewPosition.z  = ((1 - cosTheta) * x * z - y * sinTheta)    * vPos.x;
    vNewPosition.z += ((1 - cosTheta) * y * z + x * sinTheta)    * vPos.y;
    vNewPosition.z += (cosTheta + (1 - cosTheta) * z * z)        * vPos.z;

    // Now we just add the newly rotated vector to our position to set
    // our new rotated position of our camera.
    camera.position.x = vCenter.x + vNewPosition.x;
    camera.position.y = vCenter.y + vNewPosition.y;
    camera.position.z = vCenter.z + vNewPosition.z;
}





void
on_bt_exporter_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
     static GtkWidget *win_exporte = NULL;

     if (win_exporte == NULL)
     {
          win_exporte = create_fs_exporter();
          gtk_window_set_transient_for (GTK_WINDOW(win_exporte),
                                        GTK_WINDOW(win_main));
          gtk_window_set_resizable (GTK_WINDOW(win_exporte), FALSE);
     }
     gtk_window_present(GTK_WINDOW(win_exporte));
}


void
on_bt_importer_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
     static GtkWidget *win_importe = NULL;

     if (win_importe == NULL)
     {
          win_importe = create_fs_importer();
          gtk_window_set_transient_for (GTK_WINDOW(win_importe),
                                        GTK_WINDOW(win_main));
          gtk_window_set_resizable (GTK_WINDOW(win_importe), FALSE);
     }
     gtk_window_present(GTK_WINDOW(win_importe));
}


void
on_bt_importer_ok_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
const gchar *selected_filename;

selected_filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (gtk_widget_get_toplevel (GTK_WIDGET (button))));
   g_print ("Selected filename: %s\n", selected_filename);
 gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
 XML2MySQL(selected_filename);
}


void
on_bt_exporter_ok_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
     const gchar *selected_filename;

     selected_filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (gtk_widget_get_toplevel (GTK_WIDGET (button))));
     g_print ("Selected filename: %s\n", selected_filename);
 gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
 MySQL2XML(selected_filename);
}


void
on_cancel_button1_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
     gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}


void
on_cancel_button2_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
     gtk_widget_hide (gtk_widget_get_toplevel (GTK_WIDGET (button)));
}
