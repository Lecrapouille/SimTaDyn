#! /bin/bash

rm -fr *~ prog

g++ -W -Wall  shapefile.cpp main.cpp -o prog -std=c++11
