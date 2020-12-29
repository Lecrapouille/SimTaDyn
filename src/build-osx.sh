#!/bin/bash

# Install:
# brew install gtk+ gtkglext mariadb openssl

echo "Cleaning ..."
rm -fr *~ */*~ SimTaDyn SimTaDyn-generator SimTaDyn-xml2mysql

export PKG_CONFIG_PATH=/opt/X11/lib/pkgconfig
export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/opt/openssl/lib/

echo "Compiling SimTaDyn ..."
gcc -w -Wall -O3 *.c -o SimTaDyn `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0` -I/usr/local/include -I/opt/X11/include -L/usr/local/lib -I/opt/X11/lib -framework OpenGL -lm -lglib-2.0 -lpangocairo-1.0 -latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0 -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0 -lcairo `mysql_config --cflags --libs`

echo "Compiling SimTaDyn-generator ..."
gcc -w -Wall -O3 generator/generator.c -o SimTaDyn-generator `mysql_config --cflags --libs`

echo "Compiling SimTaDyn-xml2mysql ..."
gcc -w -Wall -O3 generator/xml2mysql.c -o SimTaDyn-xml2mysql `mysql_config --cflags --libs`
