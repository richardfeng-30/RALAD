Here is the optimized C code with HLS pragmas:

```c
void kernel_ludcmp(int n, float A[120][120], float b[120], float x[120], float y[120]) {
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=b offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=x offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=y offset=slave bundle=gmem

  int i;
  int j;
  int k;
  float w;

  // Partial Pivoting
  #pragma HLS DATAFLOW

  // Gaussian Elimination
  for (i = 0; i < 120; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=1 max=120
    #pragma HLS PIPELINE II=1

    for (j = 0; j < i; j++) {
      w = A[i][j];
      #pragma HLS UNROLL
      for (k = 0; k < j; k++) {
        w -= A[i][k] * A[k][j];
      }
      A[i][j] = w / A[j][j];
    }

    for (j = i; j < 120; j++) {
      w = A[i][j];
      #pragma HLS UNROLL
      for (k = 0; k < i; k++) {
        w -= A[i][k] * A[k][j];
      }
      A[i][j] = w;
    }
  }

  // Forward Substitution
  for (i = 0; i < 120; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=1 max=120
    #pragma HLS PIPELINE II=1

    w = b[i];
    #pragma HLS UNROLL
    for (j = 0; j < i; j++) {
      w -= A[i][j] * y[j];
    }
    y[i] = w;
  }

  // Back Substitution
  for (i = 119; i >= 0; i--) {
    #pragma HLS LOOP_TRIPCOUNT min=1 max=120
    #pragma HLS PIPELINE II=1

    w = y[i];
    #pragma HLS UNROLL
    for (j = i + 1; j < 120; j++) {
      w -= A[i][j] * x[j];
    }
    x[i] = w / A[i][i];
  }
}
```

In this code, I have added the following HLS pragmas:

1.  `#pragma HLS INTERFACE m_axi` pragmas to specify the memory interfaces for the arrays A, b, x, and y.
2.  `#pragma HLS DATAFLOW` to indicate that the code is a dataflow program.
3.  `#pragma HLS LOOP_TRIPCOUNT` pragmas to specify the minimum and maximum number of iterations for each loop.
4.  `#pragma HLS PIPELINE II=1` pragmas to specify a pipeline with a latency of 1 clock cycle for each loop.
5.  `#pragma HLS UNROLL` pragmas to unroll the inner loops to improve performance.

These pragmas can help improve the performance of the code by reducing the number of memory accesses, increasing the parallelism, and improving the pipelining.