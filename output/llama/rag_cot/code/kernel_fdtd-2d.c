To optimize the given code, we will consider the following HLS optimizations:

1.  **Dataflow**: The dataflow directive can be used to specify a dataflow architecture for the kernel. This is particularly useful for algorithms that have a clear input-output dependency graph.

2.  **Pipeline**: The pipeline directive can be used to specify that a loop should be pipelined. This is particularly useful for algorithms that have a high degree of parallelism and a clear input-output dependency graph.

3.  **Unroll**: The unroll directive can be used to specify that a loop should be unrolled. This is particularly useful for algorithms that have a high degree of parallelism and a clear input-output dependency graph.

Here is the optimized C code with pragma HLS:

```c
void kernel_fdtd_2d(int tmax,int nx,int ny,float ex[60][80],float ey[60][80],float hz[60][80],float _fict_[40])
{
  #pragma HLS dataflow
  int t;
  int i;
  int j;
  {
    
    // Pipeline the outer loop
    #pragma HLS pipeline II=1
    for (t = 0; t < 40; t++) {
      
      // Pipeline the inner loops
      #pragma HLS pipeline II=1
      for (j = 0; j < 80; j++) {
        ey[0][j] = _fict_[t];
      }
      
      
      
      for (i = 1; i < 60; i++) {
        
        #pragma HLS unroll
        for (j = 0; j < 80; j++) {
          ey[i][j] = ey[i][j] - 0.5 * (hz[i][j] - hz[i - 1][j]);
        }
      }
      
      
      
      for (i = 0; i < 60; i++) {
        
        #pragma HLS unroll
        for (j = 1; j < 80; j++) {
          ex[i][j] = ex[i][j] - 0.5 * (hz[i][j] - hz[i][j - 1]);
        }
      }
      
      
      
      for (i = 0; i < 60 - 1; i++) {
        
        #pragma HLS unroll
        for (j = 0; j < 80 - 1; j++) {
          hz[i][j] = hz[i][j] - 0.7 * (ex[i][j + 1] - ex[i][j] + ey[i + 1][j] - ey[i][j]);
        }
      }
    }
  }
}
```

In this optimized code, we have used the pipeline directive to pipeline the outer loop and the inner loops. We have also used the unroll directive to unroll the inner loops. This should improve the performance of the kernel by reducing the latency and increasing the throughput.