/* matrix summation using OpenMP

   usage with gcc (version 4.2 or higher required):
     gcc -O -fopenmp -o matrixSum-openmp matrixSum-openmp.c
     ./matrixSum-openmp size numWorkers

*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

double start_time, end_time;
#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 8   /* maximum number of workers */

int numWorkers;
int size;
int matrix[MAXSIZE][MAXSIZE];

typedef struct {
  long value;
  long i;
  long j;
} Index;

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  int i, j, total=0;

  Index globalMax, globalMin;

  globalMax.value = LONG_MIN;
  globalMin.value = LONG_MAX;

  int max = INT_MIN;
  int min = INT_MAX;

  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;

  omp_set_num_threads(numWorkers);

  /* initialize the matrix */
  srand(time(NULL));
  for (i = 0; i < size; i++) {
	  for (j = 0; j < size; j++) {
      matrix[i][j] = rand()%99;
	  }
  }

  #ifdef DEBUG
  for(i = 0; i < size; i++){
    printf("[");
    for(j = 0; j < size; j++){
      printf("%*d", 4, matrix[i][j]);
    }
    printf("]\n");
  }
  #endif

  start_time = omp_get_wtime();
#pragma omp parallel for reduction (+:total), reduction(max:max), reduction(min:min) private(j)
  for (i = 0; i < size; i++)
    for (j = 0; j < size; j++){
      total += matrix[i][j];

      if(matrix[i][j] >= max){
        max = matrix[i][j];
        #pragma omp critical
        {
          if(globalMax.value < max){
            globalMax.value = max;
            globalMax.i = i;
            globalMax.j = j;
          }
        }
      }
      if(matrix[i][j] < min){
        min = matrix[i][j];
        #pragma omp critical
        {
          if(globalMin.value > min){
            globalMin.value = min;
            globalMin.i = i;
            globalMin.j = j;
          }
        }
      }
      if(omp_get_thread_num() == 0 && )
    }
    printf("I am: %d\n", omp_get_thread_num());
    printf("the total is %d\n", total);
    printf("Max: %ld Index: [%ld, %ld]\n", globalMax.value, globalMax.i, globalMax.j);
    printf("Min: %ld Index: [%ld, %ld]\n", globalMin.value, globalMin.i, globalMin.j);
    printf("it took %g seconds\n", end_time - start_time);
  //implicit barrier
  end_time = omp_get_wtime();

}
