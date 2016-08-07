#! /bin/bash

rm -fr *~ prog observer_example

g++ -W -Wall main.cpp -o prog -std=c++11

#g++ -W -Wall observer_example.cpp -o observer_example -std=c++11
