#!/bin/bash

> resultsParBig.txt
mpicc -std=c99 parMC.c -o parMC.o -lm

points=1000000000
echo === RESULT FOR $points POINTS === >> resultsParBig.txt
for ((i=1; i<=12; i++)); do
    mpiexec -machinefile ./allnodes -np $i ./parMC.o $points >> resultsParBig.txt
done

cat resultsParBig.txt