#!/bin/bash

rm 10.txt 100.txt 1000.txt

for i in {1..10}
do
    echo -e "$i\n">>10.txt
done

for i in {1..100}
do
    echo -e "$i\n">>100.txt
done

for i in {1..1000};
do
    echo -e "$i\n">>1000.txt
done
