void kernel_3mm(int ni,int nj,int nk,int nl,int nm,
                float E[40][50], float A[40][60], float B[60][50],
                float F[50][70], float C[50][80], float D[80][70],
                float G[40][70])
{
  int i, j, k;

  // Partition arrays to improve parallel access
  #pragma HLS ARRAY_PARTITION variable=A complete dim=2
  #pragma HLS ARRAY_PARTITION variable=B complete dim=1
  #pragma HLS ARRAY_PARTITION variable=C complete dim=2
  #pragma HLS ARRAY_PARTITION variable=D complete dim=1
  #pragma HLS ARRAY_PARTITION variable=E complete dim=2
  #pragma HLS ARRAY_PARTITION variable=F complete dim=2
  #pragma HLS ARRAY_PARTITION variable=G complete dim=2

  // First matrix multiplication: E = A * B
  for (i = 0; i < 40; i++) {
    for (j = 0; j < 50; j++) {
      #pragma HLS PIPELINE II=1
      E[i][j] = 0.0;
      for (k = 0; k < 60; ++k) {
        #pragma HLS UNROLL factor=4
        E[i][j] += A[i][k] * B[k][j];
      }
    }
  }

  // Second matrix multiplication: F = C * D
  for (i = 0; i < 50; i++) {
    for (j = 0; j < 70; j++) {
      #pragma HLS PIPELINE II=1
      F[i][j] = 0.0;
      for (k = 0; k < 80; ++k) {
        #pragma HLS UNROLL factor=4
        F[i][j] += C[i][k] * D[k][j];
      }
    }
  }

  // Third matrix multiplication: G = E * F
  for (i = 0; i < 40; i++) {
    for (j = 0; j < 70; j++) {
      #pragma HLS PIPELINE II=1
      G[i][j] = 0.0;
      for (k = 0; k < 50; ++k) {
        #pragma HLS UNROLL factor=4
        G[i][j] += E[i][k] * F[k][j];
      }
    }
  }
}