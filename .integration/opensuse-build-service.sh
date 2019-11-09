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

# This script allows to create a tarball of SimTaDyn for OpenSuse Build
# (OBS) and upload it to OBS servers. OBS is a compilation farm service,
# its allows to release binaries for different kind of *nix architectures.

#set -x

function out-print
{
    echo -e "\033[35m*** $1:\033[00m \033[36m$2\033[00m => \033[33m$3\033[00m"
}

### Create a temporary folder holding the a git cloned version of SimTaDyn. The aim is to:
###   - upload to OBS a proper tarball
###   - avoid modifying/impacting files within the working space (possibly be a dirty work)
out-print "Getting" "SimTaDyn" "/tmp/simtadyn-obs"
rm -fr /tmp/simtadyn-obs 2> /dev/null
mkdir -p /tmp/simtadyn-obs
(cd /tmp/simtadyn-obs
 git clone --recurse-submodules https://github.com/Lecrapouille/SimTaDyn.git --depth=1 > /dev/null 2> /dev/null
)

### Clone external projects
make download-external-libs

### Replace the SimTaDyn version in the .spec file by the real one.
VERSION=`cat VERSION`
L="s/^Version.*/Version: $VERSION/"
sed -i "$L" .integration/SimTaDyn.spec

### Create the tarball
make targz

### Upload the tarball in the OpenSUSE Build Service. A password is requested (not given here).
### Note to developers: if needed, adapt theses lines of code to your own OBS account.
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
