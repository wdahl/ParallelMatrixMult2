#include <stdio.h>
#include <stdlib.h>
#include "timing.h"

//global varibales
double **a, **b, **c, **d, **p1, **p2, **p3, **p4, **p5, **p6, **p7;

//prints a matrix fir testing
void print_mat(double **m, int N){
  int i,j;
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
double **matmul_naive(double **A, double **B, int n){
  int i, j, k;//indices of the entries in the matrices

  //Allocates memory for result matrix
  double **C = (double **)malloc(n * sizeof(double *));  

  //loops through matrices and perfomrs matrix multiplication
  for(i=0; i<n; i++){
    C[i] = (double *)malloc(n * sizeof(double));
    for(j=0; j<n; j++){
      for(k=0; k<n; k++){
        C[i][j] = C[i][j] + (A[i][k]*B[k][j]);
      }
    }
  }

  return C;//returns resulting matrix
}

//performs matrix multiplication using strassens algorithm.
double **matmul_optimum(double **A, double** B, int n){
  int i, j, k;

  //Allocates the sub-matrices
  a = (double **)malloc(n/2 * sizeof(double *));
  b = (double **)malloc(n/2 * sizeof(double *));
  c = (double **)malloc(n/2 * sizeof(double *));
  d = (double **)malloc(n/2 * sizeof(double *));
  for(i=0; i<n; i++){
    a[i] = (double *)malloc(n/2 * sizeof(double));
    b[i] = (double *)malloc(n/2 * sizeof(double));
    c[i] = (double *)malloc(n/2 * sizeof(double));
    d[i] = (double *)malloc(n/2 * sizeof(double));
  }

 //intilazes the sub matrices
  for (i=0; i<n/2; i++){
    for (j=0; j<n/2; j++){
      a[i][j] = A[i][j];
    }
  }

  for (i=0; i<n/2; i++){
    for (j=n/2; j<n; j++){
      b[i][j-(n/2)] = A[i][j];
    }
  }

  for(i=n/2; i<n; i++){
    for(j=0; j<n/2; j++){
      c[i-(n/2)][j] = A[i][j];
    }
  }

  for(i=n/2; i<n; i++){
    for(j=n/2; j<n; j++){
      d[i-(n/2)][j-(n/2)] = A[i][j];
    }
  }

  //strassens algorithm
  p1 = matmul_naive(add(a,d,n/2), add(a,d,n/2), n/2);
  p2 = matmul_naive(add(c,d,n/2), a, n/2);
  p3 = matmul_naive(a, sub(b,d,n/2) ,n/2);
  p4 = matmul_naive(d, sub(c,a,n/2), n/2);
  p5 = matmul_naive(add(a,b,n/2), d, n/2);
  p6 = matmul_naive(sub(c,a,n/2), add(a,b,n/2), n/2);
  p7 = matmul_naive(sub(b,d,n/2), add(c,d,n/2), n/2);

  //makes the sub-matrices of the result matrix
  a = add(sub(add(p1,p4,n/2),p5,n/2),p7,n/2);
  b = add(p3,p5,n/2);
  c = add(p2,p4,n/2); 
  d = add(add(sub(p1,p2,n/2),p3,n/2),p6,n/2);

  //intilazes the sub matrices
  for (i=0; i<n/2; i++){
    for (j=0; j<n/2; j++){
      A[i][j] = a[i][j];
    }
  }

  for (i=0; i<n/2; i++){
    for (j=n/2; j<n; j++){
      A[i][j] = b[i][j-(n/2)];
    }
  }

  for(i=n/2; i<n; i++){
    for(j=0; j<n/2; j++){
      A[i][j] = c[i-(n/2)][j];
    }
  }

  for(i=n/2; i<n; i++){
    for(j=n/2; j<n; j++){
      A[i][j] = d[i-(n/2)][j-(n/2)];
    }
  }

  return A;
}

int main(){
  FILE *fp = fopen("inputMatrix.csv", "r");
  char matrixSize_str[10];
  int i=0, j=0, k=0;
  char c;
  char entry_str[25];
  double entry;
  fgets(matrixSize_str, 10, fp);
  int matrixSize = atoi(matrixSize_str);

  double **matrix = (double **)malloc(matrixSize * sizeof(double *));
  for(i=0; i<matrixSize; i++){
    matrix[i] = (double *)malloc(matrixSize * sizeof(double));
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

  timing_start();
  if(matrixSize % 2 == 0){
    matrix = matmul_optimum(matrix, matrix, matrixSize);
  }
  else{
    matrix = matmul_naive(matrix, matrix, matrixSize);
  }
  timing_stop();
  print_timing();

  char comma[2];
  fp = fopen("outputMatrix_serial.csv", "w");
  fprintf(fp, "%d\n", matrixSize);
  for(i=0;i<matrixSize;i++)
  {
    sprintf(comma, "%s", "");
    for(j=0;j<matrixSize;j++)
    {
      fprintf(fp, "%s%lf",comma,matrix[i][j]);
      sprintf(comma, "%s", ",");
    }
    
    fprintf(fp, "\n");
  }

  fclose(fp);
  return 0;
}