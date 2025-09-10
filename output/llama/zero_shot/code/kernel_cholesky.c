#include <math.h>

#pragma HLS INTERFACE m_axi port=A[0] dimension=2
#pragma HLS INTERFACE s_axilite port=n
#pragma HLS INTERFACE s_axilite port=return

void kernel_cholesky(int n,float A[120][120])
{
  int i;
  int j;
  int k;
  #pragma HLS DATAFLOW
  #pragma HLS ARRAY_PARTITION variable=A cyclic factor=10

  for (i = 0; i < 120; i++) {
    #pragma HLS LOOP_FLATTEN
    for (j = 0; j < i; j++) {
      #pragma HLS LOOP_FLATTEN
      for (k = 0; k < j; k++) {
        #pragma HLS PIPELINE II=1
        A[i][j] -= A[i][k] * A[j][k];
      }
      #pragma HLS PIPELINE II=1
      A[i][j] /= A[j][j];
    }
    // i==j case
    for (k = 0; k < i; k++) {
      #pragma HLS PIPELINE II=1
      A[i][i] -= A[i][k] * A[i][k];
    }
    #pragma HLS PIPELINE II=1
    A[i][i] = sqrt(A[i][i]);
  }
}