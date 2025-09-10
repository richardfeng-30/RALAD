void kernel_syr2k(int n, int m, float alpha, float beta, float C[80][80], float A[80][60], float B[80][60]) {
#pragma HLS INTERFACE m_axi port=C offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem2
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=m bundle=control
#pragma HLS INTERFACE s_axilite port=alpha bundle=control
#pragma HLS INTERFACE s_axilite port=beta bundle=control
#pragma HLS INTERFACE s_axilite port=C bundle=control
#pragma HLS INTERFACE s_axilite port=A bundle=control
#pragma HLS INTERFACE s_axilite port=B bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  // Partition arrays to enable parallel access
#pragma HLS ARRAY_PARTITION variable=C complete dim=2
#pragma HLS ARRAY_PARTITION variable=A complete dim=2
#pragma HLS ARRAY_PARTITION variable=B complete dim=2

  for (int i = 0; i < 80; i++) {
#pragma HLS LOOP_TRIPCOUNT min=80 max=80
    // Scale C by beta
    for (int j = 0; j <= i; j++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=80 max=80
      C[i][j] *= beta;
    }

    // Accumulate rank-2k update
    for (int k = 0; k < 60; k++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=60 max=60
      for (int j = 0; j <= i; j++) {
#pragma HLS UNROLL factor=4
#pragma HLS LOOP_TRIPCOUNT min=80 max=80
        C[i][j] += A[j][k] * alpha * B[i][k] + B[j][k] * alpha * A[i][k];
      }
    }
  }
}