// compile: gcc -fopenmp -o bsort bucketsort.c 

// set the number of threads via the environment variable
// OMP_NUM_THREADS, e.g. in the C shell

// setenv OMP_NUM_THREADS 8

#include <omp.h>	// required

#include <stdlib.h>
#include <stdio.h>

#define TYPE_BASIC 0
#define TYPE_SCALING 1

typedef long long int llint;

/* globals */
int type;
llint tableSize;
int numberOfThreads;
int* table;
double spentTime;
FILE* timeFile;
/* end globals  */


/* helper function declarations */
void initParams(int, char**);
void initTable();
void printTable(int*, llint);
void checkIfSorted(int*, llint);

int int_cmp(const void*, const void*);
void grab(int, int*, llint*);
void findminmax(int*, llint, int*, int*);
void bsort(int*, llint);

void openFiles();
void closeFiles();
/* end helper function declarations */


int main(int argc, char** argv) {
	if(argc != 4) {
		fprintf(stderr, "Wrong arguments. Usage: %s <-b(asic)/-s(caling)> <table-size> <number-of-threads>\n", argv[0]);
		return EXIT_FAILURE;
	}
	initParams(argc, argv);
	
	openFiles();

	spentTime = omp_get_wtime();	

	initTable();

	if(tableSize < 100) {
		printf("x before sorting:\n");
		printTable(table, tableSize);
	}
	
	bsort(table, tableSize);

	checkIfSorted(table, tableSize);
	
	if(tableSize < 100) {
		printf("x after sorting:\n");
		printTable(table, tableSize);
	}

	spentTime = omp_get_wtime() - spentTime;

	fprintf(timeFile, "%d %lld %f\n", numberOfThreads, tableSize, spentTime);	

	closeFiles();
}


/* helper function decfinitions */
void initParams(int argc, char** argv) {
	if(strcmp(argv[1], "-s") == 0) {
		type = TYPE_SCALING;
	} else {
		type = TYPE_BASIC;
	}
	tableSize = atoll(argv[2]);
	numberOfThreads = atoi(argv[3]);

	omp_set_dynamic(0);     				// Explicitly disable dynamic teams
	omp_set_num_threads(numberOfThreads);	// Set number of threads
}

void initTable() {
	table = malloc(tableSize*sizeof(int));
	llint i;
	for(i=0; i<tableSize; i++) {
		table[i] = rand()%tableSize;
	}
}

void printTable(int* tab, llint tabSize) {
	llint i;
	for(i=0; i<tabSize; i++) {
		printf("%d ", tab[i]);
	}
	printf("\n");
}

void checkIfSorted(int* tab, llint tabSize) {
	int sorted = 1;
	llint i=1;
	while(sorted && i<tabSize) {
		if(tab[i-1] > tab[i]) {
			sorted = 0;
		}
		i++;
	}
	printf("(%d, %lld) ", numberOfThreads, tableSize);
	if(sorted) {
		printf("Table has been sorted.\n");
	} else {
		printf("Table has NOT been sorted.\n");
	}
	
}

// needed for call to qsort()
int int_cmp(const void *a, const void *b) { 
    const int* ia = (const int*)a; // casting pointer types 
    const int* ib = (const int*)b;
    return *ia  - *ib; 
	/* integer comparison: returns negative if b > a 
	and positive if a > b */ 
}

// adds xi to the part array, increments npart, the length of part
void grab(int xi, int* part, llint* npart) {
	part[*npart] = xi;
	*npart += 1;
}

// finds the min and max in y, length ny, placing them in miny and maxy
void findminmax(int* y, llint ny, int* miny, int* maxy) {
	llint i, yi;
	*miny = *maxy = y[0];
	for(i=1; i<ny; i++) {
		yi = i[y];
		if(yi < *miny) *miny = yi;
		else if(yi > *maxy) *maxy = yi;
	}
}

// sort the array x of length n
void bsort(int* x, llint n) {
	// these are local to this function, but shared among the threads
	float* bdries; int* counts;
	#pragma omp parallel
	// entering this block activates the threads, each executing it
	{
		// variables declared below are local to each thread
		int me = omp_get_thread_num();
		// have to do the next call within the block, while the threads are active
		int nth = omp_get_num_threads();
		llint i;
		int minx, maxx, start;
		int* mypart;
		float increm;
		int SAMPLESIZE;
		// now determine the bucket boundaries; nth -1 of them, by sampling the array to get an idea of its range
		#pragma omp single	// only 1 thread does this, implied barrier at end
		{
			if(n > 1000) SAMPLESIZE = 1000;
			else SAMPLESIZE = n/2;
			findminmax(x, SAMPLESIZE, &minx, &maxx);
			bdries = malloc((nth-1)*sizeof(float));
			increm = (maxx-minx) / (float)nth;
			for(i=0; i<nth-1; i++) {
				bdries[i] = minx + (i+1)*increm;
				// array to serve as count of the numbers of elements of x in each bucket
			}
			counts = malloc(nth*sizeof(int));
		}
		// now have this thread grab its portion of the array; thread 0 takes everything below bdries[0], thread 1 everything between bdries[0] and bdries[1], etc., with thread nth-1 taking everything over bdries[nth-1]
		mypart = malloc(n*sizeof(int)); 
		llint nummypart = 0;
		for(i=0; i<n; i++) {
			if(me == 0) {
				if(x[i] <= bdries[0]) {
					grab(x[i], mypart, &nummypart);
				}
			} else if(me < nth-1) {
				if(x[i] > bdries[me-1] && x[i] <= bdries[me]) {
					grab(x[i], mypart, &nummypart);
				}
			} else {
				if(x[i] > bdries[me-1]) {
					grab(x[i], mypart, &nummypart);
				}
			}
		}
		// now record how many this thread got
		counts[me] = nummypart;
		
		// sort my part
		qsort(mypart, nummypart, sizeof(int), int_cmp);
		#pragma omp barrier	//other threads need to know all of counts
		// copy sorted chunk back to the original array; first find start point
		start = 0;
		for(i=0; i<me; i++) {
			start += counts[i];
		}
		for(i=0; i<nummypart; i++) {
			x[start+i] = mypart[i];
		}
	}
	// implied barrier here; main thread won't resume until all threads are done
}

void openFiles() {
	char filenameBuffer[100];
	char* typeString;
	if(type == TYPE_SCALING) {
		typeString = "scaling";
	} else {
		typeString = "basic";
	}
	sprintf(filenameBuffer, "%s_%lld.txt", typeString, tableSize);
	timeFile = fopen(filenameBuffer, "a+");
}

void closeFiles() {
	fclose(timeFile);
}
/* end helper function decfinitions */
