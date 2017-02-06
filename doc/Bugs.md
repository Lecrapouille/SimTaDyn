# Known bugs

## 1

Q: The OpenGL context cannot be created.

A: Be sure your graphic card supports OpenGL 3.3 to use the new
SimTaDyn version else use the legacy SimTaDyn. OpenGL 3.3 and previous
context have incompatible context making difficult to make a portage
for both version. In addition gtksourceviewmm needs gtkmm-3.0,
gtkmm-3.0 only manages OpenGL 3.3 and gtkmm-2.4 only manages legacy
OpenGL. And yeah Qt manages better OpenGL than gtk+ !

## 2

Q: When I run SimTaDyn I have no OpenGL graphic and an error says
"OpenGL context creation failed" and a message in the console
GLib-GObject-CRITICAL **: g_value_take_object: assertion 'G_IS_OBJECT
(v_object)' failed

A: With Ubuntu 16.04 gtkmm-3.18 is installed but an bug is present:
the GLArea widget cannot be derived and when it's derived an error
produces this error. In SimTaDyn: we created a class GlDrawingArea:
public Gtk::GLArea, public Renderer because this was the better way to
do things. This bug is known and fiwed in gtkmm >= 3.20. See bug
report https://www.mail-archive.com/gtkmm-list@gnome.org/msg19689.html

In Ubuntu 16.04 with gtkmm-3.22 (installed from
http://www.linuxfromscratch.org/blfs/view/svn/x/gtkmm3.html), the
OpenGL context is correctly created but be careful with Ubuntu 16.04,
gtk >= 3.20 is known to break gtk-3.18 themes giving awful GUIs with
tons of warning messages. Else udpates to Ubuntu 16.10 I think it uses
gtkmm-3.22

## 3

Q: When I run a script I have for example unrocignize words like
'DO' !

A: This is not a bug. Please go to SimTaDyn sources and run make
install-data to install Forth scripts and map datum.  See
[Install](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/Install.md)
for more informations.
