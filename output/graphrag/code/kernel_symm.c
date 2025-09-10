void kernel_symm(int m, int n, float alpha, float beta, float C[60][80], float A[60][60], float B[60][80]) {
  float temp2;
  int i, j, k;

  // Apply array partitioning to enable parallel access
  #pragma HLS ARRAY_PARTITION variable=A complete dim=2
  #pragma HLS ARRAY_PARTITION variable=B complete dim=2
  #pragma HLS ARRAY_PARTITION variable=C complete dim=2

  // Pipeline the outer loop over i
  for (i = 0; i < 60; i++) {
    // Pipeline the loop over j with initiation interval 1
    for (j = 0; j < 80; j++) {
      temp2 = 0.0f;

      // Pipeline the inner loop over k with II=1
      // Unroll partially to balance resource usage
      #pragma HLS PIPELINE II=1
      #pragma HLS LOOP_TRIPCOUNT min=60 max=60
      for (k = 0; k < i; k++) {
        // Update C[k][j] - potential dependency, so add dependence pragma
        #pragma HLS DEPENDENCE variable=C inter false
        C[k][j] += alpha * B[i][j] * A[i][k];
        temp2 += B[k][j] * A[i][k];
      }

      // Pipeline the write back to C[i][j]
      #pragma HLS PIPELINE II=1
      C[i][j] = beta * C[i][j] + alpha * B[i][j] * A[i][i] + alpha * temp2;
    }
  }
}