#!/bin/bash

for j in {1..10}
do
for i in 10 100 1000
do
./hilos.o $i $i 10 >> resultado2.txt
done
done
