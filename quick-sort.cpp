#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include "Timer.h"
#include "Timer.cpp"

int printArray(int*, int);
int output(int*, int);
int quickSort(int*, int, int);
int swap(int*, int, int);

int main(void) {
    Timer ti;
    int n = 0;

    // Mở file và đọc dữ liệu
    FILE *f = fopen("input-array.txt", "r");
    if (!f) {
        puts("Could not open the file.");
        return 0;
    }

    fscanf(f, "%d", &n);  // Đọc số lượng phần tử
    int *a = (int*)malloc(n * sizeof(int));
    for (size_t i = 0; i < n; i++) {
        fscanf(f, "%d", a + i);  // Đọc các phần tử vào mảng
    }
    fclose(f);

    // Sắp xếp mảng bằng Quick Sort
    quickSort(a, 0, n - 1);

    double y = ti.getElapsedTime();
    printf("Time 1 core (Quick Sort): %.5f\n", y);

    // Ghi kết quả vào file
    output(a, n);
    free(a);
    return 0;
}

// Hàm in mảng (nếu cần kiểm tra)
int printArray(int *p, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", p[i]);
    }
    puts("");
    return 0;
}

// Hàm xuất mảng đã sắp xếp vào file
int output(int *p, int n) {
    FILE *f = fopen("output-quick-sort.txt", "wt");
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d\n", p[i]);
    }
    fclose(f);
    return 0;
}

// Hàm Quick Sort
int quickSort(int *a, int left, int right) {
    if (left < right) {
        int pivot = a[(left + right) / 2];
        int i = left, j = right;

        // Phân vùng xung quanh pivot
        while (i <= j) {
            while (a[i] < pivot) i++;
            while (a[j] > pivot) j--;
            if (i <= j) {
                swap(a, i, j);
                i++;
                j--;
            }
        }

        // Đệ quy sắp xếp hai phần
        quickSort(a, left, j);
        quickSort(a, i, right);
    }
    return 0;
}

// Hàm hoán đổi vị trí
int swap(int *a, int i, int j) {
    int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
    return 0;
}
