#!/bin/bash

for j in {1..10}
do
for i in 10 100 1000
do
./matrices2.o $i $i >> resultado.txt
done
done
