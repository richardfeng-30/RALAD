void kernel_gemm(int ni, int nj, int nk, float alpha, float beta, float C[60][70], float A[60][80], float B[80][70]) {
#pragma HLS INTERFACE m_axi port=C offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=return bundle=control
#pragma HLS INTERFACE s_axilite port=ni bundle=control
#pragma HLS INTERFACE s_axilite port=nj bundle=control
#pragma HLS INTERFACE s_axilite port=nk bundle=control
#pragma HLS INTERFACE s_axilite port=alpha bundle=control
#pragma HLS INTERFACE s_axilite port=beta bundle=control

  int i;
  int j;
  int k;
  
  for (i = 0; i < 60; i++) {
    for (j = 0; j < 70; j++) {
      C[i][j] *= beta;
    }
    #pragma HLS pipeline II=1
    for (k = 0; k < 80; k++) {
      for (j = 0; j < 70; j++) {
        C[i][j] += alpha * A[i][k] * B[k][j];
      }
    }
  }
}