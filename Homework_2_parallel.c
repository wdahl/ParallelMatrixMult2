#include <stdio.h>
#include <stdlib.h>
#include "timing.h"
#include <pthread.h>

//global varibales
double **matrix, **result;
int matrixSize, threadCount;

//prints a matrix for testing
void print_mat(double **m){
  int i,j, N=matrixSize;
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      printf("%lf\t", m[i][j]);
    }
    printf("\n");
  }
}

//adds two matrices
double **add(double **A, double **B, int n){
  int i, j;
  double **C = (double **)malloc(n * sizeof(double *));  

  for(i=0;i<n;i++){
    C[i] = (double *)malloc(n * sizeof(double));
    for(j=0;j<n;j++){
      C[i][j] = A[i][j] + B[i][j];
    }
  }

  return C;
}

//subtacts two matricies
double **sub(double **A, double **B, int n){
  int i, j;
  double **C = (double **)malloc(n * sizeof(double *));  

  for(i=0;i<n;i++){
    C[i] = (double *)malloc(n * sizeof(double));
    for(j=0;j<n;j++){
      C[i][j] = A[i][j] - B[i][j];
    }
  }

  return C;
}

//perfomrs matrix multiplication on the matrices passed to the method
void *matmul(void *arg){
  int *rows = (int *)arg;
  int i, j, k;
  for(i=rows[0]; i<=rows[1]; i++){
    for(j=0; j<matrixSize; j++){
      for(k=0; k<matrixSize; k++){
        result[i][j] = result[i][j] + (matrix[i][k]*matrix[k][j]);
      }
    }
  }
}

int main(int argc, char* argv[]){
  if(argc != 2){
    printf("%s: Usage: %s <number of threads>\n", argv[0], argv[0]);
    exit(-1);
  }
  threadCount = atoi(argv[1]);
  FILE *fp = fopen("inputMatrix.csv", "r");
  char matrixSize_str[10];
  int i=0, j=0, k=0;
  char c;
  char entry_str[25];
  double entry;
  fgets(matrixSize_str, 10, fp);
  matrixSize = atoi(matrixSize_str);
  pthread_t threads[threadCount];
  int range;
  int **rows = (int **)malloc(threadCount * sizeof(int *));

  matrix = (double **)malloc(matrixSize * sizeof(double *));
  result = (double **)malloc(matrixSize * sizeof(double *));
  for(i=0; i<matrixSize; i++){
    matrix[i] = (double *)malloc(matrixSize * sizeof(double));
    result[i] = (double *)malloc(matrixSize * sizeof(double));
  }

  i=0;

  c = fgetc(fp);
  while(c != EOF){
    if(c == ','){
      sscanf(entry_str, "%lf", &entry);
      matrix[i][j] = entry;
      j++;
      entry_str[0] = '\0';
      k=0;
    }
    else if(c == '\n'){
      sscanf(entry_str, "%lf", &entry);
      matrix[i][j] = entry;
      i++;
      j=0;
      entry_str[0] = '\0';
      k=0;
    }
    else{
      entry_str[k] = c;
      k++;
    }
    c = fgetc(fp);
  }
  
  if(matrixSize % threadCount == 0){
    range = matrixSize / threadCount;
  }
  else{
    range = (matrixSize / threadCount) + 1;
  }

  for(i=0; i<threadCount; i++){
    rows[i] = (int *)malloc(2 * sizeof(int));
  }

  i=0, j=0, k=0;

  timing_start();
  while(i<matrixSize){
    if(k == threadCount-1){
      rows[k][1] = matrixSize-1;
      break;
    }
    else if(j == range-1){
      j=0;
      rows[k][1] = i;
      pthread_create(&threads[k], NULL, matmul, (void *)rows[k]);
      k++;
      i++;
      rows[k][0]=i;
    }
    else{
      i++;
      j++;
    }
  }

  matmul((void *)rows[k]);  

  for(i=0; i<threadCount-1; i++){
    pthread_join(threads[i], NULL);
  }
  timing_stop();
  print_timing();

  char comma[2];
  fp = fopen("outputMatrix_parallel.csv", "w");
  fprintf(fp, "%d\n", matrixSize);
  for(i=0;i<matrixSize;i++)
  {
    sprintf(comma, "%s", "");
    for(j=0;j<matrixSize;j++)
    {
      fprintf(fp, "%s%lf",comma,result[i][j]);
      sprintf(comma, "%s", ",");
    }
    
    fprintf(fp, "\n");
  }

  fclose(fp);
  return 0;
}