#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "Timer.h"
#include "Timer.cpp"

const int OUTPUT = 1; // Đặt thành 1 nếu muốn ghi mảng đã sắp xếp vào file
const char* inputFile = "input-array.txt";  // Tên file đầu vào
const char* outputFile = "output-quick-sort-mpi.txt"; // Tên file đầu ra

void quick_sort(int* array, int left, int right) {
    int i = left, j = right;
    int pivot = array[(left + right) / 2];
    while (i <= j) {
        while (array[i] < pivot) i++;
        while (array[j] > pivot) j--;
        if (i <= j) {
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i++;
            j--;
        }
    }
    if (left < j) quick_sort(array, left, j);
    if (i < right) quick_sort(array, i, right);
}

int output(int *p, int n){
	FILE *f=fopen(outputFile, "wt");
	int i;
	for(i=0; i<n; i++)
		fprintf(f, "%i\n", p[i]);
	fclose(f);
	return 0;
}

int main(int argc, char** argv) {
    int rank, size, n;
    int* array = NULL;
    int* sub_array = NULL;
    int local_n;
    Timer ti;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        FILE *f = fopen(inputFile, "r");
        if (!f) {
            puts("Could not open the input file.");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            return 0;
        }

        fscanf(f, "%d", &n);  // Đọc số phần tử từ file
        array = (int*)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            fscanf(f, "%d", &array[i]);  // Đọc từng phần tử vào mảng
        }
        fclose(f);
    }

    // Phát số lượng phần tử cho tất cả các tiến trình
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Tính số phần tử cho mỗi tiến trình
    local_n = n / size;
    sub_array = (int*)malloc(local_n * sizeof(int));

    // Chia dữ liệu cho mỗi tiến trình
    MPI_Scatter(array, local_n, MPI_INT, sub_array, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Sắp xếp mảng con của mỗi tiến trình
    quick_sort(sub_array, 0, local_n - 1);

    // Thu thập kết quả về tiến trình 0
    MPI_Gather(sub_array, local_n, MPI_INT, array, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Tiến trình 0 sắp xếp lại toàn bộ mảng
    if (rank == 0) {
        quick_sort(array, 0, n - 1);
        // Ghi kết quả vào file nếu OUTPUT = 1
        if (OUTPUT) {
            double y = ti.getElapsedTime();
		    printf("Time 4 core: %.5f\n", y);
            output(array, n);
        }
        free(array);
    }

    free(sub_array);
    MPI_Finalize();
    return 0;
}
