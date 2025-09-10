Here is the optimized C code with HLS pragmas:

```c
void kernel_covariance(int m, int n, float float_n, float data[100][80], float cov[80][80], float mean[80]) {
  #pragma HLS interface m_axi port=data[0] dim=2
  #pragma HLS interface m_axi port=cov[0] dim=2
  #pragma HLS interface m_axi port=mean[0] dim=1
  
  int i;
  int j;
  int k;
  
  // Calculate mean
  #pragma HLS dataflow
  for (j = 0; j < 80; j++) {
    #pragma HLS loop_tripcount 80
    mean[j] = 0.0;
    #pragma HLS pipeline
    for (i = 0; i < 100; i++) {
      #pragma HLS loop_tripcount 100
      mean[j] += data[i][j];
    }
    mean[j] /= float_n;
  }
  
  // Calculate variance
  #pragma HLS dataflow
  for (i = 0; i < 100; i++) {
    #pragma HLS loop_tripcount 100
    for (j = 0; j < 80; j++) {
      #pragma HLS loop_tripcount 80
      data[i][j] -= mean[j];
    }
  }
  
  // Calculate covariance
  #pragma HLS dataflow
  for (i = 0; i < 80; i++) {
    #pragma HLS loop_tripcount 80
    for (j = i; j < 80; j++) {
      #pragma HLS loop_tripcount 80
      cov[i][j] = 0.0;
      #pragma HLS pipeline
      for (k = 0; k < 100; k++) {
        #pragma HLS loop_tripcount 100
        cov[i][j] += data[k][i] * data[k][j];
      }
      cov[i][j] /= float_n - 1.0;
      cov[j][i] = cov[i][j];
    }
  }
}
```

Here's a step-by-step explanation of the HLS optimizations:

1.  The first step is to identify the loops and data dependencies in the code. In this case, the loops are nested, and there are data dependencies between the innermost loops.
2.  To optimize the code, we can use the `#pragma HLS loop_tripcount` directive to specify the number of iterations for each loop. This helps the compiler to generate more efficient code.
3.  We can also use the `#pragma HLS pipeline` directive to specify that the innermost loops should be pipelined. This allows the compiler to generate code that can execute multiple iterations of the loop in parallel.
4.  To optimize memory access, we can use the `#pragma HLS interface m_axi` directive to specify the memory interface for the arrays. This helps the compiler to generate code that can access the arrays in a more efficient way.
5.  Finally, we can use the `#pragma HLS dataflow` directive to specify that the code should be executed as a dataflow program. This allows the compiler to generate code that can execute the loops in parallel and optimize memory access.

By applying these HLS optimizations, we can significantly improve the performance of the code.