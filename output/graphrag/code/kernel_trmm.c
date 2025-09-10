void kernel_trmm(int m, int n, float alpha, float A[60][60], float B[60][80]) {
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem1
#pragma HLS INTERFACE s_axilite port=m bundle=control
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=alpha bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

  // Partition arrays to enable parallel access
#pragma HLS ARRAY_PARTITION variable=A cyclic factor=16 dim=1
#pragma HLS ARRAY_PARTITION variable=B cyclic factor=16 dim=1

  int i, j, k;

  // Pipeline the outer loop over i with initiation interval 1
  outer_i: for (i = 0; i < 60; i++) {
#pragma HLS PIPELINE II=1

    // Pipeline the middle loop over j with initiation interval 1
    outer_j: for (j = 0; j < 80; j++) {
#pragma HLS PIPELINE II=1

      float sum = B[i][j];

      // Unroll the inner loop partially to balance resource usage
      inner_k: for (k = i + 1; k < 60; k++) {
#pragma HLS UNROLL factor=4
        sum += A[k][i] * B[k][j];
      }

      B[i][j] = alpha * sum;
    }
  }
}