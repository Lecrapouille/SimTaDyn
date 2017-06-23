#!/bin/sh -e

brew update

brew install gtkmm3 libsoil glm cppunit glfw glew

brew link --force gtkmm3 libsoil glm cppunit glfw glew
