# SimTaDyn Bugs

## Reporting a bug

Feel free to open issues in github ! There are different traces than you can add with the issue for helping solving faster the issue:
* In debug mode, SimTaDyn is compiled with [backward](https://github.com/bombela/backward-cpp) a beautiful stack trace pretty printer for C++.
When a crash occurs a stack trace is printed on the console.
* SimTaDyn creates a log file. When starting SimTaDyn writes on the console the location of the logfile. For example
```
Log created: '/tmp/SimTaDyn/SimTaDyn.log'
```
* You also can call valgrind on SimTaDyn, this will help tracing memory corruptions.

## Known bugs

### Bug 1

> Q: The OpenGL context cannot be created.

A: Be sure your graphic card supports the modern OpenGL. Since OpenGL version 3.0 all API routines version 2.1 were marked deprecated and finally removed since version 3.3 with an incompatible context. The API changed to follow the need for modern 3D games. So now, two OpenGL contexts exist: one for old hardwares and the second for new hardwared. This makes difficult the portage for managing both versions in the same application and in addition this is not possible in SimTaDyn because of gtk developers. Indeed the C++ version of [gtksourceview](https://wiki.gnome.org/Projects/GtkSourceView) is used which needs gtkmm-3.0 and this last lib only uses the modern OpenGL context. In opposition, the gtkmm-2.4 only manages the legacy OpenGL and not gtksourceviewmm. So f*** up and yeah Qt manages better OpenGL than gtk !

* SimTaDyn on the master branch uses the modern OpenGL because of gtkmm-3.0.
* Legacy SimTaDyn on the branch Original-Version-EPITA-2004 uses the legacy OpenGL.
* SimTaDyn on the SimTaDyn-gtkmm-2.4 branch uses the legacy OpenGL because of gtkmm-2.4 but this branch is abandonned.

Type:
``glxinfo | grep version``
and look for OpenGL version string which shall be >= 3.0

### Bug 2

> Q: I have a graphic card compatible with OpenGL 3.0 but when I run SimTaDyn I have no OpenGL graphic and an error says "OpenGL context creation failed" and a message in the console GLib-GObject-CRITICAL **: g_value_take_object: assertion 'G_IS_OBJECT (v_object)' failed

A: With Ubuntu 16.04 gtkmm-3.18 is installed and an bug is present: the Gtk::GLArea widget (the gtkmm OpenGL area) cannot be derived and if it is the case an issue produces this message error. For more information see the [bug report](https://www.mail-archive.com/gtkmm-list@gnome.org/msg19689.html) In SimTaDyn we made the choice to create a class heriting both from Gtk::GLArea and a rendering class because that was the better way to do things and hoping Ubuntu will updates gtkmm version because fixed in gtkmm >= 3.20. I'm not sure but Ubuntu 16.10 has the good version and for Ubuntu 16.04 gtkmm-3.22 can be easily installed from http://www.linuxfromscratch.org/blfs/view/svn/x/gtkmm3.html but be careful gtk >= 3.20 is known to break gtk-3.18 themes giving awful GUIs with tons of warning messages and missing icons. (Yeah again Qt is better than GTK).

### Bug 3

> Q: When I run a script I have for example unrecognized words like 'DO' !

A: This is not a bug. Please go to SimTaDyn sources and type in a console``make install-data`` to install Forth scripts and other datum. See [Install](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/Install.md) for more informations.
