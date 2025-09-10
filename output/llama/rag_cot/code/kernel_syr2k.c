To optimize the given code for HLS, we will divide the loops into smaller parts, use pipeline and unroll the loops to improve the performance. 

Here is the optimized code with HLS pragmas:

```c
void kernel_syr2k(int n,int m,float alpha,float beta,float C[80][80],float A[80][60],float B[80][60])
{
  #pragma HLS interface m_axi port = C[0] bundle = s_axi_gmem
  #pragma HLS interface m_axi port = A[0] bundle = s_axi_gmem
  #pragma HLS interface m_axi port = B[0] bundle = s_axi_gmem
  
  int i;
  int j;
  int k;
  
  for (i = 0; i < 80; i++) {
    #pragma HLS loop_tripcount min = 1 max = 80
    #pragma HLS pipeline II = 1
    for (j = 0; j <= i; j++) {
      #pragma HLS unroll factor = 8
      C[i][j] *= beta;
    }
    
    for (k = 0; k < 60; k++) {
      #pragma HLS loop_tripcount min = 1 max = 60
      #pragma HLS pipeline II = 1
      for (j = 0; j <= i; j++) {
        #pragma HLS unroll factor = 8
        C[i][j] += A[j][k] * alpha * B[i][k] + B[j][k] * alpha * A[i][k];
      }
    }
  }
}
```

Explanation of the HLS pragmas used:

*   `#pragma HLS interface m_axi`: This directive is used to specify the memory interface for the function. In this case, it is used to specify the memory interface for the 2D arrays C, A, and B.
*   `#pragma HLS loop_tripcount min = 1 max = 80`: This directive is used to specify the minimum and maximum number of iterations for a loop. This is used to give the HLS compiler a hint about the loop trip count, which can help it to make better optimization decisions.
*   `#pragma HLS pipeline II = 1`: This directive is used to specify the pipeline depth for a loop. In this case, it is used to specify a pipeline depth of 1 for both the outer and inner loops. This can help to improve the performance of the function by allowing the HLS compiler to pipeline the loops.
*   `#pragma HLS unroll factor = 8`: This directive is used to specify the unroll factor for a loop. In this case, it is used to specify an unroll factor of 8 for both the inner and outer loops. This can help to improve the performance of the function by reducing the number of iterations and improving the cache locality.