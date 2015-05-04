// C++ program to sort an array using bucket sort
#include <iostream>
#include <algorithm>
#include <vector>
#include <omp.h>
#include <string.h>
#include <assert.h>

void initialize_cmd_arguments(int argc, char **argv);
void bucket_sort_init(int argc,char **argv);
void usage(char *program_name);
void generate_array();
void divide_points_to_buckets();
void initialize_buckets();
void sort_points();
void print_sorted_points();
void initialize_output_array();
void check_if_output_array_sorted();

void print_SpentTime(double time);

using namespace std;

#define CONCURRENT_BASIC 0
#define CONCURRENT_SCALING 1

int size_of_array;
int bucket_count;
double *array_to_sort;
int *buckets;
double *sorted_array;
int concurrency_type;

void usage(char *program_name) {
	cout << "Usage:\n ./" << program_name << " [--basic|--scaling] size_of_array bucket_count" << endl;
	exit(0);
}

void initialize_cmd_arguments(int argc, char **argv) {
	if (argc != 4)
		usage(argv[0]);

	size_of_array = atoi(argv[2]);
	bucket_count = atoi(argv[3]);

	if (strcmp(argv[1],"--basic") == 0) {
		concurrency_type = CONCURRENT_BASIC;
	} else if (strcmp(argv[1],"--scaling") == 0) {
		concurrency_type = CONCURRENT_SCALING;

		size_of_array *= bucket_count;
	} else
		usage(argv[0]);
}

void generate_array() {
	srand(static_cast <unsigned> (time(0)) );
	array_to_sort = new double[size_of_array];
	for (int i = 0; i < size_of_array; ++i) {
		array_to_sort[i] = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
	}
}

void print_array() {
	for (int i = 0; i < size_of_array; ++i) {
		cout << "tab[" << i << "]=" << array_to_sort[i] << endl;
	}
}

void print_buckets() {
	for (int j = 0; j < bucket_count; ++j) {
		cout << "bucket for " << j << " = {";
		for (int i = 0; i < size_of_array; ++i) {
			if (buckets[i] == j) {
				cout << array_to_sort[i] << ",";
			}
		}
		cout << "}" << endl;
	}
}

void bucket_sort_init(int argc,char **argv) {
	initialize_cmd_arguments(argc,argv);
	generate_array();
	//print_array();

	double spentTime = omp_get_wtime();
	divide_points_to_buckets();
	//print_buckets();
	initialize_output_array();
	sort_points();
	//print_sorted_points();
	spentTime = omp_get_wtime() - spentTime;

	print_SpentTime(spentTime);

	check_if_output_array_sorted();
}

void print_SpentTime(double time) {
	if (concurrency_type == CONCURRENT_BASIC) {
		cout << "T_BASIC[" << size_of_array;
	} else {
		assert(concurrency_type == CONCURRENT_SCALING);
		cout << "T_SCALE[" << size_of_array / bucket_count;
	}
	cout << ", " << bucket_count;
	cout << "]=" << time << endl;
}

void check_if_output_array_sorted() {
	for (int i = 1; i < size_of_array; ++i) {
		if (sorted_array[i] < sorted_array[i - 1]) {
			cout << "sorted_array[" << i << "] < sorted_array[" << i-1 << "]" << endl;
		}
	}
}

void initialize_output_array() {
	sorted_array = new double[size_of_array];
}

void print_sorted_points() {
	for (int i = 0; i < size_of_array; ++i) {
		cout << "sorted_array[" << i << "]=" << sorted_array[i] << endl;
	}
}

void sort_points() {
	omp_set_dynamic(0);
	omp_set_num_threads(bucket_count);
#pragma omp parallel
	{
		int threadnum = omp_get_thread_num();
		//cout << "Sorted thread:" << threadnum << endl;

		double *own_array = new double[size_of_array];
		int own_size = 0;

		int start_offset = 0;

		for (int i = 0; i < size_of_array; ++i) {
			if (buckets[i] == threadnum)
				own_array[own_size++] = array_to_sort[i];
			else if (buckets[i] < threadnum)
				start_offset++;
		}

		sort(own_array,own_array+own_size);

		for (int j = 0; j < own_size; ++j) {
			sorted_array[start_offset+j] = own_array[j];
		}
	}
}

void divide_points_to_buckets() {
	initialize_buckets();

	omp_set_dynamic(0);
	omp_set_num_threads(bucket_count);
#pragma omp parallel for
	for (int i = 0; i < size_of_array; ++i) {
		int bucket_to_insert = static_cast<int>(array_to_sort[i]*bucket_count);
		buckets[i] = bucket_to_insert;
		//cout << "Thread:" << omp_get_thread_num() << " took " << i <<" element" << endl;
	}
}

void initialize_buckets() {
	buckets = new int[size_of_array];
}


/* Driver program to test above funtion */
int main(int argc,char **argv) {
	bucket_sort_init(argc,argv);
}