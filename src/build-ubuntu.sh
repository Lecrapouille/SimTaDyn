#!/bin/bash

echo "Cleaning ..."
rm -fr *~ */*~ SimTaDyn SimTaDyn-generator SimTaDyn-xml2mysql

echo "Compiling SimTaDyn ..."
gcc -w -Wall -O3 -I/usr/include/mysql *.c -o SimTaDyn `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0` -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0 -latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0 -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0 -lcairo `mysql_config --cflags --libs`

echo "Compiling SimTaDyn-generator ..."
gcc -w -Wall -O3 -I/usr/include/mysql generator/generator.c -o SimTaDyn-generator `mysql_config --cflags --libs`

echo "Compiling SimTaDyn-xml2mysql ..."
gcc -w -Wall -O3 -I/usr/include/mysql generator/xml2mysql.c -o SimTaDyn-xml2mysql `mysql_config --cflags --libs`
