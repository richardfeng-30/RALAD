void kernel_heat_3d(int tsteps, int n, float A[20][20][20], float B[20][20][20]) {
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=tsteps bundle=control
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=A bundle=control
#pragma HLS INTERFACE s_axilite port=B bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  // Apply array stencil pragma to optimize stencil accesses
#pragma HLS array_stencil variable=A
#pragma HLS array_stencil variable=B

  // Loop over time steps
  for (int t = 1; t <= 40; t++) {
#pragma HLS loop_tripcount min=40 max=40

    // Loop over i dimension
    for (int i = 1; i < 20 - 1; i++) {
#pragma HLS loop_tripcount min=18 max=18

      // Loop over j dimension
      for (int j = 1; j < 20 - 1; j++) {
#pragma HLS loop_tripcount min=18 max=18

        // Innermost loop over k dimension
        for (int k = 1; k < 20 - 1; k++) {
#pragma HLS pipeline II=1
#pragma HLS loop_tripcount min=18 max=18
          B[i][j][k] = 0.125 * (A[i + 1][j][k] - 2.0 * A[i][j][k] + A[i - 1][j][k])
                      + 0.125 * (A[i][j + 1][k] - 2.0 * A[i][j][k] + A[i][j - 1][k])
                      + 0.125 * (A[i][j][k + 1] - 2.0 * A[i][j][k] + A[i][j][k - 1])
                      + A[i][j][k];
        }
      }
    }

    for (int i = 1; i < 20 - 1; i++) {
#pragma HLS loop_tripcount min=18 max=18

      for (int j = 1; j < 20 - 1; j++) {
#pragma HLS loop_tripcount min=18 max=18

        for (int k = 1; k < 20 - 1; k++) {
#pragma HLS pipeline II=1
#pragma HLS loop_tripcount min=18 max=18
          A[i][j][k] = 0.125 * (B[i + 1][j][k] - 2.0 * B[i][j][k] + B[i - 1][j][k])
                      + 0.125 * (B[i][j + 1][k] - 2.0 * B[i][j][k] + B[i][j - 1][k])
                      + 0.125 * (B[i][j][k + 1] - 2.0 * B[i][j][k] + B[i][j][k - 1])
                      + B[i][j][k];
        }
      }
    }
  }
}