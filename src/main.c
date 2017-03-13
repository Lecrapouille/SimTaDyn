#include "main.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

GdkGLConfig *glconfig;


static void
print_gl_config_attrib (GdkGLConfig *glconfig,
                        const gchar *attrib_str,
                        int          attrib,
                        gboolean     is_boolean)
{
     int value;

     g_print ("%s = ", attrib_str);
     if (gdk_gl_config_get_attrib (glconfig, attrib, &value))
     {
          if (is_boolean)
               g_print ("%s\n", value == TRUE ? "TRUE" : "FALSE");
          else
               g_print ("%d\n", value);
     }
     else
          g_print ("*** Cannot get %s attribute value\n", attrib_str);
}

static void
examine_gl_config_attrib (GdkGLConfig *glconfig)
{
     g_print ("\nOpenGL visual configurations :\n\n");

     g_print ("gdk_gl_config_is_rgba (glconfig) = %s\n",
              gdk_gl_config_is_rgba (glconfig) ? "TRUE" : "FALSE");
     g_print ("gdk_gl_config_is_double_buffered (glconfig) = %s\n",
              gdk_gl_config_is_double_buffered (glconfig) ? "TRUE" : "FALSE");
     g_print ("gdk_gl_config_is_stereo (glconfig) = %s\n",
              gdk_gl_config_is_stereo (glconfig) ? "TRUE" : "FALSE");
     g_print ("gdk_gl_config_has_alpha (glconfig) = %s\n",
              gdk_gl_config_has_alpha (glconfig) ? "TRUE" : "FALSE");
     g_print ("gdk_gl_config_has_depth_buffer (glconfig) = %s\n",
              gdk_gl_config_has_depth_buffer (glconfig) ? "TRUE" : "FALSE");
     g_print ("gdk_gl_config_has_stencil_buffer (glconfig) = %s\n",
              gdk_gl_config_has_stencil_buffer (glconfig) ? "TRUE" : "FALSE");
     g_print ("gdk_gl_config_has_accum_buffer (glconfig) = %s\n",
              gdk_gl_config_has_accum_buffer (glconfig) ? "TRUE" : "FALSE");

     g_print ("\n");

     print_gl_config_attrib (glconfig, "GDK_GL_USE_GL",           GDK_GL_USE_GL,           TRUE);
     print_gl_config_attrib (glconfig, "GDK_GL_BUFFER_SIZE",      GDK_GL_BUFFER_SIZE,      FALSE);
     print_gl_config_attrib (glconfig, "GDK_GL_LEVEL",            GDK_GL_LEVEL,            FALSE);
     print_gl_config_attrib (glconfig, "GDK_GL_RGBA",             GDK_GL_RGBA,             TRUE);
     print_gl_config_attrib (glconfig, "GDK_GL_DOUBLEBUFFER",     GDK_GL_DOUBLEBUFFER,     TRUE);
     print_gl_config_attrib (glconfig, "GDK_GL_STEREO",           GDK_GL_STEREO,           TRUE);
     print_gl_config_attrib (glconfig, "GDK_GL_AUX_BUFFERS",      GDK_GL_AUX_BUFFERS,      FALSE);
     print_gl_config_attrib (glconfig, "GDK_GL_RED_SIZE",         GDK_GL_RED_SIZE,         FALSE);
     print_gl_config_attrib (glconfig, "GDK_GL_GREEN_SIZE",       GDK_GL_GREEN_SIZE,       FALSE);
     print_gl_config_attrib (glconfig, "GDK_GL_BLUE_SIZE",        GDK_GL_BLUE_SIZE,        FALSE);
     print_gl_config_attrib (glconfig, "GDK_GL_ALPHA_SIZE",       GDK_GL_ALPHA_SIZE,       FALSE);
     print_gl_config_attrib (glconfig, "GDK_GL_DEPTH_SIZE",       GDK_GL_DEPTH_SIZE,       FALSE);
     print_gl_config_attrib (glconfig, "GDK_GL_STENCIL_SIZE",     GDK_GL_STENCIL_SIZE,     FALSE);
     print_gl_config_attrib (glconfig, "GDK_GL_ACCUM_RED_SIZE",   GDK_GL_ACCUM_RED_SIZE,   FALSE);
     print_gl_config_attrib (glconfig, "GDK_GL_ACCUM_GREEN_SIZE", GDK_GL_ACCUM_GREEN_SIZE, FALSE);
     print_gl_config_attrib (glconfig, "GDK_GL_ACCUM_BLUE_SIZE",  GDK_GL_ACCUM_BLUE_SIZE,  FALSE);
     print_gl_config_attrib (glconfig, "GDK_GL_ACCUM_ALPHA_SIZE", GDK_GL_ACCUM_ALPHA_SIZE, FALSE);

     g_print ("\n");
}


int
main (int argc, char *argv[])
{
     GtkWidget *tv_debug, *tv_nouv, *tv_res, *tv_svgd_code, *treeview;
     gint major, minor;
//     GtkWidget *drawingarea;
     gtk_set_locale ();

     gtk_init (&argc, &argv);
     gtk_gl_init (&argc, &argv);
     add_pixmap_directory ("../data/pixmaps");

     /*
     ** Configure OpenGL-capable visual.
     */
     gdk_gl_query_version (&major, &minor);
     g_print ("\nOpenGL extension version - %d.%d\n",
              major, minor);

     /*
     ** Configure OpenGL-capable visual.
     */
     /* Try double-buffered visual */
     glconfig = gdk_gl_config_new_by_mode (GDK_GL_MODE_RGB    |
                                           GDK_GL_MODE_DEPTH  |
                                           GDK_GL_MODE_DOUBLE);
     if (glconfig == NULL)
     {
          g_print ("*** Cannot find the double-buffered visual.\n");
          g_print ("*** Trying single-buffered visual.\n");

          /* Try single-buffered visual */
          glconfig = gdk_gl_config_new_by_mode (GDK_GL_MODE_RGB   |
                                                GDK_GL_MODE_DEPTH);
          if (glconfig == NULL)
          {
               g_print ("*** No appropriate OpenGL-capable visual found.\n");
               exit (1);
          }
     }
     examine_gl_config_attrib(glconfig);


     /* la camera avant la creation de la drawing area */
     camera.position.x = 382;
       camera.position.y = 250.0;
       camera.position.z = 1620;
       camera.cible.x = 382;
       camera.cible.y = 250.0;
       camera.cible.z = 1.0;
     camera.orientation.x = 0.0;
     camera.orientation.y = 1.0;
     camera.orientation.z = 0.0;
     camera.zoom = 45;

     /*
     ** Creation de la fenetre principale et amelioration de widgets
     */
     win_main = create_win_main();
     win_champ = create_win_champ();

     GtkWidget *hpaned;
     hpaned = lookup_widget(GTK_WIDGET(win_main), "hpaned1");
     gtk_paned_set_position(GTK_PANED(hpaned), 760);

     GtkWidget *vbox_drawing;
     vbox_drawing = lookup_widget (GTK_WIDGET(win_main),"vbox_drawing");

     drawingarea = gtk_drawing_area_new ();
     gtk_widget_show (drawingarea);
     gtk_box_pack_start (GTK_BOX (vbox_drawing), drawingarea, TRUE, TRUE, 0);

     /* Set OpenGL-capability to the widget */
     gtk_widget_set_gl_capability (drawingarea,
                                   glconfig,
                                   NULL,
                                   TRUE,
                                   GDK_GL_RGBA_TYPE);
     gtk_widget_add_events (drawingarea,
                            GDK_BUTTON1_MOTION_MASK    |
                            GDK_BUTTON2_MOTION_MASK    |
                            GDK_BUTTON3_MOTION_MASK    |
                            GDK_BUTTON_RELEASE_MASK    |
                            GDK_BUTTON_PRESS_MASK);

     g_signal_connect ((gpointer) drawingarea, "realize",
                       G_CALLBACK (on_drawingarea_realize),
                       NULL);
     g_signal_connect ((gpointer) drawingarea, "configure_event",
                       G_CALLBACK (on_drawingarea_configure_event),
                       NULL);
     g_signal_connect ((gpointer) drawingarea, "expose_event",
                       G_CALLBACK (on_drawingarea_expose_event),
                       NULL);
     g_signal_connect ((gpointer) drawingarea, "button_press_event",
                       G_CALLBACK (on_drawingarea_button_press_event),
                       NULL);
     g_signal_connect((gpointer) drawingarea, "motion_notify_event",
                      G_CALLBACK(on_drawingarea_motion_notify), NULL);
     g_signal_connect((gpointer) drawingarea, "button_release_event",
                      G_CALLBACK(on_drawingarea_button_release), NULL);
     g_signal_connect_swapped ((gpointer) win_main, "key_press_event",
                      G_CALLBACK (on_drawingarea_key_press_event), drawingarea);

     GLADE_HOOKUP_OBJECT (win_main, drawingarea, "drawingarea");

     /* visiualisateur du dictionnaire */
     treeview = lookup_widget(GTK_WIDGET (win_main), "tree_dico");
     cl_dico = creer_visualdico();
     gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), cl_dico);
     ajouter_colonne_visualdico(GTK_TREE_VIEW (treeview));

     /* visualisateur de nom de champs */
     treeview = lookup_widget(GTK_WIDGET(win_champ), "tv_champ");
     store_champ = gtk_tree_store_new(3, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT);
     gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store_champ));
     ajouter_col_visual_champ(GTK_TREE_VIEW (treeview));

     select_champ = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
     gtk_tree_selection_set_mode(select_champ, GTK_SELECTION_SINGLE);
     g_signal_connect (select_champ,
                       "changed",
                       G_CALLBACK (selection_changed),
                       NULL);

     /* visualisateur de la pile de donnees */
     treeview = lookup_widget(GTK_WIDGET(win_champ), "tv_pile");
     store_pile = gtk_list_store_new(1, G_TYPE_STRING);
     gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store_pile));
     ajouter_col_visual_pile_donnees(GTK_TREE_VIEW(treeview));

     /* Attache un buffer aux textview */
     tv_nouv = lookup_widget(GTK_WIDGET (win_main), "tv_nouv");
     tv_res =  lookup_widget(GTK_WIDGET (win_main), "tv_res");
     tv_debug =  lookup_widget(GTK_WIDGET (win_main), "tv_debug");
     tv_svgd_code = lookup_widget(GTK_WIDGET (win_main), "tv_svgd_code");
     bf_nouv = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv_nouv));
     bf_res = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv_res));
     bf_debug = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv_debug));
     bf_svgd_code = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv_svgd_code));

     balise_erreur =
          gtk_text_buffer_create_tag(bf_res, "balise_erreur", "foreground", "red",
                                     "background", "white", "weight", PANGO_WEIGHT_BOLD,
                                     "justification", GTK_JUSTIFY_RIGHT, NULL);
     balise_soulignee =
          gtk_text_buffer_create_tag(bf_res, "balise_soulignee", "foreground", "blue",
                                     "background", "pink", "weight", PANGO_WEIGHT_BOLD,
                                     "justification", GTK_JUSTIFY_RIGHT, NULL);
     balise_bonne =
          gtk_text_buffer_create_tag(bf_res, "balise_bonne", "foreground", "blue",
                                     "background", "white", "weight", PANGO_WEIGHT_BOLD,
                                     "justification", GTK_JUSTIFY_RIGHT, NULL);

     balise_normale =
          gtk_text_buffer_create_tag(bf_res, "balise_normale", "foreground", "black",
                                     "background", "white", "weight", PANGO_WEIGHT_BOLD,
                                     "justification", GTK_JUSTIFY_RIGHT, NULL);

     printf("*==========================*\n");
     printf("|                          |\n");
     printf("|   SIMTADYN Version 1.0   |\n");
     printf("|                          |\n");
     printf("*==========================*\n\n");




     /*
     ** Initialisation des boyaux de l'interpreteur (guts est en param global)
     */
        nbr_selec_som = 0;
     lassot.x1 = lassot.y1 = lassot.x2 = lassot.y2 = 0;
     lassot.show = false;
     guts.champ = ID;
     guts.cellule = AUCUNE;
     guts.mode = MODE_EXECUTION;
     guts.debugger = false;
     guts.iter = nouveau_iterateur();
     guts.dernier_mot_cree = NULL;
     guts.dictionnaire = nouveau_hash();
     guts.pile_donnees = nouvelle_pile();
     guts.pile_retour = nouvelle_pile();
     guts.chrono = g_timer_new();
     guts.main_trash = nouveau_trash();
     execute_mot = execute_mot_sans_debuggeur;


     /*
     ** Creation d'un dico minimal et chargement de fichiers de prog FORTH
     */
     creer_dico_min();
     if ((!charger_fichier_forth("../data/kernel.fth")) || (!charger_fichier_forth("../data/simgraph.fth")))
          exit(1);
     recopier_pile_gtk(&guts.pile_donnees, store_pile, &iter_pile);

     gtk_widget_show_all(win_main);
     gtk_widget_show_all(win_champ);
     gtk_window_set_transient_for (GTK_WINDOW(win_champ),
                                   GTK_WINDOW(win_main));
     gtk_window_present(GTK_WINDOW(win_champ));

     gtk_main ();


     /* sauvegarde des databases */
     //sauver_db(liste_database);
     //g_list_free(liste_database);

     /*
     ** Liberation de la memoire
     */
     cleanall_trash(&guts.main_trash);
     g_free(guts.chrono);
     destroy_hash(&guts.dictionnaire);

     printf("\n*** bye ***\n");
     return 0;
}
