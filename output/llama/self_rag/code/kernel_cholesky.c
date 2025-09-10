#include <math.h>

void kernel_cholesky(int n, float A[120][120]) {
  int i;
  int j;
  int k;

  #pragma HLS dataflow

  for (i = 0; i < 120; i++) {
    #pragma HLS loop_tripcount min=1 max=120
    #pragma HLS pipeline II=1

    for (j = 0; j < i; j++) {
      #pragma HLS loop_tripcount min=1 max=i
      #pragma HLS pipeline II=1

      for (k = 0; k < j; k++) {
        #pragma HLS pipeline II=1
        A[i][j] -= A[i][k] * A[j][k];
      }
      #pragma HLS pipeline II=1
      A[i][j] /= A[j][j];
    }
    #pragma HLS pipeline II=1

    // i==j case
    for (k = 0; k < i; k++) {
      #pragma HLS pipeline II=1
      A[i][i] -= A[i][k] * A[i][k];
    }
    #pragma HLS pipeline II=1
    A[i][i] = sqrt(A[i][i]);
  }
}