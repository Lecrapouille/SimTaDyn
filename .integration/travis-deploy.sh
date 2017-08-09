#!/bin/sh -e

make package
curl --ftp-create-dirs -T simtadyn_*.deb -u $QQ $FB
