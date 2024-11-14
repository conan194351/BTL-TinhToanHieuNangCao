#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include "Timer.h"
#include "Timer.cpp"

int printArray(int*, int);
int output(int*, int);
int merge(int*, int, int, int);
int mergeSort(int*, int, int);
int swap(int*, int, int);

int main(void){
	Timer ti;
	int i;
	int n = 0;
	FILE *f = fopen("input-array.txt", "r");
	if (!f){
		puts("Could not open the file.");
		return 0;
	}

	int fs1 = fscanf(f, "%d", &n);
	int *a = (int*) malloc(n * sizeof(int));
	for (size_t i = 0; i < n; i++) {
		int fs2 = fscanf(f, "%d", a + i);
	}
	mergeSort(a, 0, n-1);
	// printArray(a, n);
	double y = ti.getElapsedTime();
	printf("Time 1 core: %.5f\n", y);
	output(a, n);
	free(a);
	return 0;
}

int printArray(int *p, int n){
	int i;
	for(i=0; i<n; i++)
		printf("%i\n", p[i]);
	puts("");
	return 0;
}

int output(int *p, int n){
	FILE *f=fopen("output-merge-sort.txt", "wt");
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