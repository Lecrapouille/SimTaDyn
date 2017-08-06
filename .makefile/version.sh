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

### Commentary:
# Generate build/version.h from the VERSION file

VERSION=`grep "[0-9]\+\.[0-9]\+" $1 2> /dev/null`
if [ "$VERSION" == "" ];
then
  echo "ERROR: VERSION file is missing or badly formed. Abort !"
  exit 1
fi

MAJOR_VERSION=`echo "$VERSION" | cut -d'.' -f1`
MINOR_VERSION=`echo "$VERSION" | cut -d'.' -f2`

echo "SimTaDyn version $MAJOR_VERSION.$MINOR_VERSION"

echo "#ifndef VERSION_H_" > $2
echo "#  define VERSION_H_" >> $2
echo "" >> $2
echo "#  define SIMTADYN_MAJOR_VERSION $MAJOR_VERSION""u" >> $2
echo "#  define SIMTADYN_MINOR_VERSION $MINOR_VERSION""u" >> $2
echo "" >> $2
echo "#endif /* VERSION_H_ */" >> $2

exit 0
