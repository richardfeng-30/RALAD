void kernel_lu(int n, float A[120][120]) {
#pragma HLS array_partition variable=A complete dim=2
  int i, j, k;

  for (i = 0; i < 120; i++) {
    // Pipeline the outer j loop to improve throughput
    for (j = 0; j < i; j++) {
#pragma HLS pipeline II=1
      for (k = 0; k < j; k++) {
#pragma HLS unroll factor=4
        A[i][j] -= A[i][k] * A[k][j];
      }
      A[i][j] /= A[j][j];
    }

    for (j = i; j < 120; j++) {
#pragma HLS pipeline II=1
      for (k = 0; k < i; k++) {
#pragma HLS unroll factor=4
        A[i][j] -= A[i][k] * A[k][j];
      }
    }
  }
}