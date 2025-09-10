void kernel_seidel_2d(int tsteps, int n, float A[120][120]) {
  #pragma HLS INTERFACE m_axi port=A[0] depth=14400
  #pragma HLS INTERFACE s_axilite port=tsteps
  #pragma HLS INTERFACE s_axilite port=n
  #pragma HLS INTERFACE s_axilite port=A

  int t;
  int i;
  int j;
  #pragma HLS DATAFLOW

  for (t = 0; t <= 40 - 1; t++) {
    #pragma HLS LOOP_TRIPCOUNT min=1 max=40
    #pragma HLS UNROLL factor=4

    for (i = 1; i <= 120 - 2; i++) {
      #pragma HLS LOOP_TRIPCOUNT min=1 max=119
      #pragma HLS UNROLL factor=4

      for (j = 1; j <= 120 - 2; j++) {
        #pragma HLS LOOP_TRIPCOUNT min=1 max=119
        #pragma HLS UNROLL factor=4

        A[i][j] = (A[i - 1][j - 1] + A[i - 1][j] + A[i - 1][j + 1] + A[i][j - 1] + A[i][j] + A[i][j + 1] + A[i + 1][j - 1] + A[i + 1][j] + A[i + 1][j + 1]) / 9.0;
      }
    }
  }
}