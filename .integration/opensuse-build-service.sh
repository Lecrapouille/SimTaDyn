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

### Deploy SimTaDyn to OpenSuse Build: a compilation farm service
#set -x

function out-print
{
    echo -e "\033[35m*** $1:\033[00m \033[36m$2\033[00m => \033[33m$3\033[00m"
}

### Create a temporary dir to clone the SimTaDyn project
out-print "Getting" "SimTaDyn" "/tmp/simtadyn-obs"
rm -fr /tmp/simtadyn-obs 2> /dev/null
mkdir -p /tmp/simtadyn-obs
cd /tmp/simtadyn-obs
git clone https://github.com/Lecrapouille/SimTaDyn.git --depth=1 > /dev/null 2> /dev/null
cd SimTaDyn

### Clone and compile external projects
make download-external-libs

### Change the SimTaDyn version
VERSION=`cat VERSION`
L="s/^Version.*/Version: $VERSION/"
sed -i "$L" .integration/SimTaDyn.spec

### Create the tarball
make targz

### Commit the tarball into the OpenSUSE Build Service
out-print "Commiting" "SimTaDyn" "OpenSUSE Build Service"
cd .. > /dev/null
osc checkout home:Lecrapouille
cd home:Lecrapouille/SimTaDyn
osc rm `ls SimTaDyn* 2> /dev/null`
cp ../../SimTaDyn/SimTaDyn-$VERSION.tar.gz .
cp ../../SimTaDyn/.integration/SimTaDyn.spec .
osc add SimTaDyn-$VERSION.tar.gz SimTaDyn.spec
MSG="Auto-commit made on "`date +%Y-%m-%d`
osc commit -m "$MSG"
