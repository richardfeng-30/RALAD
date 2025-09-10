void kernel_2mm(int ni,int nj,int nk,int nl,float alpha,float beta,float tmp[40][50],float A[40][70],float B[70][50],float C[50][80],float D[40][80])
{
  int i, j, k;

  // Partition tmp and D arrays to enable parallel access
  #pragma HLS ARRAY_PARTITION variable=tmp complete dim=2
  #pragma HLS ARRAY_PARTITION variable=D complete dim=2

  // First matrix multiplication: tmp = alpha * A * B
  for (i = 0; i < 40; i++) {
    for (j = 0; j < 50; j++) {
      #pragma HLS PIPELINE II=1
      tmp[i][j] = 0.0f;
      for (k = 0; k < 70; ++k) {
        #pragma HLS UNROLL factor=5
        tmp[i][j] += alpha * A[i][k] * B[k][j];
      }
    }
  }

  // Second matrix multiplication: D = beta * D + tmp * C
  for (i = 0; i < 40; i++) {
    for (j = 0; j < 80; j++) {
      #pragma HLS PIPELINE II=1
      D[i][j] *= beta;
      for (k = 0; k < 50; ++k) {
        #pragma HLS UNROLL factor=5
        D[i][j] += tmp[i][k] * C[k][j];
      }
    }
  }
}