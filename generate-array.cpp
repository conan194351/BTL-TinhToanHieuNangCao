#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv){
	srand(time(NULL));
	int n = atoi(argv[1]);
	FILE *f = fopen("input-array.txt", "w");
	if (!f){
		puts("Could not generate input.");
		return 0;
	}
	fprintf(f, "%d\n", n);
	for (size_t i = 0; i < n; i++) {
		fprintf(f, "%d\n", rand());
	}
	fclose(f);
	return 0;
}