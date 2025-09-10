void kernel_gemm(int ni, int nj, int nk, float alpha, float beta, float C[60][70], float A[60][80], float B[80][70]) {
  #pragma HLS INTERFACE m_axi port=C offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem

  #pragma HLS ARRAY RESHAPE variable=C complete dim=2
  #pragma HLS ARRAY RESHAPE variable=A complete dim=2
  #pragma HLS ARRAY RESHAPE variable=B complete dim=2

  int i;
  int j;
  int k;

  //BLAS PARAMS
  //TRANSA = 'N'
  //TRANSB = 'N'
  // => Form C := alpha*A*B + beta*C,
  //A is NIxNK
  //B is NKxNJ
  //C is NIxNJ

  for (i = 0; i < 60; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=1 max=60 avg=30
    for (j = 0; j < 70; j++) {
      #pragma HLS PIPELINE II=1
      C[i][j] *= beta;
    }

    for (k = 0; k < 80; k++) {
      #pragma HLS LOOP_TRIPCOUNT min=1 max=80 avg=40
      for (j = 0; j < 70; j++) {
        #pragma HLS PIPELINE II=1
        C[i][j] += alpha * A[i][k] * B[k][j];
      }
    }
  }
}