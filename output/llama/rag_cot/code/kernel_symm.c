Here are the optimized C code with HLS pragmas for each of the examples:

Example 1:
```c
void kernel_symm(int m,int n,float alpha,float beta,float C[60][80],float A[60][60],float B[60][80])
{
  #pragma HLS INTERFACE m_axi port=C offset=slave
  #pragma HLS INTERFACE m_axi port=A offset=slave
  #pragma HLS INTERFACE m_axi port=B offset=slave
  #pragma HLS ARRAY_PARTITION variable=C cyclic factor=10
  #pragma HLS ARRAY_PARTITION variable=A cyclic factor=10
  #pragma HLS ARRAY_PARTITION variable=B cyclic factor=10

  float temp2;
  int i;
  int j;
  int k;
  
  for (i = 0; i < 60; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=60 max=60
    #pragma HLS PIPELINE II=1
    for (j = 0; j < 80; j++) {
      temp2 = ((float)0);
      for (k = 0; k < i; k++) {
        #pragma HLS LOOP_TRIPCOUNT min=1 max=59
        C[k][j] += alpha * B[i][j] * A[i][k];
        temp2 += B[k][j] * A[i][k];
      }
      C[i][j] = beta * C[i][j] + alpha * B[i][j] * A[i][i] + alpha * temp2;
    }
  }
}
```

Example 2:
```c
void matrixmul(int A[N][M], int B[M][P], int AB[N][P])
{
  #pragma HLS ARRAY RESHAPE variable=A complete dim=2
  #pragma HLS ARRAY RESHAPE variable=B complete dim=1
  #pragma HLS INTERFACE m_axi port=A offset=slave
  #pragma HLS INTERFACE m_axi port=B offset=slave
  #pragma HLS INTERFACE m_axi port=AB offset=slave

  row: for(int i = 0; i < N; ++i) {
    col: for(int j = 0; j < P; ++j) {
      #pragma HLS PIPELINE II=1
      #pragma HLS LOOP_TRIPCOUNT min=1 max=80
      int sum = 0;
      for(int k = 0; k < M; ++k) {
        sum += A[i][k] * B[k][j];
      }
      AB[i][j] = sum;
    }
  }
}
```

Example 3:
```c
void kernel_symm(int m,int n,float alpha,float beta,float C[60][80],float A[60][60],float B[60][80])
{
  #pragma HLS ARRAY_PARTITION variable=C cyclic factor=10
  #pragma HLS ARRAY_PARTITION variable=A cyclic factor=10
  #pragma HLS ARRAY_PARTITION variable=B cyclic factor=10
  #pragma HLS INTERFACE m_axi port=C offset=slave
  #pragma HLS INTERFACE m_axi port=A offset=slave
  #pragma HLS INTERFACE m_axi port=B offset=slave

  float temp2;
  int i;
  int j;
  int k;
  
  for (i = 0; i < 60; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=60 max=60
    for (j = 0; j < 80; j++) {
      temp2 = ((float)0);
      for (k = 0; k < i; k++) {
        #pragma HLS LOOP_TRIPCOUNT min=1 max=59
        C[k][j] += alpha * B[i][j] * A[i][k];
        temp2 += B[k][j] * A[i][k];
      }
      C[i][j] = beta * C[i][j] + alpha * B[i][j] * A[i][i] + alpha * temp2;
    }
  }
}
```

Explanation:

1.  Example 1: In this example, the code is optimized for memory access and data transfer. The `#pragma HLS ARRAY_PARTITION` directive is used to partition the arrays into smaller chunks, which can be accessed in parallel. The `#pragma HLS INTERFACE m_axi` directive is used to specify the memory interfaces for the arrays.
2.  Example 2: In this example, the code is optimized for data transfer and computation. The `#pragma HLS PIPELINE` directive is used to specify a pipeline of computations, which can be executed in parallel. The `#pragma HLS LOOP_TRIPCOUNT` directive is used to specify the minimum and maximum trip count for each loop.
3.  Example 3: In this example, the code is optimized for memory access and data transfer. The `#pragma HLS ARRAY_PARTITION` directive is used to partition the arrays into smaller chunks, which can be accessed in parallel. The `#pragma HLS INTERFACE m_axi` directive is used to specify the memory interfaces for the arrays.

In general, the HLS pragmas are used to guide the compiler to generate more efficient code by specifying the memory interfaces, data transfer, and computation. The `#pragma HLS ARRAY_PARTITION` directive is used to partition arrays into smaller chunks, which can be accessed in parallel. The `#pragma HLS PIPELINE` directive is used to specify a pipeline of computations, which can be executed in parallel. The `#pragma HLS LOOP_TRIPCOUNT` directive is used to specify the minimum and maximum trip count for each loop.