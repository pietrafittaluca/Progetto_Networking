#!/bin/bash

for i in *.c;
do
    primo=$i;
    secondo=${i%.*}".out";
    string="gcc "$primo" -o "$secondo" && ./"$secondo;

    gnome-terminal -e "bash -c '$string;exec $SHELL'"
done