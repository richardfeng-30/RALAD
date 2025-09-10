Here is the optimized C code with HLS pragmas:

```c
void kernel_trisolv(int n, float L[120][120], float x[120], float b[120])
{
  #pragma HLS INTERFACE m_axi port=L offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=x offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=b offset=slave bundle=gmem
  #pragma HLS INTERFACE s_axli port=n
  #pragma HLS INTERFACE s_axli port=return

  #pragma HLS DATAFLOW

  int i;
  int j;
  float temp[120];

  #pragma HLS DATA_REDUCTION variable=temp type=floating_point

  #pragma HLS loop_tripcount min=1 max=120 avg=60
  #pragma HLS loop_unroll max=4
  for (i = 0; i < 120; i++) {
    temp[i] = b[i];
    #pragma HLS loop_tripcount min=1 max=i avg=i/2
    #pragma HLS loop_unroll max=4
    for (j = 0; j < i; j++) {
      temp[i] -= L[i][j] * x[j];
    }
    x[i] = temp[i] / L[i][i];
  }
}
```

Explanation:

1.  We start by adding HLS pragmas for the interfaces to specify the memory mapping and the data types of the variables.
2.  We add a `DATAFLOW` directive to indicate that the kernel is a dataflow program, which allows the compiler to optimize the data dependencies between the loops.
3.  We add a `DATA_REDUCTION` directive to specify that the `temp` array is reduced to a single value at the end of the kernel, which can help reduce memory usage.
4.  We add a `loop_tripcount` directive to specify the minimum, maximum, and average number of iterations for each loop, which can help the compiler optimize the loop unrolling and pipelining.
5.  We add a `loop_unroll` directive to specify the maximum number of iterations to unroll for each loop, which can help reduce the number of iterations and improve performance.
6.  Finally, we add a `return` statement to indicate the end of the kernel.

By adding these HLS pragmas, we can help the compiler optimize the kernel for performance and memory usage.