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
## along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
##=====================================================================

# This scripts allows to upload SimTaDyn to OpenSuse Build (OBS).
# OBS is a compilation farm service, its allows to release binaries for
# different kind of *nix architected.

#set -x

function out-print
{
    echo -e "\033[35m*** $1:\033[00m \033[36m$2\033[00m => \033[33m$3\033[00m"
}

### Create a temporary dir for cloning the SimTaDyn project in the aim to:
###   - to download to OBS a proper tarball
###   - to modify files with no risk of wrong commit on github
out-print "Getting" "SimTaDyn" "/tmp/simtadyn-obs"
rm -fr /tmp/simtadyn-obs 2> /dev/null
mkdir -p /tmp/simtadyn-obs
cd /tmp/simtadyn-obs
git clone https://github.com/Lecrapouille/SimTaDyn.git --depth=1 > /dev/null 2> /dev/null
cd SimTaDyn

### Clone and compile external projects
make download-external-libs

### Replace the SimTaDyn version in the .spec file by the real one.
VERSION=`cat VERSION`
L="s/^Version.*/Version: $VERSION/"
sed -i "$L" .integration/SimTaDyn.spec

### Create the tarball
make targz

### Upload the tarball in the OpenSUSE Build Service.
### A password is requested. If needed, adapt these
### lines of code to your own OBS account.
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
