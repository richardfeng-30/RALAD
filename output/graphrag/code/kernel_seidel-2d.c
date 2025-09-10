void kernel_seidel_2d(int tsteps, int n, float A[120][120]) {
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem0
#pragma HLS INTERFACE s_axilite port=tsteps bundle=control
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=A bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS ARRAY_PARTITION variable=A complete dim=2
#pragma HLS ARRAY_STENCIL variable=A

  int t, i, j;

  // Provide tripcount pragmas to help optimization
#pragma HLS LOOP_TRIPCOUNT min=40 max=40
  for (t = 0; t < tsteps; t++) {
#pragma HLS LOOP_TRIPCOUNT min=118 max=118
    for (i = 1; i < n - 1; i++) {
#pragma HLS LOOP_TRIPCOUNT min=118 max=118
#pragma HLS PIPELINE II=1
      for (j = 1; j < n - 1; j++) {
        A[i][j] = (A[i - 1][j - 1] + A[i - 1][j] + A[i - 1][j + 1] +
                   A[i][j - 1] + A[i][j] + A[i][j + 1] +
                   A[i + 1][j - 1] + A[i + 1][j] + A[i + 1][j + 1]) / 9.0f;
      }
    }
  }
}