void kernel_heat_3d(int tsteps, int n, float A[20][20][20], float B[20][20][20]) {
  int t;
  int i;
  int j;
  int k;

  #pragma HLS INTERFACE m_axi port=A offset=slave
  #pragma HLS INTERFACE m_axi port=B offset=slave

  for (t = 1; t <= 40; t++) {
    #pragma HLS PIPELINE II=1

    for (i = 1; i < 20 - 1; i++) {
      #pragma HLS UNROLL factor=4
      for (j = 1; j < 20 - 1; j++) {
        #pragma HLS UNROLL factor=4
        for (k = 1; k < 20 - 1; k++) {
          #pragma HLS DEPENDENCE variable=B[i][j][k] false
          B[i][j][k] = 0.125 * (A[i + 1][j][k] - 2.0 * A[i][j][k] + A[i - 1][j][k]) + 0.125 * (A[i][j + 1][k] - 2.0 * A[i][j][k] + A[i][j - 1][k]) + 0.125 * (A[i][j][k + 1] - 2.0 * A[i][j][k] + A[i][j][k - 1]) + A[i][j][k];
        }
      }
    }

    #pragma HLS PIPELINE II=1

    for (i = 1; i < 20 - 1; i++) {
      #pragma HLS UNROLL factor=4
      for (j = 1; j < 20 - 1; j++) {
        #pragma HLS UNROLL factor=4
        for (k = 1; k < 20 - 1; k++) {
          #pragma HLS DEPENDENCE variable=A[i][j][k] false
          A[i][j][k] = 0.125 * (B[i + 1][j][k] - 2.0 * B[i][j][k] + B[i - 1][j][k]) + 0.125 * (B[i][j + 1][k] - 2.0 * B[i][j][k] + B[i][j - 1][k]) + 0.125 * (B[i][j][k + 1] - 2.0 * B[i][j][k] + B[i][j][k - 1]) + B[i][j][k];
        }
      }
    }
  }
}