#!/bin/bash
##=====================================================================
## SimTaDyn: A GIS in a spreadsheet.
## Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
##
## This file is part of SimTaDyn.
##
## SimTaDyn is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
##=====================================================================

# This script is called by continuous integration tools for checking
# if main project and sub-projects still compile, as well as uni tests.
# And if unit-tests can be compiled and if project has no regression.
# This script is architecture agnostic and should be launched for any
# operating systems (Linux, OS X, Windows).

function exists
{
  test -e $1 || (echo "The file $1 does not exist" && exit 1)
}

# For OS X and homebrew >= 2.60
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/opt/libffi/lib/pkgconfig

# Clone and compile 3thpart librairies that SimTaDyn depends on.
make download-external-libs || exit 1
make compile-external-libs || exit 1

# FIXME Disable OpenGL tests because docker (called by
# Travis-CI) does not manage export display so OpenGL
# tests will failed just because its context failed to
# be created.
# Edit: Now we can test OpenGL wrapper classes without
# necessary creating OpenGL context.
## export CI=true

# Installation directory when CI
CI_DESTDIR=/tmp

# Build, install SimTaDyn executable and check if
# installed files exist.
make install DESTDIR=$CI_DESTDIR $JCORES || exit 1
VERSION=`cat VERSION`
exists $CI_DESTDIR/usr/share/SimTaDyn/$VERSION/data
exists $CI_DESTDIR/usr/share/SimTaDyn/$VERSION/doc
exists $CI_DESTDIR/usr/share/SimTaDyn/$VERSION/AUTHORS
exists $CI_DESTDIR/usr/share/SimTaDyn/$VERSION/README.md
exists $CI_DESTDIR/usr/share/SimTaDyn/$VERSION/LICENSE
exists $CI_DESTDIR/usr/share/SimTaDyn/$VERSION/ChangeLog
exists $CI_DESTDIR/usr/bin/SimTaDyn-$VERSION

# Build SimTaDyn unit tests.
(cd tests && make unit-tests DESTDIR=$CI_DESTDIR $JCORES) || exit 1

# Build standalone Forth.
# TODO: launch Forth unit tests when they will be done:
# && ./build/SimForth -f ../core/system.fs -f../core/tester.fs
(cd src/forth/standalone && make $JCORES) || exit 1

# Build SpreadSheet standalone
(cd src/core/standalone/ClassicSpreadSheet && make $JCORES) || exit 1

# Build OpenGL examples
(cd src/common/graphics/OpenGL/examples/ && make $JCORES) || exit 1
