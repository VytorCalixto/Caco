#!/bin/bash

for i in {1..10}
do
    echo -e "$i\n">>10.txt
done

for i in {1..100}
do
    echo -e "$i\n">>10.txt
done

for i in {1..1000};
do
    echo -e "$i\n">>1000.txt
done
