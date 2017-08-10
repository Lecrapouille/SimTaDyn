#!/bin/sh -e

./.makefile/package.sh
ls -lah simtadyn_*.deb
$DEPLOY simtadyn_*.deb
