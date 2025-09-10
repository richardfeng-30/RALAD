void kernel_jacobi_2d(int tsteps, int n, float A[90][90], float B[90][90]) {
#pragma HLS ARRAY_PARTITION variable=A complete dim=2
#pragma HLS ARRAY_PARTITION variable=B complete dim=2

  int t, i, j;

  for (t = 0; t < 40; t++) {
    for (i = 1; i < 90 - 1; i++) {
#pragma HLS PIPELINE II=1
      for (j = 1; j < 90 - 1; j++) {
#pragma HLS UNROLL factor=4
        B[i][j] = 0.2 * (A[i][j] + A[i][j - 1] + A[i][j + 1] + A[i + 1][j] + A[i - 1][j]);
      }
    }

    for (i = 1; i < 90 - 1; i++) {
#pragma HLS PIPELINE II=1
      for (j = 1; j < 90 - 1; j++) {
#pragma HLS UNROLL factor=4
        A[i][j] = 0.2 * (B[i][j] + B[i][j - 1] + B[i][j + 1] + B[i + 1][j] + B[i - 1][j]);
      }
    }
  }
}