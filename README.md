# SimTaDyn

SIMulateur TAbleur DYNamique

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/SimTaDyn.jpg)

Compilation (Linux or OS X):
You'll need MySQL, GTK+2 and Legacy OpenGL and sorry the original Makefile has been lost ! Call one of the script shell depending on your OS:

```
cd src/
./build-ubuntu.sh
```
or:
```
cd src/
./build-osx.sh
```

Note concerning OSX:
* if an error message "ld: library not found for -lssl clang: error: linker command failed with exit code 1 (use -v to see invocation)" try to type in your console:
xcode-select --install
* OpenGL is now badly supported with GTK+ (while working well in 2004): the current bug is the OpenGL background color covering all GTK+ widgets :(

## How to use SimTaDyn ?

This is the version made in 2004 by students having just learnt C. So this is not a finished application.

In a frist Linux console, start MySQL:
```
/etc/init.d/mysql start
```

Since that make >15 years I did not practice MySQL and lost root password I do the following hack and working on root mode. Please do not follow this bad practice ! Live safe !
```
/etc/init.d/mysql stop
sudo mysqld_safe --skip-grant-tables
```

In a second Linux console, start SimTaDyn:
```
./SimTaDyn
```

### Connexion to MySQL

- Drag to the left the vertical pan to show the Forth menu.
- Connect to MySQL server:
  - Click from menu `Carte/Fichier > Connexion to MySQL`
  - On the popup window, configure:
    - `Host: localhost`
    - `User: root` (for example)
    - `Password: admin` (for example)
    - If this is the first time: `Gestion des carte: Nouvelle: mymap` or if one has already been created `Gestion des carte: Database: mymap`. Click on the corresponding `Validate` button.

If this step is not done, SimTaDyn will crash when trying to add a cell !

### Create a map

- Click on the red button `Passer en mode SOMMET` and click on the plus button `Ajouter une cellule`.
- Click two times at 2 different position on the black window on the left. You have added two nodes.
- Click on the green button `Passer en mode ARC` and click on the plus button `Ajouter une cellule`.
- Click at on each node. You have added an arc.
- Continue the operation to create a triangle shape.
- Click on the house button `Passer en mode ZONE` and click on the plus button `Ajouter une cellule`.
- Click at on each arc. Click again on the first arc. You have added an area.
- To save it, type `>TABLE` on the Forth intrepeter in the right side, near the notebook tab `Nouveau`.

### See you saved MySQL

In the frist Linux console, type:
```
mysql -u root
```

Inside MySQL, type:
```
use mymap;
show tables;
SELECT * FROM arc;
```

### Script Forth

- On the Forth intrepeter in the right side, near the notebook tab `Nouveau`, you can type
```
1 2 +
```

- Press `Ctl-Enter`key or press the `Executer` button. You will see `*** OK ***`.
- Click on the floating window `Montreur de champs` and click on `Pile`, you will in the stack the number `3`.
- On the Forth intrepeter again, execute `.` you will notice the number `3` has disapeared in the stack and a message `3 *** OK ***` appeared.

### Modify fields of the map

- Now on the tab `Carte` in the floating window `Montreur de champs` unroll list and click on the different fields, you will see the map shows the selected information (cost, name, id, forth words ...).
- Click on the green button `Passer en mode ARC` and click on the plus button `Selectionner une cellule`.
- Now, select `Arcs > Cout`.
- With the mouse select a region of the map to select several arcs.
- Type on the entry box on the top position of the map and type a number ie 42. Type enter. The selected arcs have now the cost 42.
- Now, select `Sommets > Forth`, select and type a forth word, see the selected arcs have the new forth word on them.
- Note that selected cells are pushed on the stack `Pile`. When deselecting cells they are poped off. You can execute Forth command on selected cells.

### Execute SimTaDyn map

- Click on the button `Executer N times the map`. A basic example could be each node computes the mean of costs of its neighbor.

### Modify SimTaDyn map

- Rhe Forth interpreter have some words manipulating the map. For example
  - `1 2 A+` allows to add a new arc from nodes 1 and 2 (Note a bug the window has to be refreshed to see the operation).
  - `1 DEG .` show the degree of the node 1.

### 2D/3D mode

The 2d mode is the most evolved but you cannot move the map, the 3d mode has not really be made and the camera control (mouse and keyboard) is a total disaster :)
