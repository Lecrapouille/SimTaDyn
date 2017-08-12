#!/bin/bash

./.makefile/package.sh
ls -lah simtadyn_*.deb
$DEPLOY simtadyn_*.deb
