#!/bin/bash

# This script is called when Travis-CI succeeded its job.

### Deploy the unit tests coverage report
RAPPORT=build/rapport.info
cd tests
lcov --quiet --directory .. -c -o $RAPPORT
lcov --quiet --remove $RAPPORT '/usr*' 'external/*' 'tests/*' -o $RAPPORT
coveralls-lcov --source-encoding=ISO-8859-1 $RAPPORT
cd ..

### Generate RPM and Debian packages
# TODO: call this script when SimTaDyn changed of version.
# TODO: currently I prefer to do it manually.
# cd ../.integration
# ./opensuse-build-service.sh
# cd ..
