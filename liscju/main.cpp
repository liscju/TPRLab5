// C++ program to sort an array using bucket sort
#include <iostream>
#include <algorithm>
#include <vector>
#include <omp.h>

void initialize_cmd_arguments(int argc, char **argv);
void bucket_sort_init(int argc,char **argv);
void usage(char *program_name);

using namespace std;

int size_of_array;

void usage(char *program_name) {
	printf("Usage:\n ./%s size_of_array",program_name);
	exit(0);
}

void initialize_cmd_arguments(int argc, char **argv) {
	if (argc == 1)
		usage(argv[0]);

	size_of_array = atoi(argv[1]);
}

void bucket_sort_init(int argc,char **argv) {
	initialize_cmd_arguments(argc,argv);
	printf("Size of array = %d",size_of_array);
}


/* Driver program to test above funtion */
int main(int argc,char **argv) {
/*
	omp_set_num_threads(3);
	#pragma omp parallel for
	for (int i=0;i<10;i++) {
		printf("Thread id: %d iter: %d\n", omp_get_thread_num(), i);
	}*/

	bucket_sort_init(argc,argv);
}

