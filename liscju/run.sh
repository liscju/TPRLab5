#!/bin/bash

g++ -fopenmp -o bsort main.cpp

rm -rf results
mkdir results

tableSizes=(100000 1000000 10000000 100000000)
#tableSizes=(50 100 200 300)
threadCounts=(1 2 4 5 6 7 8 9 11 12)

for tableSize in "${tableSizes[@]}"
do
    touch results/basic_results_${tableSize}
    touch results/scale_results_${tableSize}
    for threadCount in "${threadCounts[@]}"
    do
        ./bsort --basic $tableSize $threadCount >> results/basic_results_${tableSize}
        ./bsort --scaling $tableSize $threadCount >> results/scale_results_${tableSize}
    done
done
