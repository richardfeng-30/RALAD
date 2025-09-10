void kernel_symm(int m, int n, float alpha, float beta, float C[60][80], float A[60][60], float B[60][80]) {
  #pragma HLS INTERFACE m_axi port=C offset=slave
  #pragma HLS INTERFACE m_axi port=A offset=slave
  #pragma HLS INTERFACE m_axi port=B offset=slave
  #pragma HLS INTERFACE s_axli port=m
  #pragma HLS INTERFACE s_axli port=n
  #pragma HLS INTERFACE s_axli port=alpha
  #pragma HLS INTERFACE s_axli port=beta

  float temp2;
  int i;
  int j;
  int k;

  #pragma HLS DATAFLOW

  #pragma HLS PIPELINE II=1
  for (i = 0; i < 60; i++) {
    #pragma HLS LOOP_TRIPCOUNT min=60 max=60
    for (j = 0; j < 80; j++) {
      temp2 = ((float)0);
      #pragma HLS LOOP_TRIPCOUNT min=80 max=80
      for (k = 0; k < i; k++) {
        #pragma HLS UNROLL
        C[k][j] += alpha * B[i][j] * A[i][k];
        temp2 += B[k][j] * A[i][k];
      }
      #pragma HLS UNROLL
      C[i][j] = beta * C[i][j] + alpha * B[i][j] * A[i][i] + alpha * temp2;
    }
  }
}