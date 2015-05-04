#!/bin/bash
gcc -fopenmp -o bsort bucketsort.c

tableSizes=(100000 1000000 10000000 100000000)
threadCounts=(1 2 4 6 8 10 12)

for tableSize in "${tableSizes[@]}"
do
	for threadCount in "${threadCounts[@]}"
	do
		./bsort -b $tableSize $threadCount
		./bsort -s $tableSize $threadCount
	done
done
#./bsort -b 10000000 1

