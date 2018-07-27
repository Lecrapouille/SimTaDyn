#!/bin/bash

### Generate build/version.h from the VERSION file

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
