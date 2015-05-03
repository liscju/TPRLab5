#!/bin/bash
gcc -fopenmp -o bsort bucketsort.c

tableSizes=(100000 1000000 10000000 100000000)
threadCounts=(1 2 4 5 6 7 8 9 10 11 12)

for tableSize in "${tableSizes[@]}"
do
	for threadCount in "${threadCounts[@]}"
	do
		./bsort -b $tableSize $threadCount
		./bsort -s $tableSize $threadCount
	done
done
#./bsort -b 10000000 1
