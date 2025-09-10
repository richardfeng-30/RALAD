Here's the optimized C code with HLS pragmas:

```c
void kernel_mvt(int n, float x1[120], float x2[120], float y_1[120], float y_2[120], float A[120][120])
{
  #pragma HLS interface m_axi port=x1[0] bundle=gmem
  #pragma HLS interface m_axi port=x2[0] bundle=gmem
  #pragma HLS interface m_axi port=y_1[0] bundle=gmem
  #pragma HLS interface m_axi port=y_2[0] bundle=gmem
  #pragma HLS interface m_axi port=A[0] bundle=gmem
  
  #pragma HLS dataflow
  
  #pragma HLS pipeline II=1
  #pragma HLS pipeline enable_reset
  
  int i;
  int j;
  
  #pragma HLS loop_tripcount min=120 max=120
  #pragma HLS loop_bounds c=0 e=119
  
  for (i = 0; i < 120; i++) {
    #pragma HLS loop_tripcount min=120 max=120
    #pragma HLS loop_bounds c=0 e=119
    
    float sum = 0.0;
    
    #pragma HLS loop_pipelineable
    for (j = 0; j < 120; j++) {
      sum += A[i][j] * y_1[j];
    }
    
    x1[i] = sum;
  }
  
  #pragma HLS loop_tripcount min=120 max=120
  #pragma HLS loop_bounds c=0 e=119
  
  for (i = 0; i < 120; i++) {
    #pragma HLS loop_tripcount min=120 max=120
    #pragma HLS loop_bounds c=0 e=119
    
    float sum = 0.0;
    
    #pragma HLS loop_pipelineable
    for (j = 0; j < 120; j++) {
      sum += A[j][i] * y_2[j];
    }
    
    x2[i] = sum;
  }
}
```

Here's the reasoning behind the HLS pragmas:

1.  We use `#pragma HLS interface` to specify the memory interface for each array. This allows the compiler to optimize the memory access and reduce the memory bandwidth.
2.  We use `#pragma HLS dataflow` to specify that the function is a dataflow program. This allows the compiler to optimize the dataflow and reduce the number of memory accesses.
3.  We use `#pragma HLS pipeline` to specify that the loops are pipelined. This allows the compiler to optimize the pipeline and reduce the latency.
4.  We use `#pragma HLS loop_tripcount` and `#pragma HLS loop_bounds` to specify the trip count and bounds for each loop. This allows the compiler to optimize the loop and reduce the number of iterations.
5.  We use `#pragma HLS loop_pipelineable` to specify that the inner loop is pipelineable. This allows the compiler to optimize the pipeline and reduce the latency.

By using these HLS pragmas, we can optimize the code for performance and reduce the memory bandwidth.