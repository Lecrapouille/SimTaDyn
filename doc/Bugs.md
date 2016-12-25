# Known bugs

With Ubuntu 16.04, gtkmm-3.18 is installed but an bug is present: the GLArea widget cannot be derived and
in SimTaDyn: we created a class GlDrawingArea: public Gtk::GLArea, public Renderer.

With gtkmm-3.18, the OpenGL context is not created. See bug report https://www.mail-archive.com/gtkmm-list@gnome.org/msg19689.html

With gtkmm-3.22 (installed from http://www.linuxfromscratch.org/blfs/view/svn/x/gtkmm3.html), the OpenGL context is correctly created but gtk >= 3.20 is known to break gtk-3.18 themes giving awful GUIs with tons of warning messages.
