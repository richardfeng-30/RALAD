void kernel_symm(int m, int n, float alpha, float beta, float C[60][80], float A[60][60], float B[60][80]) {
  #pragma HLS INTERFACE m_axi port=C offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=A offset=slave bundle=gmem
  #pragma HLS INTERFACE m_axi port=B offset=slave bundle=gmem
  #pragma HLS INTERFACE s_axilite port=m
  #pragma HLS INTERFACE s_axilite port=n
  #pragma HLS INTERFACE s_axilite port=alpha
  #pragma HLS INTERFACE s_axilite port=beta
  #pragma HLS INTERFACE s_axilite port=return

  #pragma HLS ARRAY_PARTITION variable=C cyclic factor=4
  #pragma HLS ARRAY_PARTITION variable=A cyclic factor=4
  #pragma HLS ARRAY_PARTITION variable=B cyclic factor=4

  float temp2;
  int i;
  int j;
  int k;

  for (i = 0; i < 60; i++) {
    #pragma HLS LOOP_TRIPCOUNT avg=30 max=60
    for (j = 0; j < 80; j++) {
      temp2 = ((float)0);
      for (k = 0; k <= i; k++) {
        #pragma HLS UNROLL
        C[k][j] += alpha * B[i][j] * A[i][k];
        temp2 += B[k][j] * A[i][k];
      }
      C[i][j] = beta * C[i][j] + alpha * B[i][j] * A[i][i] + alpha * temp2;
    }
  }
}