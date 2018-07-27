#!/bin/bash

FOO=/tmp/f1.txt

rm -fr $FOO 2> /dev/null

find . -name "*.[ch]pp" -exec grep -nH "fixme" {} \; >> $FOO
find . -name "*.[ch]pp" -exec grep -nH "FIXME" {} \; >> $FOO
find . -name "*.[ch]pp" -exec grep -nH "tbd" {} \; >> $FOO
find . -name "*.[ch]pp" -exec grep -nH "TBD" {} \; >> $FOO
find . -name "*.[ch]pp" -exec grep -nH "todo" {} \; >> $FOO
find . -name "*.[ch]pp" -exec grep -nH "TODO" {} \; >> $FOO
find . -name "*.[ch]pp" -exec grep -nH "QQ" {} \; >> $FOO

sort -u $FOO > fixme.txt
