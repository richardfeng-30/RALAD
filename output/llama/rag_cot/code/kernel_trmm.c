To optimize the given code, we can use the following HLS pragmas:

1.  `#pragma HLS ARRAY RESHAPE`: This directive can be used to reshape the arrays to improve memory access and reduce memory usage.

2.  `#pragma HLS LOOP FLATTEN`: This directive can be used to flatten the loops and reduce the number of loop iterations.

3.  `#pragma HLS PIPELINE`: This directive can be used to create a pipeline to improve the performance of the kernel.

Here is the optimized code:

```c
void kernel_trmm(int m,int n,float alpha,float A[60][60],float B[60][80])
{
  #pragma HLS ARRAY RESHAPE variable=A complete dim=2
  #pragma HLS ARRAY RESHAPE variable=B complete dim=2

  int i;
  int j;
  int k;

  for (i = 0; i < 60; i++) {
    #pragma HLS LOOP_FLATTEN
    for (j = 0; j < 80; j++) {
      #pragma HLS LOOP_FLATTEN
      for (k = i + 1; k < 60; k++) {
        #pragma HLS PIPELINE II=1
        B[i][j] += A[k][i] * B[k][j];
      }
      #pragma HLS PIPELINE II=1
      B[i][j] = alpha * B[i][j];
    }
  }
}
```

The `#pragma HLS ARRAY RESHAPE` directives are used to reshape the arrays to improve memory access and reduce memory usage.

The `#pragma HLS LOOP_FLATTEN` directives are used to flatten the loops and reduce the number of loop iterations.

The `#pragma HLS PIPELINE` directives are used to create a pipeline to improve the performance of the kernel. The `II=1` argument specifies the pipeline initiation interval, which is the number of clock cycles between the initiation of each iteration.

The above code can be further optimized by using the `#pragma HLS DATAFLOW` directive to enable dataflow processing, which can improve the performance of the kernel by reducing the number of memory accesses and improving the data locality. However, this requires the use of the Vivado HLS compiler and the creation of a dataflow design.

Here is the code with `#pragma HLS DATAFLOW`:

```c
void kernel_trmm(int m,int n,float alpha,float A[60][60],float B[60][80])
{
  #pragma HLS ARRAY RESHAPE variable=A complete dim=2
  #pragma HLS ARRAY RESHAPE variable=B complete dim=2

  int i;
  int j;
  int k;

  #pragma HLS DATAFLOW

  for (i = 0; i < 60; i++) {
    for (j = 0; j < 80; j++) {
      for (k = i + 1; k < 60; k++) {
        #pragma HLS PIPELINE II=1
        B[i][j] += A[k][i] * B[k][j];
      }
      #pragma HLS PIPELINE II=1
      B[i][j] = alpha * B[i][j];
    }
  }
}
```

The `#pragma HLS DATAFLOW` directive enables dataflow processing, which can improve the performance of the kernel by reducing the number of memory accesses and improving the data locality.