void kernel_jacobi_2d(int tsteps, int n, float A[90][90], float B[90][90]) {
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
  #pragma HLS INTERFACE s_axilite port=tsteps
  #pragma HLS INTERFACE s_axilite port=n
  #pragma HLS INTERFACE s_axilite port=A
  #pragma HLS INTERFACE s_axilite port=B

  #pragma HLS DATAFLOW

  int t;
  int i;
  int j;

  for (t = 0; t < 40; t++) {
    #pragma HLS LOOP_TRIPCOUNT min=40 max=40
    #pragma HLS UNROLL factor=1

    for (i = 1; i < 90 - 1; i++) {
      #pragma HLS LOOP_TRIPCOUNT min=88 max=88
      #pragma HLS UNROLL factor=1

      for (j = 1; j < 90 - 1; j++) {
        #pragma HLS LOOP_TRIPCOUNT min=88 max=88
        #pragma HLS UNROLL factor=1

        B[i][j] = 0.2 * (A[i][j] + A[i][j - 1] + A[i][1 + j] + A[1 + i][j] + A[i - 1][j]);
      }
    }

    for (i = 1; i < 90 - 1; i++) {
      #pragma HLS LOOP_TRIPCOUNT min=88 max=88
      #pragma HLS UNROLL factor=1

      for (j = 1; j < 90 - 1; j++) {
        #pragma HLS LOOP_TRIPCOUNT min=88 max=88
        #pragma HLS UNROLL factor=1

        A[i][j] = 0.2 * (B[i][j] + B[i][j - 1] + B[i][1 + j] + B[1 + i][j] + B[i - 1][j]);
      }
    }
  }
}