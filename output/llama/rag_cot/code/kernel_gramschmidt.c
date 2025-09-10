Here is the optimized C code with HLS pragmas:

```c
#include <math.h>

void kernel_gramschmidt(int m,int n,float A[60][80],float R[80][80],float Q[60][80])
{
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=R offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=Q offset=slave bundle=gmem

  int i;
  int j;
  int k;
  float nrm;
  
  #pragma HLS dataflow
  for (k = 0; k < 80; k++) {
    #pragma HLS loop_tripcount min=1 max=80
    nrm = 0.0;
    
    #pragma HLS pipeline II=1
    for (i = 0; i < 60; i++) {
      #pragma HLS loop_tripcount min=1 max=60
      nrm += A[i][k] * A[i][k];
    }
    R[k][k] = sqrt(nrm);
    
    #pragma HLS pipeline II=1
    for (i = 0; i < 60; i++) {
      #pragma HLS loop_tripcount min=1 max=60
      Q[i][k] = A[i][k] / R[k][k];
    }
    
    #pragma HLS pipeline II=1
    for (j = k + 1; j < 80; j++) {
      #pragma HLS loop_tripcount min=1 max=79-k
      R[k][j] = 0.0;
      
      #pragma HLS pipeline II=1
      for (i = 0; i < 60; i++) {
        #pragma HLS loop_tripcount min=1 max=60
        R[k][j] += Q[i][k] * A[i][j];
      }
      
      #pragma HLS pipeline II=1
      for (i = 0; i < 60; i++) {
        #pragma HLS loop_tripcount min=1 max=60
        A[i][j] = A[i][j] - Q[i][k] * R[k][j];
      }
    }
  }
}
```

In this optimized code, I have added the following HLS pragmas:

1. `#pragma HLS INTERFACE m_axi`: This pragma is used to specify the interface for the AXI memory-mapped slave interface. It is used to connect the kernel to the memory.

2. `#pragma HLS dataflow`: This pragma is used to specify that the kernel is a dataflow kernel. This means that the kernel will execute in a dataflow manner, where each operation is executed as soon as its input is available.

3. `#pragma HLS loop_tripcount min=1 max=80`: This pragma is used to specify the trip count of a loop. In this case, the trip count is specified for the outer loop, which iterates over the rows of the matrix.

4. `#pragma HLS pipeline II=1`: This pragma is used to specify the pipeline depth for a loop. In this case, the pipeline depth is specified for each of the inner loops, which iterate over the columns of the matrix.

These pragmas are used to optimize the performance of the kernel by specifying the interface, dataflow, and pipeline depth. The `#pragma HLS loop_tripcount` and `#pragma HLS pipeline II=1` pragmas are used to specify the trip count and pipeline depth for each loop, which can help to improve the performance of the kernel by reducing the number of iterations and improving the pipeline efficiency.