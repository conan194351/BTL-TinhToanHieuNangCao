#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <mpi.h>
#include "Timer.h"
#include "Timer.cpp"

// change this constant to 1 to write sorted array to file.
const int OUTPUT = 0;

//name of input file
char fi[] = "input-array.txt";

// name of output file
char fo[] = "output-merge-sort-mpi.txt";


int printArray(int*, int);
int output(int*, int);
int merge(int*, int, int, int);
int mergeSort(int*, int, int);
int swap(int*, int, int);

int main(int argc, char **argv){
	Timer ti;
	int rank, size;
	MPI_Init(&argc, &argv);
	int mpirank = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int mpisize = MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status stat;

	int start = 0;
	int finish = 0;
	int n = 0;
	int *a;

	FILE *f = fopen(fi, "r");
	if (!f){
		puts("Could not open the file.");
		MPI_Finalize();
		return 0;
	}

	int fs1 = fscanf(f, "%d", &n);
	int range = n / size;
	// Core 0 read data
	if (rank == 0){
		a = (int*) malloc(n * sizeof(int));
		for (size_t i = 0; i < n; i++) {
			int fs2 = fscanf(f, "%d", a + i);
		}
	}

	int *ac = (int*) calloc(range, sizeof(int));
	fclose(f);

	// Core 0 sends data to other cores
	MPI_Scatter(a, range, MPI_INT, ac, range, MPI_INT, 0, MPI_COMM_WORLD);

	// sort ac
	mergeSort(ac, 0, range - 1);

	// Send result back to Core 0
	MPI_Gather(ac, range, MPI_INT, a, range, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) {

		// sort the original array the final time.

		int parts = size;
		while (parts > 1){
			for (int i = 0; i < parts; i += 2) {
				int left = i * (int) (n / parts);
				int mid = left + (int) (n / parts) - 1;
				int right = mid + (int) (n / parts);
				merge(a, left, mid, right);
			}
			parts = parts >> 1;
		}
	}

	MPI_Finalize();

	if (rank == 0){
		double y = ti.getElapsedTime();
		printf("Time 4 core: %.5f\n", y);
		// printArray(a,n);
		output(a, n);
		free(a);
	}
	return 0;
}

int printArray(int *p, int n){
	int i;
	for(i = 0; i < n; i++)
		printf("%i\n", p[i]);
	puts("");
	return 0;
}

int output(int *p, int n){
	FILE *f=fopen(fo, "wt");
	int i;
	for(i=0; i<n; i++)
		fprintf(f, "%i\n", p[i]);
	fclose(f);
	return 0;
}

int mergeSort(int *a, int left, int right){
	if (left < right){
		int mid = (left + right) / 2;
		mergeSort(a, left, mid);
		mergeSort(a, mid + 1, right);
		merge(a, left, mid, right);
	}
	return 0;
}

int merge(int *a, int left, int mid, int right){
	int count = right - left + 1;
	int *tmp = (int*) calloc(count, sizeof(int));
	int i = left;
	int j = mid + 1;
	int k;

	for(k = 0; k < count; k++){
		if (i > mid){
			tmp[k] = a[j];
			j++;
		}
		else if (j > right){
			tmp[k] = a[i];
			i++;
		}
		else if (a[i] < a[j]){
			tmp[k] = a[i];
			i++;
		}
		else{
			tmp[k] = a[j];
			j++;
		}
	}
	for(k = 0; k < count; k++)
		a[left + k] = tmp[k];
	free(tmp);
}

int swap(int *a, int i, int j){
	int tmp=a[i];
	a[i]=a[j];
	a[j]=tmp;
	return 0;
}