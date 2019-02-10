# Description of files

* Gtkmm.[it]pp: (do not confuse with gtkmm.h which is the official file). This file adds some routines to
  Gtk for this project like spliting the gui with Gtk::Paned or searching widgets inside the gui.

* ToggleButtons.[ch]pp: Mimic a Gtk::RadioButtons inside a Gtk::Toolbar. Said differently, make a single
  button active in a set a buttons. Therefore, only one button is pressed and others are released.

* KeyBoardHandler.[ch]pp: Facade for Gtk keyboard events. Memorize keyboard key which was pressed, released.
  Allow to set a callback timeout to get keys at fixed rate.

* Redirection.[ch]pp: redirect std::cout and std::cerr into a Gtk::TextBuffer. This is used for redirecting
  Forth console results into the GUI.

* DialogException.[ch]pp: Facade for Gtk::MessageDialog used for displaying C++ exception in a Gtk+ window.
  The stuff I do not like with Gtk window is that the parent window shall be given and it's something boring.
  So the getRootWindow() will do it.

* DrawingArea.[ch]pp: Facade for Gtk::GLArea coupled with the OpenGL Renderer class. Hides all the misery
  code/callbacks for implementing the OpenGL contect, draw() callbacks ...

* Renderer.[ch]pp, NodesRenderer.[ch]pp, ArcsRenderer.[ch]pp, AreasRenderer.[ch]pp : OpenGL renderer for
  drawing nodes, arcs, polygons.

* MapEditor.[ch]pp: GIS editor. Manage a set of Presenter.

* SimTaDynMapExplorer.[ch]pp displays the content of a SimTaDyn map file in a Gtk::TreeView. Allow to modify
  the content of a node of a SceenGraph (= SimTaDyn sheet).

* TextEditor.[ch]pp: Facade for gtksourceviewmm. Implement a simple text editor in a notebook (like gedit)
  with basic fetures like undo/redo, search, replace, goto line, ask saving the document when closing.

* ForthEditor.[ch]pp: Derived class from TextEditor. Implement specific code for Forth (syntax hightling,
  word completion.

* ForthInspector.[ch]pp Gtk widgets for inspector the Forth states (data stack, dictionary)

* MapEditorWindow.[ch]pp, ForthEditorWindow.[ch]pp: encapuslate editors in a Gtk::Window.


