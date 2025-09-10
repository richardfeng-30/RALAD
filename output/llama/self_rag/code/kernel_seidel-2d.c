void kernel_seidel_2d(int tsteps, int n, float A[120][120]) {
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE s_axli port=n offset=slave
  #pragma HLS INTERFACE s_axli port=tsteps offset=slave
  #pragma HLS INTERFACE s_axli port=return offset=slave

  int t;
  int i;
  int j;

  #pragma HLS DATAFLOW
  #pragma HLS LOOP_TRIPCOUNT min=41 max=41
  for (t = 0; t <= 40 - 1; t++) {
    #pragma HLS LOOP_TRIPCOUNT min=119 max=119
    for (i = 1; i <= 120 - 2; i++) {
      #pragma HLS LOOP_TRIPCOUNT min=119 max=119
      for (j = 1; j <= 120 - 2; j++) {
        A[i][j] = (A[i - 1][j - 1] + A[i - 1][j] + A[i - 1][j + 1] + A[i][j - 1] + A[i][j] + A[i][j + 1] + A[i + 1][j - 1] + A[i + 1][j] + A[i + 1][j + 1]) / 9.0;
      }
    }
  }
}