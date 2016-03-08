#!/bin/bash
clear
cd generate-scala

# Required python2.7 modules:
# jinja2
# yaml

python bridge.py -o ../peripheral/output.scala ../badhash_1.xml ../badhash_2.xml
cd ../peripheral
make test
cd ..
