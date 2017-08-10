#!/bin/bash
##=====================================================================
## SimTaDyn: A GIS in a spreadsheet.
## Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
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
## along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
##=====================================================================

MAINTAINERS=`cat AUTHORS | sed 's/</\\\\</g; s/>/\\\\>/g'`
#MAJOR_VERSION=`grep "define[[:space:]]\+SIMTADYN_MAJOR_VERSION[[:space:]]\+[0-9]\+" src/version.h | grep -o '[0-9]\+'`
#MINOR_VERSION=`grep "define[[:space:]]\+SIMTADYN_MINOR_VERSION[[:space:]]\+[0-9]\+" src/version.h | grep -o '[0-9]\+'`
#VERSION="$MAJOR_VERSION.$MINOR_VERSION"
VERSION=`cat VERSION`

function generate_description
{
    echo "SimTaDyn a GIS in a spreadsheet" > description-pak
}

function make_package()
{
    checkinstall --default \
         --install=no \
         --pkgname="simtadyn" \
         --pkgsource=. \
         --pkglicense="GPL3" \
         --pkgversion="$VERSION" \
         --maintainer="$MAINTAINERS" \
         --requires='libgtksourceviewmm-3.0-dev,freeglut3,freeglut3-dev,mesa-common-dev,libglu1-mesa-dev,libglew-dev,libglfw3-dev,libdw-dev,libsoil-dev' \
         make install
}

generate_description
make_package
