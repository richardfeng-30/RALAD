To optimize the given code, we need to consider the following HLS pragmas:

1.  ARRAY RESHAPE: This directive is used to reshape arrays in the memory space to optimize memory access. We can apply this directive to the arrays `A`, `B`, and `C` to reduce memory bank conflicts and improve memory access.
2.  PIPELINE: This directive is used to pipeline the loops to improve the throughput of the kernel. We can apply this directive to the inner loops to pipeline the operations and improve the performance.
3.  UNROLL: This directive is used to unroll the loops to reduce the overhead of loop control statements. We can apply this directive to the inner loops to unroll the operations and improve the performance.

Here's the optimized code with HLS pragmas:

```c
void kernel_gemm(int ni,int nj,int nk,float alpha,float beta,float C[60][70],float A[60][80],float B[80][70])
{
  #pragma HLS ARRAY RESHAPE variable=A complete dim=2
  #pragma HLS ARRAY RESHAPE variable=B complete dim=1
  #pragma HLS ARRAY RESHAPE variable=C complete dim=2
  
  int i;
  int j;
  int k;
  
  //BLAS PARAMS
  //TRANSA = 'N'
  //TRANSB = 'N'
  // => Form C := alpha*A*B + beta*C,
  //A is NIxNK
  //B is NKxNJ
  //C is NIxNJ
  
  {
    for (i = 0; i < 60; i++) {
      
      #pragma HLS PIPELINE II=1
      for (j = 0; j < 70; j++) {
        C[i][j] *= beta;
      }
      
      
      
      for (k = 0; k < 80; k++) {
        
        #pragma HLS UNROLL factor=8
        for (j = 0; j < 70; j++) {
          C[i][j] += alpha * A[i][k] * B[k][j];
        }
      }
    }
  }
}
```

In this optimized code, we have applied the following HLS pragmas:

*   `#pragma HLS ARRAY RESHAPE variable=A complete dim=2` to reshape the array `A` to reduce memory bank conflicts and improve memory access.
*   `#pragma HLS ARRAY RESHAPE variable=B complete dim=1` to reshape the array `B` to reduce memory bank conflicts and improve memory access.
*   `#pragma HLS ARRAY RESHAPE variable=C complete dim=2` to reshape the array `C` to reduce memory bank conflicts and improve memory access.
*   `#pragma HLS PIPELINE II=1` to pipeline the inner loop to improve the throughput of the kernel.
*   `#pragma HLS UNROLL factor=8` to unroll the inner loop to reduce the overhead of loop control statements and improve the performance.

These HLS pragmas will help to optimize the performance of the kernel by reducing memory bank conflicts, improving memory access, and improving the throughput of the kernel.