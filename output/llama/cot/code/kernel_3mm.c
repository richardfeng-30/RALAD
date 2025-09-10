void kernel_3mm(int ni, int nj, int nk, int nl, int nm, float E[40][50], float A[40][60], float B[60][50], float F[50][70], float C[50][80], float D[80][70], float G[40][70]) {
  #pragma HLS dataflow

  #pragma HLS pipeline II=1

  int i;
  int j;
  int k;

  // First matrix multiplication
  #pragma HLS loop_tripcount min=40 max=40
  for (i = 0; i < 40; i++) {
    #pragma HLS loop_tripcount min=50 max=50
    for (j = 0; j < 50; j++) {
      E[i][j] = 0.0;
      #pragma HLS loop_tripcount min=60 max=60
      for (k = 0; k < 60; ++k) {
        E[i][j] += A[i][k] * B[k][j];
      }
    }
  }

  // Second matrix multiplication
  #pragma HLS loop_tripcount min=50 max=50
  for (i = 0; i < 50; i++) {
    #pragma HLS loop_tripcount min=70 max=70
    for (j = 0; j < 70; j++) {
      F[i][j] = 0.0;
      #pragma HLS loop_tripcount min=80 max=80
      for (k = 0; k < 80; ++k) {
        F[i][j] += C[i][k] * D[k][j];
      }
    }
  }

  // Third matrix multiplication
  #pragma HLS loop_tripcount min=40 max=40
  for (i = 0; i < 40; i++) {
    #pragma HLS loop_tripcount min=70 max=70
    for (j = 0; j < 70; j++) {
      G[i][j] = 0.0;
      #pragma HLS loop_tripcount min=50 max=50
      for (k = 0; k < 50; ++k) {
        G[i][j] += E[i][k] * F[k][j];
      }
    }
  }
}