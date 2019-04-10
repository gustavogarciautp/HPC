#!/bin/bash

for j in {1..10}
do
for i in 10 100 1000 5000
do
./matriz.o $i $i >> resultado.txt
done
done
