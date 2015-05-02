// C++ program to sort an array using bucket sort
#include <iostream>
#include <algorithm>
#include <vector>
#include <omp.h>

int bucketMain();

void initialize_cmd_arguments(int argc, char **argv);

using namespace std;
/*
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

int bucketSortMain() {
	float arr[] = {0.897, 0.565, 0.656, 0.1234, 0.665, 0.3434};
	int n = sizeof(arr)/sizeof(arr[0]);
	bucketSort(arr, n);

	cout << "Sorted array is \n";
	for (int i=0; i<n; i++)
		cout << arr[i] << " ";
	return 0;
}*/

void usage() {
	printf("./a.out");
	exit(0);
}

void initialize_cmd_arguments(int argc, char **argv) {

}

void bucket_sort_init(int argc,char **argv) {
	initialize_cmd_arguments(argc,argv);
}


/* Driver program to test above funtion */
int main(int argc,char **argv) {

	omp_set_num_threads(3);
	#pragma omp parallel for
	for (int i=0;i<10;i++) {
		printf("Thread id: %d iter: %d\n", omp_get_thread_num(), i);
	}
}

