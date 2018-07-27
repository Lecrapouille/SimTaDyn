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

#set -x

HERE=${1##*/}
DATE='-'`date +%Y-%m-%d`
VERSION='-'`cat VERSION`
NTH=0
TARGET_TGZ="SimTaDyn$DATE-v$NTH.tar.gz"

while [ -e "$TARGET_TGZ" ];
do
    NTH=`echo "$TARGET_TGZ" | cut -d"v" -f2 | cut -d"." -f1`
    if [ "$NTH" == "" ];
    then
        echo "ERROR: cannot manage this case $TARGET_TGZ"
        exit 1
    else
        NTH=$(( NTH + 1 ))
        TARGET_TGZ="SimTaDyn$DATE-v$NTH.tar.gz"
    fi
done

echo -e "\033[35m*** Tarball:\033[00m \033[36m$TARGET_TGZ\033[00m <= \033[33m$1\033[00m"

cd .. > /dev/null
tar --transform s/SimTaDyn/SimTaDyn$VERSION/ \
    --exclude='.git' --exclude="SimTaDyn-*.tar.gz" --exclude="doc/coverage" \
    --exclude "doc/coverage" --exclude "*/build" -czvf /tmp/$TARGET_TGZ $HERE \
    > /dev/null && mv /tmp/$TARGET_TGZ $HERE

#FIXME OK it's ugly
cd - > /dev/null
cp $TARGET_TGZ SimTaDyn$VERSION.tar.gz
