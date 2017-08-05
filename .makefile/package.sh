#!/bin/bash

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
    sudo checkinstall --default \
         --install=no \
         --pkgname="simtadyn" \
         --pkglicense=GPL3 \
         --pkgversion="$VERSION" \
         --maintainer="$MAINTAINERS" \
         --require='libgtksourceviewmm-3.0-dev,freeglut3,freeglut3-dev,mesa-common-dev,libglu1-mesa-dev,libglew-dev,libglfw3-dev,libdw-dev,libsoil-dev'
}

generate_description
make_package
