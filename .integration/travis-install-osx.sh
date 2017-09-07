#!/bin/bash

function brew_install
{
  (brew outdated "$1" || brew install "$1") || (echo "Error installing $1"; return 1)
}

#brew update
#brew upgrade
brew_install gtkmm3
brew_install gtksourceviewmm3
brew_install glm
brew_install cppunit
brew_install glfw
brew_install glew

#
brew link --force gtkmm3 gtksourceviewmm3 libsoil glm cppunit glfw glew
