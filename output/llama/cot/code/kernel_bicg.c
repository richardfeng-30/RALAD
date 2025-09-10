#include <hls_math.h>
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=s offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=q offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=p offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=r offset=slave bundle=gmem
#pragma HLS INTERFACE s_axli port=m
#pragma HLS INTERFACE s_axli port=n
#pragma HLS INTERFACE s_axli port=return

void kernel_bicg(int m, int n, float A[124][116], float s[116], float q[124], float p[116], float r[124])
{
  int i;
  int j;
  #pragma HLS loop_tripcount min=1 max=124
  #pragma HLS loop_tripcount min=1 max=116
  
  // Initialize s and q arrays
  #pragma HLS loop_unroll factor=4
  for (i = 0; i < 116; i++) {
    s[i] = ((float )0);
  }
  
  // Main computation
  #pragma HLS loop_unroll factor=4
  for (i = 0; i < 124; i++) {
    q[i] = 0.0;
    
    #pragma HLS loop_unroll factor=4
    for (j = 0; j < 116; j++) {
      s[j] = s[j] + r[i] * A[i][j];
      q[i] = q[i] + A[i][j] * p[j];
    }
  }
}