#include <math.h>

void kernel_cholesky(int n, float A[120][120]) {
  #pragma HLS INTERFACE m_axi port=A[0] depth=120
  #pragma HLS INTERFACE s_axilite port=n
  #pragma HLS INTERFACE s_axilite port=A

  int i;
  int j;
  int k;

  #pragma HLS DATAFLOW

  #pragma HLS LOOP_TRIPCOUNT min=1 max=120
  for (i = 0; i < 120; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=1 max=i
    for (j = 0; j < i; j++) {
      #pragma HLS LOOP_TRIPCOUNT min=1 max=j
      for (k = 0; k < j; k++) {
        A[i][j] -= A[i][k] * A[j][k];
      }
      A[i][j] /= A[j][j];
    }
    // i==j case
    for (k = 0; k < i; k++) {
      A[i][i] -= A[i][k] * A[i][k];
    }
    A[i][i] = sqrt(A[i][i]);
  }
}