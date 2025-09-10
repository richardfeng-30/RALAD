#include <math.h>

void kernel_cholesky(int n, float A[120][120]) {
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  // Partition the second dimension of A completely to allow parallel access to columns
#pragma HLS ARRAY_PARTITION variable=A complete dim=2

  int i, j, k;

  for (i = 0; i < 120; i++) {
    // Pipeline the j loop to improve throughput
    for (j = 0; j < i; j++) {
#pragma HLS PIPELINE II=1
      // Pipeline the k loop with II=1 to allow concurrent multiply-accumulate
      for (k = 0; k < j; k++) {
#pragma HLS PIPELINE II=1
        A[i][j] -= A[i][k] * A[j][k];
      }
      A[i][j] /= A[j][j];
    }

    // Pipeline the k loop for diagonal element update
    for (k = 0; k < i; k++) {
#pragma HLS PIPELINE II=1
      A[i][i] -= A[i][k] * A[i][k];
    }
    A[i][i] = sqrtf(A[i][i]);
  }
}