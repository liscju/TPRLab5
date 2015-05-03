// C++ program to sort an array using bucket sort
#include <iostream>
#include <algorithm>
#include <vector>
#include <omp.h>

void initialize_cmd_arguments(int argc, char **argv);
void bucket_sort_init(int argc,char **argv);
void usage(char *program_name);

void generate_array();

void divide_points_to_buckets();

void initialize_buckets();

using namespace std;

int size_of_array;
int bucket_count;
float *array_to_sort;
vector<float> *buckets;

void usage(char *program_name) {
	printf("Usage:\n ./%s size_of_array bucket_count",program_name);
	exit(0);
}

void initialize_cmd_arguments(int argc, char **argv) {
	if (argc != 3)
		usage(argv[0]);

	size_of_array = atoi(argv[1]);
	bucket_count = atoi(argv[2]);
}

void generate_array() {
	srand(static_cast <unsigned> (time(0)) );
	array_to_sort = new float[size_of_array];
	for (int i = 0; i < size_of_array; ++i) {
		array_to_sort[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	}
}

void print_array() {
	for (int i = 0; i < size_of_array; ++i) {
		cout << "tab[" << i << "]=" << array_to_sort[i] << endl;
	}
}

void print_buckets() {
	for (int i = 0; i < bucket_count; ++i) {
		cout << "bucket[" << i << "]={";
		for (auto iter = buckets[i].begin(); iter != buckets[i].end(); ++iter) {
			cout << (*iter) << ",";
		}
		cout << "}" << endl;
	}
}

void bucket_sort_init(int argc,char **argv) {
	initialize_cmd_arguments(argc,argv);
	generate_array();
	print_array();
	divide_points_to_buckets();
	print_buckets();
}

void divide_points_to_buckets() {
	initialize_buckets();


	for (int i = 0; i < size_of_array; ++i) {
		int bucket_to_insert = static_cast<int>(array_to_sort[i]*bucket_count);
		buckets[bucket_to_insert].push_back(array_to_sort[i]);
	}
}

void initialize_buckets() {
	buckets = new vector<float>[bucket_count];
	for (int i = 0; i < bucket_count; ++i) {
		buckets[i] = vector<float>();
	}
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


// Function to sort arr[] of size n using bucket sort
void bucketSort(float arr[], int n)
{
	// 1) Create n empty buckets
	vector<float> b[n];

	// 2) Put array elements in different buckets
	for (int i=0; i<n; i++)
	{
		int bi = n*arr[i]; // Index in bucket
		b[bi].push_back(arr[i]);
	}

	// 3) Sort individual buckets
	for (int i=0; i<n; i++)
		sort(b[i].begin(), b[i].end());

	// 4) Concatenate all buckets into arr[]
	int index = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < b[i].size(); j++)
			arr[index++] = b[i][j];
}

