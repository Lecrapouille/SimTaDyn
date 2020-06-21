#!/bin/bash

echo "Cleaning ..."
rm -fr *~ */*~ SimTaDyn SimTaDyn-generator SimTaDyn-xml2mysql

echo "Compiling SimTaDyn ..."
gcc -w -Wall -O3 *.c -o SimTaDyn `pkg-config --cflags --libs mariadb gtk+-2.0 gtkglext-1.0` -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0 -latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0 -lgmodule-2.0 -lgobject-2.0 -lgthread-2.0 -lcairo

echo "Compiling SimTaDyn-generator ..."
gcc -w -Wall -O3 generator/generator.c -o SimTaDyn-generator `pkg-config --cflags --libs mariadb`

echo "Compiling SimTaDyn-xml2mysql ..."
gcc -w -Wall -O3 generator/xml2mysql.c -o SimTaDyn-xml2mysql `pkg-config --cflags --libs mariadb`
